/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleState.cc
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

#include "RestFrames/InvisibleState.hh"
#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/InvisibleRecoFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleState class
  ///////////////////////////////////////////////

  InvisibleState::InvisibleState() : State(){
    Init();
  }

  InvisibleState::InvisibleState(const RFKey& key) : State(key){
    Init();
  }

  InvisibleState::~InvisibleState(){
    
  }

  void InvisibleState::Init(){
    m_Type = kInvisibleState;
  }

  InvisibleState& InvisibleState::Empty(){
    return InvisibleState::m_Empty;
  }

  void InvisibleState::Clear(){
    State::Clear();
  }

  void InvisibleState::AddFrame(RestFrame& frame){
    if(!frame) return;
    if(frame.IsInvisibleFrame() &&
       frame.IsRecoFrame())
      m_Frames += frame;

  }

  void InvisibleState::SetParentJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(jigsaw.IsInvisibleJigsaw())
      m_ParentJigsawPtr = &jigsaw;
  }

  void InvisibleState::SetChildJigsaw(Jigsaw& jigsaw){
    if(!jigsaw) return;
    if(jigsaw.IsInvisibleJigsaw())
      m_ChildJigsawPtr = &jigsaw;
  }

  InvisibleJigsaw const& InvisibleState::GetParentJigsaw() const {
    if(m_ParentJigsawPtr)
      return static_cast<InvisibleJigsaw&>(*m_ParentJigsawPtr);
    else
      return InvisibleJigsaw::Empty();
  }

  InvisibleJigsaw const& InvisibleState::GetChildJigsaw() const {
    if(m_ChildJigsawPtr)
      return static_cast<InvisibleJigsaw&>(*m_ChildJigsawPtr);
    else
      return InvisibleJigsaw::Empty();
  }

  double InvisibleState::GetMinimumMass() const {
    if(!GetChildJigsaw().IsEmpty())
      return GetChildJigsaw().GetMinimumMass();
    
    if(GetNFrames() == 1)
      return static_cast<InvisibleRecoFrame&>(m_Frames[0]).GetMinimumMass();
    else
      return 0.;
  }

  void InvisibleState::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const {
    GetChildJigsaw().FillInvisibleMassJigsawDependancies(jigsaws);
  }

  InvisibleState InvisibleState::m_Empty;
}
