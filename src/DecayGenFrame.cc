/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayGenFrame class
  ///////////////////////////////////////////////

  DecayGenFrame::DecayGenFrame(const std::string& sname, 
			       const std::string& stitle) 
    : DecayFrame<GeneratorFrame>(sname,stitle)
  {
    Init();
  }
  
  DecayGenFrame::~DecayGenFrame(){ }

  void DecayGenFrame::Init(){
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
      m_Log << val << ". Setting to zero." << LogEnd;
      m_Mass = 0.;
    } else {
      m_Mass = val;
    }
 
  }

  void DecayGenFrame::SetCosDecayAngle(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "CosDecay angle must be in [-1, 1]: ";
      m_Log << val << ". Setting to random." << LogEnd;
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
      m_Log << "for its mass." << LogEnd;
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
      m_Log << min_mass << ")" << LogEnd;
      return SetMind(false);
    }

    m_ChildIndexMCMC.clear();
    m_ChildMassMCMC.clear();
    m_ChildProbMCMC.clear();
    double Mass = GetMass();
    m_Log << LogInfo;
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

    std::vector<double> InterMassFrac;
    InterMassFrac.push_back(0.);
    for(int i = 1; i < N-1; i++) 
      InterMassFrac.push_back(GetRandom());
    qsort((double*)(&InterMassFrac[0])+1,N-2,sizeof(double),DoubleMax);
    InterMassFrac.push_back(1.);
    
    double probOld = GetProbMCMC();

    std::vector<double> InterMassFracOld = m_InterMassFracMCMC;
    m_InterMassFracMCMC = InterMassFrac;

    double probNew = GetProbMCMC();

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

      probOld *= GetProbMCMC();
      SetMassMCMC(ChildMass, child);
      probNew *= GetProbMCMC();
      
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
    if(mass < 0.)
      mass = GetMass();

    double SumChildMass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      SumChildMass += GetChildFrame(i).GetMass();

    if(mass < SumChildMass)
      return 0.;

    double ETOT = mass - SumChildMass;
    std::vector<double> InterMass;
    for(int i = 0; i < N; i++){
      InterMass.push_back(m_InterMassFracMCMC[N-1-i]*ETOT + SumChildMass);
      SumChildMass -= GetChildFrame(i).GetMass();
    }

    double prob = 1.;
    for(int i = 0; i < N-1; i++)
      prob *= GetP(InterMass[i], InterMass[i+1], GetChildFrame(i).GetMass())/mass;
    
    prob /= mass*mass;
    //prob /= mass*mass*mass*mass;

    return prob;
  }

  void DecayGenFrame::GenerateMassMCMC(double& mass, double& prob, 
				       double max) const {
    int N = GetNChildren();
    double SumMinChildMass = 0.;
    for(int i = 0; i < N; i++){
      GeneratorFrame& child = GetChildFrame(i);
      if(!child.IsVariableMassMCMC())
	SumMinChildMass += child.GetMass();
      else 
	SumMinChildMass += child.GetMinimumMassMCMC();
    }

    if(SumMinChildMass > max && max > 0){
      mass = max;
      prob = 0;
      return;
    }

    double SumChildMass = 0.;
    for(int i = 0; i < N; i++){
      GeneratorFrame& child = GetChildFrame(i);
      if(child.IsVariableMassMCMC()){	double cmass, cprob, cmax;
	cmass = max-SumMinChildMass+child.GetMinimumMassMCMC();
	child.GenerateMassMCMC(cmass, cprob, cmass);
	SumChildMass += cmass;
      } else 
	SumChildMass += child.GetMass();
    }

    double T = SumChildMass;
    double min = SumMinChildMass;
    double SL = (T > 0 ? T/10. : 10.);
    double SU = (T > 0 ? T : 100.); 
    SU = (max > 0 ? std::max(max/100.,T) : 100.);
    double IL = SL*(1.-exp(-(T-min)/SL));
    double IU = (max > 0 ? SU*(1.-exp(-(max-T)/SU)) : 1.);

    double R = GetRandom();
    if(R > IL/(IL+IU)){
      R = R*(IL+IU)/IU - IL/IU;
      mass = T - log(1-R*IU/SU)*SU;
      prob = exp(-(mass-T)/SU);
    } else {
      R = R*(IL+IU)/IL;
      mass = T + log(1-R*IL/SL)*SL;
      prob = exp((mass-T)/SL);
    }
    
  }

  bool DecayGenFrame::GenerateFrame(){
    if(!IsSoundMind()){ 
      m_Log << LogWarning;
      m_Log << "Unable to generate event for frame";
      m_Log << LogEnd;
      return SetSpirit(false);
    }

    std::vector<double> ChildMasses;
    double SumChildMass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      ChildMasses.push_back(GetChildFrame(i).GetMass());
      SumChildMass += GetChildFrame(i).GetMass();
    }
   
    double ETOT = GetMass() - SumChildMass;
    std::vector<double> InterMass;
    for(int i = 0; i < N; i++){
      InterMass.push_back(m_InterMassFracMCMC[N-1-i]*ETOT + SumChildMass);
      SumChildMass -= GetChildFrame(i).GetMass();
    }

    SetSpirit(true);

    std::vector<TLorentzVector> ChildVectors;
    GenerateTwoBodyRecursive(InterMass, ChildMasses,
			     GetParentBoostVector(),
			     GetParentFrame().GetDecayPlaneNormalVector(),
			     ChildVectors);
    SetChildren(ChildVectors);

    return SetSpirit(true);
  }

  void DecayGenFrame::GenerateTwoBodyRecursive(const std::vector<double>& M_p, 
					       const std::vector<double>& M_c,
					       const TVector3& axis_par, 
					       const TVector3& axis_perp,
					       std::vector<TLorentzVector>& P_c) {
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
    std::vector<double> M_pR;
    std::vector<double> M_cR;
    for(int i = 1; i < N_c; i++) M_pR.push_back(M_p[i]);
    for(int i = 1; i < N_c; i++) M_cR.push_back(M_c[i]);
    TVector3 boost = P_child[1].BoostVector();
    std::vector<TLorentzVector> P_cR;
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
