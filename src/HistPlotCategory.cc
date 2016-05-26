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

  int HistPlotCategory::m_class_key = 0;

  HistPlotCategory::HistPlotCategory() : RFBase() {}

  HistPlotCategory::HistPlotCategory(const std::string& name, 
				     const std::string& title)
    : RFBase(name, title, HistPlotCategory::m_class_key++) {}
  
  HistPlotCategory::~HistPlotCategory() {}

  RFList<const HistPlotCategory> 
  HistPlotCategory::operator+(const HistPlotCategory& cat) const {
    RFList<const HistPlotCategory> list;
    list += *this;
    list += cat;
    return list;
  }

  RFList<const HistPlotCategory> 
  HistPlotCategory::operator+(const RFList<const HistPlotCategory>& cats) const {
    RFList<const HistPlotCategory> list;
    list += *this;
    list += cats;
    return list;
  }

  HistPlotCategory::operator RFList<const HistPlotCategory>() const {
    RFList<const HistPlotCategory> list;
    list += *this;
    return list;
  }

  HistPlotCategory& HistPlotCategory::Empty(){
    return HistPlotCategory::m_Empty;
  }

  HistPlotCategory HistPlotCategory::m_Empty;

}
