#ifndef MinimizeMassesCombinatoricJigsaw_HH
#define MinimizeMassesCombinatoricJigsaw_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/CombinatoricJigsaw.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Jigsaw;
  class JigsawList;
  class CombinatoricJigsaw;
  class State;
  class StateList;

  ///////////////////////////////////////////////
  // MinimizeMassesCombinatoricJigsaw class
  ///////////////////////////////////////////////
  class MinimizeMassesCombinatoricJigsaw : public CombinatoricJigsaw{
  public:
    MinimizeMassesCombinatoricJigsaw(const string& sname, const string& stitle);
    MinimizeMassesCombinatoricJigsaw(const string& sname, const string& stitle, int ikey);
    virtual ~MinimizeMassesCombinatoricJigsaw();

    virtual bool AnalyzeEvent();

  private:
    void Init();

  };

}

#endif
