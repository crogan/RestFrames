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
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // Jigsaw class methods
  ///////////////////////////////////////////////
  int Jigsaw::m_class_key = 0;

  Jigsaw::Jigsaw(const string& sname, const string& stitle)
    : RFBase(sname, stitle) 
  {
    Init();
  }

  Jigsaw::~Jigsaw(){ }

  void Jigsaw::Init(){
    SetKey(GenKey());
    m_GroupPtr = nullptr;
    m_InputStatePtr = nullptr;
    m_Log.SetSource("Jigsaw "+GetName());
  }

  void Jigsaw::Clear(){
    m_OutputStates.Clear();
    m_DependancyStates.clear();
    m_DependancyJigsaws.Clear();
    RFBase::Clear();
  }

  int Jigsaw::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  int Jigsaw::GetNOutputStates() const {
    return m_OutputFrames.size();
  }

  int Jigsaw::GetNDependancyStates() const {
    return m_DependancyFrames.size();
  }

  bool Jigsaw::IsInvisibleJigsaw() const {
    return m_Type == JInvisible;
  }

  bool Jigsaw::IsCombinatoricJigsaw() const {
    return m_Type == JCombinatoric;
  }

  string Jigsaw::PrintString(LogType type) const {
    string output = RFBase::PrintString(type);
    if(IsInvisibleJigsaw())
      output += "   Type: Invisible \n";
    if(IsCombinatoricJigsaw())
      output += "   Type: Combinatoric \n";

    return output;
  }

  void Jigsaw::SetGroup(Group* groupPtr){
    m_GroupPtr = groupPtr;
  }

  Group* Jigsaw::GetGroup() const{
    return m_GroupPtr;
  }

  bool Jigsaw::CanSplit(const RFList<RestFrame>& frames){
    int NoF = m_OutputFrames.size();
    RFList<RestFrame> iframes;
    for(int i = 0; i < NoF; i++)
      iframes.Add(m_OutputFrames[i]);
    
    return iframes.IsSame(frames);
  }

  bool Jigsaw::CanSplit(const State* statePtr){
    if(!statePtr) return false;
    RFList<RestFrame> frames = statePtr->GetFrames();
    return CanSplit(frames);
  }

  State* Jigsaw::NewOutputState(){
    State* statePtr = new State();
    AddDependent(statePtr);
    m_OutputStates.Add(statePtr);
    return statePtr;
  }

  void Jigsaw::ClearOutputStates(){
    m_OutputStates.Clear();
  }

  StateList Jigsaw::InitializeOutputStates(State* statePtr){
    ClearOutputStates();

    if(!CanSplit(statePtr)) return StateList();

    m_InputStatePtr = statePtr;
    statePtr->SetChildJigsaw(this);

    int NoF = m_OutputFrames.size();
    for(int i = 0; i < NoF; i++){
      State* new_statePtr = NewOutputState();
      new_statePtr->AddFrame(m_OutputFrames[i]);
      new_statePtr->SetParentJigsaw(this);
    }
    return m_OutputStates.Copy();
  }

  bool Jigsaw::InitializeDependancyStates(const StateList& states, const RFList<Group>& groups){
    m_DependancyStates.clear();
    
    if(!IsSoundBody()){
      SetMind(false);
      return false;
    }

    int Ngroup = groups.GetN();
    vector<RFList<RestFrame> > group_frames;
    for(int i = 0; i < Ngroup; i++)
      group_frames.push_back(RFList<RestFrame>());

    int Ndep = m_DependancyFrames.size();
    for(int d = 0; d < Ndep; d++){

      m_DependancyStates.push_back(StateList());
      for(int i = 0; i < Ngroup; i++)
	group_frames[i].Clear();
     
      int Nf = m_DependancyFrames[d].GetN();
      for(int f = 0; f < Nf; f++){
	RestFrame* framePtr = m_DependancyFrames[d].Get(f);

	bool no_group = true;
	for(int g = 0; g < Ngroup; g++){
	  if(groups.Get(g)->ContainsFrame(framePtr)){
	    group_frames[g].Add(framePtr);
	    no_group = false;
	    break;
	  }
	}

	if(no_group){
	  int index = states.GetIndexFrame(framePtr);
	  if(index < 0){
	    m_Log << LogWarning;
	    m_Log << "Cannot find States corresponding to frame: " << endl;
	    m_Log << Log(framePtr) << m_End;
	    SetMind(false);
	    return false;
	  }
	  m_Log << LogVerbose;
	  m_Log << "Adding dependancy State for hemishpere " << d;
	  m_Log << " corresponding to frame:" << endl;
	  m_Log << Log(framePtr) << m_End;
	  m_DependancyStates[d].Add(states.Get(index));
	}
      }
      for(int g = 0; g < Ngroup; g++){
	if(group_frames[g].GetN() == 0) continue;
	StateList group_states = groups.Get(g)->GetStates(group_frames[g]);
	if(group_states.GetN() == 0){
	  m_Log << "Cannot find States in Group:" << endl;
	  m_Log << " Frames:" << endl << "   ";
	  m_Log << Log(group_frames[g]) << endl;
	  m_Log << " Group:" << endl;
	  m_Log << Log(groups.Get(g)) << m_End;
	  SetMind(false);
	  return false;
	}
	m_Log << LogVerbose;
	m_Log << "Adding dependancy States for hemishpere " << d << endl;
	int Nf = group_frames[g].GetN();
	int Ns = group_states.GetN();
	m_Log << " Frames:" << endl << "   ";
	m_Log << Log(group_frames[g]) << endl;
	m_Log << " States:" << endl;
	for(int s = 0; s < Ns; s++){
	  m_Log << "   state " << s << ": " << Log(group_states.Get(s)->GetFrames()) << endl;
	}
	m_Log << m_End;
	m_DependancyStates[d].Add(group_states);
      }
    }
    
    SetMind(true);
    return true;
  } 

  bool Jigsaw::InitializeDependancyJigsaws(){
    if(!IsSoundMind()) return false;

    m_DependancyJigsaws.Clear();

    RFList<Jigsaw> jigsaws;
    FillStateJigsawDependancies(jigsaws);
    jigsaws.Remove(this);
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillGroupJigsawDependancies(jigsaws);
    jigsaws.Remove(this);
    m_DependancyJigsaws.Add(jigsaws);

    return true;
  }

  bool Jigsaw::DependsOnJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return false;
    return m_DependancyJigsaws.Contains(jigsawPtr);
  }

  void Jigsaw::FillGroupJigsawDependancies(RFList<Jigsaw>& jigsaws){
    if(jigsaws.Contains(this)) return;
    jigsaws.Add(this);
    if(m_InputStatePtr) m_InputStatePtr->FillGroupJigsawDependancies(jigsaws);
  }

  void Jigsaw::FillStateJigsawDependancies(RFList<Jigsaw>& jigsaws){
    if(jigsaws.Contains(this)) return;
    jigsaws.Add(this);

    int N = m_DependancyStates.size();
    for(int i = 0; i < N; i++){
      int M = m_DependancyStates[i].GetN();
      for(int j = 0; j < M; j++){
	m_DependancyStates[i].Get(j)->FillStateJigsawDependancies(jigsaws);
      }
    } 
  }

  void Jigsaw::AddOutputFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    if(!m_GroupPtr) return;
    if(!m_GroupPtr->ContainsFrame(framePtr)){
      m_Log << LogWarning;
      m_Log << "Unable to add output frame not in same Group:" << endl;
      m_Log << "    Frame:" << endl;
      m_Log << Log(framePtr) << endl;
      m_Log << "    Group:" << endl;
      m_Log << Log(m_GroupPtr) << m_End;
      return;
    }

    while(i >= int(m_OutputFrames.size())){
      m_OutputFrames.push_back(RFList<RestFrame>());
    }
    m_OutputFrames[i].Add(framePtr);
  }

  void Jigsaw::AddOutputFrame(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++) AddOutputFrame(frames.Get(f), i);
  }

  void Jigsaw::AddDependancyFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    
    while(i >= int(m_DependancyFrames.size())){
      m_DependancyFrames.push_back(RFList<RestFrame>());
    }
    m_DependancyFrames[i].Add(framePtr);
  }

  void Jigsaw::AddDependancyFrame(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++) AddDependancyFrame(frames.Get(f), i);
  }

  bool Jigsaw::IsSoundBody() const {
    int Nout = m_OutputFrames.size();
    for(int i = 0; i < Nout-1; i++)
      for(int j = i+1; j < Nout; j++)
	if(m_OutputFrames[i].SizeIntersection(m_OutputFrames[j]) > 0){
	  SetBody(false);
	  return false;
	}
    for(int i = 0; i < Nout; i++)
      if(m_OutputFrames[i].GetN() == 0){
	SetBody(false);
	return false;
      }
    SetBody(true);
    return true;
  }


  int Jigsaw::GetNChildStates() const {
    return m_OutputFrames.size();
  }
  State* Jigsaw::GetChildState(int i) const {
    if(i < 0 || i >= GetNChildStates()) return nullptr;
    return m_OutputStates.Get(i);
  }

  RFList<RestFrame> Jigsaw::GetChildFrames(int i) const {
    if(i < 0 || i >= GetNChildStates()) RFList<RestFrame>();
    RFList<RestFrame> frames = m_OutputFrames[i].Copy();
    if(i < int(m_DependancyFrames.size())) frames.Add(m_DependancyFrames[i]);
    return frames;
  }
  
  RFList<RestFrame> Jigsaw::GetChildFrames() const {
    RFList<RestFrame> child_frames;

    int N = GetNChildStates();
    for(int i = 0; i < N; i++)
      child_frames.Add(GetChildFrames(i));
      
    return child_frames;
  }

}
