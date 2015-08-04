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
#include "RestFrames/ResonanceGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayGenFrame class
  ///////////////////////////////////////////////

  // Number of trials to discard in Metropilis-Hastings MCMC
  int DecayGenFrame::m_N_MCMC_BurnIn = 1000;

  DecayGenFrame::DecayGenFrame(const string& sname, const string& stitle) 
    : DecayFrame<GeneratorFrame>(sname,stitle)
  {
    Init();
  }
  
  DecayGenFrame::~DecayGenFrame(){ }

  void DecayGenFrame::Init(){
    m_Mass = -1.;
    m_MassSet = false;

    m_GType = DGVanilla;

    // frame generation param
    m_ChildP = -1.;
    m_ChildGamma = -1.;
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;

    // MCMC for resonance children
    m_MarkovChainMC = false;
    m_Resonances.Clear();
    m_ResIndex.clear();
    m_ResPrevProb.clear();
    m_ResPrevMass.clear();
    m_ResPrevMTOT = -1.;
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
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set mass to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_Mass = 0.;
    } else {
      m_Mass = val;
    }
    m_MassSet = true;
    m_ChildP = -1.;
    m_ChildGamma = 0.;
  }

  void DecayGenFrame::SetChildMomentum(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set child momentum to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_ChildP = 0.;
    } else {
      m_ChildP = val;
    }
    m_Mass = -1.;
    m_MassSet = false;
    m_ChildGamma = 0.;
  }

  void DecayGenFrame::SetChildGamma(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set child gamma less than one: ";
      m_Log << val << ". Setting to one." << m_End;
      m_ChildGamma = 1.;
    } else {
      m_ChildGamma = val;
    }
    m_Mass = -1.;
    m_MassSet = false;
    m_ChildP = -1.;
    m_ChildGamma = val;
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
    if(m_ChildP > 0. || m_ChildGamma >= 1.) m_MassSet = false;
    ResetDecayAngles();
  }

  void DecayGenFrame::ResetDecayAngles(){
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;
  }

  double DecayGenFrame::GetMass() const{
    if(m_MassSet) return m_Mass;

    int Nchild = GetNChildren();
    if(Nchild != 2) return -1.;
    double m[2];
    for(int i = 0; i < 2; i++) m[i] = GetChildFrame(i).GetMass();
    double Mass = 0;
    if(m_ChildP >= 0.){
      Mass = 0.;
      for(int i = 0; i < 2; i++) 
	Mass += sqrt(m_ChildP*m_ChildP + m[i]*m[i]);
    }
    if(m_ChildGamma >= 0.){
      Mass = sqrt(4.*m[0]*m[1]*m_ChildGamma*m_ChildGamma
		  + (m[0]-m[1])*(m[0]-m[1]));
    }
    m_Mass = Mass;
    m_MassSet = true;
    return m_Mass;
  }

  bool DecayGenFrame::IsResonanceFrame() const {
    return m_GType == DGResonance;
  }

  bool DecayGenFrame::InitializeGenAnalysis(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      SetMind(false);
      return false;
    }

    // check for resonance children
    m_Resonances.Clear();
    m_ResIndex.clear();
    m_ResPrevMass.clear();
    m_ResPrevProb.clear();
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      if(dynamic_cast<ResonanceGenFrame*>(&GetChildFrame(i))){
	m_Resonances.Add(*dynamic_cast<ResonanceGenFrame*>(&GetChildFrame(i)));
	m_ResIndex[&GetChildFrame(i)] = i;
      }
      
    int Nres = m_Resonances.GetN();
    for(int i = Nres-1; i >= 0; i--)
      if(m_Resonances[i].GetWidth() <= 0.){
	m_ResIndex.erase(&m_Resonances[i]);
	m_Resonances.Remove(m_Resonances[i]);
      }
     
    Nres = m_Resonances.GetN();
    if(Nres > 0){
      m_MarkovChainMC = true;
      m_Burnt = false;
      for(int i = 0; i < Nres; i++){
	m_ResPrevMass[&m_Resonances[i]] = 0.;
	m_ResPrevProb[&m_Resonances[i]] = 0.;
      }
    } else {
      m_MarkovChainMC = false;
    }
    return SetMind(true);
  }

  bool DecayGenFrame::MCMC_BurnIn(){
    m_Log << LogVerbose;
    m_Log << "Burning in Markov Chain MC with ";
    m_Log << m_N_MCMC_BurnIn;
    m_Log << " trials to be discarded..." << m_End;

    int N    = GetNChildren();
    int Nres = m_Resonances.GetN();
    double M = GetMass();

    // initialize MCMC parameters
    double Msum = 0.;
    for(int i = 0; i < N; i++)
      if(!m_Resonances.Contains(GetChildFrame(i).GetKey()))
	Msum += GetChildFrame(i).GetMass();
    double Mpolesum = Msum;
    for(int i = 0; i < Nres; i++){
      Msum     += m_Resonances[i].GetMinimumMass();
      Mpolesum += m_Resonances[i].GetPoleMass();
    }

    for(int i = 0; i < Nres; i++)
      m_ResPrevProb[&m_Resonances[i]]  = 0.;
    
    if(Msum >= M){
      m_Log << LogWarning;
      m_Log << "Sum of child masses is in excess of parent mass: ";
      m_Log << M << " < " << Msum << m_End;
      return SetSpirit(false);
    }

    if(Mpolesum < M)
      for(int i = 0; i < Nres; i++)
	m_ResPrevMass[&m_Resonances[i]] = m_Resonances[i].GetPoleMass();
    else
      for(int i = 0; i < Nres; i++)
	m_ResPrevMass[&m_Resonances[i]] = m_Resonances[i].GetMinimumMass() + (M-Msum)/double(Nres+1);
    m_ResPrevMTOT = M;

    for(int i = 0; i < m_N_MCMC_BurnIn; i++){
      if(!MCMC_Generate()){
	m_Log << LogWarning;
	m_Log << "Problem with Markov-chain MC generation";
	m_Log << m_End;
	m_Burnt = false;
	return false;
      }
    }
    m_Log << LogVerbose << "...Done" << m_End;

    m_Burnt = true;
    return true;
  }

  bool DecayGenFrame::MCMC_Generate(){
    int N    = GetNChildren();
    int Nres = m_Resonances.GetN();
    double M = GetMass();

    // Set res masses to previous iter val
    for(int ires = 0; ires < Nres; ires++){
      if(m_ResPrevMTOT > 0)
	m_ResPrevMass[&m_Resonances[ires]] = max(m_Resonances[ires].GetMinimumMass()*1.01, 
				  m_ResPrevMass[&m_Resonances[ires]]*M/m_ResPrevMTOT);
      
      m_Resonances[ires].SetEvtMass(m_ResPrevMass[&m_Resonances[ires]]);
    }

    // Update masses 1-by-1, 
    // keeping others fixed
    for(int ires = 0; ires < Nres; ires++){
      // calculate permissible mass range
      double Mmin = m_Resonances[ires].GetMinimumMass();
      double Mmax = M;
      vector<double> ChildMasses;
      for(int i = 0; i < N; i++){
	Mmax -= GetChildFrame(i).GetMass();
	ChildMasses.push_back(GetChildFrame(i).GetMass());
      }
      Mmax += m_ResPrevMass[&m_Resonances[ires]];
      double Mass = -1.;
      Mass = m_Resonances[ires].GenerateMass(Mmin,Mmax);
      ChildMasses[m_ResIndex[&m_Resonances[ires]]] = Mass;

      vector<double> TwoBodyMass;
      double Prob = (Mass/M)*GenerateTwoBodyMasses(M, ChildMasses, TwoBodyMass);
      if(Prob >= GetRandom()*m_ResPrevProb[&m_Resonances[ires]]){
	m_ResPrevMass[&m_Resonances[ires]] = Mass;
	m_ResPrevProb[&m_Resonances[ires]] = Prob;
	m_Resonances[ires].SetEvtMass(Mass); 
      } else {
	ChildMasses[m_ResIndex[&m_Resonances[ires]]] = m_ResPrevMass[&m_Resonances[ires]];
	m_Resonances[ires].SetEvtMass(m_ResPrevMass[&m_Resonances[ires]]); 
      }
    }
    m_ResPrevMTOT = M;
    return true;
  }

  bool DecayGenFrame::GenerateFrame(){
    if(!IsSoundBody()){ 
      m_Log << LogWarning;
      m_Log << "Unable to generate event for frame";
      m_Log << m_End;
      return SetSpirit(false);
    }

    if(m_MarkovChainMC){
      if(!m_Burnt)
	if(!MCMC_BurnIn())
	  return SetSpirit(false);
      if(!MCMC_Generate()){
	m_Log << "Problem with Markov-chain MC generation";
	m_Log << m_End;
	return SetSpirit(false);
      }
    }

    int Nchild = GetNChildren();

    vector<double> ChildMasses;
    double ChildMassTOT = 0.;
    for(int i = 0; i < Nchild; i++){
      double ChildMass = max(0.,GetChildFrame(i).GetMass());
      ChildMasses.push_back(ChildMass);
      ChildMassTOT += ChildMass;
    }

    double Mass = GetMass();
    if(Mass <= ChildMassTOT){
      m_Log << LogWarning;
      m_Log << "Problem in event generation: frame mass is less ";
      m_Log << "than or equal to the sum of child masses: ";
      m_Log << Mass << " <= " << ChildMassTOT << m_End;
      SetSpirit(false);
      return false;
    }
    SetSpirit(true);

    vector<double> TwoBodyMass;
    GenerateTwoBodyMasses(Mass,ChildMasses,TwoBodyMass);

    vector<TLorentzVector> ChildVectors;
    GenerateTwoBodyRecursive(TwoBodyMass, ChildMasses,
			     GetParentBoostVector(),
			     GetParentFrame().GetDecayPlaneNormalVector(),
			     ChildVectors);
    SetChildren(ChildVectors);
    
    return true;
  }

  double DecayGenFrame::GenerateTwoBodyMasses(double M, const vector<double>& M_c, vector<double>& M_2b){
    int N_c = M_c.size();

    if(N_c == 2){
      M_2b.push_back(M);
      M_2b.push_back(M_c[1]);
      return GetProb(M, M_c[0], M_c[1])/M;
    }

    double ETOT = M;
    for(int i = 0; i < N_c; i++)
      ETOT -= M_c[i];
    
    double Emax = ETOT + M_c[0];
    double Emin = 0;
    double probMAX = 1.;
    for(int n = 1; n < N_c; n++){
      Emin += M_c[n-1];
      Emax += M_c[n];
      probMAX *= GetProb(Emax,Emin,M_c[n])/M;
    }

    // accept/reject for intermediate
    // two-body decay masses
    double prob = -1.;
    while(prob/probMAX < GetRandom()){
      vector<double> ran;
      ran.push_back(0.);
      for(int i = 1; i < N_c-1; i++) 
	ran.push_back(GetRandom());
      qsort((double*)(&ran[0])+1,N_c-2,sizeof(double),DoubleMax);
      ran.push_back(1.);

      M_2b.clear();
      double Msum = M-ETOT;
      for(int i = 0; i < N_c; i++){
	M_2b.push_back(ran[N_c-1-i]*ETOT + Msum);
	Msum -= M_c[i];
      }

      prob = 1.;
      for(int i = 0; i < N_c-1; i++)
	prob *= GetProb(M_2b[i],M_2b[i+1],M_c[i])/M;
    }
    return prob;
  }

  double DecayGenFrame::GenerateTwoBodyRecursive(const vector<double>& M_p, const vector<double>& M_c,
						 const TVector3& axis_par, const TVector3& axis_perp,
						 vector<TLorentzVector>& P_c){
    TVector3 n_par = axis_par.Unit();
    TVector3 n_perp = axis_perp.Unit();

    int N_c = M_c.size();

    double m[2], Mp = M_p[0];
    m[0] = M_c[0];
    m[1] = M_p[1];
    double Pcm = GetProb(Mp,m[0],m[1]);
    TVector3 V_c[2];

    V_c[0] = Pcm*n_par;
    V_c[1] = -Pcm*n_par;

    if(m_CosDecayAngle < -1.) m_CosDecayAngle = 1.-2.*GetRandom();
    if(m_DeltaPhiDecayPlane < 0.) m_DeltaPhiDecayPlane = 2.*acos(-1.)*GetRandom();
   
    for(int i = 0; i < 2; i++) V_c[i].Rotate(-acos(m_CosDecayAngle),n_perp);
    for(int i = 0; i < 2; i++) V_c[i].Rotate(-m_DeltaPhiDecayPlane,n_par);
    ResetDecayAngles();

    TLorentzVector P_child[2];
    for(int i = 0; i < 2; i++) 
      P_child[i].SetVectM(V_c[i], m[i]);
    P_c.push_back(P_child[0]);
     
    if(N_c == 2){
      P_c.push_back(P_child[1]);
      return Pcm;
    }

    // Recursively generate other two-body decays for N > 2
    vector<double> M_pR;
    vector<double> M_cR;
    for(int i = 1; i < N_c; i++) M_pR.push_back(M_p[i]);
    for(int i = 1; i < N_c; i++) M_cR.push_back(M_c[i]);
    TVector3 boost = P_child[1].BoostVector();
    vector<TLorentzVector> P_cR;
    Pcm *= GenerateTwoBodyRecursive(M_pR, M_cR, boost, V_c[0].Cross(axis_par), P_cR);
    for(int i = 0; i < N_c-1; i++) P_cR[i].Boost(boost);
    for(int i = 0; i < N_c-1; i++) P_c.push_back(P_cR[i]);

    return Pcm;
  }

  int DoubleMax(const void *a, const void *b){
    Double_t aa = *((double*)a);
    Double_t bb = *((double*)b);
    if (aa > bb) return  1;
    if (aa < bb) return -1;
    return 0; 
  }

}
