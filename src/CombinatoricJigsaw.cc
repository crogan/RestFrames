/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricJigsaw.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/CombinatoricJigsaw.hh"
#include "RestFrames/CombinatoricState.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  CombinatoricJigsaw::CombinatoricJigsaw(const string& sname,const string& stitle)
    : Jigsaw(sname, stitle)
  {
    Init();
  }

  CombinatoricJigsaw::~CombinatoricJigsaw(){ }

  void CombinatoricJigsaw::Init(){
    m_Type = JCombinatoric;
  }

  void CombinatoricJigsaw::Clear(){
    ClearOutputStates();
    Jigsaw::Clear();
  }

  void CombinatoricJigsaw::AddFrame(RestFrame& frame, int i){
    if(frame.IsEmpty()) return;
    if(!m_GroupPtr) return;
    m_Log << LogVerbose;
    m_Log << "Adding frame " << frame.GetName();
    m_Log << " to hemisphere " << i << m_End;
    vector<FrameType> terminals;
    terminals.push_back(FVisible);
    terminals.push_back(FInvisible);
    RFList<RestFrame> frames = frame.GetListFramesType(terminals);
    int N = frames.GetN();
    for(int f = 0; f < N; f++){
      if(m_GroupPtr->ContainsFrame(frames.Get(f))){
	AddOutputFrame(frames.Get(f), i);
      } else {
	AddDependancyFrame(frames.Get(f), i);
      }
    }
  }

  void CombinatoricJigsaw::AddFrame(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      AddFrame(frames.Get(f),i);
  }

  State& CombinatoricJigsaw::NewOutputState(){
    CombinatoricState* statePtr = new CombinatoricState();
    AddDependent(statePtr);
    m_OutputStates.Add(*statePtr);
    m_CombinatoricOutputStates.Add(*statePtr);
    return *statePtr;
  }

  void CombinatoricJigsaw::ClearOutputStates(){
    m_OutputStates.Clear();
    m_CombinatoricOutputStates.Clear();
  }

  bool CombinatoricJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>& chain_jigsaws){
    if(!IsSoundMind()) return false;
    if(chain_jigsaws.Contains(*this)) return true;

    m_ExecuteJigsaws.Clear();

    // Add group dependancy jigsaws first
    RFList<Jigsaw> group_jigsaws; 
    FillGroupJigsawDependancies(group_jigsaws);
    group_jigsaws.Remove(*this);

    int Ngroup = group_jigsaws.GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw& jigsaw = group_jigsaws.Get(i);
      m_DependancyJigsaws.Remove(jigsaw);
      if(!chain_jigsaws.Contains(jigsaw)){
	if(!jigsaw.InitializeJigsawExecutionList(chain_jigsaws)){
	  SetMind(false);
	  return false;
	}
      }
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsaws.GetN() > 0){
      Jigsaw& jigsaw = m_DependancyJigsaws.Get(m_DependancyJigsaws.GetN()-1);
      // Remove dependancy if already in chain
      if(chain_jigsaws.Contains(jigsaw)){
	m_DependancyJigsaws.Remove(jigsaw);
	continue;
      }
      
      if(!jigsaw.DependsOnJigsaw(*this)){
	if(!jigsaw.InitializeJigsawExecutionList(chain_jigsaws)){
	  SetMind(false);
	  return false;
	}
	m_DependancyJigsaws.Remove(jigsaw);
	continue;
      }
      RFList<Jigsaw> temp_chain = chain_jigsaws.Copy();
      temp_chain.Add(m_ExecuteJigsaws);
      temp_chain.Add(*this);
      if(!jigsaw.InitializeJigsawExecutionList(temp_chain)){
	SetMind(false);
	return false;
      }
      temp_chain.Remove(*this);
      temp_chain.Remove(chain_jigsaws);
      m_DependancyJigsaws.Remove(temp_chain);
      m_ExecuteJigsaws.Add(temp_chain);
    }
    chain_jigsaws.Add(*this);
    chain_jigsaws.Add(m_ExecuteJigsaws);
    return true;
  }

  bool CombinatoricJigsaw::ExecuteDependancyJigsaws(){
    int N = m_ExecuteJigsaws.GetN();
    for(int i = 0; i < N; i++){
      if(!m_ExecuteJigsaws.Get(i).AnalyzeEvent()) return false;
    }
    return true;
  }

  bool CombinatoricJigsaw::InitializeEvent(){
    if(!m_Mind) return false;

    const CombinatoricState* input_statePtr = dynamic_cast<const CombinatoricState*>(m_InputStatePtr);
    if(!input_statePtr) return false;

    CombinatoricGroup* groupPtr = dynamic_cast<CombinatoricGroup*>(m_GroupPtr);
    if(!groupPtr) return false;

    m_Inputs.clear();
    const RFList<State> elements = input_statePtr->GetElements();
    int Ninput = elements.GetN();
    for(int i = 0; i < Ninput; i++)
      m_Inputs.push_back(&elements.Get(i));

    m_Outputs.clear();
    m_NForOutput.clear();
    m_NExclusive.clear();
    int Noutput = m_CombinatoricOutputStates.GetN();
    for(int i = 0; i < Noutput; i++){
      CombinatoricState& state = m_CombinatoricOutputStates.Get(i);
      m_Outputs.push_back(&state);
      RFList<RestFrame> frames = state.GetFrames();
      int Nf = frames.GetN();
      int NTOT = 0;
      bool exclTOT = true;
      for(int f = 0; f < Nf; f++){
	int N = -1;
	bool excl = false;
	groupPtr->GetNElementsForFrame(frames.Get(f), N, excl);
	if(N < 0) return false;
	NTOT += N;
	exclTOT = exclTOT && excl;
      }
      m_NForOutput.push_back(NTOT);
      m_NExclusive.push_back(exclTOT);
    }

    return true;
  }

}
