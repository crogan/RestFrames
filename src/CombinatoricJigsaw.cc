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

  CombinatoricJigsaw::~CombinatoricJigsaw(){
    
  }

  void CombinatoricJigsaw::Init(){
    m_Type = JCombinatoric;
  }

  void CombinatoricJigsaw::Clear(){
    Jigsaw::Clear();
  }

  void CombinatoricJigsaw::AddFrame(RestFrame& frame, int i){
    AddFrame(&frame,i);
  }

  void CombinatoricJigsaw::AddFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    if(!m_GroupPtr) return;
    m_Log << LogVerbose;
    m_Log << "Adding frame " << framePtr->GetName();
    m_Log << " to hemisphere " << i << m_End;
    vector<FrameType> terminals;
    terminals.push_back(FVisible);
    terminals.push_back(FInvisible);
    RFList<RestFrame> *framesPtr = framePtr->GetListFramesType(terminals);
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      if(m_GroupPtr->ContainsFrame(framesPtr->Get(f))){
	AddOutputFrame(framesPtr->Get(f), i);
      } else {
	AddDependancyFrame(framesPtr->Get(f), i);
      }
    }
    delete framesPtr;
  
  }

  void CombinatoricJigsaw::AddFrame(RFList<RestFrame>* framesPtr, int i){
    if(!framesPtr) return;
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddFrame(framesPtr->Get(f),i);
    }
  }

  State* CombinatoricJigsaw::NewOutputState(){
    return new CombinatoricState();
  }

  bool CombinatoricJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr){
    if(!m_Mind) return false;
    if(chain_jigsawPtr->Contains(this)) return true;
    m_ExecuteJigsaws.Clear();

    // Add group dependancy jigsaws first
    RFList<Jigsaw>* group_jigsawsPtr = new RFList<Jigsaw>();
    FillGroupJigsawDependancies(group_jigsawsPtr);
    group_jigsawsPtr->Remove(this);
    int Ngroup = group_jigsawsPtr->GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw* jigsawPtr = group_jigsawsPtr->Get(i);
      m_DependancyJigsaws.Remove(jigsawPtr);
      if(!chain_jigsawPtr->Contains(jigsawPtr)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
      }
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsaws.GetN() > 0){
      Jigsaw* jigsawPtr = m_DependancyJigsaws.Get(m_DependancyJigsaws.GetN()-1);
      // Remove dependancy if already in chain
      if(chain_jigsawPtr->Contains(jigsawPtr)){
	m_DependancyJigsaws.Remove(jigsawPtr);
	continue;
      }
      
      if(!jigsawPtr->DependsOnJigsaw(this)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
	m_DependancyJigsaws.Remove(jigsawPtr);
	continue;
      }
      RFList<Jigsaw>* temp_chainPtr = chain_jigsawPtr->Copy();
      temp_chainPtr->Add(&m_ExecuteJigsaws);
      temp_chainPtr->Add(this);
      if(!jigsawPtr->InitializeJigsawExecutionList(temp_chainPtr)){
	m_Mind = false;
	return false;
      }
      temp_chainPtr->Remove(this);
      temp_chainPtr->Remove(chain_jigsawPtr);
      m_DependancyJigsaws.Remove(temp_chainPtr);
      m_ExecuteJigsaws.Add(temp_chainPtr);
      
      delete temp_chainPtr;
    }
    chain_jigsawPtr->Add(this);
    chain_jigsawPtr->Add(&m_ExecuteJigsaws);
    return true;
  }

  bool CombinatoricJigsaw::ExecuteDependancyJigsaws(){
    int N = m_ExecuteJigsaws.GetN();
    for(int i = 0; i < N; i++){
      if(!m_ExecuteJigsaws.Get(i)->AnalyzeEvent()) return false;
    }
    return true;
  }

  bool CombinatoricJigsaw::InitializeEvent(){
    if(!m_Mind) return false;

    CombinatoricState* input_statePtr = dynamic_cast<CombinatoricState*>(m_InputStatePtr);
    if(!input_statePtr) return false;

    CombinatoricGroup* groupPtr = dynamic_cast<CombinatoricGroup*>(m_GroupPtr);
    if(!groupPtr) return false;

    m_Inputs.clear();
    const StateList* elementsPtr = input_statePtr->GetElements();
    int Ninput = elementsPtr->GetN();
    for(int i = 0; i < Ninput; i++)
      m_Inputs.push_back(elementsPtr->Get(i));

    m_Outputs.clear();
    m_NForOutput.clear();
    m_NExclusive.clear();
    int Noutput = m_OutputStates.GetN();
    for(int i = 0; i < Noutput; i++){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(m_OutputStates.Get(i));
      if(!statePtr) return false;
      m_Outputs.push_back(statePtr);
      RFList<RestFrame>* framesPtr = statePtr->GetFrames();
      int Nf = framesPtr->GetN();
      int NTOT = 0;
      bool exclTOT = true;
      for(int f = 0; f < Nf; f++){
	int N = -1;
	bool excl = false;
	groupPtr->GetNElementsForFrame(framesPtr->Get(f), N, excl);
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
