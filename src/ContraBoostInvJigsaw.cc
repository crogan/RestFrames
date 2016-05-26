/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ContraBoostInvJigsaw.cc
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

#include "RestFrames/ContraBoostInvJigsaw.hh"
#include "RestFrames/InvisibleState.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  //ContraBoostInvJigsaw class methods
  ///////////////////////////////////////////////
  ContraBoostInvJigsaw::ContraBoostInvJigsaw(const std::string& sname,
					     const std::string& stitle) : 
    InvisibleJigsaw(sname, stitle, 2, 2) {}

  ContraBoostInvJigsaw::ContraBoostInvJigsaw() : InvisibleJigsaw() {}
 
  ContraBoostInvJigsaw::~ContraBoostInvJigsaw() {}

  ContraBoostInvJigsaw& ContraBoostInvJigsaw::Empty(){
    return ContraBoostInvJigsaw::m_Empty;
  }

  void ContraBoostInvJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const { 
    int Nchild = GetNChildren();
    for(int i = 0 ; i < Nchild; i++){
      GetChildState(i).FillInvisibleMassJigsawDependancies(jigsaws);
      int N = m_DependancyStates[i].GetN();
      for(int j = 0; j < N; j++)
	m_DependancyStates[i][j].FillGroupJigsawDependancies(jigsaws);
    }
  }

  double ContraBoostInvJigsaw::GetMinimumMass() const {
    double Minv1 = GetChildState(0).GetMinimumMass();
    double Minv2 = GetChildState(1).GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    double Mvis1 = fabs(Pvis1.M());
    double Mvis2 = fabs(Pvis2.M());
    double Minv = std::max(0.,std::max(Minv1,Minv2));
    double Mvismin = std::min(Mvis1,Mvis2);
    double Mvismax = std::max(Mvis1,Mvis2);

    if(Mvismin <= 0.0 && Minv > 0.0) return (Pvis1+Pvis2).M();

    if(Minv <= Mvismin){
      return sqrt( (Pvis1+Pvis2).M2() + 4.*(Minv-Mvismin)*(Minv+Mvismax) );
    } else {
      return (Pvis1+Pvis2).M()*Minv/Mvismin;
    }
  }

  bool ContraBoostInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind())
      return SetSpirit(false);
    
    CalcCoef();
    double c1 = GetC1();
    double c2 = GetC2();


    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    TLorentzVector INV = GetParentState().GetFourVector();

    // go to the rest frame of (Pvis1+Pvis2+INV system)
    TVector3 Boost = (Pvis1+Pvis2+INV).BoostVector();
    Pvis1.Boost(-Boost);
    Pvis2.Boost(-Boost);
    INV.Boost(-Boost);

    double E1 = Pvis1.E();
    double E2 = Pvis2.E();
    TVector3 P1 = Pvis1.Vect();
    TVector3 P2 = Pvis2.Vect();

    double N = (E1+E2 + sqrt( (E1+E2)*(E1+E2) - (Pvis1+Pvis2).M2() + INV.M2() ))/(c1*E1+c2*E2)/2.;

    c1 *= N;
    c2 *= N;

    TLorentzVector INV1,INV2;
    double Einv1 = (c1-1.)*E1 + c2*E2;
    double Einv2 = c1*E1 + (c2-1.)*E2;
    TVector3 Pinv1 = (c1-1.)*P1 - c2*P2;
    TVector3 Pinv2 = (c2-1.)*P2 - c1*P1;

    INV1.SetPxPyPzE(Pinv1.X(),Pinv1.Y(),Pinv1.Z(),Einv1);
    INV2.SetPxPyPzE(Pinv2.X(),Pinv2.Y(),Pinv2.Z(),Einv2);
    INV1.Boost(Boost);
    INV2.Boost(Boost);

    GetChildState(0).SetFourVector(INV1);
    GetChildState(1).SetFourVector(INV2);
    
    return SetSpirit(true);
  }

  void ContraBoostInvJigsaw::CalcCoef(){
    double Minv1 = GetChildState(0).GetMinimumMass();
    double Minv2 = GetChildState(1).GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    double m1 = std::max(0.,Pvis1.M());
    double m2 = std::max(0.,Pvis2.M());
    double Minv = std::max(Minv1,Minv2);
    double Mvis = std::min(m1,m2);

    if(Minv >= Mvis){
      m_C1 = 1.;
      m_C2 = 1.;
      return;
    }
    // go to the rest frame of the vis+inv system
    TLorentzVector INV = GetParentState().GetFourVector();
    TVector3 Boost = (Pvis1+Pvis2+INV).BoostVector();
    Pvis1.Boost(-Boost);
    Pvis2.Boost(-Boost);
  
    double MC2 = 2.*( Pvis1.E()*Pvis2.E() + Pvis1.Vect().Dot(Pvis2.Vect()) );
    double k1 =  (m1+m2)*(m1-m2)*(1.-Minv/Mvis) + MC2-2*m1*m2 + (m1+m2)*fabs(m1-m2)*Minv/Mvis;
    double k2 = -(m1+m2)*(m1-m2)*(1.-Minv/Mvis) + MC2-2*m1*m2 + (m1+m2)*fabs(m1-m2)*Minv/Mvis;
    double Xbar = sqrt( (k1+k2)*(k1+k2)*(MC2*MC2-4*m1*m1*m2*m2) + 16.*Minv*Minv*(k1*k1*m1*m1 + k2*k2*m2*m2 + k1*k2*MC2) );
    double N = ( fabs(k1*m1*m1-k2*m2*m2) - 0.5*fabs(k2-k1)*MC2 + 0.5*Xbar )/(k1*k1*m1*m1 + k2*k2*m2*m2 + k1*k2*MC2);
    m_C1 = 0.5*(1.+N*k1);
    m_C2 = 0.5*(1.+N*k2);
    N = sqrt(m_C1*m_C2);
    m_C1 /= N;
    m_C2 /= N;
  }

  ContraBoostInvJigsaw ContraBoostInvJigsaw::m_Empty;

}
