#ifndef RInvisibleFrame_HH
#define RInvisibleFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/InvisibleFrame.hh"
#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RFrame;
  class InvisibleFrame;

  ///////////////////////////////////////////////
  // RInvisibleFrame class
  ///////////////////////////////////////////////
  class RInvisibleFrame : public InvisibleFrame, public RFrame{
  public:
    RInvisibleFrame(const string& sname, const string& stitle, int key);
    RInvisibleFrame(const string& sname, const string& stitle);
    virtual ~RInvisibleFrame();

    double GetMinimumMass() const { return minMASS; }
    void SetMinimumMass(double M){ minMASS = M; }

  protected:
    double minMASS;

  private:
    void Init();
   
  };

}

#endif
