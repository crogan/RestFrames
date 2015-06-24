/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   GLabFrame.hh
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

#ifndef GLabFrame_HH
#define GLabFrame_HH

#include "RestFrames/LabFrame.hh"
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GLabFrame class
  ///////////////////////////////////////////////
  class GLabFrame : public LabFrame, public GFrame {
  public:
    GLabFrame(const string& sname, const string& stitle);
    virtual ~GLabFrame();
  
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

    virtual void SetThreeVector(const TVector3& P);
    virtual void SetTransverseMomenta(double val);
    virtual void SetLongitudinalMomenta(double val);
    virtual void SetPhi(double val);
    
  protected:
    double m_PT;
    double m_PL;
    double m_Phi;
    double m_Theta;

    virtual void ResetFrame();
    virtual bool GenerateFrame();

    void ResetProductionAngles();

  private:
    void Init();
    
  };

}

#endif
