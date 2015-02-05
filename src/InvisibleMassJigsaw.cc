#include "RestFrames/InvisibleMassJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //InvisibleMassJigsaw class methods
  ///////////////////////////////////////////////
  InvisibleMassJigsaw::InvisibleMassJigsaw(const string& sname, const string& stitle) : 
    NVisibleMInvisibleJigsaw(sname, stitle, 0, 1)
  {
    Init();
  }
  InvisibleMassJigsaw::InvisibleMassJigsaw(const string& sname, const string& stitle, int ikey) : 
    NVisibleMInvisibleJigsaw(sname, stitle, ikey, 0, 1)
  {
    Init();
  }
  InvisibleMassJigsaw::~InvisibleMassJigsaw(){
  
  }

  void InvisibleMassJigsaw::Init(){
    m_Priority = -2;
  }

  bool InvisibleMassJigsaw::AnalyzeEvent(){
    m_Spirit = false;
    if(!m_Mind || !m_GroupPtr) return m_Spirit;

    TLorentzVector inv_P = m_InputStatePtr->GetFourVector();
    double M = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(0))->GetMinimumMass();

    inv_P.SetVectM(inv_P.Vect(),M);
    m_OutputStatesPtr->Get(0)->SetFourVector(inv_P);

    m_Spirit = true;
    return m_Spirit;
  }

}
