/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleGroup.cc
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

#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/InvisibleState.hh"
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleGroup class
  ///////////////////////////////////////////////

  InvisibleGroup::InvisibleGroup(const string& sname, const string& stitle) : 
    Group(sname, stitle)
  {
    Init();
  }

  InvisibleGroup::InvisibleGroup() : Group() {}
 

  InvisibleGroup::~InvisibleGroup() {}

  void InvisibleGroup::Init(){
    m_Type = kInvisibleGroup;
  }

  InvisibleGroup& InvisibleGroup::Empty(){
    return InvisibleGroup::m_Empty;
  }

  void InvisibleGroup::Clear(){
    Group::Clear();
  }

  void InvisibleGroup::AddFrame(RestFrame& frame){
    if(!frame) return;
    if(!frame.IsInvisibleFrame()) return;
    if(!frame.IsRecoFrame()) return;
    Group::AddFrame(frame);
  }

  void InvisibleGroup::AddJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(!jigsaw.IsInvisibleJigsaw()) return;
    Group::AddJigsaw(jigsaw);
  }

  InvisibleState& InvisibleGroup::InitializeParentState(){
    return *(new InvisibleState());
  }

  InvisibleState& InvisibleGroup::GetParentState() const {
    if(m_GroupStatePtr)
      return static_cast<InvisibleState&>(*m_GroupStatePtr);
    else
      return InvisibleState::Empty();
  }

  InvisibleState& InvisibleGroup::GetChildState(int i) const {
    if(!m_States[i])
      return InvisibleState::Empty();
    else
      return static_cast<InvisibleState&>(m_States[i]);
  }

  bool InvisibleGroup::ClearEvent(){
    if(!IsSoundMind()) 
      return SetSpirit(false);
    m_Lab_P.SetPxPyPzE(0.,0.,0.,0.);
    return true;
  }
  
  void InvisibleGroup::SetLabFrameFourVector(const TLorentzVector& V){
    m_Lab_P.SetVectM(V.Vect(),V.M());
  }

  void InvisibleGroup::SetLabFrameThreeVector(const TVector3& V){
    m_Lab_P.SetVectM(V,0.0);
  }

  TLorentzVector InvisibleGroup::GetLabFrameFourVector() const {
    return m_Lab_P;
  }

  bool InvisibleGroup::AnalyzeEvent(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    m_GroupStatePtr->SetFourVector(m_Lab_P);
    return SetSpirit(true);;
  }
  
  InvisibleGroup InvisibleGroup::m_Empty;

}
