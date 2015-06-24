#ifndef InvisibleState_HH
#define InvisibleState_HH

#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  class State;

  ///////////////////////////////////////////////
  // InvisibleState class
  ///////////////////////////////////////////////
  class InvisibleState : public State {
  public:
    InvisibleState();
    virtual ~InvisibleState();

    virtual double GetMinimumMass();
    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsawsPtr);

  protected:
    void Init();
 
  };

}

#endif
