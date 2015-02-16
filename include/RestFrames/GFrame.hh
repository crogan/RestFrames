#ifndef GFrame_HH
#define GFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TRandom.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/FrameLink.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class FrameLink;

  ///////////////////////////////////////////////
  // GFrame class
  ///////////////////////////////////////////////
  class GFrame : public virtual RestFrame {
  public:
    GFrame(const string& sname, const string& stitle, int key);
    GFrame(const string& sname, const string& stitle);
    virtual ~GFrame();

    virtual void ClearGFrame();

    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    virtual void ResetFrame() = 0;
    virtual bool GenerateFrame() = 0;

    void SetChildren(const vector<TLorentzVector>& P_children);

    double GetRandom();

  private:
    void Init();
    TRandom *m_Random;
   
  };

}

#endif
