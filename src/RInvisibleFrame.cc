/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RInvisibleFrame.hh
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

#include "RestFrames/RInvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RInvisibleFrame class
  ///////////////////////////////////////////////
  RInvisibleFrame::RInvisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle),
    InvisibleFrame(sname, stitle),
    RFrame(sname, stitle)
  {
    Init();
  }

  RInvisibleFrame::~RInvisibleFrame(){
    
  }

  void RInvisibleFrame::Init(){
    minMASS = 0.;
  }

}
