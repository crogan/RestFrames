/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   DecayGenFrame.cc
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

#include <stdlib.h>
#include "RestFrames/DecayGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayGenFrame class
  ///////////////////////////////////////////////

  DecayGenFrame::DecayGenFrame(const string& sname, const string& stitle) 
    : DecayFrame<GeneratorFrame>(sname,stitle)
  {
    Init();
  }
  
  DecayGenFrame::~DecayGenFrame(){ }

  void DecayGenFrame::Init(){
    m_Mass = 0.;

    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;
  }

  bool DecayGenFrame::IsSoundBody() const{
    if(RFBase::IsSoundBody())
      return true;
    if(!RestFrame::IsSoundBody())
      return false;
    int Nchild = GetNChildren();
    if(Nchild < 2 || GetParentFrame().IsEmpty()) 
      return SetBody(false);
    return SetBody(true);
  }

  void DecayGenFrame::SetMass(double val){
    SetMind(false);
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set mass to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_Mass = 0.;
    } else {
      m_Mass = val;
    }
 
  }

  void DecayGenFrame::SetCosDecayAngle(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "CosDecay angle must be in [-1, 1]: ";
      m_Log << val << ". Setting to random." << m_End;
      m_CosDecayAngle = -2.;
    } else {
      m_CosDecayAngle = val;
    }
  }

  void DecayGenFrame::SetDeltaPhiDecayPlane(double val){
    while(val > acos(-1.)*2.) val -= acos(-1.)*2.;
    while(val < 0.) val += acos(-1.)*2.;
    m_DeltaPhiDecayPlane = val;
  }

  void DecayGenFrame::ResetFrame(){
    SetSpirit(false);
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;
  }

  double DecayGenFrame::GetMass() const {
    return m_Mass;
  }

  void DecayGenFrame::SetVariableMass(bool varymass) {
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return;
    } 

    if(!varymass){
      SetVariableMassMCMC(false);
      SetMind(false);
      return;
    }

    int N = GetNChildren();
    bool var = false;
    for(int i = 0; i < N; i++){
      if(GetChildFrame(i).IsVariableMassMCMC()){
	var = true;
	break;
      }
    }
    if(!var){
      m_Log << LogWarning;
      m_Log << "Unable to set to variable mass. ";
      m_Log << "No children have variable masses and ";
      m_Log << "DecayGenFrame has no PDF defined ";
      m_Log << "for its mass." << m_End;
      SetVariableMassMCMC(false);
      SetMind(false);
      return;
    }
    SetVariableMassMCMC(true);
    SetMind(false);
  }

  bool DecayGenFrame::InitializeGenAnalysis(){
    if(!IsSoundBody())
      return SetMind(false);

    double min_mass = GetMinimumMassMCMC();
    if(m_Mass < min_mass){
      m_Log << LogWarning;
      m_Log << "Unable to initialize analysis: ";
      m_Log << "decay frame mass (" << m_Mass << ") ";
      m_Log << "is less than required child masses (";
      m_Log << min_mass << ")" << m_End;
      return SetMind(false);
    }

    m_ChildIndexMCMC.clear();
    m_ChildMassMCMC.clear();
    m_ChildProbMCMC.clear();
    double Mass = GetMass();
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      double cmass = 0.;
      double cprob = 1.;
      GeneratorFrame& child = GetChildFrame(i);
      if(child.IsVariableMassMCMC()){
	child.GenerateMassMCMC(cmass, cprob, Mass);
	SetMassMCMC(cmass, child);
	m_ChildIndexMCMC.push_back(i);
	m_ChildMassMCMC.push_back(cmass);
	m_ChildProbMCMC.push_back(cprob);
      } else {
	cmass = child.GetMass();
      }
      Mass -= cmass;
    }

    m_InterMassFracMCMC.clear();
    m_InterMassFracMCMC.push_back(0.);
    for(int i = 1; i < N-1; i++) 
      m_InterMassFracMCMC.push_back(GetRandom());
    qsort((double*)(&m_InterMassFracMCMC[0])+1,N-2,sizeof(double),DoubleMax);
    m_InterMassFracMCMC.push_back(1.);

    return SetMind(true);
  }

  bool DecayGenFrame::IterateMCMC(){
    int N = GetNChildren();
    vector<double> InterMassFrac;
    InterMassFrac.push_back(0.);
    for(int i = 1; i < N-1; i++) 
      InterMassFrac.push_back(GetRandom());
    qsort((double*)(&InterMassFrac[0])+1,N-2,sizeof(double),DoubleMax);
    InterMassFrac.push_back(1.);
    
    double probOld = GetProbMCMC(GetMass());

    vector<double> InterMassFracOld = m_InterMassFracMCMC;
    m_InterMassFracMCMC = InterMassFrac;

    double probNew = GetProbMCMC(GetMass());

    if(probOld > 0.)
      if(probNew/probOld < GetRandom())
    	m_InterMassFracMCMC = InterMassFracOld;

    int Nvar = m_ChildIndexMCMC.size();
    for(int v = 0; v < Nvar; v++){
      int index = m_ChildIndexMCMC[v];
      GeneratorFrame& child = GetChildFrame(index);
      
      double massMax = GetMass();
      for(int i = 0; i < N; i++)
	if(i != index)
	  massMax -= GetChildFrame(i).GetMass();

      double ChildMass = 0.;
      double ChildProb = 0.;
      child.GenerateMassMCMC(ChildMass, ChildProb, massMax);
      double probOld = child.GetProbMCMC(m_ChildMassMCMC[v]);
      double probNew = child.GetProbMCMC(ChildMass);
      probOld /= m_ChildProbMCMC[v];
      probNew /= ChildProb;

      probOld *= GetProbMCMC(GetMass());
      SetMassMCMC(ChildMass, child);
      probNew *= GetProbMCMC(GetMass());
      
      if(probOld > 0){
	if(probNew/probOld < GetRandom()){
	  SetMassMCMC(m_ChildMassMCMC[v], child);
	} else {
	  m_ChildMassMCMC[v] = ChildMass;
	  m_ChildProbMCMC[v] = ChildProb;
	}
      }	 
    }
    
    return SetMind(true);
  }

  double DecayGenFrame::GetProbMCMC(double mass) const {
    double SumChildMass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      SumChildMass += GetChildFrame(i).GetMass();

    if(mass < SumChildMass)
      return 0.;

    double ETOT = mass - SumChildMass;
    vector<double> InterMass;
    for(int i = 0; i < N; i++){
      InterMass.push_back(m_InterMassFracMCMC[N-1-i]*ETOT + SumChildMass);
      SumChildMass -= GetChildFrame(i).GetMass();
    }

    double prob = 1.;
    for(int i = 0; i < N-1; i++)
      prob *= GetP(InterMass[i], InterMass[i+1], GetChildFrame(i).GetMass());
    
    prob /= mass*mass;

    return prob;
  }

  void DecayGenFrame::GenerateMassMCMC(double& mass, double& prob, 
				       double max) const {
    int N = GetNChildren();
    double SumChildMass = 0.;
    double ProdProb = 1.;
    for(int i = 0; i < N; i++){
      GeneratorFrame& child = GetChildFrame(i);
      if(!child.IsVariableMassMCMC())
	SumChildMass += child.GetMass();
      else
	SumChildMass += child.GetMinimumMassMCMC();
    }

    if(SumChildMass > max && max > 0)
      return;

    for(int i = 0; i < N; i++){
      GeneratorFrame& child = GetChildFrame(i);
      if(child.IsVariableMassMCMC()){
	double cmass, cprob, cmax;
	SumChildMass += child.GetMinimumMassMCMC();
	child.GetMassMCMC(cmass, cprob, max-SumChildMass);
	SumChildMass += cmass;
	ProdProb *= cprob;
      } 
    }
    mass = sqrt(2.)*SumChildMass;
    if(mass > max && max > 0)
      mass = max - fabs(max-SumChildMass);
    prob = ProdProb;
  }

  bool DecayGenFrame::GenerateFrame(){
    if(!IsSoundMind()){ 
      m_Log << LogWarning;
      m_Log << "Unable to generate event for frame";
      m_Log << m_End;
      return SetSpirit(false);
    }

    vector<double> ChildMasses;
    double SumChildMass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      ChildMasses.push_back(GetChildFrame(i).GetMass());
      SumChildMass += GetChildFrame(i).GetMass();
    }

    double ETOT = GetMass() - SumChildMass;
    vector<double> InterMass;
    for(int i = 0; i < N; i++){
      InterMass.push_back(m_InterMassFracMCMC[N-1-i]*ETOT + SumChildMass);
      SumChildMass -= GetChildFrame(i).GetMass();
    }

    SetSpirit(true);

    vector<TLorentzVector> ChildVectors;
    GenerateTwoBodyRecursive(InterMass, ChildMasses,
			     GetParentBoostVector(),
			     GetParentFrame().GetDecayPlaneNormalVector(),
			     ChildVectors);
    SetChildren(ChildVectors);

    return SetSpirit(true);
  }

  void DecayGenFrame::GenerateTwoBodyRecursive(const vector<double>& M_p, const vector<double>& M_c,
					       const TVector3& axis_par, const TVector3& axis_perp,
					       vector<TLorentzVector>& P_c) {
    TVector3 n_par = axis_par.Unit();
    TVector3 n_perp = axis_perp.Unit();

    int N_c = M_c.size();

    double m[2], Mp = M_p[0];
    m[0] = M_c[0];
    m[1] = M_p[1];
    TVector3 V_c[2];

    double Pcm = GetP(Mp, m[0], m[1]);

    V_c[0] = Pcm*n_par;
    V_c[1] = -Pcm*n_par;

    if(m_CosDecayAngle < -1.) m_CosDecayAngle = 1.-2.*GetRandom();
    if(m_DeltaPhiDecayPlane < 0.) m_DeltaPhiDecayPlane = 2.*acos(-1.)*GetRandom();
   
    for(int i = 0; i < 2; i++) V_c[i].Rotate(-acos(m_CosDecayAngle),n_perp);
    for(int i = 0; i < 2; i++) V_c[i].Rotate(-m_DeltaPhiDecayPlane,n_par);
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;

    TLorentzVector P_child[2];
    for(int i = 0; i < 2; i++) 
      P_child[i].SetVectM(V_c[i], m[i]);
    P_c.push_back(P_child[0]);
     
    if(N_c == 2){
      P_c.push_back(P_child[1]);
      return;
    }

    // Recursively generate other two-body decays for N > 2
    vector<double> M_pR;
    vector<double> M_cR;
    for(int i = 1; i < N_c; i++) M_pR.push_back(M_p[i]);
    for(int i = 1; i < N_c; i++) M_cR.push_back(M_c[i]);
    TVector3 boost = P_child[1].BoostVector();
    vector<TLorentzVector> P_cR;
    GenerateTwoBodyRecursive(M_pR, M_cR, boost, V_c[0].Cross(axis_par), P_cR);
    for(int i = 0; i < N_c-1; i++) P_cR[i].Boost(boost);
    for(int i = 0; i < N_c-1; i++) P_c.push_back(P_cR[i]);
  }

  int DoubleMax(const void *a, const void *b){
    Double_t aa = *((double*)a);
    Double_t bb = *((double*)b);
    if (aa > bb) return  1;
    if (aa < bb) return -1;
    return 0; 
  }

}
