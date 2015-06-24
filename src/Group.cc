/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   Group.cc
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

#include "RestFrames/Group.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // Group class methods
  ///////////////////////////////////////////////
  int Group::m_class_key = 0;

  Group::Group(const string& sname, const string& stitle)
    : RFBase(sname, stitle)
  {
    Init();
  }

  Group::~Group(){
    Clear();
    delete m_JigsawsPtr;
    delete m_JigsawsToUsePtr;
    delete m_StatesPtr;
    delete m_StatesToSplitPtr;
  }

  void Group::Init(){
    SetKey(GenKey());
    m_GroupStatePtr = nullptr;
    m_JigsawsPtr = new RFList<Jigsaw>();
    m_JigsawsToUsePtr = new RFList<Jigsaw>();
    m_StatesPtr = new StateList();
    m_StatesToSplitPtr = new StateList();
    m_Log.SetSource("Group "+GetName());
  }

  void Group::Clear(){
    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_Frames.Clear();
    m_StatesPtr->Clear();
    m_JigsawsPtr->Clear();
    m_StatesToSplitPtr->Clear();
    m_JigsawsToUsePtr->Clear(); 
  }

  int Group::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  bool Group::IsInvisibleGroup() const{
    return m_Type == GInvisible;
  }
  
  bool Group::IsCombinatoricGroup() const{
    return m_Type == GCombinatoric;
  }

  void Group::RemoveFrame(const RestFrame* framePtr){
    if(!framePtr) return;
    m_Frames.Remove(framePtr);
  }

  bool Group::ContainsFrame(const RestFrame* framePtr) const {
    return m_Frames.Contains(framePtr);
  }

  RFList<RestFrame>* Group::GetFrames() const {
    return m_Frames.Copy();
  }

  RFList<Jigsaw>* Group::GetJigsaws() const {
    return m_JigsawsPtr->Copy();
  }

  State* Group::GetGroupState() const {
    return m_GroupStatePtr;
  }

  bool Group::InitializeAnalysis(){
    m_StatesPtr->Clear();
    m_StatesToSplitPtr->Clear();
    if(m_GroupStatePtr) delete m_GroupStatePtr;

    m_GroupStatePtr = InitializeGroupState();
    m_GroupStatePtr->AddFrame(&m_Frames);
    m_StatesPtr->Add(m_GroupStatePtr);
    m_StatesToSplitPtr->Add(m_GroupStatePtr);
 
    if(!InitializeJigsaws()){
      SetBody(false);
      SetMind(false);
      return false;
    }
    m_Log << LogVerbose;
    m_Log << "Successfully initialized Group for analysis";
    m_Log << m_End;
    SetBody(true);
    SetMind(true);
    return true;
  }

  State* Group::InitializeGroupState(){
    return new State();
  }

  bool Group::InitializeJigsaws(){
    while(m_StatesToSplitPtr->GetN() > 0){
      State* statePtr = m_StatesToSplitPtr->Get(0);
      if(!SplitState(statePtr)){
	if(statePtr->GetNFrames() != 1) return false; 
	m_StatesToSplitPtr->Remove(statePtr);
      }
    }
    return true;
  }

  bool Group::SplitState(const State* statePtr){
    int N = m_JigsawsToUsePtr->GetN();
    
    Jigsaw* jigsawForSplitPtr = nullptr;
    for(int i = 0; i < N; i++){
      Jigsaw* jigsawPtr = m_JigsawsToUsePtr->Get(i);
      if(jigsawPtr->CanSplit(statePtr)){
	if(!jigsawForSplitPtr) jigsawForSplitPtr = jigsawPtr;
	if(jigsawPtr->GetPriority() < jigsawForSplitPtr->GetPriority()) jigsawForSplitPtr = jigsawPtr;
      }
    }
    if(!jigsawForSplitPtr) return false;
    InitializeJigsaw(jigsawForSplitPtr);
    m_JigsawsToUsePtr->Remove(jigsawForSplitPtr);
    return true;
  }

  void Group::InitializeJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return;
    if(!jigsawPtr->IsSoundBody()) return;

    State* statePtr = m_StatesToSplitPtr->Get(0);
    StateList* statesPtr = jigsawPtr->InitializeOutputStates(statePtr);
    m_StatesToSplitPtr->Remove(statePtr);
    m_StatesToSplitPtr->Add(statesPtr);
    m_StatesPtr->Add(statesPtr);
    m_JigsawsPtr->Add(jigsawPtr);
    if(statesPtr) delete statesPtr;
    return;
  }

  State* Group::GetState(const RestFrame* framePtr) const{
    if(!framePtr) return nullptr;
    int Ns = m_StatesPtr->GetN();
    for(int i = Ns-1; i >= 0; i--){
      State* statePtr = m_StatesPtr->Get(i);
      if(statePtr->IsFrame(framePtr)){
	return statePtr;
      }
    }
    m_Mind = false;
    return nullptr;
  }
  
  bool Group::GetState(const RFList<RestFrame>* framesPtr, StateList*& statesPtr){
    if(statesPtr){
      delete statesPtr;
      statesPtr = nullptr;
    }
    if(!framesPtr) return false;

    statesPtr = new StateList();
    int Ns = m_StatesPtr->GetN();
    for(int i = 0; i < Ns; i++){
      State* istatePtr = m_StatesPtr->Get(i);
      RFList<RestFrame>* iframesPtr = istatePtr->GetFrames();
      if(framesPtr->Contains(iframesPtr)){
	int Nsol = statesPtr->GetN();
	bool isnew = true;
	for(int j = 0; j < Nsol; j++){
	  State* jstatePtr = statesPtr->Get(j);
	  RFList<RestFrame>* jframesPtr = jstatePtr->GetFrames();
	  if(iframesPtr->Contains(jframesPtr)){
	    statesPtr->Remove(jstatePtr);
	    delete jframesPtr;
	    break;
	  }
	  if(jframesPtr->Contains(iframesPtr)){
	    isnew = false;
	    delete jframesPtr;
	    break;
	  }
	  delete jframesPtr;
	}
	if(isnew) statesPtr->Add(istatePtr);
      }
      delete iframesPtr;
    }
    RFList<RestFrame>* match_framesPtr = new RFList<RestFrame>();
    Ns = statesPtr->GetN();
    for(int i = 0; i < Ns; i++){
      match_framesPtr->Add(statesPtr->Get(i)->GetFrames());
    }
    if(!framesPtr->IsSame(match_framesPtr)){
      delete match_framesPtr;
      delete statesPtr;
      statesPtr = nullptr;
      m_Mind = false;
      return false;
    }
    delete match_framesPtr;
    return true;
  }

}
