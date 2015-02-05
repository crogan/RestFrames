#ifndef RVisibleFrame_HH
#define RVisibleFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/VisibleFrame.hh"
#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RFrame;
  class VisibleFrame;

  ///////////////////////////////////////////////
  // RVisibleFrame class
  ///////////////////////////////////////////////
  class RVisibleFrame : public VisibleFrame, public RFrame{
  public:
    RVisibleFrame(const string& sname, const string& stitle, int key);
    RVisibleFrame(const string& sname, const string& stitle);
    virtual ~RVisibleFrame();

  private:
    void Init();

  };

}

#endif
