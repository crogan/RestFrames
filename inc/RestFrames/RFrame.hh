#ifndef RFrame_HH
#define RFrame_HH

#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class Group;
  class StateList;

  ///////////////////////////////////////////////
  // RFrame class
  ///////////////////////////////////////////////
  class RFrame : public virtual RestFrame {
  public:
    RFrame(const string& sname, const string& stitle);
    virtual ~RFrame();

    virtual void ClearRFrame();

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const { return m_GroupPtr; }

    RFList<Group>* GetListGroups() const;

    virtual bool InitializeStates(const StateList* statesPtr, const RFList<Group>* groupsPtr);
    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    vector<StateList*> m_ChildStates;
    Group* m_GroupPtr;

    virtual bool InitializeStatesRecursive(const StateList* statesPtr, const RFList<Group>* groupsPtr);
    virtual bool InitializeNoGroupStates(const StateList* statesPtr);
    virtual bool InitializeGroupStates(const RFList<Group>* groupsPtr);

    void FillListGroupsRecursive(RFList<Group>* groupsPtr) const;

  private:
    void Init();
    void ClearStates();
  };

}

#endif
