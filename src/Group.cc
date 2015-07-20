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
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  int Group::m_class_key = 0;

  Group::Group(const string& sname, const string& stitle)
    : RFBase(sname, stitle)
  {
    Init();
  }

  Group::Group() : RFBase() {}

  Group::~Group(){
    if(m_GroupStatePtr) delete m_GroupStatePtr;
  }

  Group& Group::Empty(){
    return InvisibleGroup::Empty();
  }

  void Group::Init(){
    SetKey(GenKey());
    m_Type = kVanillaGroup;
    m_GroupStatePtr = nullptr;
    m_Log.SetSource("Group "+GetName());
  }

  void Group::Clear(){
    SetBody(false);
    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_Frames.Clear();
    m_States.Clear();
    m_Jigsaws.Clear();
    m_StatesToResolve.Clear();
    m_JigsawsToUse.Clear(); 
    RFBase::Clear();
  }

  int Group::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  bool Group::IsInvisibleGroup() const{
    return m_Type == kInvisibleGroup;
  }
  
  bool Group::IsCombinatoricGroup() const{
    return m_Type == kCombinatoricGroup;
  }

  void Group::AddFrame(RestFrame& frame){
    SetBody(false);
    if(!frame) return;
    if(!frame.IsRecoFrame()) return;
    static_cast<ReconstructionFrame&>(frame).SetGroup(*this);
    m_Frames.Add(frame);
  }

  void Group::AddJigsaw(Jigsaw& jigsaw){
    SetBody(false);
    if(!jigsaw) return;

    if(!jigsaw.GetGroup().IsEmpty()){
      if(jigsaw.GetGroup() == *this) return;
      Group& group = jigsaw.GetGroup();
      jigsaw.SetGroup();
      group.RemoveJigsaw(jigsaw);
    }
      
    if(m_JigsawsToUse.Add(jigsaw))
      jigsaw.SetGroup(*this);
  }

  void Group::RemoveFrame(const RestFrame& frame){
    SetBody(false);
    m_Frames.Remove(frame);
  }

  void Group::RemoveJigsaw(const Jigsaw& jigsaw){
    SetBody(false);
    m_Jigsaws.Remove(jigsaw);
  }

  bool Group::ContainsFrame(const RestFrame& frame) const {
    return m_Frames.Contains(frame);
  }

  const RFList<RestFrame>& Group::GetListFrames() const {
    return m_Frames;
  }

  const RFList<Jigsaw>& Group::GetListJigsaws() const {
    return m_Jigsaws;
  }

  State& Group::GetParentState() const {
    if(m_GroupStatePtr)
      return *m_GroupStatePtr;
    else
      return State::Empty();
  }

  bool Group::InitializeAnalysis(){
    m_Log << LogVerbose;
    m_Log << "Initializing Group for analysis...";
    m_Log << m_End;

    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_GroupStatePtr = &InitializeParentState();
    m_GroupStatePtr->AddFrames(m_Frames);
 
    if(!ResolveUnknowns()){
      m_Log << LogWarning;
      m_Log << "Unable to resolve unknowns associated with ";
      m_Log << "Frames in this Group with available Jigsaws";
      m_Log << m_End;
      return SetBody(false);
    }

    m_Log << LogVerbose << "...Done" << m_End;
    SetBody(true);
    return SetMind(true);
  }

  bool Group::ResolveUnknowns(){
    m_States.Clear();
    m_StatesToResolve.Clear();
    m_States.Add(*m_GroupStatePtr);
    m_StatesToResolve.Add(*m_GroupStatePtr);

    while(m_StatesToResolve.GetN() > 0){
      State& state = m_StatesToResolve.Get(0);
      if(!ResolveState(state)){
	if(state.GetNFrames() != 1){
	  m_Log << LogWarning;
	  m_Log << "Cannot find Jigsaw to Resolve State for frames:";
	  m_Log << endl << "   " << Log(state.GetListFrames());
	  m_Log << m_End;
	  return false; 
	}
	m_StatesToResolve.Remove(state);
      }
    }
    return true;
  }

  bool Group::ResolveState(const State& state){
    Jigsaw* jigsawSolutionPtr = nullptr;

    int N = m_JigsawsToUse.GetN();
    for(int i = 0; i < N; i++){
      Jigsaw* jigsawPtr = &m_JigsawsToUse[i];
      if(jigsawPtr->CanResolve(state)){
	if(!jigsawSolutionPtr){
	  jigsawSolutionPtr = jigsawPtr;
	  continue;
	}
	if(jigsawPtr->GetNChildren() <= jigsawSolutionPtr->GetNChildren())
	  if((jigsawPtr->GetNChildren() < jigsawSolutionPtr->GetNChildren()) ||
	     (jigsawPtr->GetNDependancyStates() < jigsawSolutionPtr->GetNDependancyStates())) 
	    jigsawSolutionPtr = jigsawPtr;
      }
    }
    if(!jigsawSolutionPtr)
      return false;
    
    m_Log << LogVerbose;
    m_Log << "Found Jigsaw to resolve State:" << endl; 
    m_Log << " Frames:" << endl << "   ";
    m_Log << Log(state.GetListFrames()) << endl;
    m_Log << " Jigsaw:" << Log(jigsawSolutionPtr);
    m_Log << m_End;
    InitializeJigsaw(*jigsawSolutionPtr);
    m_JigsawsToUse.Remove(*jigsawSolutionPtr);
    return true;
  }

  void Group::InitializeJigsaw(Jigsaw& jigsaw){
    State& state = m_StatesToResolve[0];
    jigsaw.SetParentState(state);
    if(!jigsaw.InitializeTree()){
      m_Log << LogWarning;
      m_Log << "Unable to initialize Jigsaw:";
      m_Log << Log(jigsaw) << m_End;
    }
    m_States.Add(jigsaw.GetChildStates());
    m_StatesToResolve.Remove(state);
    m_StatesToResolve.Add(jigsaw.GetChildStates());
    m_Jigsaws.Add(jigsaw);
    return;
  }
 
  State& Group::GetChildState(const RestFrame& frame) const {
    if(!frame) return State::Empty();
    int Ns = m_States.GetN();
    for(int i = Ns-1; i >= 0; i--)
      if(m_States[i].IsFrame(frame))
	return m_States[i];
    SetMind(false);
    return State::Empty();
  }
  
  RFList<State> Group::GetChildStates(const RestFrames::RFList<RestFrame>& frames) const {
    // Find States that correspond to these frames, giving
    // preference to States that include more frames (less dependancies)
    RFList<State> states;
    int Ns = m_States.GetN();
    for(int i = 0; i < Ns; i++){
      RFList<RestFrame> iframes = m_States[i].GetListFrames();
      if(frames.Contains(iframes)){
	int Nsol = states.GetN();
	bool isnew = true;
	for(int j = 0; j < Nsol; j++){
	  // if new copy of existing frame list appears, discard old
	  RFList<RestFrame> jframes = states[j].GetListFrames();
	  if(iframes.Contains(jframes)){
	    states.Remove(states[j]);
	    break;
	  }
	  // if superset of existing frame list appears, discard new
	  if(jframes.Contains(iframes)){
	    isnew = false;
	    break;
	  }
	}
	if(isnew) states.Add(m_States[i]);
      }
    }

    RFList<RestFrame> match_frames;
    Ns = states.GetN();
    for(int i = 0; i < Ns; i++)
      match_frames.Add(states[i].GetListFrames());
    
    if(!(frames == match_frames)){
      m_Log << LogWarning;
      m_Log << "Unable to find States corresponding to Frames: " << endl;
      m_Log << Log(frames) << m_End;
      SetMind(false);
      return RFList<State>();
    }
    return states;
  }

  RestFrame const& Group::GetLabFrame() const {
    if(m_Frames.GetN() < 1)
      return RestFrame::Empty();
    else
      return m_Frames[0].GetLabFrame();
  }

}
