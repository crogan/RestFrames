/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassesSqInvJigsaw.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Feb
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

#include "RestFrames/MinMassesSqInvJigsaw.hh"
#include "RestFrames/InvisibleState.hh"

namespace RestFrames {

  MinMassesSqInvJigsaw::MinMassesSqInvJigsaw(const std::string& sname, 
					     const std::string& stitle,
					     int N_vis_inv_pair) : 
    InvisibleJigsaw(sname, stitle, N_vis_inv_pair, N_vis_inv_pair), 
    m_Npair(N_vis_inv_pair) {}

  MinMassesSqInvJigsaw::MinMassesSqInvJigsaw() 
    : InvisibleJigsaw(), m_Npair(0) {}
 
  MinMassesSqInvJigsaw::~MinMassesSqInvJigsaw() {}

  MinMassesSqInvJigsaw& MinMassesSqInvJigsaw::Empty(){
    return MinMassesSqInvJigsaw::m_Empty;
  }

  double MinMassesSqInvJigsaw::GetMinimumMass() const {
    if(!IsSoundMind())
      return 0.;

    if(m_Npair <= 0) 
      return 0.;
    if(m_Npair == 1) 
      return std::max(0.,GetChildState(0).GetMinimumMass());
    
    TLorentzVector PvisTOT(0.,0.,0.,0.);
    std::vector<TLorentzVector> Pvis;
    for(int i = 0; i < m_Npair; i++){
      Pvis.push_back(GetDependancyStates(i).GetFourVector());
      PvisTOT += Pvis[i];
    }

    double ECM = 0.;
    double Minv;
    TVector3 Boost = PvisTOT.BoostVector();
    for(int i = 0; i < m_Npair; i++){
      Pvis[i].Boost(-Boost);
      Minv = std::max(0.,GetChildState(i).GetMinimumMass());
      ECM += sqrt(Pvis[i].P()*Pvis[i].P() + Minv*Minv);
    }

    return ECM;
  }

  bool MinMassesSqInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind())
      return SetSpirit(false);

    if(m_Npair <= 1) 
      return false;

    TLorentzVector INV = GetParentState().GetFourVector();
    double Minv = INV.M();

    TLorentzVector VIS(0.,0.,0.,0.);
    std::vector<TLorentzVector> Pvis;
    for(int i = 0; i < m_Npair; i++){
      Pvis.push_back(GetDependancyStates(i).GetFourVector());
      VIS += Pvis[i];
    }

    // go to INV+VIS CM frame
    TVector3 BoostCM = (VIS+INV).BoostVector();
    INV.Boost(-BoostCM);
    VIS.Boost(-BoostCM);
    for(int i = 0; i < m_Npair; i++)
      Pvis[i].Boost(-BoostCM);

    // INV states defined in the VIS frame
    TVector3 BoostVIS = VIS.BoostVector();
    m_Pinv.clear();
    m_Minv.clear();
    for(int i = 0; i < m_Npair; i++){
      m_Minv.push_back(std::max(0.,GetChildState(i).GetMinimumMass()));
      m_Pinv.push_back(Pvis[i]);
      m_Pinv[i].Boost(-BoostVIS);
      m_Pinv[i].SetVectM(m_Pinv[i].Vect(), m_Minv[i]);
    }

    // VIS states in INV frame
    TVector3 BoostINV = INV.BoostVector();
    for(int i = 0; i < m_Npair; i++)
      Pvis[i].Boost(-BoostINV);

    if(m_Npair == 2){
      TVector3 Vdiff = (Pvis[0].Vect()-Pvis[1].Vect()).Unit();
      double pinv = GetP(Minv, m_Minv[0], m_Minv[1]);
      m_Pinv[0].SetVectM( pinv*Vdiff, m_Minv[0]);
      m_Pinv[1].SetVectM(-pinv*Vdiff, m_Minv[1]);
    } else {
      TVector3 Z = BoostINV.Unit();
      TVector3 vDelta(0.,0.,0.);
      std::vector<double> delta;
      for(int i = 0; i < m_Npair; i++){
	delta.push_back(Pvis[i].Vect().Dot(Z)-
			m_Pinv[i].Vect().Dot(Z));
	vDelta += delta[i]*m_Pinv[i].Vect();
      }
      
      if(vDelta.Mag() > 0.){
	
	TVector3 Y = Z.Cross(vDelta).Unit();
	TVector3 X = Y.Cross(Z).Unit();
	
	double num = 0.;
	double den = 0.;
	for(int i = 0; i < m_Npair; i++){
	  num += delta[i]*m_Pinv[i].Vect().Dot(X);
	  den += m_Pinv[i].Vect().Dot(X)*Pvis[i].Vect().Dot(X) +
	    m_Pinv[i].Vect().Dot(Z)*Pvis[i].Vect().Dot(Z);
	}
	
	double theta = atan2(num,den);
	for(int i = 0; i < m_Npair; i++)
	  m_Pinv[i].Rotate(theta, -Y);
	
      } // non-zero vDelta
      
      double k = GetPScale(Minv);
      for(int i = 0; i < m_Npair; i++)
	m_Pinv[i].SetVectM(k*m_Pinv[i].Vect(), m_Minv[i]);

    }
    
    // return to original frame
    for(int i = 0; i < m_Npair; i++){
      m_Pinv[i].Boost(BoostINV);
      m_Pinv[i].Boost(BoostCM);
      GetChildState(i).SetFourVector(m_Pinv[i]);
    }
    
    return SetSpirit(true);
  }

  double MinMassesSqInvJigsaw::GetPScale(double Minv){
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

  MinMassesSqInvJigsaw MinMassesSqInvJigsaw::m_Empty;

}
