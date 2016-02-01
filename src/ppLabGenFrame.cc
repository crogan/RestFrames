/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ppLabGenFrame.cc
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

#include "RestFrames/ppLabGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ppLabGenFrame class
  ///////////////////////////////////////////////
  double ppLabGenFrame::m_PDF_eta_1 = 0.27871;
  double ppLabGenFrame::m_PDF_eta_2 = 3.3627;
  double ppLabGenFrame::m_PDF_eps_u = 4.4343;
  double ppLabGenFrame::m_PDF_g_u = 38.599;
  double ppLabGenFrame::m_PDF_del_S = -0.11912;
  double ppLabGenFrame::m_PDF_eta_S = 9.4189;
  double ppLabGenFrame::m_PDF_eps_S = -2.6287;
  double ppLabGenFrame::m_PDF_g_S = 18.065;
  double ppLabGenFrame::m_PDF_A_g = 3.4055;
  double ppLabGenFrame::m_PDF_del_g = -0.12178;
  double ppLabGenFrame::m_PDF_eta_g = 2.9278;
  double ppLabGenFrame::m_PDF_eps_g = -2.3210;
  double ppLabGenFrame::m_PDF_g_g = 1.9233;
  double ppLabGenFrame::m_PDF_A_g1 = -1.6189;
  double ppLabGenFrame::m_PDF_del_g1 = -0.23999;
  double ppLabGenFrame::m_PDF_eta_g1 = 24.792;

  ppLabGenFrame::ppLabGenFrame(const string& sname, const string& stitle) : 
    LabGenFrame(sname, stitle)
  {
    m_Xp1 = 0.;
    m_Xp2 = 0.;
    
    // default is 13 TeV LHC
    m_Ep1 = 6500.;
    m_Ep2 = 6500.;

    // default parton initial state is q-qbar
    m_PDFqqbar = true;
    m_PDFgg    = false;
    m_PDFgq    = false;
    m_PDFqq    = false;
  }

  ppLabGenFrame::~ppLabGenFrame() {}

  void ppLabGenFrame::Clear(){
    LabGenFrame::Clear();
  }

  void ppLabGenFrame::SetPDFqqbar(){
    SetMind(false);
    m_PDFqqbar = true;
    m_PDFgg    = false;
    m_PDFgq    = false;
    m_PDFqq    = false;
  }

  void ppLabGenFrame::SetPDFgg(){
    SetMind(false);
    m_PDFqqbar = false;
    m_PDFgg    = true;
    m_PDFgq    = false;
    m_PDFqq    = false;
  }

  void ppLabGenFrame::SetPDFgq(){
    SetMind(false);
    m_PDFqqbar = false;
    m_PDFgg    = false;
    m_PDFgq    = true;
    m_PDFqq    = false;
  }

  void ppLabGenFrame::SetPDFqq(){
    SetMind(false);
    m_PDFqqbar = false;
    m_PDFgg    = false;
    m_PDFgq    = false;
    m_PDFqq    = true;
  }

  void ppLabGenFrame::SetEnergyP1(double E){
    SetMind(false);
    if(E > 0)
      m_Ep1 = E;
  }

  void ppLabGenFrame::SetEnergyP2(double E){
    SetMind(false);
    if(E > 0)
      m_Ep2 = E;
  }
  
  double ppLabGenFrame::GetEnergyP1() const {
    return m_Ep1;
  }

  double ppLabGenFrame::GetEnergyP2() const {
    return m_Ep2;
  }

  double ppLabGenFrame::GetMass() const {
    return 2.*sqrt(m_Ep1*m_Ep2*m_Xp1*m_Xp2);
  }

  bool ppLabGenFrame::InitializeGenAnalysis(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    } 

    double sqrtS = 2.*sqrt(m_Ep1*m_Ep2);
    double Mmin = GetMinimumMassMCMC();
    if(Mmin > sqrtS){
      m_Log << LogWarning;
      m_Log << "Unable to initialize event generation. ";
      m_Log << "sqrt(S) of " << sqrtS << " ";
      m_Log << "is less than minimum child mass of " << Mmin;
      m_Log << m_End;
      return SetMind(false);
    }

    double sqrtShat;
    double prob = 1.;
    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      child.GenerateMassMCMC(sqrtShat, prob, sqrtS);
      child.SetMassMCMC(sqrtShat);
    } else {
      sqrtShat = child.GetMass();
    }
    m_deltaLogX = 0.;
    m_sqrtX1X2 = sqrtShat/sqrtS;
    m_VarMass = sqrtShat;
    m_VarProb = prob;

    return SetMind(true);
  }

  bool ppLabGenFrame::IterateMCMC(){
    // update m_deltaLogX
    double R = GetRandom();
    double deltaLogX;
    if(R < 0.5)
      deltaLogX = -1.+sqrt(2.*R);
    else
      deltaLogX = 1.-sqrt(2.*(1.-R));

    double deltaLogXOld = m_deltaLogX;
    double probOld = GetProbMCMC()/(1.-fabs(deltaLogXOld));
    m_deltaLogX = deltaLogX;
    double probNew = GetProbMCMC()/(1.-fabs(deltaLogX));

    if(probOld > 0)
      if(probNew/probOld < GetRandom())
	m_deltaLogX = deltaLogXOld;

    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      double VarMass, VarProb = 1.;
      child.GenerateMassMCMC(VarMass, VarProb, 2.*sqrt(m_Ep1*m_Ep2));
      probOld = GetProbMCMC()*child.GetProbMCMC(m_VarMass)/m_VarProb;
      double sqrtX1X2Old = m_sqrtX1X2;
      m_sqrtX1X2 = VarMass/2.*sqrt(m_Ep1*m_Ep2);
      probNew = GetProbMCMC()*child.GetProbMCMC(VarMass)/VarProb;
      if(probNew/probOld > GetRandom()){
	m_VarMass = VarMass;
	m_VarProb = VarProb;
      } else {
	m_sqrtX1X2 = sqrtX1X2Old;
      }
    }
    
    double C = m_sqrtX1X2*m_sqrtX1X2;
    double expo = -m_deltaLogX*log(C);
    double Xp1 = sqrt(C*exp(expo));
    double Xp2 = sqrt(C*exp(-expo));
    SetLongitudinalMomenta(m_Ep1*Xp1 - m_Ep2*Xp2);

    return SetMind(true);
  }

  double ppLabGenFrame::GetProbMCMC(double mass) const {
    if(mass < 0)
      mass = GetChildFrame(0).GetMass();
    double C = mass*mass/(4.*m_Ep1*m_Ep2);
    double expo = -m_deltaLogX*log(C);
    
    double Xp1 = sqrt(C*exp(expo));
    double Xp2 = sqrt(C*exp(-expo));

    double prob = 1.;
    if(m_PDFqqbar)
      prob *= pPDF_q(Xp1)*pPDF_qbar(Xp2)+pPDF_q(Xp2)*pPDF_qbar(Xp1);
    if(m_PDFgg)
      prob *= pPDF_g(Xp1)*pPDF_g(Xp2);
    if(m_PDFgq)
      prob *= pPDF_q(Xp1)*pPDF_g(Xp2)+pPDF_q(Xp2)*pPDF_g(Xp1);
    if(m_PDFqq)
      prob *= pPDF_q(Xp1)*pPDF_q(Xp2);

    return prob;
  }

  double ppLabGenFrame::pPDF_q(double x) const {
    if(x <= 0. || x >= 1.)
      return 0.;
    return fabs((1/x)*pow(x,m_PDF_eta_1)*pow(1-x,m_PDF_eta_2)*
		(1.+m_PDF_eps_u*sqrt(x)+m_PDF_g_u*x));
  }

  double ppLabGenFrame::pPDF_qbar(double x) const {
    if(x <= 0. || x >= 1.)
      return 0.;
    return fabs((1/x)*pow(x,m_PDF_del_S)*pow(1-x,m_PDF_eta_S)*
		(1.+m_PDF_eps_S*sqrt(x)+m_PDF_g_S*x));
  }
  
  double ppLabGenFrame::pPDF_g(double x) const {
    if(x <= 0. || x >= 1.)
      return 0.;
    return fabs((1/x)*(m_PDF_A_g*pow(x,m_PDF_del_g)*pow(1-x,m_PDF_eta_g)*
		       (1.+m_PDF_eps_g*sqrt(x)+m_PDF_g_g*x)+
		       m_PDF_A_g1*pow(x,m_PDF_del_g1)*pow(1-x,m_PDF_eta_g1)));
  }

  void ppLabGenFrame::SetXMCMC(double x) const {
    if(x < 0 || x > 1) 
      return;

    if(m_Mode_fL == 0){
      double X1X2 = GetChildFrame(0).GetMass()/(2.*sqrt(m_Ep1*m_Ep2));
      X1X2 *= X1X2;
      m_Xp1 = x;
      m_Xp2 = X1X2/m_Xp1;
    }

    if(m_Mode_fL == 1){
      m_Xp1 = x;
    }

    if(m_Mode_fL == 2){
      m_Xp2 = x;
    }
  }

  double ppLabGenFrame::GetXMCMC() const {
    if(m_Mode_fL == 0 || m_Mode_fL == 1)
      return m_Xp1;

    if(m_Mode_fL == 2)
      return m_Xp2;
      
    return 0.;
  }
  
}
