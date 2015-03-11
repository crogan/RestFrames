#ifndef GInvisibleFrame_HH
#define GInvisibleFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/InvisibleFrame.hh"
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class GFrame;
  class InvisibleFrame;

  ///////////////////////////////////////////////
  // GInvisibleFrame class
  ///////////////////////////////////////////////
  class GInvisibleFrame : public InvisibleFrame, public GFrame{
  public:
    GInvisibleFrame(const string& sname, const string& stitle, int key);
    GInvisibleFrame(const string& sname, const string& stitle);
    virtual ~GInvisibleFrame();

    virtual void SetMass(double val);

  protected:
    double m_Mass;

    virtual void ResetFrame();
    virtual bool GenerateFrame();

  private:
    void Init();
   
  };

}

#endif
