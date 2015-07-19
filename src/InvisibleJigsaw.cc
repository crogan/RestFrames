/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleJigsaw.cc
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

#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/InvisibleState.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleJigsaw class
  ///////////////////////////////////////////////

  InvisibleJigsaw::InvisibleJigsaw(const string& sname, const string& stitle, int Ninv, int Nvis) : 
    Jigsaw(sname, stitle)
  {
    Init(Ninv, Nvis);
  }

  InvisibleJigsaw::InvisibleJigsaw() : Jigsaw() {}

  InvisibleJigsaw::~InvisibleJigsaw(){ }

  void InvisibleJigsaw::Clear(){
    ClearOutputStates();
    Jigsaw::Clear();
  }

  void InvisibleJigsaw::Init(int Ninv, int Nvis){
    m_Type = JInvisible;
    m_Ninv = Ninv;
    m_Nvis = Nvis;
  }

  InvisibleJigsaw& InvisibleJigsaw::Empty(){
    return InvisibleJigsaw::m_Empty;
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame& frame, int i){
    if(frame.IsEmpty()) return;
    AddDependancyFrame(frame.GetListVisibleFrames(), i);
  }

  void InvisibleJigsaw::AddVisibleFrame(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      AddVisibleFrame(frames.Get(f), i);
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame& frame, int i){
    if(!m_GroupPtr) return;
    RFList<RestFrame> frames = frame.GetListInvisibleFrames();
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      if(m_GroupPtr->ContainsFrame(frames[f]))
	AddOutputFrame(frames[f], i);
  }

  void InvisibleJigsaw::AddInvisibleFrame(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++){
      AddInvisibleFrame(frames.Get(f), i);
    }
  }

  void InvisibleJigsaw::ClearOutputStates(){
    m_OutputStates.Clear();
    m_InvisibleOutputStates.Clear();
  }

  State& InvisibleJigsaw::NewOutputState(){
    InvisibleState* statePtr = new InvisibleState();
    AddDependent(statePtr);
    m_OutputStates.Add(*statePtr);
    m_InvisibleOutputStates.Add(*statePtr);
    return *statePtr;
  }

  double InvisibleJigsaw::GetMinimumMass() const{
    if(!IsSoundMind()) return 0.;
    int N = GetNChildStates();
    double M = 0.;
    for(int i = 0; i < N; i++){
      InvisibleState& state = m_InvisibleOutputStates.Get(i);
      M += state.GetMinimumMass();
    }
    return M;
  }

  void InvisibleJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const{ 
    int Nchild = GetNChildStates();
    for(int i = 0; i < Nchild; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(&GetChildState(i));
      if(statePtr) statePtr->FillInvisibleMassJigsawDependancies(jigsaws);
    }
  }

  bool InvisibleJigsaw::InitializeDependancyJigsaws(){
    if(!IsSoundMind()) return false;
    m_DependancyJigsaws.Clear();

    RFList<Jigsaw> jigsaws; 
    FillStateJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillInvisibleMassJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillGroupJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    return true;
  }

  bool InvisibleJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>& chain_jigsaws){
   if(!IsSoundMind()) return false;
    if(chain_jigsaws.Contains(*this)) return true;

    // Add group dependancy jigsaws first
    RFList<Jigsaw> group_jigsaws;
    FillGroupJigsawDependancies(group_jigsaws);
    group_jigsaws.Remove(*this);

    int Ngroup = group_jigsaws.GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw& jigsaw = group_jigsaws.Get(i);
      if(!chain_jigsaws.Contains(jigsaw)){
	if(!jigsaw.InitializeJigsawExecutionList(chain_jigsaws)){
	  SetMind(false);
	  return false;
	}
      }
      m_DependancyJigsaws.Remove(jigsaw);
    }

    // Satisfy dependancy jigsaws
    while(m_DependancyJigsaws.GetN() > 0){
      Jigsaw& jigsaw = m_DependancyJigsaws.Get(m_DependancyJigsaws.GetN()-1);
      if(chain_jigsaws.Contains(jigsaw)){
	m_DependancyJigsaws.Remove(jigsaw);
	continue;
      }
      if(jigsaw.DependsOnJigsaw(*this)) m_DependancyJigsaws.Remove(jigsaw);
      if(!jigsaw.InitializeJigsawExecutionList(chain_jigsaws)){
	SetMind(false);
	return false;
      }
      m_DependancyJigsaws.Remove(jigsaw);
      continue;
    }
    chain_jigsaws.Add(*this);
    
    return true;
  }

  void InvisibleJigsaw::SetGroup(Group& group){
    Jigsaw::SetGroup(group);
    if(m_Ninv == 1)
      AddInvisibleFrame(group.GetListFrames());
    
  }

  bool InvisibleJigsaw::IsSoundBody() const {
    if(!Jigsaw::IsSoundBody()){
      m_Log << LogWarning << "Problem with Jigsaw" << m_End;
      SetBody(false);
      return false;
    }

    int Ndep = m_DependancyFrames.size();
    int Nout = m_OutputFrames.size();

    if(Ndep != m_Nvis || Nout != m_Ninv){
      m_Log << LogWarning;
      m_Log << "Incorrect number of input/output frames for jigsaw";
      m_Log<< m_End;
      SetBody(false);
      return false;
    }
    for(int i = 0; i < Ndep; i++)
      if(m_DependancyFrames[i].GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of visible frames: " << i;
	m_Log << m_End;
	SetBody(false);
	return false;
      }
    for(int i = 0; i < Nout; i++)
      if(m_OutputFrames[i].GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of invisible frames: " << i;
	m_Log << m_End;
	SetBody(false);
	return false;
      }
   
    SetBody(true);
    return true;
  }

  InvisibleJigsaw InvisibleJigsaw::m_Empty;

}
