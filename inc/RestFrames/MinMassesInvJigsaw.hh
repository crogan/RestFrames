/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassesInvJigsaw.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Feb
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

#ifndef MinMassesInvJigsaw_HH
#define MinMassesInvJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

namespace RestFrames {

  class MinMassesInvJigsaw : public InvisibleJigsaw {
  public:
    MinMassesInvJigsaw(const std::string& sname, 
		       const std::string& stitle,
		       int Npair);
    MinMassesInvJigsaw();
    ~MinMassesInvJigsaw();

    virtual std::string GetLabel() const { return "Min Masses Sq."; }

    static MinMassesInvJigsaw& Empty();

    virtual double GetMinimumMass() const;
    virtual bool AnalyzeEvent();

    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const;

  private:

    static MinMassesInvJigsaw m_Empty;
  };

}

#endif
