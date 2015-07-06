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
#include "RestFrames/RFrame.hh"
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
    Init();
  }

  CombinatoricGroup::~CombinatoricGroup(){ }

  void CombinatoricGroup::Init(){
    m_Type = GCombinatoric;
  }

  void CombinatoricGroup::Clear(){
    ClearElements();
    m_NElementsForFrame.clear();
    m_NExclusiveElementsForFrame.clear(); 
    m_InitStates.Clear();
    Group::Clear();
  }

  void CombinatoricGroup::AddFrame(RestFrame& frame){
    if(frame.IsEmpty()) return;
    if(!frame.IsRFrame() || !frame.IsVisibleFrame()) return;
    RFrame *ptr = dynamic_cast<RFrame*>(&frame);
    if(ptr){
      ptr->SetGroup(*this);
      m_Frames.Add(frame);
      if(m_Frames.GetN() == int(m_NElementsForFrame.size())) return;
      m_NElementsForFrame.push_back(1.);
      m_NExclusiveElementsForFrame.push_back(false);
    }
  }
  
  void CombinatoricGroup::SetNElementsForFrame(const RestFrame& frame, int N, bool exclusive_N){
    int index = m_Frames.GetIndex(frame);
    if(index < 0) return;
    m_NElementsForFrame[index] = N;
    m_NExclusiveElementsForFrame[index] = exclusive_N;
  }

  void CombinatoricGroup::GetNElementsForFrame(const RestFrame& frame, int& N, 
					       bool& exclusive_N) const {
    int index = m_Frames.GetIndex(frame);
    if(index < 0){
      N = -1;
      exclusive_N = false;
      return;
    }
    N = m_NElementsForFrame[index];
    exclusive_N = m_NExclusiveElementsForFrame[index];
  }

  bool CombinatoricGroup::AddJigsaw(Jigsaw& jigsaw){
    SetBody(false);
    if(jigsaw.IsEmpty()) return false;
    if(!jigsaw.GetGroup().IsEmpty()){
      m_Log << LogWarning;
      m_Log << "Cannot add Jigsaw - already assigned to Group:";
      m_Log << endl << " Jigsaw:" << endl << Log(jigsaw);
      m_Log << endl << " Group:" << endl << Log(jigsaw.GetGroup());
      m_Log << m_End;
      return false;
    }
    if(!jigsaw.IsCombinatoricJigsaw()){
      m_Log << LogWarning;
      m_Log << "Cannot add non-Invisible Jigsaw:";
      m_Log << Log(jigsaw) << m_End;
      return false;
    }
    if(m_JigsawsToUse.Add(jigsaw)) jigsaw.SetGroup(*this);
    return true;
  }

  State& CombinatoricGroup::InitializeGroupState(){
    return *(new CombinatoricState());
  }

  void CombinatoricGroup::ClearElements(){
    m_StateElements.Clear();
  }

  void CombinatoricGroup::AddElement(State& state){
    m_StateElements.Add(state);
  }

  int CombinatoricGroup::GetNElements() const{
    return m_StateElements.GetN();
  }

  // Event analysis functions
  void CombinatoricGroup::ClearEvent(){
    ClearFourVectors();
  }
 
  bool CombinatoricGroup::AnalyzeEvent(){
    if(!IsSoundMind() || !m_GroupStatePtr){
      m_Log << LogWarning << "Unable to Analyze Event" << m_End;
      SetSpirit(false);
      return false;
    }
    
    CombinatoricState* group_statePtr = dynamic_cast<CombinatoricState*>(m_GroupStatePtr);
    if(!group_statePtr){
      m_Log << LogWarning << "Unable to get Group CombinatoricState" << m_End;
      SetSpirit(false);
      return false;
    }
    
    group_statePtr->ClearElements();
    group_statePtr->AddElement(m_StateElements);    

    SetSpirit(true);
    return true;
  }

  void CombinatoricGroup::ClearFourVectors(){
    ClearElements();
   }

  RFKey CombinatoricGroup::AddLabFrameFourVector(const TLorentzVector& V){
    State& state = GetNewState();
    
    TLorentzVector P = V;
    if(P.M() < 0.) P.SetVectM(V.Vect(),0.);
    state.SetFourVector(P);
    AddElement(state);
   
    return state.GetKey();
  }

  int CombinatoricGroup::GetNFourVectors() const{
    return GetNElements();
  }

  RestFrame const& CombinatoricGroup::GetFrame(const RFKey& key) const {
    int N = m_States.GetN();
    for(int i = N-1; i >= 0; i--){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(&m_States.Get(i));
      if(!statePtr) continue;
      if(statePtr->ContainsElement(key)){
	return statePtr->GetFrame();
      }
    }
    return g_RestFrame;
  }

  TLorentzVector CombinatoricGroup::GetLabFrameFourVector(const RFKey& key) const {
    TLorentzVector P(0.,0.,0.,0.);
    int N = m_States.GetN();
    for(int i = N-1; i >= 0; i--){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(&m_States.Get(i));
      if(!statePtr) continue;
      if(statePtr->ContainsElement(key))
	return statePtr->GetElement(key).GetFourVector();
      
    }
    return TLorentzVector(0.,0.,0.,0.);
  }

  int CombinatoricGroup::GetNElementsInFrame(const RestFrame& frame) const {
    if(!ContainsFrame(frame)) return -1;
    CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(&GetState(frame));
    if(!statePtr) return -1;
    return statePtr->GetNElements();
  }

  State& CombinatoricGroup::GetNewState(){
    if(GetNElements() < m_InitStates.GetN())
      return m_InitStates.Get(GetNElements());

    State* statePtr = new State();
    AddDependent(statePtr);
    m_InitStates.Add(*statePtr);
    return *statePtr;
  }

}
