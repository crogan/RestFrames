#ifndef RFrame_HH
#define RFrame_HH
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
  // RFrame class
  ///////////////////////////////////////////////
  class RFrame : public virtual RestFrame {
  public:
    RFrame(const string& sname, const string& stitle, int key);
    RFrame(const string& sname, const string& stitle);
    virtual ~RFrame();

    virtual void ClearRFrame();

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const { return m_GroupPtr; }

    GroupList* GetListGroups() const;

    virtual bool InitializeStates(const StateList* statesPtr, const GroupList* groupsPtr);
    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    vector<StateList*> m_ChildStates;
    Group* m_GroupPtr;

    virtual bool InitializeStatesRecursive(const StateList* statesPtr, const GroupList* groupsPtr);
    virtual bool InitializeNoGroupStates(const StateList* statesPtr);
    virtual bool InitializeGroupStates(const GroupList* groupsPtr);

    void FillListGroupsRecursive(GroupList* groupsPtr) const;

  private:
    void Init();
    void ClearStates();
  };

}

#endif
