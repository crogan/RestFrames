#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // State class
  ///////////////////////////////////////////////
  int State::m_class_key = 0;

  // constructor 
  State::State(){
    Init();
    m_Key = GenKey();
  }

  State::State(int ikey){
    Init();
    m_Key = ikey;
  }

  State::~State(){
    Clear();
  }

  void State::Init(){
    m_ParentJigsawPtr = nullptr;
    m_ChildJigsawPtr = nullptr;
  }

  void State::Clear(){
    m_ParentJigsawPtr = nullptr;
    m_ChildJigsawPtr = nullptr;
    m_P.SetPxPyPzE(0.,0.,0.,0.);
    m_Frames.Clear();
  }

  void State::ClearFrames(){
    m_Frames.Clear();
  }

  State *State::Copy() const {
    State* statePtr = new State(m_Key);
    statePtr->SetParentJigsaw(m_ParentJigsawPtr);
    statePtr->SetChildJigsaw(m_ChildJigsawPtr);
    statePtr->SetFourVector(m_P);
    int N = GetNFrames();
    for(int i = 0; i < N; i++) statePtr->AddFrame(m_Frames.Get(i));
    return statePtr;
  }

  int State::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  void State::Boost(const TVector3& B){
    m_P.Boost(B);
  }

  void State::AddFrame(RestFrame* framePtr){
    if(!framePtr) return;
    if(!framePtr->IsVisibleFrame() && !framePtr->IsInvisibleFrame()) return;
    m_Frames.Add(framePtr);
  }

  void State::AddFrame(RestFrameList* framesPtr){
    if(!framesPtr) return;
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++) AddFrame(framesPtr->Get(i));
  }

  bool State::IsFrame(const RestFrame* framePtr) const {
    if(!framePtr) return false;
    if(m_Frames.GetN() != 1) return false;
    return m_Frames.Get(0)->IsSame(framePtr);
  }

  bool State::IsFrames(const RestFrameList* framesPtr) const {
    if(!framesPtr) return false;
    return m_Frames.IsSame(framesPtr);
  }

  bool State::IsSame(const State* statePtr) const {
    if(!statePtr) return false;
    /*
    RestFrameList* framesPtr = statePtr->GetFrames();
    bool ret = m_Frames.IsSame(framesPtr);
    delete framesPtr;
    return ret;
    */
    return statePtr->GetKey() == m_Key;
  }

  RestFrameList* State::GetFrames() const {
    return m_Frames.Copy();
  }

  RestFrame* State::GetFrame() const {
    if(m_Frames.GetN() != 1) return nullptr;
    return m_Frames.Get(0);
  }

  void State::SetFourVector(const TLorentzVector& V){
    m_P.SetVectM(V.Vect(),V.M());
  }

  TLorentzVector State::GetFourVector() const {
    TLorentzVector V;
    V.SetVectM(m_P.Vect(),m_P.M());
    return V;
  }

  void State::FillGroupJigsawDependancies(JigsawList* jigsawsPtr) const {
    if(!jigsawsPtr) return;
    if(m_ParentJigsawPtr) m_ParentJigsawPtr->FillGroupJigsawDependancies(jigsawsPtr);
  }

  void State::FillStateJigsawDependancies(JigsawList* jigsawsPtr) const {
    if(!jigsawsPtr) return;
    if(m_ParentJigsawPtr) m_ParentJigsawPtr->FillStateJigsawDependancies(jigsawsPtr);
  }

}
