/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ppLabGenFrame.hh
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

#ifndef ppLabGenFrame_HH
#define ppLabGenFrame_HH

#include "RestFrames/LabFrame.hh"
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ppLabGenFrame class
  ///////////////////////////////////////////////
  class ppLabGenFrame : public LabGenFrame {
  public:
    ppLabGenFrame(const string& sname, const string& stitle);
    virtual ~ppLabGenFrame();
  
    virtual void Clear();

    void SetEnergyP1(double E);
    void SetEnergyP2(double E);
    
    double GetEnergyP1() const;
    double GetEnergyP2() const;

    double SetPDFqqbar();
    double SetPDFgg();
    double SetPDFgq();
    double SetPDFqq();

  protected:

    virtual bool InitializeGenAnalysis();
    virtual bool IterateMCMC();

    virtual double pPDF_q(double x);
    virtual double pPDF_qbar(double x);
    virtual double pPDF_g(double x);

    bool m_PDFqqbar;
    bool m_PDFgg;
    bool m_PDFgq;
    bool m_PDFqq;

    // support function
    vector<double> m_X_fL;
    vector<double> m_P_fL;
    double GetProb_fL(double x);
    double Generate_fL();
    void AddX_fL(double x);
    void Init_fL();
    void Clear_fL();

  private:
    double m_Xp1;
    double m_Xp2;
    
    double m_Ep1;
    double m_Ep2;
    
  };

}

#endif
