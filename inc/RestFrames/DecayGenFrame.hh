/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   DecayGenFrame.hh
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

#ifndef DecayGenFrame_HH
#define DecayGenFrame_HH

#include "RestFrames/DecayFrame.hh"
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayGenFrame class
  ///////////////////////////////////////////////
  class DecayGenFrame : public DecayFrame, public GeneratorFrame{
  public:
    DecayGenFrame(const string& sname, const string& stitle);
    virtual ~DecayGenFrame();

    virtual void SetMass(double val);
    virtual double GetMass() const;
    
    // For two-body decays
    virtual void SetChildMomentum(double val);
    virtual void SetChildGamma(double val);
    virtual void SetCosDecayAngle(double val);
    virtual void SetDeltaPhiDecayPlane(double val);

  protected:
    mutable double m_Mass;
    mutable bool m_MassSet;

    // For two-body decays
    double m_ChildP;
    double m_ChildGamma;
    double m_CosDecayAngle;
    double m_DeltaPhiDecayPlane;

    virtual bool IsSoundBody() const;

    virtual void ResetFrame();
    virtual bool GenerateFrame();

    void ResetDecayAngles();
    double GenerateTwoBodyRecursive(double M, const vector<double>& M_children, 
				    const TVector3& axis_par, const TVector3& axis_perp,
				    vector<TLorentzVector>& P_children);

  private:
    void Init();

  };

}

#endif
