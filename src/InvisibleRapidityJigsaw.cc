#include "RestFrames/InvisibleRapidityJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //InvisibleRapidityJigsaw class methods
  ///////////////////////////////////////////////
  InvisibleRapidityJigsaw::InvisibleRapidityJigsaw(const string& sname, const string& stitle) : 
    NVisibleMInvisibleJigsaw(sname, stitle, 1, 1)
  {
    Init();
  }
  InvisibleRapidityJigsaw::InvisibleRapidityJigsaw(const string& sname, const string& stitle, int ikey) : 
    NVisibleMInvisibleJigsaw(sname, stitle, ikey, 1, 1)
  {
    Init();
  }
  InvisibleRapidityJigsaw::~InvisibleRapidityJigsaw(){
  
  }

  void InvisibleRapidityJigsaw::Init(){
    m_Priority = -1;
  }

  bool InvisibleRapidityJigsaw::AnalyzeEvent(){
    m_Spirit = false;
    if(!m_Mind || !m_GroupPtr) return m_Spirit;
    
    TLorentzVector inv_P = m_InputStatePtr->GetFourVector();
    TLorentzVector vis_P = m_DependancyStates[0]->GetFourVector();

    // double Minv = inv_P.M();
    // TVector3 Pinv = inv_P.Vect();
    // Pinv.SetZ(0.0);
    // double Pz = vis_P.Pz()*sqrt(Minv*Minv+Pinv.Mag2())/sqrt(vis_P.E()*vis_P.E()-vis_P.Pz()*vis_P.Pz());
    // Pinv.SetXYZ(Pinv.X(),Pinv.Y(),Pz);
    // inv_P.SetVectM(Pinv,Minv);
    // //inv_P.SetVectM(inv_P.Vect(),M);

    inv_P.SetZ(0.0);
    TVector3 boostZ = vis_P.BoostVector();
    boostZ.SetX(0.);
    boostZ.SetY(0.);
    inv_P.Boost(boostZ);

    m_OutputStatesPtr->Get(0)->SetFourVector(inv_P);
    
    m_Spirit = true;
    return m_Spirit;
  }

}
