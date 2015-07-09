/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabGenFrame.cc
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

#include "RestFrames/LabGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabGenFrame class
  ///////////////////////////////////////////////
  LabGenFrame::LabGenFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle), 
    LabFrame(sname, stitle), 
    GeneratorFrame(sname, stitle)
  {
    Init();
  }

  LabGenFrame::~LabGenFrame(){
    
  }

  void LabGenFrame::Init(){
    m_PT = 0.;
    m_PL = 0.;
    m_Phi = -1.;
    m_Theta = -1.;
  }

  void LabGenFrame::SetThreeVector(const TVector3& P){
    m_PT = P.Pt();
    m_PL = P.Z();
    SetPhi(P.Phi());
  }

  void LabGenFrame::SetTransverseMomenta(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set transverse mass to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_PT = 0.;
    } else {
      m_PT = val;
    }
  }

  void LabGenFrame::SetLongitudinalMomenta(double val){
    m_PL = val;
  }

  void LabGenFrame::SetPhi(double val){
    while(val > acos(-1.)*2.) val -= acos(-1.)*2.;
    while(val < 0.) val += acos(-1.)*2.;
    m_Phi = val;
  }

  void LabGenFrame::ResetProductionAngles(){
     m_Phi = -1.;
     m_Theta = -1.;
  }

  void LabGenFrame::ResetFrame(){
    SetSpirit(false);
    ResetProductionAngles();
  }
  
  bool LabGenFrame::GenerateFrame(){
    if(!IsSoundBody()) 
      return false;

    TLorentzVector P;
    double M = GetChildFrame(0).GetMass();
    if(m_Phi < 0.) m_Phi = 2.*acos(-1.)*GetRandom();

    P.SetPxPyPzE(m_PT*cos(m_Phi), m_PT*sin(m_Phi), m_PL, sqrt(m_PT*m_PT+m_PL*m_PL+M*M));

    vector<TLorentzVector> ChildVector;
    ChildVector.push_back(P);
    SetChildren(ChildVector);
    ResetProductionAngles();

    SetSpirit(true);
    
    return true;
  }

  void LabGenFrame::ClearEvent(){
    SetSpirit(false);
    if(!IsSoundBody()) 
      return;
    ClearEventRecursive();
  }

  bool LabGenFrame::AnalyzeEvent(){
    if(!AnalyzeEventRecursive()){
      SetSpirit(false);
      return false;
    }
    SetSpirit(true);
    return m_Spirit;
  }

}
