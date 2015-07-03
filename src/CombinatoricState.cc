/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricState class
  ///////////////////////////////////////////////

  CombinatoricState::CombinatoricState() : 
    State()
  {
    Init();
  }

  CombinatoricState::~CombinatoricState(){
    ClearElements();
  }

  void CombinatoricState::Init(){
    m_Type = SCombinatoric;
  }

  void CombinatoricState::Clear(){
    ClearElements();
    State::Clear();
  }

  void CombinatoricState::ClearElements(){
    m_Elements.Clear();
  }

  void CombinatoricState::AddElement(State* statePtr){
    if(!statePtr) return;
    //m_Elements.Add(statePtr->Copy());
    m_Elements.Add(statePtr);
  }

  void CombinatoricState::AddElement(const StateList& states){
    int N = states.GetN();
    for(int i = 0; i < N; i++) AddElement(states.Get(i));
  }

  StateList CombinatoricState::GetElements() const {
    return m_Elements.Copy();
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

  const State* CombinatoricState::GetElement(const RFKey& key) const {
    return m_Elements.Get(key);
  }

  void CombinatoricState::Boost(const TVector3& B){
    m_Elements.Boost(B);
    m_P.Boost(B);
  }

  TLorentzVector CombinatoricState::GetFourVector() const {
    if(GetNElements() > 0) return m_Elements.GetFourVector();
    TLorentzVector V(0.,0.,0.,0.);
    return V;
  }

}
