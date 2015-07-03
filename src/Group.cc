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
    if(m_GroupStatePtr) delete m_GroupStatePtr;
  }

  void Group::Init(){
    SetKey(GenKey());
    m_GroupStatePtr = nullptr;
    m_Log.SetSource("Group "+GetName());
  }

  void Group::Clear(){
    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_Frames.Clear();
    m_States.Clear();
    m_Jigsaws.Clear();
    m_StatesToSplit.Clear();
    m_JigsawsToUse.Clear(); 
    RFBase::Clear();
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

  RFList<RestFrame> Group::GetFrames() const {
    return m_Frames.Copy();
  }

  RFList<Jigsaw> Group::GetJigsaws() const {
    return m_Jigsaws.Copy();
  }

  State* Group::GetGroupState() const {
    return m_GroupStatePtr;
  }

  bool Group::InitializeAnalysis(){
    m_Log << LogVerbose;
    m_Log << "Initializing Group for analysis...";
    m_Log << m_End;
    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_States.Clear();
    m_StatesToSplit.Clear();
    m_GroupStatePtr = InitializeGroupState();
    m_GroupStatePtr->AddFrame(m_Frames);
    m_States.Add(m_GroupStatePtr);
    m_StatesToSplit.Add(m_GroupStatePtr);
 
    if(!InitializeJigsaws()){
      m_Log << LogWarning;
      m_Log << "Unable to initialize jigsaws";
      m_Log << m_End;
      SetBody(false);
      SetMind(false);
      return false;
    }
    m_Log << LogVerbose;
    m_Log << "...Done";
    m_Log << m_End;
    SetBody(true);
    SetMind(true);
    return true;
  }

  State* Group::InitializeGroupState(){
    State* state = new State();
    return state;
  }

  bool Group::InitializeJigsaws(){
    while(m_StatesToSplit.GetN() > 0){
      State* statePtr = m_StatesToSplit.Get(0);
      if(!SplitState(statePtr)){
	if(statePtr->GetNFrames() != 1){
	  m_Log << LogVerbose;
	  m_Log << "Cannot find Jigsaw to split State for frames:" << endl;
	  m_Log << Log(statePtr->GetFrames());
	  m_Log << m_End;
	  return false; 
	}
	m_StatesToSplit.Remove(statePtr);
      }
    }
    return true;
  }

  bool Group::SplitState(const State* statePtr){
    int N = m_JigsawsToUse.GetN();
    
    Jigsaw* jigsawForSplitPtr = nullptr;
    for(int i = 0; i < N; i++){
      Jigsaw* jigsawPtr = m_JigsawsToUse.Get(i);
      if(jigsawPtr->CanSplit(statePtr)){
	if(!jigsawForSplitPtr){
	  jigsawForSplitPtr = jigsawPtr;
	  continue;
	}
	if(jigsawPtr->GetNOutputStates() <= jigsawForSplitPtr->GetNOutputStates())
	  if((jigsawPtr->GetNOutputStates() < jigsawForSplitPtr->GetNOutputStates()) ||
	     (jigsawPtr->GetNDependancyStates() < jigsawForSplitPtr->GetNDependancyStates())) 
	    jigsawForSplitPtr = jigsawPtr;
      }
    }
    if(!jigsawForSplitPtr)
      return false;
    
    m_Log << LogVerbose;
    m_Log << "Found Jigsaw to split State:" << endl; 
    m_Log << " Frames:" << endl << "   ";
    m_Log << Log(statePtr->GetFrames()) << endl;
    m_Log << " Jigsaw:" << Log(jigsawForSplitPtr);
    m_Log << m_End;
    InitializeJigsaw(jigsawForSplitPtr);
    m_JigsawsToUse.Remove(jigsawForSplitPtr);
    return true;
  }

  void Group::InitializeJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return;
    if(!jigsawPtr->IsSoundBody()){
      m_Log << LogWarning;
      m_Log << "Unable to initialize Jigsaw ";
      m_Log << jigsawPtr->GetName() << m_End;
      return;
    }

    State* statePtr = m_StatesToSplit.Get(0);
    StateList states = jigsawPtr->InitializeOutputStates(statePtr);
    m_StatesToSplit.Remove(statePtr);
    m_StatesToSplit.Add(states);
    m_States.Add(states);
    m_Jigsaws.Add(jigsawPtr);
    return;
  }
 
  State* Group::GetState(const RestFrame* framePtr) const {
    if(!framePtr) return nullptr;
    int Ns = m_States.GetN();
    for(int i = Ns-1; i >= 0; i--){
      State* statePtr = m_States.Get(i);
      if(statePtr->IsFrame(framePtr)){
	return statePtr;
      }
    }
    m_Mind = false;
    return nullptr;
  }
  
  StateList Group::GetStates(const RestFrames::RFList<RestFrame>& frames) const {
    StateList states;

    // Find States that correspond to these frames, giving
    // preference to States that include more frames (less dependancies)
    int Ns = m_States.GetN();
    for(int i = 0; i < Ns; i++){
      RFList<RestFrame> iframes = m_States.Get(i)->GetFrames();
      if(frames.Contains(iframes)){
	int Nsol = states.GetN();
	bool isnew = true;
	for(int j = 0; j < Nsol; j++){
	  // if new copy of existing frame list appears, discard old
	  RFList<RestFrame> jframes = states.Get(j)->GetFrames();
	  if(iframes.Contains(jframes)){
	    states.Remove(states.Get(j));
	    break;
	  }
	  // if subset of existing frame list appears, discard new
	  if(jframes.Contains(iframes)){
	    isnew = false;
	    break;
	  }
	}
	if(isnew) states.Add(m_States.Get(i));
      }
    }

    // check to see whether collection of states 
    // includes all requested frames
    RFList<RestFrame> match_frames;
    Ns = states.GetN();
    for(int i = 0; i < Ns; i++)
      match_frames.Add(states.Get(i)->GetFrames());
    
    if(!frames.IsSame(match_frames)){
      m_Log << LogWarning;
      m_Log << "Unable to find States corresponding to frames: " << endl;
      m_Log << Log(frames) << m_End;
      SetMind(false);
      return StateList();
    }
    
    return states;
  }

}
