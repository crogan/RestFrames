#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleState.hh"
#include "RestFrames/StateList.hh"

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
 
  InvisibleMassJigsaw::~InvisibleMassJigsaw(){
  
  }

  void InvisibleMassJigsaw::Init(){
    m_Priority = -2;
  }

  bool InvisibleMassJigsaw::AnalyzeEvent(){
    if(!IsSoundMind() || !m_GroupPtr){
      m_Log << LogWarning;
      m_Log << "Unable to analyze event. ";
      m_Log << "Requires successfull call to \"InitializeAnalysis()\" ";
      m_Log << "from LabFrame" << m_End;
      SetSpirit(false);
      return false;
    }

    TLorentzVector inv_P = m_InputStatePtr->GetFourVector();
    double M = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(0))->GetMinimumMass();

    inv_P.SetVectM(inv_P.Vect(),M);
    m_OutputStatesPtr->Get(0)->SetFourVector(inv_P);

    SetSpirit(true);
    return true;
  }

}
