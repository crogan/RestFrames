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
#include "RestFrames/RInvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleState class
  ///////////////////////////////////////////////

  InvisibleState::InvisibleState() : State(){
    Init();
  }

  InvisibleState::~InvisibleState(){
    
  }

  void InvisibleState::Init(){
    m_Type = SInvisible;
  }

  void InvisibleState::Clear(){
    State::Clear();
  }

  double InvisibleState::GetMinimumMass(){
    InvisibleJigsaw* jigsawPtr = dynamic_cast<InvisibleJigsaw*>(m_ChildJigsawPtr);
    if(jigsawPtr) return jigsawPtr->GetMinimumMass();
    if(GetNFrames() == 1){
      RInvisibleFrame* framePtr = dynamic_cast<RInvisibleFrame*>(&m_Frames.Get(0));
      if(framePtr) return framePtr->GetMinimumMass();
    }
    return 0.;
  }

  void InvisibleState::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws){
    InvisibleJigsaw* jigsawPtr = dynamic_cast<InvisibleJigsaw*>(m_ChildJigsawPtr);
    if(jigsawPtr) jigsawPtr->FillInvisibleMassJigsawDependancies(jigsaws);
  }

}
