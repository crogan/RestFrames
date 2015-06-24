/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ContraBoostInvariantJigsaw.hh
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

#ifndef ContraBoostInvariantJigsaw_HH
#define ContraBoostInvariantJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ContraBoostInvariantJigsaw class
  ///////////////////////////////////////////////
  class ContraBoostInvariantJigsaw : public InvisibleJigsaw {
  public:
    //constructor and destructor
    ContraBoostInvariantJigsaw(const string& sname, const string& stitle);
    ~ContraBoostInvariantJigsaw();

    virtual double GetMinimumMass();
    virtual bool AnalyzeEvent();

    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsaws);

  protected:
    void CalcCoef();
    double GetC1(){ return m_C1; }
    double GetC2(){ return m_C2; }

  private:
    double m_C1;
    double m_C2;
    void Init();
  
  };

}

#endif
