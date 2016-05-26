/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassesInvJigsaw.cc
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

#include "RestFrames/MinMassesInvJigsaw.hh"
#include "RestFrames/InvisibleState.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  //MinMassesInvJigsaw class methods
  ///////////////////////////////////////////////
  MinMassesInvJigsaw::MinMassesInvJigsaw(const std::string& sname, 
					 const std::string& stitle,
					 int Npair) : 
    InvisibleJigsaw(sname, stitle, Npair, Npair) {}

  MinMassesInvJigsaw::MinMassesInvJigsaw() : InvisibleJigsaw() {}
 
  MinMassesInvJigsaw::~MinMassesInvJigsaw() {}

  MinMassesInvJigsaw& MinMassesInvJigsaw::Empty(){
    return MinMassesInvJigsaw::m_Empty;
  }

  void MinMassesInvJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const { 
    int Nchild = GetNChildren();
    for(int i = 0 ; i < Nchild; i++){
      GetChildState(i).FillInvisibleMassJigsawDependancies(jigsaws);
      int N = m_DependancyStates[i].GetN();
      for(int j = 0; j < N; j++)
	m_DependancyStates[i][j].FillGroupJigsawDependancies(jigsaws);
    }
  }

  double MinMassesInvJigsaw::GetMinimumMass() const {
    int N = GetNChildren();
    if(N <= 0) return 0.;
    double Minv = 0.;
    double Mvis = 0.;
    TLorentzVector Pvis(0.,0.,0.,0.);
    for(int i = 0; i < N; i++){
      Minv += GetChildState(i).GetMinimumMass();
      TLorentzVector pv = m_DependancyStates[i].GetFourVector();
      Mvis += pv.M();
      Pvis += pv;
    }
    return sqrt(std::max(0.,Pvis.M2() - Mvis*Mvis + Minv*Minv));
  }

  bool MinMassesInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind())
      return SetSpirit(false);

    int N = GetNChildren();
    if(N < 0) SetSpirit(false);
     
    std::vector<double> Minv;
    std::vector<double> Mvis;
    std::vector<TLorentzVector> Pvis;
    double MinvTot = 0.;
    double MvisTot = 0.;
    TLorentzVector PvisTot(0.,0.,0.,0.);
    for(int i = 0; i < N; i++){
      Minv.push_back(std::max(0., GetChildState(i).GetMinimumMass()));
      MinvTot += Minv[i];
      TLorentzVector pv = m_DependancyStates[i].GetFourVector();
      Mvis.push_back(std::max(0., pv.M()));
      MvisTot += Mvis[i];
      Pvis.push_back(pv);
      PvisTot += pv;
    }
    TLorentzVector INV = GetParentState().GetFourVector();
    double Minv2 = std::max(0.,PvisTot.M2() - 
			    MvisTot*MvisTot + 
			    MinvTot*MinvTot);
    for(int i = 0; i < N-1; i++){
      PvisTot -= Pvis[i];
      MvisTot -= Mvis[i];
      MinvTot -= Minv[i];
      double Minv2new = std::max(0.,PvisTot.M2() - 
				 MvisTot*MvisTot + 
				 MinvTot*MinvTot);
      TVector3 Boost = INV.BoostVector();
      INV.Boost(-Boost);
      PvisTot.Boost(-Boost);
      Pvis[i].Boost(-Boost);
      TVector3 nPinv = Pvis[i].Vect()-PvisTot.Vect();
      if(nPinv.Mag() > 0.)
	nPinv = (1./nPinv.Mag())*nPinv;
      
      double Pinv = GetP(sqrt(Minv2), Minv[i], sqrt(Minv2new));
      TLorentzVector vPinv;
      vPinv.SetVectM(Pinv*nPinv, Minv[i]);
      INV -= vPinv;
      Minv2 = Minv2new;
      INV.Boost(Boost);
      PvisTot.Boost(Boost);
      vPinv.Boost(Boost);
      GetChildState(i).SetFourVector(vPinv);
    }
    GetChildState(N-1).SetFourVector(INV);
    
    return SetSpirit(true);
  }

  MinMassesInvJigsaw MinMassesInvJigsaw::m_Empty;

}
