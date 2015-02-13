#ifndef GDecayFrame_HH
#define GDecayFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/DecayFrame.hh"
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GDecayFrame class
  ///////////////////////////////////////////////
  class GDecayFrame : public DecayFrame, public GFrame{
  public:
    GDecayFrame(const string& sname, const string& stitle, int key);
    GDecayFrame(const string& sname, const string& stitle);
    virtual ~GDecayFrame();

  private:
    void Init();

  };

}

#endif
