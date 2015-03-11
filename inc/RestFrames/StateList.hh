#ifndef StateList_HH
#define StateList_HH
#include <iostream>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class State;

  ///////////////////////////////////////////////
  // StateList class
  ///////////////////////////////////////////////
  class StateList {
  public:
    StateList();
    ~StateList();
  
    void Add(State* statePtr);
    void Add(StateList* statesPtr);
    void Remove(const State* statePtr);
    void Remove(const StateList* statesPtr);
    void Clear();
    int GetN() const { return m_States.size(); }
    State* Get(int i) const;
    bool Contains(const State* statePtr) const;
    int GetIndex(const State* statePtr) const;
    int GetIndex(const RestFrame* framePtr) const;
    StateList* Copy() const;
  
    TLorentzVector GetFourVector() const;
    void Boost(const TVector3& B);

  protected:
    vector<State*> m_States;
  };

}

#endif
