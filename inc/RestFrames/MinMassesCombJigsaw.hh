/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassesCombJigsaw.hh
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

#ifndef MinMassesCombJigsaw_HH
#define MinMassesCombJigsaw_HH

#include "RestFrames/CombinatoricJigsaw.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  // MinMassesCombJigsaw class
  ///////////////////////////////////////////////
  class MinMassesCombJigsaw : public CombinatoricJigsaw {
  public:
    MinMassesCombJigsaw(const std::string& sname, const std::string& stitle);
    MinMassesCombJigsaw();
    virtual ~MinMassesCombJigsaw();

    virtual void Clear();

    virtual std::string GetLabel() const { return "Minimize Masses"; }

    virtual bool AnalyzeEvent();

    static MinMassesCombJigsaw& Empty();

  protected:
    virtual double EvaluateMetric() const;

  private:
    static MinMassesCombJigsaw m_Empty;

  };

}

#endif
