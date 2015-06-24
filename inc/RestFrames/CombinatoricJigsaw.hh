#ifndef CombinatoricJigsaw_HH
#define CombinatoricJigsaw_HH
#include <iostream>
#include <string>
#include <vector>
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  class CombinatoricState;

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  class CombinatoricJigsaw : public Jigsaw {
  public:
    //constructor and destructor
    CombinatoricJigsaw(const string& sname, const string& stitle);
    virtual ~CombinatoricJigsaw();

    void Clear();

    virtual void AddFrame(RestFrame& frame, int i = 0);
    virtual void AddFrame(RestFrame* framePtr, int i = 0);
    virtual void AddFrame(RFList<RestFrame>* framesPtr, int i = 0);

    virtual bool InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr);
  
  protected:
    virtual State* NewOutputState();

    RFList<Jigsaw> m_ExecuteJigsaws;
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
