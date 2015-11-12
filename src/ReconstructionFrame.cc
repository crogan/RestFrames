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
#include "RestFrames/VisibleRecoFrame.hh"
#include "RestFrames/LabRecoFrame.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/VisibleState.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ReconstructionFrame class methods
  ///////////////////////////////////////////////
  ReconstructionFrame::ReconstructionFrame(const string& sname, const string& stitle)
    : RestFrame(sname, stitle)
  {
    Init();
  }

  ReconstructionFrame::ReconstructionFrame()
    : RestFrame() {}

  ReconstructionFrame::~ReconstructionFrame() {}

  void ReconstructionFrame::Init(){
    m_Ana = kRecoFrame;
    m_GroupPtr = nullptr;
  }

  void ReconstructionFrame::Clear(){
    SetGroup();
    m_ChildStates.clear();
    RestFrame::Clear();
  }

  /// \brief Returns empty instance of class
  ReconstructionFrame& ReconstructionFrame::Empty(){
    return VisibleRecoFrame::Empty();
  }
  
  void ReconstructionFrame::AddChildFrame(RestFrame& frame){
    if(!frame) return;
    if(!frame.IsRecoFrame()) return;
    RestFrame::AddChildFrame(frame);
  }

  void ReconstructionFrame::SetParentFrame(RestFrame& frame){
    if(!frame) return;
    if(!frame.IsRecoFrame()) return;
    RestFrame::SetParentFrame(frame);
  }

  ReconstructionFrame const& ReconstructionFrame::GetParentFrame() const {
    const RestFrame& frame = RestFrame::GetParentFrame();
    if(!frame.IsEmpty())
      return static_cast<const ReconstructionFrame&>(frame);
    else 
      return ReconstructionFrame::Empty();
  }

  ReconstructionFrame& ReconstructionFrame::GetChildFrame(int i) const {
    RestFrame& frame = RestFrame::GetChildFrame(i);
    if(!frame.IsEmpty())
      return static_cast<ReconstructionFrame&>(frame);
    else 
      return ReconstructionFrame::Empty();
  }

  void ReconstructionFrame::SetGroup(Group& group){
    if(m_GroupPtr)
      if(*m_GroupPtr != group){
	Group* groupPtr = m_GroupPtr;
	m_GroupPtr = nullptr;
	groupPtr->RemoveFrame(*this);
      }
    if(!group)
      m_GroupPtr = nullptr;
    else
      m_GroupPtr = &group;
  }

  Group& ReconstructionFrame::GetGroup() const { 
    if(m_GroupPtr)
      return *m_GroupPtr;
    else 
      return Group::Empty();
  }

  RFList<Group> ReconstructionFrame::GetListGroups() const {
    RFList<Group> groups;
    FillListGroupsRecursive(groups);
    return groups;
  }

  void ReconstructionFrame::FillListGroupsRecursive(RFList<Group>& groups) const {
    if(m_GroupPtr) groups.Add(*m_GroupPtr);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i).FillListGroupsRecursive(groups);
  }

  bool ReconstructionFrame::InitializeVisibleStates(){
    m_ChildStates.clear();
    if(!GetLabFrame())
      return false;
      
    const RFList<VisibleState>& states = 
      static_cast<const LabRecoFrame&>(GetLabFrame()).GetTreeStates();
   
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      RestFrame& child = GetChildFrame(i);
      m_ChildStates[&child] = RFList<State>();
      RFList<ReconstructionFrame> frames = child.GetListVisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++)
	if(!frames[f].GetGroup())
	  if(!m_ChildStates[&child].Add(RFList<State>(states).GetFrame(frames[f]))){
	    m_Log << LogWarning;
	    m_Log << "Unable to associate State with Group-less Frame:";
	    m_Log << Log(frames[f]) << m_End;
	    return false;
	  }
    }
    return true;
  }

  bool ReconstructionFrame::InitializeGroupStates(){
    RFList<Group> groups = GetListGroups();
    int Ngroup = groups.GetN();
    int Nchild = GetNChildren();

    for(int c = 0; c < Nchild; c++){
      RFList<RestFrame> frames = 
	GetChildFrame(c).GetListVisibleFrames() +
	GetChildFrame(c).GetListInvisibleFrames();
      int Nframe = frames.GetN();
      for(int f = 0; f < Nframe; f++)
	for(int g = 0; g < Ngroup; g++)
	  if(groups[g].ContainsFrame(frames[f])){
	    State& state = groups[g].GetChildState(frames[f]);
	    if(!state){
	      m_Log << LogWarning;
	      m_Log << "Unable to get State associated with Group Frame: " << endl;
	      m_Log << " Frame:" << Log(frames[f]);
	      m_Log << " Group:" << Log(groups[g]) << m_End;
	      return false;
	    }
	    m_ChildStates[&GetChildFrame(c)].Add(state);
	    break;
	  }
    }  
    
    return true;
  }
  
  bool ReconstructionFrame::InitializeAnalysisRecursive(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    }

    if(!InitializeVisibleStates())
      return SetMind(false);
    if(!InitializeGroupStates())
      return SetMind(false);
   
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i).InitializeAnalysisRecursive()){
	m_Log << LogWarning;
	m_Log << "Unable to recursively initialize analysis for frame:";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return SetMind(false);
      }

    return SetMind(true);
  }

  bool ReconstructionFrame::ClearEventRecursive(){ 
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return false;
    }
   
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      if(!GetChildFrame(i).ClearEventRecursive())
	return false;

    return true;
  }

  bool ReconstructionFrame::AnalyzeEventRecursive(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    
    TLorentzVector Ptot(0,0,0,0);

    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      ReconstructionFrame& child = GetChildFrame(i);
      TLorentzVector P = m_ChildStates[&child].GetFourVector();
      TVector3 B_child = P.BoostVector();
      SetChildBoostVector(child, B_child);
      Ptot += P;

      child.SetFourVector(P,*this);
      bool terminal = child.IsVisibleFrame() || child.IsInvisibleFrame();

      if(!terminal){ 
	B_child *= -1.;
	m_ChildStates[&child].Boost(B_child);
      }
    
      if(!child.AnalyzeEventRecursive()){
	m_Log << LogWarning;
	m_Log << "Recursive event analysis failed for frame: ";
	m_Log << Log(child) << m_End;
	return SetSpirit(false);
      } 
     
      if(!terminal){ 
	B_child *= -1.;
	m_ChildStates[&child].Boost(B_child);
      }
    }
   
    if(IsLabFrame()) SetFourVector(Ptot,*this);

    return SetSpirit(true);
  }

}
