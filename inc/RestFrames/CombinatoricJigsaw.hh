#ifndef CombinatoricJigsaw_HH
#define CombinatoricJigsaw_HH
#include <iostream>
#include <string>
#include <vector>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/State.hh"
#include "RestFrames/CombinatoricState.hh"
#include "RestFrames/StateList.hh"
#include "RestFrames/CombinatoricGroup.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Jigsaw;
  class JigsawList;
  class State;
  class StateList;
  class CombinatoricState;
  class CombinatoricGroup;

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  class CombinatoricJigsaw : public Jigsaw {
  public:
    //constructor and destructor
    CombinatoricJigsaw(const string& sname, const string& stitle);
    CombinatoricJigsaw(const string& sname, const string& stitle, int ikey);
    virtual ~CombinatoricJigsaw();

    void Clear();

    virtual void AddFrame(RestFrame& frame, int i = 0);
    virtual void AddFrame(RestFrame* framePtr, int i = 0);
    virtual void AddFrame(RestFrameList* framesPtr, int i = 0);

    virtual bool InitializeJigsawExecutionList(JigsawList* chain_jigsawPtr);
  
  protected:
    virtual State* NewOutputState();

    JigsawList m_ExecuteJigsaws;
    bool ExecuteDependancyJigsaws();

    vector<State*> m_Inputs;
    vector<CombinatoricState*> m_Outputs;
    vector<int> m_NForOutput;
    vector<bool> m_NExclusive;
    virtual bool InitializeEvent();
    
  private:
    void Init();
  };

}

#endif
