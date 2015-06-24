#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/RFrame.hh"
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleGroup class
  ///////////////////////////////////////////////

  InvisibleGroup::InvisibleGroup(const string& sname, const string& stitle) : 
    Group(sname, stitle)
  {
    Init();
  }

  InvisibleGroup::~InvisibleGroup(){
   
  }

  void InvisibleGroup::Init(){
    m_Type = GInvisible;
  }

  void InvisibleGroup::AddFrame(RestFrame& frame){
    AddFrame(&frame);
  }

  void InvisibleGroup::AddFrame(RestFrame* framePtr){
    if(!framePtr) return;
    if(!framePtr->IsRFrame() || !framePtr->IsInvisibleFrame()) return;
    RFrame *ptr = dynamic_cast<RFrame*>(framePtr);
    if(ptr){
      ptr->SetGroup(this);
      m_Frames.Add(framePtr);
    }
  }

  bool InvisibleGroup::AddJigsaw(Jigsaw& jigsaw){
    return AddJigsaw(&jigsaw);
  }

  bool InvisibleGroup::AddJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return false;
    if(jigsawPtr->GetGroup()) return false;
    if(!jigsawPtr->IsInvisibleJigsaw()) return false;
    if(m_JigsawsToUsePtr->Add(jigsawPtr)) jigsawPtr->SetGroup(this);
    return true;
  }

  State* InvisibleGroup::InitializeGroupState(){
    return new InvisibleState();
  }

  // Event analysis functions
  void InvisibleGroup::ClearEvent(){
    if(!IsSoundMind()) return;
    m_Lab_P.SetPxPyPzE(0.,0.,0.,0.);
  }
  
  void InvisibleGroup::SetLabFrameFourVector(const TLorentzVector& V){
    m_Lab_P.SetVectM(V.Vect(),V.M());
  }

  void InvisibleGroup::SetLabFrameThreeVector(const TVector3& V){
    m_Lab_P.SetVectM(V,0.0);
  }

  TLorentzVector InvisibleGroup::GetLabFrameFourVector(){
    return m_Lab_P;
  }

  bool InvisibleGroup::AnalyzeEvent(){
    if(!IsSoundMind() || !m_GroupStatePtr){
      m_Log << LogWarning;
      m_Log << "Unable to analyze event. ";
      m_Log << "Requires successfull call to \"InitializeAnalysis()\" ";
      m_Log << "from LabFrame" << m_End;
      SetSpirit(false);
      return false;
    }

    m_GroupStatePtr->SetFourVector(m_Lab_P);

    SetSpirit(true);
    return true;
  }
  
}
