/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabGenFrame.hh
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

#ifndef LabGenFrame_HH
#define LabGenFrame_HH

#include "RestFrames/LabFrame.hh"
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabGenFrame class
  ///////////////////////////////////////////////
  class LabGenFrame : public LabFrame<GeneratorFrame> {
  public:
    LabGenFrame(const string& sname, const string& stitle);
    virtual ~LabGenFrame();
  
    virtual void Clear();

    virtual bool InitializeAnalysis();
    virtual bool ClearEvent();
    virtual bool AnalyzeEvent();

    virtual void SetThreeVector(const TVector3& P);
    virtual void SetTransverseMomenta(double val);
    virtual void SetLongitudinalMomenta(double val);
    virtual void SetPhi(double val);

    void SetN_MCMCBurnIn(int N);
    
  protected:
    double m_PT;
    double m_PL;
    double m_Phi;

    double m_MaxM;

    virtual bool InitializeGenAnalysis();

    virtual void ResetFrame();
    virtual bool GenerateFrame();

    double m_ChildMassMCMC;
    double m_ChildProbMCMC;

    virtual bool IterateMCMC();
    
    int m_NBurnInMCMC;

  private:
    void Init();
    
  };

}

#endif
