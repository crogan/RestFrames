#include "RestFrames/CombinatoricState.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricState class
  ///////////////////////////////////////////////

  CombinatoricState::CombinatoricState(int ikey) : 
    State(ikey)
  {
    Init();
  }

  CombinatoricState::CombinatoricState() : 
    State()
  {
    Init();
  }

  CombinatoricState::~CombinatoricState(){
    ClearElements();
  }

  void CombinatoricState::Init(){
    m_Type = SCombinatoric;
  }

  void CombinatoricState::ClearElements(){
    /*
    int N = GetNElements();
    for(int i = 0; i < N; i++){
      delete m_Elements.Get(i);
    }
    */
    m_Elements.Clear();
  }

  void CombinatoricState::AddElement(State* statePtr){
    if(!statePtr) return;
    //m_Elements.Add(statePtr->Copy());
    m_Elements.Add(statePtr);
  }

  void CombinatoricState::AddElement(StateList* statesPtr){
    if(!statesPtr) return;
    int N = statesPtr->GetN();
    for(int i = 0; i < N; i++) AddElement(statesPtr->Get(i));
  }

  StateList* CombinatoricState::GetElements() const {
    return m_Elements.Copy();
  }

  int CombinatoricState::GetNElements() const {
    return m_Elements.GetN();
  }

  bool CombinatoricState::ContainsElement(const State* statePtr) const {
    return m_Elements.Contains(statePtr);
  }

  void CombinatoricState::Boost(const TVector3& B){
    m_Elements.Boost(B);
    m_P.Boost(B);
  }

  TLorentzVector CombinatoricState::GetFourVector() const {
    if(GetNElements() > 0) return m_Elements.GetFourVector();
    TLorentzVector V(0.,0.,0.,0.);
    return V;
  }

}
