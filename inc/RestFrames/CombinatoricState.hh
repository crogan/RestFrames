#ifndef CombinatoricState_HH
#define CombinatoricState_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
//#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class State;
  class StateList;

  ///////////////////////////////////////////////
  // CombinatoricState class
  ///////////////////////////////////////////////
  class CombinatoricState : public State {
  public:
    CombinatoricState();
    CombinatoricState(int ikey);
    virtual ~CombinatoricState();
	
    virtual void Boost(const TVector3& B);
    virtual TLorentzVector GetFourVector() const; 

    void ClearElements();
    void AddElement(State* statePtr);
    void AddElement(StateList* statesPtr);
    StateList* GetElements() const;
    int GetNElements() const;
    bool ContainsElement(const State* statePtr) const;

  protected:
    StateList m_Elements;
  
  private:
    void Init();

  };

}

#endif
