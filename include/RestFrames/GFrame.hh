#ifndef GFrame_HH
#define GFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/FrameLink.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/GroupList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class FrameLink;
  class State;
  class StateList;
  class Group;
  class GroupList;

  ///////////////////////////////////////////////
  // GFrame class
  ///////////////////////////////////////////////
  class GFrame : public virtual RestFrame {
  public:
    GFrame(const string& sname, const string& stitle, int key);
    GFrame(const string& sname, const string& stitle);
    virtual ~GFrame();

    virtual void ClearGFrame();

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const { return m_GroupPtr; }

    GroupList* GetListGroups() const;
	
    virtual bool InitializeAnalysis(const StateList* statesPtr, const GroupList* groupsPtr);
    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();
    virtual bool InitializeAnalysisRecursive(const StateList* statesPtr, const GroupList* groupsPtr);

  protected:
    vector<StateList*> m_ChildStates;
    Group* m_GroupPtr;

    virtual bool InitializeStates(const StateList* statesPtr);
    virtual bool InitializeGroupStates(const GroupList* groupsPtr);

    void FillListGroupsRecursive(GroupList* groupsPtr) const;

  private:
    void Init();
    void ClearStates();
  };

}

#endif
