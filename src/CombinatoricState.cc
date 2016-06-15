/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricState.cc
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

#include "RestFrames/CombinatoricState.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/VisibleState.hh"
#include "RestFrames/RestFrame.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricState class
  ///////////////////////////////////////////////

  CombinatoricState::CombinatoricState(const std::string& sname, 
				       const std::string& stitle) 
    : State(sname, stitle)
  {
    m_Type = kCombinatoricState;
  }

  CombinatoricState::CombinatoricState() : State() {}

  CombinatoricState::~CombinatoricState() {}

  CombinatoricState& CombinatoricState::Empty(){
    return CombinatoricState::m_Empty;
  }

  void CombinatoricState::Clear(){
    ClearElements();
    State::Clear();
  }

  void CombinatoricState::AddFrame(const RestFrame& frame){
    if(IsEmpty()) return;
    
    if(!frame) return;
    if(frame.IsVisibleFrame() &&
       frame.IsRecoFrame())
      m_Frames += frame;

  }

  void CombinatoricState::SetParentJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(jigsaw.IsCombinatoricJigsaw())
      State::SetParentJigsaw(jigsaw);
  }

  void CombinatoricState::SetChildJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(jigsaw.IsCombinatoricJigsaw())
      State::SetChildJigsaw(jigsaw);
  }

  void CombinatoricState::ClearElements(){
    m_Elements.Clear();
  }

  void CombinatoricState::AddElement(VisibleState& state){
    if(IsEmpty()) return;
    m_Elements += state;
  }

  void CombinatoricState::AddElements(const VisibleStateList& states){
    if(IsEmpty()) return;
    m_Elements += states;
  }

  VisibleStateList const& CombinatoricState::GetElements() const {
    return m_Elements;
  }

  int CombinatoricState::GetNElements() const {
    return m_Elements.GetN();
  }

  bool CombinatoricState::ContainsElement(const RFKey& key) const {
    return m_Elements.Contains(key);
  }

  bool CombinatoricState::ContainsElement(const State& state) const {
    return m_Elements.Contains(state);
  }

  VisibleState const& CombinatoricState::GetElement(const RFKey& key) const {
    return m_Elements.Get(key);
  }

  void CombinatoricState::Boost(const TVector3& B){
    StateList(m_Elements).Boost(B);
    State::Boost(B);
  }

  TLorentzVector CombinatoricState::GetFourVector() const {
    return StateList(m_Elements).GetFourVector();
  }

  RFCharge CombinatoricState::GetCharge() const {
    return StateList(m_Elements).GetCharge();
  }

  CombinatoricState CombinatoricState::m_Empty;

}
