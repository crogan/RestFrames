#ifndef InvisibleState_HH
#define InvisibleState_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/State.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  class State;
  class RInvisibleFrame;
  class InvisibleJigsaw;

  ///////////////////////////////////////////////
  // InvisibleState class
  ///////////////////////////////////////////////
  class InvisibleState : public State {
  public:
    InvisibleState();
    InvisibleState(int ikey);
    virtual ~InvisibleState();

    virtual double GetMinimumMass();
    virtual void FillInvisibleMassJigsawDependancies(JigsawList* jigsawsPtr);

  protected:
    void Init();
 
  };

}

#endif
