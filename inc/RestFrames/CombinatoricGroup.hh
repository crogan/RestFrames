#ifndef CombinatoricGroup_HH
#define CombinatoricGroup_HH
#include <iostream>
#include <string>
#include <vector>

#include <TLorentzVector.h>
#include "RestFrames/Group.hh"
#include "RestFrames/CombinatoricState.hh"

using namespace std;

namespace RestFrames {

  class Group;
  class CombinatoricState;

  ///////////////////////////////////////////////
  // CombinatoricGroup class
  ///////////////////////////////////////////////
  class CombinatoricGroup : public Group {
  public:
    CombinatoricGroup(const string& sname, const string& stitle, int ikey);
    CombinatoricGroup(const string& sname, const string& stitle);
    virtual ~CombinatoricGroup();

    void Clear();

    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrame(RestFrame* framePtr);
    virtual void SetNElementsForFrame(const RestFrame& frame, int N, bool exclusive_N = false);
    virtual void SetNElementsForFrame(const RestFrame* framePtr, int N, bool exclusive_N = false);
    virtual void GetNElementsForFrame(const RestFrame* framePtr, int& N, bool& exclusive_N);

    virtual bool AddJigsaw(Jigsaw& jigsaw);
    virtual bool AddJigsaw(Jigsaw* jigsawPtr);

    // Event analysis functions
    void ClearFourVectors();
    GroupElementID AddLabFrameFourVector(const TLorentzVector& V);
    int GetNFourVectors() const;

    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

    const RestFrame* GetFrame(const GroupElementID elementID);
    TLorentzVector GetLabFrameFourVector(const GroupElementID elementID);
    int GetNElementsInFrame(const RestFrame& frame);
    int GetNElementsInFrame(const RestFrame* framePtr);
	
  protected:
    StateList m_StateElements;
    vector<int> m_NElementsForFrame;
    vector<bool> m_NExclusiveElementsForFrame; 
    
    vector<int> m_StateKeys;

    virtual State* InitializeGroupState();
    void ClearElements();
    void AddElement(State* statePtr);
    int GetNElements() const;

  private:
    void Init();

  };

}

#endif
