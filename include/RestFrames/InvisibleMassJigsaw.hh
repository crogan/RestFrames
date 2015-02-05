#ifndef InvisibleMassJigsaw_HH
#define InvisibleMassJigsaw_HH
#include <iostream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/NVisibleMInvisibleJigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Jigsaw;
  class NVisibleMInvisibleJigsaw;
  class State;
  class StateList;

  ///////////////////////////////////////////////
  // InvisibleMassJigsaw class
  ///////////////////////////////////////////////
  class InvisibleMassJigsaw : public NVisibleMInvisibleJigsaw {
  public:
    InvisibleMassJigsaw(const string& sname, const string& stitle);
    InvisibleMassJigsaw(const string& sname, const string& stitle, int ikey);
    virtual ~InvisibleMassJigsaw();

    virtual bool AnalyzeEvent();
    
  protected:
    void Init();
  
  };

}

#endif
