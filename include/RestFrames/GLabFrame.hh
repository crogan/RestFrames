#ifndef GLabFrame_HH
#define GLabFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "VisibleFrame.hh"
#include "InvisibleFrame.hh"
#include "RestFrames/LabFrame.hh"
#include "RestFrames/DecayFrame.hh"
#include "RestFrames/GFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/FrameLink.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class FrameLink;
  class Jigsaw;
  class JigsawList;
  class RestFrame;
  class GFrame;
  class DecayFrame;
  class LabFrame;
  class RestFrameList;
  class State;
  class StateList;

  ///////////////////////////////////////////////
  // GLabFrame class
  ///////////////////////////////////////////////
  class GLabFrame : public LabFrame, public GFrame {
  public:
    GLabFrame(const string& sname, const string& stitle, int key);
    GLabFrame(const string& sname, const string& stitle);
    virtual ~GLabFrame();
  
    virtual bool InitializeAnalysis();
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

  protected:
    GroupList  m_LabGroups;
    StateList  m_LabStates;
    JigsawList m_LabJigsaws;
  
    bool InitializeLabStates();
    bool InitializeLabGroups();
    bool InitializeLabJigsaws();

    
    bool ExecuteJigsaws();

  private:
    void Init();
    void ClearStates();
  };

}

#endif
