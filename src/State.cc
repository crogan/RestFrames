/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   State.cc
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

#include "RestFrames/State.hh"
#include "RestFrames/VisibleState.hh"
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // State class
  ///////////////////////////////////////////////
  int State::m_class_key = 0;

  // constructor 
  State::State(const string& sname, const string& stitle)
    : RFBase(sname, stitle) 
  {
    Init();
  }

  State::State() : RFBase() {}

  State::~State() {}

  void State::Init(){
    SetKey(GenKey());
    m_ParentJigsawPtr = nullptr;
    m_ChildJigsawPtr = nullptr;
    m_Log.SetSource("State");
    m_Type = kVanillaState;
    m_P.SetPxPyPzE(0.,0.,0.,0.);
    m_Frames.Clear();
  }

  void State::Clear(){
    m_ParentJigsawPtr = nullptr;
    m_ChildJigsawPtr = nullptr;
    m_P.SetPxPyPzE(0.,0.,0.,0.);
    m_Frames.Clear();
    RFBase::Clear();
  }

  State& State::Empty(){
    return VisibleState::Empty();
  }

  int State::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  /// \brief Returns State (*StateType*) type 
  StateType State::GetType() const {
    return m_Type;
  }
    
  /// \brief Is this a VisibleState? (yes/no)
  bool State::IsVisibleState() const {
    return m_Type == kVisibleState;
  }
    
  /// \brief Is this an InvisibleState? (yes/no)
  bool State::IsInvisibleState() const {
    return m_Type == kInvisibleState;
  }

  /// \brief Is this a CombinatoricState? (yes/no)
  bool State::IsCombinatoricState() const {
    return m_Type == kCombinatoricState;
  }

  void State::AddFrames(const RFList<RestFrame>& frames){
    int N = frames.GetN();
    for(int i = 0; i < N; i++)
      AddFrame(frames[i]);
  }

  RFList<RestFrame> const& State::GetListFrames() const {
    return m_Frames;
  }

  int State::GetNFrames() const {
    return m_Frames.GetN();
  }

  bool State::IsFrame(const RestFrame& frame) const {
    if(!frame) return false;
    if(m_Frames.GetN() != 1) return false;
    return m_Frames[0] == frame;
  }

  bool State::IsFrames(const RFList<RestFrame>& frames) const {
    return m_Frames == frames;
  }

  Jigsaw const& State::GetParentJigsaw() const { 
    if(m_ParentJigsawPtr)
      return *m_ParentJigsawPtr;
    else 
      return Jigsaw::Empty();
  }
  
  Jigsaw const& State::GetChildJigsaw() const { 
    if(m_ChildJigsawPtr)
      return *m_ChildJigsawPtr;
    else
      return Jigsaw::Empty();
  }

  void State::Boost(const TVector3& B){
    m_P.Boost(B);
  }

  void State::SetFourVector(const TLorentzVector& V){
    m_P.SetVectM(V.Vect(),V.M());
  }

  TLorentzVector State::GetFourVector() const {
    TLorentzVector V;
    V.SetVectM(m_P.Vect(),max(0.,m_P.M()));
    return V;
  }

  void State::FillGroupJigsawDependancies(RFList<Jigsaw>& jigsaws) const {
    if(m_ParentJigsawPtr) m_ParentJigsawPtr->FillGroupJigsawDependancies(jigsaws);
  }

  void State::FillStateJigsawDependancies(RFList<Jigsaw>& jigsaws) const {
    if(m_ParentJigsawPtr) m_ParentJigsawPtr->FillStateJigsawDependancies(jigsaws);
  }

}
