#ifndef InvisibleRapidityJigsaw_HH
#define InvisibleRapidityJigsaw_HH
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
  // InvisibleRapidityJigsaw class
  ///////////////////////////////////////////////
  class InvisibleRapidityJigsaw : public NVisibleMInvisibleJigsaw {
  public:
    //constructor and destructor
    InvisibleRapidityJigsaw(const string& sname, const string& stitle);
    InvisibleRapidityJigsaw(const string& sname, const string& stitle, int ikey);
    virtual ~InvisibleRapidityJigsaw();

    virtual bool AnalyzeEvent();

  protected:
    void Init();
  
  };

}

#endif
