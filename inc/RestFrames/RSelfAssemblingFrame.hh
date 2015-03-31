#ifndef RSelfAssemblingFrame_HH
#define RSelfAssemblingFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/CombinatoricState.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RSelfAssemblingFrame class
  ///////////////////////////////////////////////
  class RSelfAssemblingFrame : public RDecayFrame {
  public:
    RSelfAssemblingFrame(const string& sname, const string& stitle, int key);
    RSelfAssemblingFrame(const string& sname, const string& stitle);
    virtual ~RSelfAssemblingFrame();

    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

    const RestFrame* GetFrame(GroupElementID obj) const;

  private:
    void Init();
    
    bool m_Body_UnAssembled;
    bool m_Mind_UnAssembled; 
    RestFrameList m_ChildFrames_UnAssembled;
    vector<StateList*> m_ChildStates_UnAssembled;

    RestFrameList m_VisibleFrames;
    RestFrameList m_DecayFrames;
    int m_Nvisible;
    int m_Ndecay;

    RestFrame* GetNewDecayFrame(const string& sname, const string& stitle);
    RestFrame* GetNewVisibleFrame(const string& sname, const string& stitle);

    bool m_IsAssembled;
    bool m_IsBackedUp;
    void Disassemble();
    void Assemble();
    void AssembleRecursive(RestFrame* framePtr, vector<RestFrame*>& frames, vector<TLorentzVector>& Ps); 

    //const RestFrame* GetFrame(const State* statePtr) const;
  };

}

#endif
