#include "RestFrames/InvisibleGroup.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleGroup class
  ///////////////////////////////////////////////

  InvisibleGroup::InvisibleGroup(const string& sname, const string& stitle, int ikey) : 
    Group(sname, stitle, ikey)
  {
    Init();
  }

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
    if(!m_Mind) return;
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
    m_Spirit = false;
    if(!m_Mind || !m_GroupStatePtr){
      return m_Spirit;
    }

    m_GroupStatePtr->SetFourVector(m_Lab_P);

    m_Spirit = true;
    return m_Spirit;
  }
  
}
