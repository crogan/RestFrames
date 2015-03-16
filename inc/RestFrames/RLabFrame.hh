#ifndef RLabFrame_HH
#define RLabFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "VisibleFrame.hh"
#include "InvisibleFrame.hh"
#include "RestFrames/LabFrame.hh"
#include "RestFrames/DecayFrame.hh"
#include "RFrame.hh"
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
  class RFrame;
  class DecayFrame;
  class LabFrame;
  class RestFrameList;
  class State;
  class StateList;

  ///////////////////////////////////////////////
  // RLabFrame class
  ///////////////////////////////////////////////
  class RLabFrame : public LabFrame, public RFrame {
  public:
    RLabFrame(const string& sname, const string& stitle, int key);
    RLabFrame(const string& sname, const string& stitle);
    virtual ~RLabFrame();
  
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
