/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ReconstructionFrame.cc
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

#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ReconstructionFrame class methods
  ///////////////////////////////////////////////
  ReconstructionFrame::ReconstructionFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  ReconstructionFrame::ReconstructionFrame() : 
    RestFrame()
  {
    Init();
  }

  ReconstructionFrame::~ReconstructionFrame(){ }

  void ReconstructionFrame::Init(){
    m_Ana = FReco;
    m_GroupPtr = nullptr;
  }

  void ReconstructionFrame::Clear(){
    if(m_GroupPtr)
      m_GroupPtr->RemoveFrame(*this);
    m_ChildStates.clear();
    RestFrame::Clear();
  }
  
  void ReconstructionFrame::SetGroup(Group& group){
    if(group.IsEmpty()) return;

    if(m_GroupPtr)
      m_GroupPtr->RemoveFrame(*this);
    
    m_GroupPtr = &group;
  }

  Group& ReconstructionFrame::GetGroup() const { 
    if(m_GroupPtr)
      return *m_GroupPtr;
    else 
      return g_Group;
  }

  RFList<Group> ReconstructionFrame::GetListGroups() const {
    RFList<Group> groups;
    FillListGroupsRecursive(groups);
    return groups;
  }

  void ReconstructionFrame::FillListGroupsRecursive(RFList<Group>& groups) const {
    if(m_GroupPtr) groups.Add(*m_GroupPtr);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      ReconstructionFrame* childPtr = dynamic_cast<ReconstructionFrame*>(&GetChildFrame(i));
      if(childPtr) childPtr->FillListGroupsRecursive(groups);
    }
  }

  bool ReconstructionFrame::InitializeNoGroupStates(const RFList<State>& states){
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      RestFrame& child = GetChildFrame(i);
      m_ChildStates.push_back(RFList<State>());
      RFList<RestFrame> frames = child.GetListFrames(FVisible);
      int Nframe = frames.GetN();
      for(int f = 0; f < Nframe; f++){
	ReconstructionFrame* rframePtr = dynamic_cast<ReconstructionFrame*>(&frames.Get(f));
	if(!rframePtr) return false;
	if(!rframePtr->GetGroup().IsEmpty())
	  continue;
       
	int index = states.GetIndexFrame(frames.Get(f));
	if(index >= 0)  m_ChildStates[i].Add(states.Get(index));
      }
    }
    return true;
  }

  bool ReconstructionFrame::InitializeGroupStates(const RFList<Group>& groups){
    int Ngroup = groups.GetN();
    int Nchild = GetNChildren();
    
    vector<FrameType> terminal_types;
    terminal_types.push_back(FVisible);
    terminal_types.push_back(FInvisible);

    for(int c = 0; c < Nchild; c++){
      RFList<RestFrame> frames = GetChildFrame(c).GetListFrames(terminal_types);
      int Nframe = frames.GetN();
      for(int f = 0; f < Nframe; f++){
	RestFrame& frame = frames.Get(f);
	for(int g = 0; g < Ngroup; g++){
	  Group& group = groups.Get(g);
	  if(group.IsEmpty()) return false;
	  if(group.ContainsFrame(frame)){
	    State& state = group.GetState(frame);
	    if(state.IsEmpty()) return false;
	    m_ChildStates[c].Add(state);
	    break;
	  }
	}
      }
    }
    return true;
  }

  bool ReconstructionFrame::InitializeStates(const RFList<State>& states, 
				const RFList<Group>& groups){
    m_ChildStates.clear();
    m_Mind = false;
    if(!m_Body){
      cout << endl << "Initialize Analysis Failure --: ";
      cout << "UnSound frame " << m_Name.c_str() << " in tree" << endl;
      return false;
    }
    if(!InitializeNoGroupStates(states)) return false;
    if(!InitializeGroupStates(groups)) return false;
    
    m_Mind = true;
    return true;
  }
  
  bool ReconstructionFrame::InitializeStatesRecursive(const RFList<State>& states, 
					 const RFList<Group>& groups){
    if(!InitializeStates(states, groups)) return false;

    int Nchild = GetNChildren();
    bool child_mind = true;
    for(int i = 0; i < Nchild; i++){
      ReconstructionFrame *childPtr = dynamic_cast<ReconstructionFrame*>(&GetChildFrame(i));
      if(!childPtr) return false;
      if(!childPtr->InitializeStatesRecursive(states, groups)) child_mind = false;;
    }
    m_Mind = child_mind;
    return m_Mind;
  }

  void ReconstructionFrame::ClearEventRecursive(){ 
    SetSpirit(false);
   
    if(!IsSoundBody() || !IsSoundMind()) return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i).ClearEventRecursive();
  }

  bool ReconstructionFrame::AnalyzeEventRecursive(){
    if(!IsSoundMind()){
      m_Log << LogWarning;
      m_Log << "Unable to analyze event. ";
      m_Log << "Requires successfull call to \"InitializeAnalysis()\" ";
      m_Log << "from LabFrame" << m_End;
      SetSpirit(false);
      return false;
    }
    TLorentzVector Ptot(0,0,0,0);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      TLorentzVector P = m_ChildStates[i].GetFourVector();
      TVector3 B_child = P.BoostVector();
      SetChildBoostVector(i, B_child);
      Ptot += P;

      ReconstructionFrame* childPtr = dynamic_cast<ReconstructionFrame*>(&GetChildFrame(i));
      childPtr->SetFourVector(P,*this);
      if(!childPtr->IsVisibleFrame() && !childPtr->IsInvisibleFrame()){ 
	B_child *= -1.;
	m_ChildStates[i].Boost(B_child);
      }
      if(!childPtr->AnalyzeEventRecursive()){
	m_Log << LogWarning;
	m_Log << "Recursive event analysis failed for child frame: ";
	m_Log << Log(childPtr) << m_End;
	SetSpirit(false);
	return false;
      } 
      if(!childPtr->IsVisibleFrame() && !childPtr->IsInvisibleFrame()){ 
	B_child *= -1.;
	m_ChildStates[i].Boost(B_child);
      }
    }
    if(m_Type == FLab) SetFourVector(Ptot,*this);
    SetSpirit(true);
    return true;
  }

}
