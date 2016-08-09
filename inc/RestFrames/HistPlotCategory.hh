/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlotCategory.hh
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

#ifndef HistPlotCategory_HH
#define HistPlotCategory_HH

#include "RestFrames/RFBase.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  // HistPlotCategory class
  ///////////////////////////////////////////////
  class HistPlotCategory : public RFBase {

  public:
    HistPlotCategory(const std::string& name, const std::string& title);
    HistPlotCategory();
    ~HistPlotCategory();

    /// \brief Combines HistPlotCategory into list
    ///
    /// \param cat    additional category to add in list
    ///
    /// Returns a list of HistPlotCategory containing __cat__ and this
    RestFrames::RFList<const HistPlotCategory> 
    operator + (const HistPlotCategory& cat) const; 

    /// \brief Combines HistPlotCategory into list
    ///
    /// \param cats    list of additional categories to add in list
    ///
    /// Returns a list of RestFrame s containing __cats__ and this
    RestFrames::RFList<const HistPlotCategory> 
    operator + (const RestFrames::RFList<const HistPlotCategory>& cats) const; 

    /// \brief Casts HistPlotCategory into list
    operator RestFrames::RFList<const RestFrames::HistPlotCategory>() const;

    static HistPlotCategory& Empty();

  private:
    /// \brief HistPlotCategory ID key
    static int m_class_key;

    static HistPlotCategory m_Empty;

  };

}

#endif
