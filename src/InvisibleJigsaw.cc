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

  InvisibleJigsaw::~InvisibleJigsaw(){
   
  }

  void InvisibleJigsaw::Init(int Ninv, int Nvis){
    m_Type = JInvisible;
    m_Ninv = Ninv;
    m_Nvis = Nvis;
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame& frame, int i){
    AddVisibleFrame(&frame,i);
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    RFList<RestFrame>* framesPtr = framePtr->GetListVisibleFrames();
    AddDependancyFrame(framesPtr, i);
    delete framesPtr;
  }

  void InvisibleJigsaw::AddVisibleFrame(RFList<RestFrame>* framesPtr, int i){
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddVisibleFrame(framesPtr->Get(f), i);
    }
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame& frame, int i){
    AddInvisibleFrame(&frame,i);
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame* framePtr, int i){
    if(!m_GroupPtr) return;
  
    RFList<RestFrame>* framesPtr = framePtr->GetListInvisibleFrames();
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      if(m_GroupPtr->ContainsFrame(framesPtr->Get(f))) 
	AddOutputFrame(framesPtr->Get(f), i);
    }  
    delete framesPtr;
  }

  void InvisibleJigsaw::AddInvisibleFrame(RFList<RestFrame>* framesPtr, int i){
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddInvisibleFrame(framesPtr->Get(f), i);
    }
  }

  State* InvisibleJigsaw::NewOutputState(){
    return new InvisibleState();
  }

  double InvisibleJigsaw::GetMinimumMass(){
    if(!IsSoundMind()) return 0.;
    int N = GetNChildStates();
    double M = 0.;
    for(int i = 0; i < N; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(!statePtr) return 0.;
      M += statePtr->GetMinimumMass();
    }

    return M;
  }

  void InvisibleJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsawsPtr){ 
    if(!jigsawsPtr) return;
   
    int Nchild = GetNChildStates();
    for(int i = 0; i < Nchild; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(statePtr) statePtr->FillInvisibleMassJigsawDependancies(jigsawsPtr);
    }
  }

  bool InvisibleJigsaw::InitializeDependancyJigsaws(){
    if(!m_Mind) return false;
    m_DependancyJigsawsPtr->Clear();

    RFList<Jigsaw>* jigsawsPtr = new RFList<Jigsaw>();
    FillStateJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    jigsawsPtr->Clear();
    FillInvisibleMassJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    jigsawsPtr->Clear();
    FillGroupJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    delete jigsawsPtr;
    return m_Mind;
  }

  bool InvisibleJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr){
    if(!m_Mind) return false;
    if(chain_jigsawPtr->Contains(this)) return true;

    // Add group dependancy jigsaws first
    RFList<Jigsaw>* group_jigsawsPtr = new RFList<Jigsaw>();
    FillGroupJigsawDependancies(group_jigsawsPtr);
    group_jigsawsPtr->Remove(this);
    int Ngroup = group_jigsawsPtr->GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw* jigsawPtr = group_jigsawsPtr->Get(i);
      if(!chain_jigsawPtr->Contains(jigsawPtr)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
      }
      m_DependancyJigsawsPtr->Remove(jigsawPtr);
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsawsPtr->GetN() > 0){
      Jigsaw* jigsawPtr = m_DependancyJigsawsPtr->Get(m_DependancyJigsawsPtr->GetN()-1);
      if(chain_jigsawPtr->Contains(jigsawPtr)){
	m_DependancyJigsawsPtr->Remove(jigsawPtr);
	continue;
      }
      if(jigsawPtr->DependsOnJigsaw(this)) m_DependancyJigsawsPtr->Remove(jigsawPtr);
      if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	m_Mind = false;
	return false;
      }
      m_DependancyJigsawsPtr->Remove(jigsawPtr);
      continue;
    }
    chain_jigsawPtr->Add(this);
    
    return true;
  }

  void InvisibleJigsaw::SetGroup(Group* groupPtr){
    Jigsaw::SetGroup(groupPtr);
    if(!groupPtr) return;
    if(m_Ninv == 1) AddInvisibleFrame(groupPtr->GetFrames());
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
      if(m_DependancyFrames[i]->GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of visible frames: " << i;
	m_Log << m_End;
	SetBody(false);
	return false;
      }
    for(int i = 0; i < Nout; i++)
      if(m_OutputFrames[i]->GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of invisible frames: " << i;
	m_Log << m_End;
	SetBody(false);
	return false;
      }
   
    SetBody(true);
    return true;
  }

}
