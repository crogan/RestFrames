#ifndef Jigsaw_HH
#define Jigsaw_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/GroupList.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"
#include "RestFrames/JigsawList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Group;
  class GroupList;
  class JigsawList;
  class State;
  class StateList;

  enum JigsawType { JInvisible, JCombinatoric };

  ///////////////////////////////////////////////
  // Jigsaw class
  ///////////////////////////////////////////////
  class Jigsaw {
  public:
    Jigsaw(const string& sname, const string& stitle);
    Jigsaw(const string& sname, const string& stitle, int ikey);
    virtual ~Jigsaw();
  
    virtual void Clear();

    virtual bool IsSoundBody() const;
    virtual bool IsSoundMind() const;
    virtual bool IsSoundSpirit() const;

    string GetName() const;
    string GetTitle() const;
    int GetKey() const;
    JigsawType GetType() const { return m_Type; }
  
    int GetPriority() const { return m_Priority; }
    void SetPriority(int p){ m_Priority = p; }

    bool IsSame(const Jigsaw* jigsawPtr) const;
    bool IsInvisibleJigsaw() const;
    bool IsCombinatoricJigsaw() const;

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const;

    bool CanSplit(const State* statePtr);
    bool CanSplit(const RestFrameList* framesPtr);
  
    virtual StateList* InitializeOutputStates(State* statePtr);
    virtual bool InitializeDependancyStates(const StateList* statesPtr, const GroupList* groupsPtr);
    virtual bool InitializeDependancyJigsaws();

    virtual int GetNChildStates() const;
    virtual State* GetChildState(int i) const;
    virtual RestFrameList* GetChildFrames(int i) const;
    virtual RestFrameList* GetChildFrames() const;

    virtual void FillGroupJigsawDependancies(JigsawList* jigsawsPtr);
    virtual void FillStateJigsawDependancies(JigsawList* jigsawsPtr);

    virtual bool InitializeJigsawExecutionList(JigsawList* chain_jigsawPtr) = 0;
    bool DependsOnJigsaw(Jigsaw* jigsawPtr);

    virtual bool AnalyzeEvent() = 0;
  
  protected:
    static int m_class_key;

    mutable bool m_Body;
    mutable bool m_Mind;
    mutable bool m_Spirit;

    int m_Key;
    string m_Name;
    string m_Title;
    JigsawType m_Type;
    int m_Priority;

    Group* m_GroupPtr;
    State* m_InputStatePtr;

    vector<RestFrameList*> m_OutputFrames;
    vector<RestFrameList*> m_DependancyFrames;

    StateList* m_OutputStatesPtr;
    vector<StateList*> m_DependancyStates;

    JigsawList* m_DependancyJigsawsPtr;

    virtual State* NewOutputState();
    void AddOutputFrame(RestFrame* framePtr, int i = 0);
    void AddOutputFrame(RestFrameList* framesPtr, int i = 0);
    void AddDependancyFrame(RestFrame* framePtr, int i = 0);
    void AddDependancyFrame(RestFrameList* framesPtr, int i = 0);

  private:
    void Init(const string& sname, const string& stitle);
    int GenKey();
    void ClearOutputStates();
    void ClearDependancyStates();
  };
}

#endif
