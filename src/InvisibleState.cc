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

  InvisibleState::InvisibleState(const string& sname, const string& stitle)
    : State(sname, stitle)
  {
    Init();
  }

  InvisibleState::InvisibleState() : State() {}

  InvisibleState::~InvisibleState() {}

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

  double InvisibleState::GetMinimumMass() const {
    if(!GetChildJigsaw().IsEmpty())
      return static_cast<const InvisibleJigsaw&>(GetChildJigsaw()).GetMinimumMass();
    if(GetNFrames() == 1)
      return static_cast<InvisibleRecoFrame&>(m_Frames[0]).GetMinimumMass();
    else
      return 0.;
  }

  void InvisibleState::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const {
    static_cast<const InvisibleJigsaw&>(GetChildJigsaw()).FillInvisibleMassJigsawDependancies(jigsaws);
  }

  InvisibleState InvisibleState::m_Empty;
}
