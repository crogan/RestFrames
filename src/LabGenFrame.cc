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
    LabFrame<GeneratorFrame>(sname, stitle)
  {
    m_PT = 0.;
    m_PL = 0.;
    m_Phi = -1.;

    m_MaxM = -1.;

    m_NBurnInMCMC = 1;
  }

  LabGenFrame::~LabGenFrame() {}

  void LabGenFrame::Clear(){
    GeneratorFrame::Clear();
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

  void LabGenFrame::ResetFrame(){
    SetSpirit(false);
    m_Phi = -1.;
  }

  bool LabGenFrame::InitializeAnalysis(){
    m_Log << LogVerbose << "Initializing this tree for analysis..." << m_End;
   
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    }

    if(!InitializeAnalysisRecursive()){
      m_Log << LogWarning << "...Unable to recursively initialize analysis" << m_End;
      return SetMind(false);
    }

    for(int i = 0; i < m_NBurnInMCMC; i++)
      if(!IterateRecursiveMCMC()){
	m_Log << LogWarning << "...Unable to recursively initialize analysis" << m_End;
	return SetMind(false);
      }

    m_Log << LogVerbose << "...Done" << m_End;
    return SetMind(true);
  }
  
  void LabGenFrame::SetN_MCMCBurnIn(int N){
    m_NBurnInMCMC = max(0,N);
  }

  bool LabGenFrame::InitializeGenAnalysis(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    } 

    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      double ChildMass, ChildProb;
      child.GenerateMassMCMC(ChildMass, ChildProb, m_MaxM);
      m_ChildMassMCMC = ChildMass;
      m_ChildProbMCMC = ChildProb;
      SetMassMCMC(ChildMass, child);
    }

    return SetMind(true);
  }

  bool LabGenFrame::IterateMCMC(){
    GeneratorFrame& child = GetChildFrame(0);
    if(child.IsVariableMassMCMC()){
      double ChildMass, ChildProb = 0.;
      child.GenerateMassMCMC(ChildMass, ChildProb, m_MaxM);

      double probOld = GetProbMCMC(m_ChildMassMCMC)*
	child.GetProbMCMC(m_ChildMassMCMC)/m_ChildProbMCMC;
      double probNew = GetProbMCMC(ChildMass)*
	child.GetProbMCMC(ChildMass)/ChildProb;

      if(probNew/probOld > GetRandom()){
	m_ChildMassMCMC = ChildMass;
	m_ChildProbMCMC = ChildProb;
	SetMassMCMC(ChildMass, child);
      } else {
	SetMassMCMC(m_ChildMassMCMC, child);
      }
    }

    return SetMind(true);
  }

  bool LabGenFrame::GenerateFrame(){
    if(!IsSoundBody()) 
      return false;

    TLorentzVector P;
    double M = GetChildFrame(0).GetMass();
    if(m_Phi < 0.) m_Phi = 2.*acos(-1.)*GetRandom();

    P.SetPxPyPzE(m_PT*cos(m_Phi), m_PT*sin(m_Phi), m_PL, sqrt(m_PT*m_PT+m_PL*m_PL+M*M));
    m_Phi = -1.;

    vector<TLorentzVector> ChildVector;
    ChildVector.push_back(P);
    SetChildren(ChildVector);
    
    return SetSpirit(true);
  }

  bool LabGenFrame::ClearEvent(){
    SetSpirit(false);
    if(!IsSoundBody()) 
      return false;
    return ClearEventRecursive();
  }

  bool LabGenFrame::AnalyzeEvent(){
    if(!IterateRecursiveMCMC())
      return SetSpirit(false);

    if(!AnalyzeEventRecursive()){
      return SetSpirit(false);
    }

    return SetSpirit(true);
  }

}
