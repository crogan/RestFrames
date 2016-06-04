/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlotVar.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jul
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

#include "RestFrames/HistPlotVar.hh"

namespace RestFrames {

  int HistPlotVar::m_class_key = 0;

  HistPlotVar::HistPlotVar(const std::string& name, 
			   const std::string& title, 
			   double minval, double maxval,
			   const std::string& unit = "")
    : m_Name(name), m_Title(title), 
      m_Min(minval), m_Max(maxval), m_Unit(unit), m_Val(0.), m_Key(m_class_key++) {}
  
  HistPlotVar::~HistPlotVar() {}

  bool HistPlotVar::operator==(const HistPlotVar& var) const {
    return var.m_Key == m_Key;
  }

  void HistPlotVar::operator=(double val) const {
    m_Val = val;
  }

  void HistPlotVar::operator+=(double val) const {
    m_Val += val;
  }
  
  void HistPlotVar::operator-=(double val) const {
    m_Val -= val;
  }
  
  void HistPlotVar::operator*=(double val) const {
    m_Val *= val;
  }
  
  void HistPlotVar::operator/=(double val) const {
    m_Val /= val;
  }

  HistPlotVar::operator double() const {
    return m_Val;
  }

  double HistPlotVar::GetVal() const {
    return m_Val;
  }
  
  std::string HistPlotVar::GetName() const {
    return m_Name;
  }

  std::string HistPlotVar::GetTitle() const {
    return m_Title;
  }

  std::string HistPlotVar::GetUnit() const {
    return m_Unit;
  }
  
  double HistPlotVar::GetMin() const {
    return m_Min;
  }
  
  double HistPlotVar::GetMax() const {
    return m_Max;
  }

}
