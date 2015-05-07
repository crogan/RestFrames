#include "RestFrames/CombinatoricGroup.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricGroup class
  // a combinatoric collection of particles
  ///////////////////////////////////////////////

  CombinatoricGroup::CombinatoricGroup(const string& sname,const string& stitle,int ikey) : 
    Group(sname, stitle, ikey)
  {
    Init();
  }

  CombinatoricGroup::CombinatoricGroup(const string& sname,const string& stitle) : 
    Group(sname, stitle)
  {
    Init();
  }

  CombinatoricGroup::~CombinatoricGroup(){
    Clear();
  }

  void CombinatoricGroup::Init(){
    m_Type = GCombinatoric;
  }

  void CombinatoricGroup::Clear(){
    ClearElements();
    m_NElementsForFrame.clear();
    m_NExclusiveElementsForFrame.clear(); 
  }

  void CombinatoricGroup::AddFrame(RestFrame& frame){
    AddFrame(&frame);
  }

  void CombinatoricGroup::AddFrame(RestFrame* framePtr){
    if(!framePtr->IsRFrame() || !framePtr->IsVisibleFrame()) return;
    RFrame *ptr = dynamic_cast<RFrame*>(framePtr);
    if(ptr){
      ptr->SetGroup(this);
      m_Frames.Add(framePtr);
      if(m_Frames.GetN() == int(m_NElementsForFrame.size())) return;
      m_NElementsForFrame.push_back(1.);
      m_NExclusiveElementsForFrame.push_back(false);
    }
  }

  vector<int> m_NElementsForFrame;
    vector<bool> m_NExclusiveElementsForFrame; 

  void CombinatoricGroup::SetNElementsForFrame(const RestFrame& frame, int N, bool exclusive_N){
    SetNElementsForFrame(&frame,N,exclusive_N);
  }
  
  void CombinatoricGroup::SetNElementsForFrame(const RestFrame* framePtr, int N, bool exclusive_N){
    int index = m_Frames.GetIndex(framePtr);
    if(index < 0) return;
    m_NElementsForFrame[index] = N;
    m_NExclusiveElementsForFrame[index] = exclusive_N;
  }

  void CombinatoricGroup::GetNElementsForFrame(const RestFrame* framePtr, int& N, bool& exclusive_N){
    int index = m_Frames.GetIndex(framePtr);
    if(index < 0){
      N = -1;
      exclusive_N = false;
      return;
    }
    N = m_NElementsForFrame[index];
    exclusive_N = m_NExclusiveElementsForFrame[index];
  }

  bool CombinatoricGroup::AddJigsaw(Jigsaw& jigsaw){
    return AddJigsaw(&jigsaw);
  }

  bool CombinatoricGroup::AddJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return false;
    if(jigsawPtr->GetGroup()) return false;
    if(!jigsawPtr->IsCombinatoricJigsaw()) return false;
    if(m_JigsawsToUsePtr->Add(jigsawPtr)) jigsawPtr->SetGroup(this);
    return true;
  }

  State* CombinatoricGroup::InitializeGroupState(){
    return new CombinatoricState();
  }

  void CombinatoricGroup::ClearElements(){
    int N = GetNElements();
   
    for(int i = 0; i < N; i++){
      delete m_StateElements.Get(i);
    }
    m_StateElements.Clear();
  }

  void CombinatoricGroup::AddElement(State* statePtr){
    m_StateElements.Add(statePtr);
  }

  int CombinatoricGroup::GetNElements() const{
    return m_StateElements.GetN();
  }

  // Event analysis functions
  void CombinatoricGroup::ClearEvent(){
    ClearFourVectors();
  }
 
  bool CombinatoricGroup::AnalyzeEvent(){
    m_Spirit = false;
    if(!m_Mind || !m_GroupStatePtr){
      return m_Spirit;
    }
    
    CombinatoricState* group_statePtr = dynamic_cast<CombinatoricState*>(m_GroupStatePtr);
    if(!group_statePtr) return m_Spirit;
    
    group_statePtr->ClearElements();
    group_statePtr->AddElement(&m_StateElements);    

    m_Spirit = true;
    return m_Spirit;
  }

  void CombinatoricGroup::ClearFourVectors(){
    ClearElements();
   }

  GroupElementID CombinatoricGroup::AddLabFrameFourVector(const TLorentzVector& V){
    State* statePtr;
    int Nelements = GetNElements();
    int Nkeys = m_StateKeys.size();
    if(Nelements < Nkeys){
      statePtr = new State(m_StateKeys[Nelements]);
    } else {
      statePtr = new State();
      m_StateKeys.push_back(statePtr->GetKey());
    }
    TLorentzVector P = V;
    if(P.M() < 0.) P.SetVectM(V.Vect(),0.);
    statePtr->SetFourVector(P);
    AddElement(statePtr);
   
    return statePtr;
  }

  int CombinatoricGroup::GetNFourVectors() const{
    return GetNElements();
  }

  const RestFrame* CombinatoricGroup::GetFrame(const GroupElementID elementID){
    //State* elementPtr = (State*)elementID;
    const State* elementPtr = elementID;
    int N = m_StatesPtr->GetN();
    for(int i = N-1; i >= 0; i--){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(m_StatesPtr->Get(i));
      if(!statePtr) continue;
      if(statePtr->ContainsElement(elementPtr)){
	RestFrame* framePtr = statePtr->GetFrame();
	if(framePtr) return framePtr;
      }
    }
    return nullptr;
  }

  TLorentzVector CombinatoricGroup::GetLabFrameFourVector(const GroupElementID elementID){
    const State* elementPtr = elementID;
    TLorentzVector P(0.,0.,0.,0.);
    int N = m_StatesPtr->GetN();
    for(int i = N-1; i >= 0; i--){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(m_StatesPtr->Get(i));
      if(!statePtr) continue;
      if(statePtr->ContainsElement(elementPtr)){
	P = elementPtr->GetFourVector();
	break;
      }
    }
    return P;
  }

  int CombinatoricGroup::GetNElementsInFrame(const RestFrame& frame){
    return GetNElementsInFrame(&frame);
  }
  int CombinatoricGroup::GetNElementsInFrame(const RestFrame* framePtr){
    if(!framePtr) return -1;
    if(!ContainsFrame(framePtr)) return -1;
    CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(GetState(framePtr));
    if(!statePtr) return -1;
    return statePtr->GetNElements();
  }

}
