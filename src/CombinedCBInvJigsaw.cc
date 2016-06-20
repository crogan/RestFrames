/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinedCBInvJigsaw.cc
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

#include "RestFrames/CombinedCBInvJigsaw.hh"
//#include "RestFrames/InvisibleState.hh"

namespace RestFrames {

  CombinedCBInvJigsaw::CombinedCBInvJigsaw(const std::string& sname,
					     const std::string& stitle,
					     int N_CBjigsaw)
    : InvisibleJigsaw(sname, stitle, 2*N_CBjigsaw, 2*N_CBjigsaw),
      m_NCB(N_CBjigsaw)
  {
    m_InvMassDependancy = true;
  }

  CombinedCBInvJigsaw::CombinedCBInvJigsaw() : InvisibleJigsaw() {}
 
  CombinedCBInvJigsaw::~CombinedCBInvJigsaw() {}

  void CombinedCBInvJigsaw::AddVisibleFrame(const RestFrame& frame,
					    int ijigsaw, int j){
    AddVisibleFrame(frame, 2*ijigsaw+j);
  }
  
  void CombinedCBInvJigsaw::AddVisibleFrames(const ConstRestFrameList& frames,
					     int ijigsaw, int j){
    AddVisibleFrames(frames, 2*ijigsaw+j);
  }
  
  void CombinedCBInvJigsaw::AddInvisibleFrame(const RestFrame& frame,
					      int ijigsaw, int j){
    AddInvisibleFrame(frame, 2*ijigsaw+j);
  }
  
  void CombinedCBInvJigsaw::AddInvisibleFrames(const ConstRestFrameList& frames,
					       int ijigsaw, int j){
    AddInvisibleFrame(frames, 2*ijigsaw+j);
  }
  
  void CombinedCBInvJigsaw::AddJigsaw(const ContraBoostInvJigsaw& jigsaw, int ijigsaw){
    if(!jigsaw) return;
    if(ijigsaw < 0 || ijigsaw >= m_NCB) return;
    
    AddInvisibleFrames(jigsaw.GetChildFrames(0), ijigsaw, 0);
    AddInvisibleFrames(jigsaw.GetChildFrames(1), ijigsaw, 1);
    AddVisibleFrames(jigsaw.GetDependancyFrames(0), ijigsaw, 0);
    AddVisibleFrames(jigsaw.GetDependancyFrames(1), ijigsaw, 1);
  }
  
  double CombinedCBInvJigsaw::GetMinimumMass() const {
    if(m_NCB < 1) return 0.;
    if(m_NCB == 1) return GetCBMinimumMass(0);
    
    double Mmin2 = 0.;
    
    std::vector<double> Minv2;
    for(int i = 0; i < m_NCB; i++){
      Minv2.push_back(GetCBMinimumMass(i));
      Minv2[i] *= Minv2[i];
      Mmin2 += Minv2[i];
    }

    std::vector<TLorentzVector> Pvis;
    std::vector<double> Mvis2;
    for(int i = 0; i < m_NCB; i++){
      Pvis.push_back(GetDependancyStates(2*i+0).GetFourVector()+
		     GetDependancyStates(2*i+1).GetFourVector());
      Mvis2.push_back(Pvis[i].M2());
    }

    for(int i = 0; i < m_NCB-1; i++){
      for(int j = i+1; j < m_NCB; j++){
	if(Minv2[i] > Mvis2[i] || Minv2[j] > Mvis2[j]){
	  Mmin2 += std::max(0.,(Pvis[i]+Pvis[j]).M2()-Mvis2[i]-Mvis2[j])*
	    std::max(((Minv2[i] > 0. && Mvis2[i] <= 0.) ? 1. : Minv2[i]/Mvis2[i]),
		     ((Minv2[j] > 0. && Mvis2[j] <= 0.) ? 1. : Minv2[j]/Mvis2[j]));
	    
	} else {
	  Mmin2 += std::max(0.,(Pvis[i]+Pvis[j]).M2()-Mvis2[i]-Mvis2[j]);
	  Mmin2 += std::max(Minv2[i] - Mvis2[i], Minv2[j] - Mvis2[j])*2.;
	}
      }
    }
    
    return sqrt(std::max(0., Mmin2));
  }
  
  double CombinedCBInvJigsaw::GetCBMinimumMass(int i) const {
    if(!IsSoundMind())
      return 0.;

    double Minv1 = GetChildState(2*i+0).GetMinimumMass();
    double Minv2 = GetChildState(2*i+1).GetMinimumMass();
    TLorentzVector Pvis1 = GetDependancyStates(2*i+0).GetFourVector();
    TLorentzVector Pvis2 = GetDependancyStates(2*i+1).GetFourVector();
    double M12 = (Pvis1+Pvis2).M();
    double Mvis1 = std::max(Pvis1.M(), 0.);
    double Mvis2 = std::max(Pvis2.M(), 0.);

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

  bool CombinedCBInvJigsaw::AnalyzeEvent(){
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

}
