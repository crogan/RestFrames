#ifndef RLabFrame_HH
#define RLabFrame_HH

#include "RestFrames/LabFrame.hh"
#include "RestFrames/RFrame.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class Jigsaw;

  ///////////////////////////////////////////////
  // RLabFrame class
  ///////////////////////////////////////////////
  class RLabFrame : public LabFrame, public RFrame {
  public:
    RLabFrame(const string& sname, const string& stitle);
    virtual ~RLabFrame();
  
    virtual bool InitializeAnalysis();
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

  protected:
    RFList<Group>  m_LabGroups;
    RFList<Jigsaw> m_LabJigsaws;
    StateList      m_LabStates;
  
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
