/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ContraBoostInvariantJigsaw.cc
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

#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/InvisibleState.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //ContraBoostInvariantJigsaw class methods
  ///////////////////////////////////////////////
  ContraBoostInvariantJigsaw::ContraBoostInvariantJigsaw(const string& sname,const string& stitle) : 
    InvisibleJigsaw(sname, stitle, 2, 2)
  {
    Init();
    cout << "ADADADADA" << endl;
  }
 
  ContraBoostInvariantJigsaw::~ContraBoostInvariantJigsaw(){
  
  }

  void ContraBoostInvariantJigsaw::Init(){
   
  }

  void ContraBoostInvariantJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws){ 
    int Nchild = GetNChildStates();
    for(int i = 0 ; i < Nchild; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(statePtr) statePtr->FillInvisibleMassJigsawDependancies(jigsaws);
      StateList states;
      states.Add(m_DependancyStates[i]);
      int N = states.GetN();
      for(int j = 0; j < N; j++){
	State* jstatePtr = states.Get(j);
	if(jstatePtr) jstatePtr->FillGroupJigsawDependancies(jigsaws);
      }
    }
  }

  double ContraBoostInvariantJigsaw::GetMinimumMass(){
    double Minv1 = dynamic_cast<InvisibleState*>(m_OutputStates.Get(0))->GetMinimumMass();
    double Minv2 = dynamic_cast<InvisibleState*>(m_OutputStates.Get(1))->GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    double Mvis1 = fabs(Pvis1.M());
    double Mvis2 = fabs(Pvis2.M());
    double Minv = max(0.,max(Minv1,Minv2));
    double Mvismin = min(Mvis1,Mvis2);
    double Mvismax = max(Mvis1,Mvis2);

    if(Mvismin <= 0.0 && Minv > 0.0) return (Pvis1+Pvis2).M();

    if(Minv <= Mvismin){
      return sqrt( (Pvis1+Pvis2).M2() + 4.*(Minv-Mvismin)*(Minv+Mvismax) );
    } else {
      return (Pvis1+Pvis2).M()*Minv/Mvismin;
    }
  }

  bool ContraBoostInvariantJigsaw::AnalyzeEvent(){
    if(!IsSoundMind()){
      SetSpirit(false);
      return false;
    }
    
    CalcCoef();
    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    TLorentzVector INV = m_InputStatePtr->GetFourVector();

    // go to the rest frame of (Pvis1+Pvis2+INV system)
    TVector3 Boost = (Pvis1+Pvis2+INV).BoostVector();
    Pvis1.Boost(-Boost);
    Pvis2.Boost(-Boost);
    INV.Boost(-Boost);

    double E1 = Pvis1.E();
    double E2 = Pvis2.E();
    TVector3 P1 = Pvis1.Vect();
    TVector3 P2 = Pvis2.Vect();
    double c1 = GetC1();
    double c2 = GetC2();

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

    m_OutputStates.Get(0)->SetFourVector(INV1);
    m_OutputStates.Get(1)->SetFourVector(INV2);
    
    m_Spirit = true;
    return m_Spirit;
  }

  void ContraBoostInvariantJigsaw::CalcCoef(){
    double Minv1 = dynamic_cast<InvisibleState*>(m_OutputStates.Get(0))->GetMinimumMass();
    double Minv2 = dynamic_cast<InvisibleState*>(m_OutputStates.Get(1))->GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0].GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1].GetFourVector();
    double m1 = fabs(Pvis1.M());
    double m2 = fabs(Pvis2.M());
    double Minv = max(Minv1,Minv2);
    double Mvis = min(m1,m2);

    if(Minv >= Mvis){
      m_C1 = 1.;
      m_C2 = 1.;
      return;
    }
    // go to the rest frame of the vis+inv system
    TLorentzVector INV = m_InputStatePtr->GetFourVector();
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

}
