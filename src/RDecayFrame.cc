/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RDecayFrame.cc
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

#include "RestFrames/RDecayFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RDecayFrame class
  // Class for an intermediate state
  // inheriting from DecayFrame and RFrame
  ///////////////////////////////////////////////
  RDecayFrame::RDecayFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle), 
    DecayFrame(sname,stitle),
    RFrame(sname, stitle)
  {
    Init();
  }
  
  RDecayFrame::~RDecayFrame(){ }

  void RDecayFrame::Init(){
    m_RType = RDVanilla;
  }

  bool RDecayFrame::IsSelfAssemblingFrame() const {
    return m_RType == RDSelfAssembling;
  }
}
