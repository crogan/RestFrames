#include "RestFrames/InvisibleState.hh"
#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/RInvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleState class
  ///////////////////////////////////////////////

  InvisibleState::InvisibleState() : State(){
    Init();
  }

  InvisibleState::~InvisibleState(){
    
  }

  void InvisibleState::Init(){
    m_Type = SInvisible;
  }

  double InvisibleState::GetMinimumMass(){
    InvisibleJigsaw* jigsawPtr = dynamic_cast<InvisibleJigsaw*>(m_ChildJigsawPtr);
    if(jigsawPtr) return jigsawPtr->GetMinimumMass();
    if(GetNFrames() == 1){
      RInvisibleFrame* framePtr = dynamic_cast<RInvisibleFrame*>(m_Frames.Get(0));
      if(framePtr) return framePtr->GetMinimumMass();
    }
    return 0.;
  }

  void InvisibleState::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsawsPtr){
    InvisibleJigsaw* jigsawPtr = dynamic_cast<InvisibleJigsaw*>(m_ChildJigsawPtr);
    if(jigsawPtr) jigsawPtr->FillInvisibleMassJigsawDependancies(jigsawsPtr);
  }

}
