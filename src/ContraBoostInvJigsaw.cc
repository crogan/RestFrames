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
    InvisibleJigsaw(sname, stitle, 2, 2)
  {
    m_InvMassDependancy = true;
  }

  ContraBoostInvJigsaw::ContraBoostInvJigsaw() : InvisibleJigsaw() {}
 
  ContraBoostInvJigsaw::~ContraBoostInvJigsaw() {}

  ContraBoostInvJigsaw& ContraBoostInvJigsaw::Empty(){
    return ContraBoostInvJigsaw::m_Empty;
  }

  double ContraBoostInvJigsaw::GetMinimumMass() const {
    if(!IsSoundMind())
      return 0.;

    double Minv1 = GetChildState(0).GetMinimumMass();
    double Minv2 = GetChildState(1).GetMinimumMass();
    TLorentzVector Pvis1 = GetDependancyStates(0).GetFourVector();
    TLorentzVector Pvis2 = GetDependancyStates(1).GetFourVector();
    double M12 = (Pvis1+Pvis2).M();
    double Mvis1 = std::max(Pvis1.M(), 0.);
    double Mvis2 = std::max(Pvis2.M(), 0.);

    if(Minv1 < -0.5 && Minv2 < -0.5) // children can go tachyonic
      //return sqrt(std::max((Pvis1+Pvis2).M2() - (Mvis1+Mvis2)*(Mvis1+Mvis2),0.));
      return 2.*GetP(M12,Mvis1,Mvis2);

    Minv1 = std::max(Minv1,0.);
    Minv2 = std::max(Minv2,0.);

    double Minv = std::max(0.,std::max(Minv1,Minv2));
    
    double Mvismin = std::min(Mvis1,Mvis2);
    double Mvismax = std::max(Mvis1,Mvis2);
		     
    if(Mvismin <= 0.0 && Minv > 0.) return M12;

    if(Minv <= Mvismin)
      return sqrt( M12*M12 + 4.*(Minv-Mvismin)*(Minv+Mvismax) );
    
    double v1v2 = M12*M12 - Mvis1*Mvis1 - Mvis2*Mvis2;

    if((Mvis1 >= Mvis2 && Mvis1-Mvis2 > Minv2-Minv1) ||
       (Mvis2 >  Mvis1 && Mvis1-Mvis1 > Minv1-Minv2))
      return sqrt(Minv*Minv + v1v2*Minv*Minv/Mvismin/Mvismin +
		  (Minv+Mvismax-Mvismin)*(Minv+Mvismax-Mvismin));
    return sqrt(2.*Minv*Minv + v1v2*Minv*Minv/Mvismin/Mvismin);
  }

  bool ContraBoostInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind())
      return SetSpirit(false);

    TLorentzVector Pvis1 = GetDependancyStates(0).GetFourVector();
    TLorentzVector Pvis2 = GetDependancyStates(1).GetFourVector();
    TLorentzVector INV = GetParentState().GetFourVector();

    // go to the rest frame of (Pvis1+Pvis2+INV system)
    TVector3 Boost = (Pvis1+Pvis2+INV).BoostVector();
    Pvis1.Boost(-Boost);
    Pvis2.Boost(-Boost);
    INV.Boost(-Boost);

    double E1 = Pvis1.E();
    double E2 = Pvis2.E();
    double m1 = std::max(0.,Pvis1.M());
    double m2 = std::max(0.,Pvis2.M());
    TVector3 P1 = Pvis1.Vect();
    TVector3 P2 = Pvis2.Vect();

    double Minv1 = GetChildState(0).GetMinimumMass();
    double Minv2 = GetChildState(1).GetMinimumMass();
    double Minv = std::max(0.,std::max(Minv1,Minv2));
    double Mvis = std::min(m1,m2);

    double k1 = 1.;
    double k2 = 1.;
    if(Minv < Mvis){
      double MC2 = 2.*(E1*E2 + P1.Dot(P2));
      k1 =  (m1+m2)*(m1-m2)*(1.-Minv/Mvis) + MC2-2*m1*m2 + (m1+m2)*fabs(m1-m2)*Minv/Mvis;
      k2 = -(m1+m2)*(m1-m2)*(1.-Minv/Mvis) + MC2-2*m1*m2 + (m1+m2)*fabs(m1-m2)*Minv/Mvis;
    }

    double sumE  = E1+E2;
    double sumkE = k1*E1+k2*E2;

    double N = sumkE > 0. ? sqrt(std::max(INV.M2()-(Pvis1+Pvis2).M2()+sumE*sumE,0.))/sumkE : 0.;

    k1 = 0.5*(1.+N*k1);
    k2 = 0.5*(1.+N*k2);

    TLorentzVector INV1,INV2;
    double Einv1 = (k1-1.)*E1 + k2*E2;
    double Einv2 = k1*E1 + (k2-1.)*E2;
    TVector3 Pinv1 = (k1-1.)*P1 - k2*P2;
    TVector3 Pinv2 = (k2-1.)*P2 - k1*P1;

    INV1.SetPxPyPzE(Pinv1.X(),Pinv1.Y(),Pinv1.Z(),Einv1);
    INV2.SetPxPyPzE(Pinv2.X(),Pinv2.Y(),Pinv2.Z(),Einv2);

    if(Minv1 >= 0. && INV1.M() < Minv1)
      INV1.SetVectM(Pinv1,Minv1);
    if(Minv2 >= 0. && INV2.M() < Minv2)
      INV2.SetVectM(Pinv2,Minv2);

    INV1.Boost(Boost);
    INV2.Boost(Boost);

    GetChildState(0).SetFourVector(INV1);
    GetChildState(1).SetFourVector(INV2);
    
    return SetSpirit(true);
  }

  ContraBoostInvJigsaw ContraBoostInvJigsaw::m_Empty;

}
