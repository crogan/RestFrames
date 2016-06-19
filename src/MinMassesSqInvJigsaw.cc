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

#include <TMatrixD.h>
#include <TDecompSVD.h>

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
    m_Pvis.clear();
    for(int i = 0; i < m_Npair; i++){
      m_Pvis.push_back(GetDependancyStates(i).GetFourVector());
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

  bool MinMassesSqInvJigsaw::AnalyzeEvent(){
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
      ApplyOptimalRotation();
      
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

  // Based on Newton-Raphson root finding
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

  // Optimal rotation found using Singular Value Decomposition
  void MinMassesSqInvJigsaw::ApplyOptimalRotation(){
    // first, check dimensionality of points
    TVector3 Z(0.,0.,0.);
    int index = 0;

    while(Z.Mag() <= 0. && index < m_Npair){
      Z = m_Pinv[index].Vect().Cross(m_Pvis[index].Vect());
      index++;
    }
    if(Z.Mag() <= 0.) return; // already aligned
    Z = Z.Unit();

    bool b_2D = true;
    for(int i = 0; i < m_Npair; i++){
      if(Z.Dot(m_Pvis[i].Vect().Unit()) > 1e-8){
	b_2D = false;
	break;
      }
      if(Z.Dot(m_Pinv[i].Vect().Unit()) > 1e-8){
	b_2D = false;
	break;
      }
    }
    
    // two dimensional problem, one rotation
    if(b_2D){
      TVector3 X(0.,0.,0.);
      for(int i = 0; i < m_Npair; i++)
	X += m_Pvis[i].Vect() - m_Pinv[i].Vect();
      if(X.Mag() <= 0) return; // can't improve

      X = X.Unit();
      TVector3 Y = Z.Cross(X).Unit();

      double num = 0.;
      double den = 0.;
      for(int i = 0; i < m_Npair; i++){
	num += m_Pvis[i].Vect().Dot(Y)*
	  (m_Pvis[i].Vect().Dot(X)-m_Pinv[i].Vect().Dot(X));
	den += m_Pvis[i].Vect().Dot(m_Pinv[i].Vect());
      }
     
      double theta = atan2(num,den);
	for(int i = 0; i < m_Npair; i++)
	  m_Pinv[i].Rotate(theta, -Z);

      return;
    }
    
    // three dimensional problem - R from SVD
    TMatrixD H(3,3);
    double val;
    for(int i = 0; i < 3; i++)
      for(int j = 0; j < 3; j++)
	H(i,j) = 0.;
    
    for(int p = 0; p < m_Npair; p++){
      H(0,0) += m_Pinv[p].Px()*m_Pvis[p].Px();
      H(1,0) += m_Pinv[p].Px()*m_Pvis[p].Py();
      H(2,0) += m_Pinv[p].Px()*m_Pvis[p].Pz();
      H(0,1) += m_Pinv[p].Py()*m_Pvis[p].Px();
      H(1,1) += m_Pinv[p].Py()*m_Pvis[p].Py();
      H(2,1) += m_Pinv[p].Py()*m_Pvis[p].Pz();
      H(0,2) += m_Pinv[p].Pz()*m_Pvis[p].Px();
      H(1,2) += m_Pinv[p].Pz()*m_Pvis[p].Py();
      H(2,2) += m_Pinv[p].Pz()*m_Pvis[p].Pz();
    }

    TDecompSVD SVD(H);
    SVD.Decompose();
    TMatrixD UT(TMatrixD::kTransposed,SVD.GetU());
    TMatrixD R(SVD.GetV(),TMatrixD::kMult,UT);

    TVector3 V;
    for(int p = 0; p < m_Npair; p++){
      V = m_Pinv[p].Vect();
      V.SetXYZ(R(0,0)*V.Px()+R(1,0)*V.Py()+R(2,0)*V.Pz(),
      	       R(0,1)*V.Px()+R(1,1)*V.Py()+R(2,1)*V.Pz(),
      	       R(0,2)*V.Px()+R(1,2)*V.Py()+R(2,2)*V.Pz());
      m_Pinv[p].SetVectM(V, m_Pinv[p].M());
    }

    return;
  }

  MinMassesSqInvJigsaw MinMassesSqInvJigsaw::m_Empty;

}
