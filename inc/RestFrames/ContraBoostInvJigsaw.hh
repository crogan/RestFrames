/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ContraBoostInvJigsaw.hh
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

#ifndef ContraBoostInvJigsaw_HH
#define ContraBoostInvJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

namespace RestFrames {

  class ContraBoostInvJigsaw : public InvisibleJigsaw {
  public:
    ContraBoostInvJigsaw(const std::string& sname, 
			 const std::string& stitle);
    ContraBoostInvJigsaw();
    ~ContraBoostInvJigsaw();

    virtual std::string GetLabel() const { return "Contra-boost Invariant"; }

    static ContraBoostInvJigsaw& Empty();

    virtual double GetMinimumMass() const;
    virtual bool AnalyzeEvent();

    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const;

  protected:
    void CalcCoef();
    double GetC1(){ return m_C1; }
    double GetC2(){ return m_C2; }

  private:
    double m_C1;
    double m_C2;

    static ContraBoostInvJigsaw m_Empty;
  };

}

#endif
