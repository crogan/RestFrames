/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFrame.cc
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

#include "RestFrames/RFrame.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFrame class methods
  ///////////////////////////////////////////////
  RFrame::RFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  RFrame::~RFrame(){
   RemoveChildStates();
  }

  void RFrame::Init(){
    m_Ana = FReco;
    m_GroupPtr = nullptr;
  }

  void RFrame::RemoveChildStates(){
    m_ChildStates.clear();
  }
  
  void RFrame::SetGroup(Group* groupPtr){
    if(m_GroupPtr){
      m_GroupPtr->RemoveFrame(this);
    }
    m_GroupPtr = groupPtr;
  }

  RFList<Group> RFrame::GetListGroups() const {
    RFList<Group> groups;
    FillListGroupsRecursive(groups);
    return groups;
  }

  void RFrame::FillListGroupsRecursive(RFList<Group>& groups) const {
    groups.Add(m_GroupPtr);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      RFrame* childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      if(childPtr) childPtr->FillListGroupsRecursive(groups);
    }
  }

  bool RFrame::InitializeNoGroupStates(const StateList& states){
    int Nchild = GetNChildren();
  
    for(int i = 0; i < Nchild; i++){
      RestFrame* childPtr = GetChildFrame(i);
      if(!childPtr) return false;

      m_ChildStates.push_back(StateList());
    
      RFList<RestFrame> frames = childPtr->GetListFramesType(FVisible);
      int Nframe = frames.GetN();
      for(int f = 0; f < Nframe; f++){
	RFrame* rframePtr = dynamic_cast<RFrame*>(frames.Get(f));
	if(!rframePtr) return false;
	if(rframePtr->GetGroup()){
	  continue;
	}
	int index = states.GetIndexFrame(frames.Get(f));
	if(index >= 0)  m_ChildStates[i].Add(states.Get(index));
      }
    }
    return true;
  }

  bool RFrame::InitializeGroupStates(const RFList<Group>& groups){
    int Ngroup = groups.GetN();
    int Nchild = GetNChildren();
    
    vector<FrameType> terminal_types;
    terminal_types.push_back(FVisible);
    terminal_types.push_back(FInvisible);

    for(int c = 0; c < Nchild; c++){
      RFList<RestFrame> frames = GetChildFrame(c)->GetListFramesType(terminal_types);
      int Nframe = frames.GetN();
      for(int f = 0; f < Nframe; f++){
	RestFrame* framePtr = frames.Get(f);
	if(!framePtr) return false;
	for(int g = 0; g < Ngroup; g++){
	  Group* groupPtr = groups.Get(g);
	  if(!groupPtr) return false;
	  if(groupPtr->ContainsFrame(framePtr)){
	    State* statePtr = groupPtr->GetState(framePtr);
	    if(!statePtr) return false;
	    m_ChildStates[c].Add(statePtr);
	    break;
	  }
	}
      }
    }
    return true;
  }

  bool RFrame::InitializeStates(const StateList& states, const RFList<Group>& groups){
    RemoveChildStates();
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
  
  bool RFrame::InitializeStatesRecursive(const StateList& states, const RFList<Group>& groups){
    if(!InitializeStates(states, groups)) return false;

    int Nchild = GetNChildren();
    bool child_mind = true;
    for(int i = 0; i < Nchild; i++){
      RFrame *childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      if(!childPtr) return false;
      if(!childPtr->InitializeStatesRecursive(states, groups)) child_mind = false;;
    }
    m_Mind = child_mind;
    return m_Mind;
  }

  void RFrame::ClearEventRecursive(){ 
    SetSpirit(false);
   
    if(!IsSoundBody() || !IsSoundMind()) return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i)->ClearEventRecursive();
  }

  bool RFrame::AnalyzeEventRecursive(){
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

      RFrame *childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      childPtr->SetFourVector(P,this);
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
    if(m_Type == FLab) SetFourVector(Ptot,this);
    SetSpirit(true);
    return true;
  }

}
