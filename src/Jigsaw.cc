/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   Jigsaw.cc
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

#include "RestFrames/Jigsaw.hh"
#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/LabRecoFrame.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // Jigsaw class methods
  ///////////////////////////////////////////////
  int Jigsaw::m_class_key = 0;

  Jigsaw::Jigsaw(const string& sname, const string& stitle)
    : RFBase(sname, stitle, Jigsaw::m_class_key++) 
  {
    m_Log.SetSource("Jigsaw "+GetName());
    m_GroupPtr = nullptr;
    m_ParentStatePtr = nullptr;
  }

  Jigsaw::Jigsaw() : RFBase() { m_Type = kVanillaJigsaw; }

  Jigsaw::~Jigsaw() {}

  Jigsaw& Jigsaw::Empty(){
    return InvisibleJigsaw::Empty();
  }

  void Jigsaw::Clear(){
    SetGroup();
    m_GroupPtr = nullptr;
    m_ParentStatePtr = nullptr;
    m_ChildStates.Clear();
    m_ChildFrames.clear();
    m_DependancyStates.clear();
    m_DependancyFrames.clear();
    m_DependancyJigsaws.Clear();
    RFBase::Clear();
  }

  bool Jigsaw::IsInvisibleJigsaw() const {
    return m_Type == kInvisibleJigsaw;
  }

  bool Jigsaw::IsCombinatoricJigsaw() const {
    return m_Type == kCombinatoricJigsaw;
  }

  string Jigsaw::PrintString(LogType type) const {
    string output = RFBase::PrintString(type);
    if(IsInvisibleJigsaw())
      output += "   Type: Invisible \n";
    if(IsCombinatoricJigsaw())
      output += "   Type: Combinatoric \n";
    return output;
  }

  void Jigsaw::SetGroup(Group& group){
    SetBody(false);
    if(m_GroupPtr){
      if(*m_GroupPtr == group){
	return;
      } else {
	Group* groupPtr = m_GroupPtr;
	m_GroupPtr = nullptr;
	groupPtr->RemoveJigsaw(*this);
      }
    }
    if(!group)
      m_GroupPtr = nullptr;
    else
      m_GroupPtr = &group;
  }

  Group& Jigsaw::GetGroup() const {
    if(m_GroupPtr)
      return *m_GroupPtr;
    else 
      return Group::Empty();
  }

  bool Jigsaw::CanResolve(const RFList<RestFrame>& frames) const {
    return GetParentFrames() == frames;
  }

  bool Jigsaw::CanResolve(const State& state) const {
    if(!state) return false;
    return CanResolve(state.GetListFrames());
  }

  void Jigsaw::SetParentState(State& state){
    SetBody(false);
    if(m_ParentStatePtr){
      if(*m_ParentStatePtr == state)
	return;
      else {
	State* statePtr = m_ParentStatePtr;
	m_ParentStatePtr = nullptr;
	statePtr->SetChildJigsaw();
      } 
    }
    if(!state)
      m_ParentStatePtr = nullptr;
    else {
      m_ParentStatePtr = &state;
      state.SetChildJigsaw(*this);
    } 
  }

  State& Jigsaw::GetParentState() const {
    if(m_ParentStatePtr)
      return *m_ParentStatePtr;
    else
      return State::Empty();
  }

  int Jigsaw::GetNChildren() const {
    return m_ChildStates.GetN();
  }

  int Jigsaw::GetNDependancyStates() const {
    return m_DependancyFrames.size();
  }

  State& Jigsaw::GetChildState(int i) const {
    return m_ChildStates[i];
  }

  RFList<State> Jigsaw::GetChildStates() const {
    return m_ChildStates;
  }

  RFList<State> Jigsaw::GetDependancyStates(int i) const {
    if(i < 0 || i >= int(m_DependancyStates.size()))
      return RFList<State>();
    return m_DependancyStates[i];
  }

  RFList<RestFrame> Jigsaw::GetParentFrames() const {
    RFList<RestFrame> frames;
    int N = m_ChildFrames.size();
    for(int i = 0; i < N; i++)
      frames += GetChildFrames(i);
    return frames;
  }

  RFList<RestFrame> Jigsaw::GetChildFrames(int i) const {
    if(i < 0 || i >= int(m_ChildFrames.size())) 
      return RFList<RestFrame>();
    return m_ChildFrames[i];
  }

  RFList<RestFrame> Jigsaw::GetDependancyFrames(int i) const {
    if((i < 0) || (i >= int(m_DependancyFrames.size()))) 
      return RFList<RestFrame>();
    return m_DependancyFrames[i];
  }

  bool Jigsaw::InitializeTree(){
    if(!IsSoundBody())
      return SetMind(false);

    if(!GetParentState()){
      m_Log << LogWarning;
      m_Log << "Unable to initialize Jigsaw. ";
      m_Log << "No parent State set." << LogEnd;
      return SetMind(false);
    }

    if(!CanResolve(GetParentState())){
      m_Log << LogWarning;
      m_Log << "Unable to resolve input parent State. ";
      m_Log << "  Frames (capable): " << Log(GetParentFrames()) << endl;
      m_Log << "  Frames (requested): " << Log(GetParentState().GetListFrames());
      m_Log << LogEnd;
      return SetMind(false);
    }
      
    int N = m_ChildFrames.size();
    for(int i = 0; i < N; i++){
      State& new_state = GetNewChildState();
      new_state.AddFrames(m_ChildFrames[i]);
      new_state.SetParentJigsaw(*this);
      m_ChildStates += new_state;
    }
    return SetBody(true);
  }
  
  bool Jigsaw::InitializeAnalysis(){
    m_DependancyStates.clear();
    if(!IsSoundBody())
      return SetMind(false);

    if(!GetGroup())
      return SetMind(false);

    // get list of states and groups from lab frame
    const LabRecoFrame& lab_frame = 
      static_cast<const LabRecoFrame&>(GetGroup().GetLabFrame());
    RFList<State> states = lab_frame.GetTreeStates();
    RFList<Group> groups = lab_frame.GetListGroups();

    int Ngroup = groups.GetN();
    vector<RFList<RestFrame> > group_frames;
    for(int i = 0; i < Ngroup; i++)
      group_frames.push_back(RFList<RestFrame>());

    int Ndep = m_DependancyFrames.size();
    for(int d = 0; d < Ndep; d++){

      m_DependancyStates.push_back(RFList<State>());
      for(int i = 0; i < Ngroup; i++)
	group_frames[i].Clear();
     
      int Nf = m_DependancyFrames[d].GetN();
      for(int f = 0; f < Nf; f++){
	RestFrame& frame = m_DependancyFrames[d][f];

	bool no_group = true;
	for(int g = 0; g < Ngroup; g++){
	  if(groups[g].ContainsFrame(frame)){
	    group_frames[g].Add(frame);
	    no_group = false;
	    break;
	  }
	}

	if(no_group){
	  State& state = states.GetFrame(frame);
	  if(!state){
	    m_Log << LogWarning;
	    m_Log << "Cannot find States corresponding to frame: " << endl;
	    m_Log << Log(frame) << " " << Log(states) << LogEnd;
	    return SetMind(false);
	  }
	  m_Log << LogVerbose;
	  m_Log << "Adding dependancy State for index " << d;
	  m_Log << " corresponding to frame:";
	  m_Log << Log(frame) << LogEnd;
	  m_DependancyStates[d] += state;
	}
      }
      for(int g = 0; g < Ngroup; g++){
	if(group_frames[g].GetN() == 0) continue;
	RFList<State> group_states = groups[g].GetChildStates(group_frames[g]);
	if(group_states.GetN() == 0){
	  m_Log << "Cannot find States in Group:" << endl;
	  m_Log << " Frames:" << endl << "   ";
	  m_Log << Log(group_frames[g]) << endl;
	  m_Log << " Group:" << endl;
	  m_Log << Log(groups.Get(g)) << LogEnd;
	  return SetMind(false);
	}
	m_Log << LogVerbose;
	m_Log << "Sucessfully found dependancy States for index " << d << endl;
	int Ns = group_states.GetN();
	m_Log << " Frames:" << endl << "   ";
	m_Log << Log(group_frames[g]) << endl;
	m_Log << " States:" << endl;
	for(int s = 0; s < Ns; s++){
	  m_Log << "   state " << s << ": ";
	  m_Log << Log(group_states[s].GetListFrames()) << endl;
	}
	m_Log << LogEnd;
	m_DependancyStates[d].Add(group_states);
      }
    }
    
    return SetMind(true);
  } 

  bool Jigsaw::InitializeDependancyJigsaws(){
    if(!IsSoundMind()) 
      return false;

    m_DependancyJigsaws.Clear();

    RFList<Jigsaw> jigsaws;
    FillStateJigsawDependancies(jigsaws);
    jigsaws -= *this;
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillGroupJigsawDependancies(jigsaws);
    jigsaws -= *this;
    m_DependancyJigsaws.Add(jigsaws);

    return true;
  }

  void Jigsaw::AddChildFrame(RestFrame& frame, int i){
    SetBody(false);
    if(!frame) return;
    if(!GetGroup()) return;
    if(!GetGroup().ContainsFrame(frame)){
      m_Log << LogWarning;
      m_Log << "Unable to add child frame not in same Group:" << endl;
      m_Log << "    Frame:" << endl;
      m_Log << Log(frame) << endl;
      m_Log << "    Group:" << endl;
      m_Log << Log(GetGroup()) << LogEnd;
      return;
    }
    
    while(i >= int(m_ChildFrames.size())){
      m_ChildFrames.push_back(RFList<RestFrame>());
    }
    m_ChildFrames[i] += frame;
  }

  void Jigsaw::AddDependancyFrame(RestFrame& frame, int i){
    SetBody(false);
    if(!frame) return;
    
    while(i >= int(m_DependancyFrames.size())){
      m_DependancyFrames.push_back(RFList<RestFrame>());
    }
    m_DependancyFrames[i] += frame;
  }

  bool Jigsaw::IsSoundBody() const {
    if(RFBase::IsSoundBody()) 
      return true;
    int Nout = m_ChildFrames.size();
    for(int i = 0; i < Nout-1; i++)
      for(int j = i+1; j < Nout; j++)
	if(m_ChildFrames[i].Intersection(m_ChildFrames[j]).GetN() > 0){
	  m_Log << LogWarning;
	  m_Log << "Child frames are repeated between ";
	  m_Log << "more than one output index: ";
	  m_Log << Log(m_ChildFrames[i].Intersection(m_ChildFrames[j]));
	  m_Log << LogEnd;
	  return SetBody(false);
	}
    for(int i = 0; i < Nout; i++)
      if(m_ChildFrames[i].GetN() == 0){
	m_Log << "No child frames at index ";
	m_Log << i << LogEnd;
	return SetBody(false);
      }
    return SetBody(true);
  }

  bool Jigsaw::DependsOnJigsaw(const Jigsaw& jigsaw) const {
    return m_DependancyJigsaws.Contains(jigsaw);
  }

  void Jigsaw::FillGroupJigsawDependancies(RFList<Jigsaw>& jigsaws){
    if(jigsaws.Contains(*this)) return;
    jigsaws.Add(*this);
    if(m_ParentStatePtr) m_ParentStatePtr->FillGroupJigsawDependancies(jigsaws);
  }

  void Jigsaw::FillStateJigsawDependancies(RFList<Jigsaw>& jigsaws){
    if(jigsaws.Contains(*this)) return;
    jigsaws += *this;

    int N = m_DependancyStates.size();
    for(int i = 0; i < N; i++){
      int M = m_DependancyStates[i].GetN();
      for(int j = 0; j < M; j++){
	m_DependancyStates[i][j].FillStateJigsawDependancies(jigsaws);
      }
    } 
  }

}
