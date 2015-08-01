/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricGroup.cc
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

#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/CombinatoricState.hh"
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricGroup class
  // a combinatoric collection of particles
  ///////////////////////////////////////////////

  CombinatoricGroup::CombinatoricGroup(const string& sname,const string& stitle) : 
    Group(sname, stitle)
  {
    m_Type = kCombinatoricGroup;
  }

  CombinatoricGroup::CombinatoricGroup() : Group() {}

  CombinatoricGroup::~CombinatoricGroup() {}

  CombinatoricGroup& CombinatoricGroup::Empty(){
    return CombinatoricGroup::m_Empty;
  }

  void CombinatoricGroup::Clear(){
    m_Elements.Clear();
    m_NElementsForFrame.clear();
    m_NExclusiveElementsForFrame.clear(); 
    m_InitStates.Clear();
    Group::Clear();
  }

  void CombinatoricGroup::AddFrame(RestFrame& frame){
    if(!frame) return;
    if(!frame.IsVisibleFrame()) return;
    if(!frame.IsRecoFrame()) return;
    int N = GetNFrames();
    Group::AddFrame(frame);
    if(GetNFrames() == N) 
      return;
    m_NElementsForFrame[&frame] = 1;
    m_NExclusiveElementsForFrame[&frame] = true;
  }
  
  void CombinatoricGroup::SetNElementsForFrame(const RestFrame& frame, 
					       int N, bool exclusive_N){
    if(!ContainsFrame(frame)) return;
    m_NElementsForFrame[&frame] = max(0, N);
    m_NExclusiveElementsForFrame[&frame] = exclusive_N;
  }

  void CombinatoricGroup::GetNElementsForFrame(const RestFrame& frame, int& N, 
					       bool& exclusive_N) const {
    if(!ContainsFrame(frame)) return;
    N = m_NElementsForFrame[&frame];
    exclusive_N = m_NExclusiveElementsForFrame[&frame];
  }

  void CombinatoricGroup::AddJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(!jigsaw.IsCombinatoricJigsaw()) return;
    Group::AddJigsaw(jigsaw);
  }

  CombinatoricState& CombinatoricGroup::InitializeParentState(){
    string name = GetName()+"_parent";
    return *(new CombinatoricState(name, name));
  }

  CombinatoricState& CombinatoricGroup::GetParentState() const {
    if(m_GroupStatePtr)
      return static_cast<CombinatoricState&>(*m_GroupStatePtr);
    else
      return CombinatoricState::Empty();
  }

  CombinatoricState& CombinatoricGroup::GetChildState(int i) const {
    if(!Group::GetChildState(i))
      return CombinatoricState::Empty();
    else
      return static_cast<CombinatoricState&>(Group::GetChildState(i));
  }

  // Event analysis functions
  bool CombinatoricGroup::ClearEvent(){
    m_Elements.Clear();
    return true;
  }
 
  bool CombinatoricGroup::AnalyzeEvent(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    
    GetParentState().ClearElements();
    GetParentState().AddElements(m_Elements);    

    return SetSpirit(true);
  }

  RFKey CombinatoricGroup::AddLabFrameFourVector(const TLorentzVector& V){
    VisibleState& state = GetNewElement();
    
    TLorentzVector P = V;
    if(P.M() < 0.) P.SetVectM(V.Vect(),0.);
    state.SetFourVector(P);
    m_Elements.Add(state);
   
    return state.GetKey();
  }

  RestFrame const& CombinatoricGroup::GetFrame(const RFKey& key) const {
    int N = GetNChildStates();
    for(int i = N-1; i >= 0; i--)
      if(GetChildState(i).ContainsElement(key))
	return GetChildState(i).GetListFrames()[0];
    return RestFrame::Empty();
  }

  TLorentzVector CombinatoricGroup::GetLabFrameFourVector(const RFKey& key) const {
    TLorentzVector P(0.,0.,0.,0.);
    int N = GetNChildStates();
    for(int i = N-1; i >= 0; i--)
      if(GetChildState(i).ContainsElement(key))
	return GetChildState(i).GetElement(key).GetFourVector();
    return TLorentzVector(0.,0.,0.,0.);
  }

  int CombinatoricGroup::GetNElementsInFrame(const RestFrame& frame) const {
    if(!ContainsFrame(frame)) return -1;
    return static_cast<CombinatoricState&>(Group::GetChildState(frame)).GetNElements();
  }

  VisibleState& CombinatoricGroup::GetNewElement(){
    if(m_Elements.GetN() < m_InitStates.GetN())
      return m_InitStates[m_Elements.GetN()];
    string name = GetName()+"_"+std::to_string(m_Elements.GetN()+1);
    VisibleState* statePtr = new VisibleState(name,name);
    AddDependent(statePtr);
    m_InitStates.Add(*statePtr);
    return *statePtr;
  }

  CombinatoricGroup CombinatoricGroup::m_Empty;
  
}
