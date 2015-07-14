/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ResonanceGenFrame.hh
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

#ifndef ResonanceGenFrame_HH
#define ResonanceGenFrame_HH

#include "RestFrames/DecayGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ResonanceGenFrame class
  ///////////////////////////////////////////////
  class ResonanceGenFrame : public DecayGenFrame {
  public:
    ResonanceGenFrame(const string& sname, const string& stitle);
    ResonanceGenFrame();
    virtual ~ResonanceGenFrame();

    virtual void SetMass(double mass);
    virtual void SetEvtMass(double mass);
    virtual void SetWidth(double w);
    
    virtual void ResetFrame();

    virtual double GetMass() const;
    virtual double GetPoleMass() const;
    virtual double GetMinimumMass() const;
    virtual double GetWidth() const;
    virtual double GetProb(double mass) const;

  protected:
    double m_PoleMass;
    double m_Width;
    void Init();
    virtual double GenerateMass(double min_mass = 0.) const;

  };

  double GetProb(double Mp, double Mc1, double Mc2);
  int DoubleMax(const void *a, const void *b);

}

#endif
