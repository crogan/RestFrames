#include "RestFrames/ContraBoostInvariantJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //ContraBoostInvariantJigsaw class methods
  ///////////////////////////////////////////////
  ContraBoostInvariantJigsaw::ContraBoostInvariantJigsaw(const string& sname,const string& stitle) : 
    NVisibleMInvisibleJigsaw(sname, stitle, 2, 2)
  {
    Init();
  }
  ContraBoostInvariantJigsaw::ContraBoostInvariantJigsaw(const string& sname,const string& stitle,int ikey) : 
    NVisibleMInvisibleJigsaw(sname, stitle, ikey, 2, 2)
  {
    Init();
  }
  ContraBoostInvariantJigsaw::~ContraBoostInvariantJigsaw(){
  
  }

  void ContraBoostInvariantJigsaw::Init(){
   
  }

  void ContraBoostInvariantJigsaw::FillInvisibleMassJigsawDependancies(JigsawList* jigsawsPtr){ 
    if(!jigsawsPtr) return;
    
    int Nchild = GetNChildStates();
    for(int i = 0 ; i < Nchild; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(statePtr) statePtr->FillInvisibleMassJigsawDependancies(jigsawsPtr);
      StateList* statesPtr = m_DependancyStates[i];
      int N = statesPtr->GetN();
      for(int j = 0; j < N; j++){
	State* jstatePtr = statesPtr->Get(j);
	if(jstatePtr) jstatePtr->FillGroupJigsawDependancies(jigsawsPtr);
      }
    }
  }

  double ContraBoostInvariantJigsaw::GetMinimumMass(){
    double Minv1 = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(0))->GetMinimumMass();
    double Minv2 = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(1))->GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0]->GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1]->GetFourVector();
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
    m_Spirit = false;
    if(!m_Mind || !m_GroupPtr) return m_Spirit;
    
    CalcCoef();
    TLorentzVector Pvis1 = m_DependancyStates[0]->GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1]->GetFourVector();
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

    m_OutputStatesPtr->Get(0)->SetFourVector(INV1);
    m_OutputStatesPtr->Get(1)->SetFourVector(INV2);
    
    m_Spirit = true;
    return m_Spirit;
  }

  void ContraBoostInvariantJigsaw::CalcCoef(){
    double Minv1 = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(0))->GetMinimumMass();
    double Minv2 = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(1))->GetMinimumMass();
    TLorentzVector Pvis1 = m_DependancyStates[0]->GetFourVector();
    TLorentzVector Pvis2 = m_DependancyStates[1]->GetFourVector();
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
