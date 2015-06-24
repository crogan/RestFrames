#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // StateList class methods
  ///////////////////////////////////////////////
  // StateList::StateList() {}

  // StateList::~StateList(){
  //   Clear();
  // }

  // void StateList::Clear(){
  //   m_States.clear();
  // }

  // State* StateList::Get(int i) const { 
  //   if(i < 0 || i >= GetN()) return nullptr;
  //   return m_States[i];
  // }

  // void StateList::Add(State* statePtr){
  //   int N = m_States.size();
  //   for(int i = 0; i < N; i++)
  //     if(m_States[i]->IsSame(statePtr)) return;
  //   m_States.push_back(statePtr);
  // }

  // void StateList::Add(StateList* statesPtr){
  //   int N = statesPtr->GetN();
  //   for(int i = 0; i < N; i++) Add(statesPtr->Get(i));
  // }

  // void StateList::Remove(const State* statePtr){
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_States[i]->IsSame(statePtr)){
  // 	m_States.erase(m_States.begin()+i);
  // 	return;
  //     }
  //   }
  // }
  // void StateList::Remove(const StateList* statesPtr){
  //   int N = statesPtr->GetN();
  //   for(int i = 0; i < N; i++) Remove(statesPtr->Get(i));
  // }

  // bool StateList::Contains(const State* statePtr) const {
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_States[i]->IsSame(statePtr)) return true;
  //   }
  //   return false;
  // }

  // int StateList::GetIndex(const State* statePtr) const {
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_States[i]->IsSame(statePtr)) return i;
  //   }
  //   return -1;
  // }

  // int StateList::GetIndexFrame(const RestFrame* framePtr) const {
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_States[i]->IsFrame(framePtr)) return i;
  //   }
  //   return -1;
  // }

  // StateList* StateList::Copy() const {
  //   StateList* statesPtr = new StateList();
  //   int N = GetN();
  //   for(int i = 0; i < N; i++) statesPtr->Add(m_States[i]);
  //   return statesPtr;
  // }

  // TLorentzVector StateList::GetFourVector() const {
  //   TLorentzVector V(0.,0.,0.,0.);
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     V += m_States[i]->GetFourVector();
  //   }
  //   return V;
  // }

  // void StateList::Boost(const TVector3& B){
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     m_States[i]->Boost(B);
  //   }
  // }

  StateList::StateList() : RFList<State>() {}

  StateList::~StateList(){}

  StateList::StateList(const RFList<State>& list){
    int N = list.GetN();
    for(int i = 0; i < N; i++)
      Add(list.Get(i));
  }

  StateList::StateList(const RFList<State>* listPtr){
    if(!listPtr) return;
    int N = listPtr->GetN();
    for(int i = 0; i < N; i++)
      Add(listPtr->Get(i));
  }

   StateList::StateList(const vector<State*>& list){
    int N = list.size();
    for(int i = 0; i < N; i++)
      Add(list[i]);
  }

  StateList::StateList(const vector<State*>* listPtr){
    if(!listPtr) return;
    int N = listPtr->size();
    for(int i = 0; i < N; i++)
      Add(listPtr->at(i));
  }

  int StateList::GetIndexFrame(const RestFrame* framePtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsFrame(framePtr)) return i;
    }
    return -1;
  }

  StateList* StateList::Copy() const {
    StateList* statesPtr = new StateList(m_Objs);
    // int N = GetN();
    // for(int i = 0; i < N; i++) statesPtr->Add(m_Objs[i]);
    return statesPtr;
  }

   TLorentzVector StateList::GetFourVector() const {
    TLorentzVector V(0.,0.,0.,0.);
    int N = GetN();
    for(int i = 0; i < N; i++){
      V += m_Objs[i]->GetFourVector();
    }
    return V;
  }

  void StateList::Boost(const TVector3& B){
    int N = GetN();
    for(int i = 0; i < N; i++){
      m_Objs[i]->Boost(B);
    }
  }

}
