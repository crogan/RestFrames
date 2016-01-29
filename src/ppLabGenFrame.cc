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
  ppLabGenFrame::ppLabGenFrame(const string& sname, const string& stitle) : 
    LabGenFrame(sname, stitle)
  {
    m_Xp1 = 0.5;
    m_Xp2 = 0.5;
    
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

  double ppLabGenFrame::SetPDFqqbar(){
    m_PDFqqbar = true;
    m_PDFgg    = false;
    m_PDFgq    = false;
    m_PDFqq    = false;
  }

  double ppLabGenFrame::SetPDFgg(){
    m_PDFqqbar = false;
    m_PDFgg    = true;
    m_PDFgq    = false;
    m_PDFqq    = false;
  }

  double ppLabGenFrame::SetPDFgq(){
    m_PDFqqbar = false;
    m_PDFgg    = false;
    m_PDFgq    = true;
    m_PDFqq    = false;
  }

  double ppLabGenFrame::SetPDFqq(){
    m_PDFqqbar = false;
    m_PDFgg    = false;
    m_PDFgq    = false;
    m_PDFqq    = true;
  }

  void ppLabGenFrame::SetEnergyP1(double E){
    if(E > 0)
      m_Ep1 = E;
  }

  void ppLabGenFrame::SetEnergyP2(double E){
    if(E > 0)
      m_Ep2 = E;
  }
  
  double ppLabGenFrame::GetEnergyP1() const {
    return m_Ep1;
  }

  double ppLabGenFrame::GetEnergyP2() const {
    return m_Ep2;
  }

  bool ppLabGenFrame::InitializeGenAnalysis(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    } 

    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      child.SetMassMCMC(child.GenerateMassMCMC());
      cout << " here " << child.GetMass() << endl;
    }

    return SetMind(true);
  }

  bool ppLabGenFrame::IterateMCMC(){
    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      double massOld = child.GetMass();
      double massNew = child.GenerateMassMCMC();
      double probOld = child.GetProbMCMC(massOld);
      double probNew = child.GetProbMCMC(massNew);
      probOld /= GetGenerateProbMCMC(massOld);
      probNew /= GetGenerateProbMCMC(massNew);
      
      if(probNew/max(probOld,probNew) > GetRandom())
	child.SetMassMCMC(massNew);
    } 

    m_PL = 

    return SetMind(true);
  }

  double ppLabGenFrame::pPDF_q(double x){
    return (1-x);
  }

  double ppLabGenFrame::pPDF_qbar(double x){

  }
  
  double ppLabGenFrame::pPDF_g(double x){

  }

  void ppLabGenFrame::AddX_fL(double x){
    if(x < 0 || x > 1)
      return;

     if(m_X_fL.empty())
       m_X_fL.push_back(x);
     else {
       vector<double>::iterator it = m_X_fL.begin();
      val = *it;
      while(x > val && it < vec.end()){
	it++;
	val = *it;
      }
      m_X_fL.insert(it, x);
     }
  }
  
}
