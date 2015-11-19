/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlotCategory.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Nov
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

#include "RestFrames/HistPlotCategory.hh"

namespace RestFrames {

  HistPlotCategory::HistPlotCategory(const string& name, const string& title)
    : m_Name(name), m_Title(title) {}
  
  HistPlotCategory::~HistPlotCategory() {}
  
  string HistPlotCategory::GetName() const {
    return m_Name;
  }

  string HistPlotCategory::GetTitle() const {
    return m_Title;
  }

}
