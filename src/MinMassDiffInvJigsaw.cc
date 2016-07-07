/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassDiffInvJigsaw.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Jun
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

#include "Math/Factory.h"

#include "RestFrames/MinMassDiffInvJigsaw.hh"

namespace RestFrames {

  MinMassDiffInvJigsaw::MinMassDiffInvJigsaw(const std::string& sname, 
					     const std::string& stitle,
					     int N_vis_inv_pair) : 
    InvisibleJigsaw(sname, stitle, N_vis_inv_pair, N_vis_inv_pair), 
    m_Npair(N_vis_inv_pair)
  {
    m_minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
    m_minimizer->SetMaxFunctionCalls(10000000);
    m_minimizer->SetMaxIterations(100000);
    m_minimizer->SetTolerance(0.001);
    m_minimizer->SetPrintLevel(0);

    m_functor = new ROOT::Math::Functor(this, &MinMassDiffInvJigsaw::EvaluateMetric, 3);
    m_minimizer->SetFunction(*m_functor);

    m_minimizer->SetVariable(0, "phi1", 0., 0.001);
    m_minimizer->SetVariable(1, "phi2", 0., 0.001);
    m_minimizer->SetVariable(2, "phi3", 0., 0.001);
  }
 
  MinMassDiffInvJigsaw::~MinMassDiffInvJigsaw(){
    delete m_minimizer;
    delete m_functor;
  }

  double MinMassDiffInvJigsaw::GetMinimumMass() const {
    if(!IsSoundMind())
      return 0.;

    if(m_Npair <= 0) 
      return 0.;
    if(m_Npair == 1) 
      return std::max(0.,GetChildState(0).GetMinimumMass());
    
    TLorentzVector PvisTOT(0.,0.,0.,0.);
    m_Pvis.clear();
    for(int i = 0; i < m_Npair; i++){
      m_Pvis.push_back(GetDependancyStates(i+m_Npair).GetFourVector());
      PvisTOT += m_Pvis[i];
    }

    double ECM = 0.;
    double Minv;
    TVector3 Boost = PvisTOT.BoostVector();
    for(int i = 0; i < m_Npair; i++){
      m_Pvis[i].Boost(-Boost);
      Minv = std::max(0.,GetChildState(i).GetMinimumMass());
      ECM += sqrt(m_Pvis[i].P()*m_Pvis[i].P() + Minv*Minv);
    }

    return ECM;
  }

  bool MinMassDiffInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind())
      return SetSpirit(false);

    if(m_Npair <= 1) 
      return false;
    
    TLorentzVector INV = GetParentState().GetFourVector();
    double Minv = INV.M();

    TLorentzVector VIS(0.,0.,0.,0.);
    m_Pvis.clear();
    for(int i = 0; i < m_Npair; i++){
      m_Pvis.push_back(GetDependancyStates(i).GetFourVector());
      VIS += m_Pvis[i];
    }

    // go to INV+VIS CM frame
    TVector3 BoostCM = (VIS+INV).BoostVector();
    INV.Boost(-BoostCM);
    VIS.Boost(-BoostCM);
    for(int i = 0; i < m_Npair; i++)
      m_Pvis[i].Boost(-BoostCM);

    // INV states defined in the VIS frame
    TVector3 BoostVIS = VIS.BoostVector();
    m_Pinv.clear();
    m_Minv.clear();
    for(int i = 0; i < m_Npair; i++){
      m_Minv.push_back(std::max(0.,GetChildState(i).GetMinimumMass()));
      m_Pinv.push_back(m_Pvis[i]);
      m_Pinv[i].Boost(-BoostVIS);
      m_Pinv[i].SetVectM(m_Pinv[i].Vect(), m_Minv[i]);
    }

    // VIS states in INV frame
    TVector3 BoostINV = INV.BoostVector();
    for(int i = 0; i < m_Npair; i++)
      m_Pvis[i].Boost(-BoostINV);

    if(m_Npair == 2){
      TVector3 Vdiff = (m_Pvis[0].Vect()-m_Pvis[1].Vect()).Unit();
      double pinv = GetP(Minv, m_Minv[0], m_Minv[1]);
      m_Pinv[0].SetVectM( pinv*Vdiff, m_Minv[0]);
      m_Pinv[1].SetVectM(-pinv*Vdiff, m_Minv[1]);
    } else {
      double k = GetPScale(Minv);
      for(int i = 0; i < m_Npair; i++)
	m_Pinv[i].SetVectM(k*m_Pinv[i].Vect(), m_Minv[i]);
    }

    ApplyOptimalRotation();
    
    // return to original frame
    for(int i = 0; i < m_Npair; i++){
      m_Pinv[i].Boost(BoostINV);
      m_Pinv[i].Boost(BoostCM);
      GetChildState(i).SetFourVector(m_Pinv[i]);
    }
    
    return SetSpirit(true);
  }

  // Based on Newton-Raphson root finding
  double MinMassDiffInvJigsaw::GetPScale(double Minv){
    std::vector<double> Pinv2;
    double Ek  = 0.;
    double fk  = 0.;
    double dfk = 0.;
    for(int i = 0; i < m_Npair; i++){
      Pinv2.push_back(m_Pinv[i].P()*m_Pinv[i].P());
      Ek += sqrt(m_Minv[i]*m_Minv[i]+Pinv2[i]);
      fk += m_Minv[i];
    }
    if(fk > Minv || Ek <= 0.) return 0.;
    
    double k2 = Minv/Ek;
    k2 *= k2;
    double dk2 = k2;
    int count = 0;
    while(fabs(dk2) >= 1e-10 && count < 100){
      fk = -Minv;
      dfk = 0.;
      for(int i = 0; i < m_Npair; i++){
	Ek = sqrt(m_Minv[i]*m_Minv[i]+k2*Pinv2[i]);
	fk  += Ek;
	dfk += Ek > 0 ? Pinv2[i]/Ek : 0.;
      }
      dk2 = 2.*fk/dfk;
      k2 -= dk2;
      count++;
    }
    
    return sqrt(std::max(0.,k2));
  }

  // Optimal rotation found using Minuit2 ROOT interface
  void MinMassDiffInvJigsaw::ApplyOptimalRotation(){
    // first, check dimensionality of points
    m_Z.SetXYZ(0.,0.,0.);
    int index = 0;

    while(m_Z.Mag() <= 0. && index < m_Npair){
      m_Z = m_Pinv[index].Vect().Cross(m_Pvis[index].Vect());
      index++;
    }
    m_Z = m_Z.Unit();

    m_2D = true;
    if(m_Npair > 2){
      for(int i = 0; i < m_Npair; i++){
	if(m_Z.Dot(m_Pvis[i].Vect().Unit()) > 1e-8){
	  m_2D = false;
	  break;
	}
	if(m_Z.Dot(m_Pinv[i].Vect().Unit()) > 1e-8){
	  m_2D = false;
	  break;
	}
      }
    }
    
    m_minimizer->SetVariableValue(0, 0.);
    m_minimizer->SetVariableValue(1, 0.);
    m_minimizer->SetVariableValue(2, 0.);

    if(m_2D){
      m_minimizer->FixVariable(1);
      m_minimizer->FixVariable(2);
    } else {
      m_minimizer->ReleaseVariable(1);
      m_minimizer->ReleaseVariable(2);
      m_X.SetXYZ(0.,0.,0.);
      for(int i = 0; i < m_Npair; i++)
	m_X += m_Pvis[i].Vect() - m_Pinv[i].Vect();
      
      if(m_X.Mag() <= 0)
	m_X = m_Pvis[0].Vect().Cross(m_Z);
      
      m_X = m_X.Unit();
      m_Y = m_Z.Cross(m_X).Unit();
    }

    m_minimizer->Minimize();
    if(m_minimizer->Status() > 0 && !m_2D){
      TVector3 temp = m_Z;
      m_Z = m_X;
      m_X = m_Y;
      m_Y = temp;
      m_minimizer->SetVariableValue(0, 0.);
      m_minimizer->SetVariableValue(1, 0.);
      m_minimizer->SetVariableValue(2, 0.);
      m_minimizer->Minimize();
    }

    const double* PHIs = m_minimizer->X();

    for(int i = 0; i < m_Npair; i++){
      m_Pinv[i].Rotate(PHIs[0], m_Z);
      if(!m_2D){
	m_Pinv[i].Rotate(PHIs[1], m_Y);
	m_Pinv[i].Rotate(PHIs[2], m_X);
      }
    }

    return;
  }

  double MinMassDiffInvJigsaw::EvaluateMetric(const double* param){
    std::vector<TLorentzVector> Pinv;
    for(int i = 0; i < m_Npair; i++){
      Pinv.push_back(m_Pinv[i]);
      Pinv[i].Rotate(param[0], m_Z);
      if(!m_2D){
	Pinv[i].Rotate(param[1], m_Y);
	Pinv[i].Rotate(param[2], m_X);
      }
      Pinv[i] += m_Pvis[i];
    }
    double diff = 0.;
    for(int i = 0; i < m_Npair-1; i++){
      for(int j = i+1; j < m_Npair; j++){
	diff += (Pinv[i].M()-Pinv[j].M())*(Pinv[i].M()-Pinv[j].M());
      }
    }

    return diff;
  }

}
