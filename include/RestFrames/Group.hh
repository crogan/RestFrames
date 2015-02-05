#ifndef Group_HH
#define Group_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/StateList.hh"
#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

  class Jigsaw;
  class JigsawList;
  class RestFrame;
  class RestFrameList;
  class RFrame;
  class State;
  class StateList;

  enum GroupType { GInvisible, GCombinatoric };

  ///////////////////////////////////////////////
  // Group class
  ///////////////////////////////////////////////
  class Group {
  public:
    Group(const string& sname, const string& stitle);
    Group(const string& sname, const string& stitle, int ikey);
    virtual ~Group();

    virtual void Clear();

    virtual bool IsSoundBody() const;
    virtual bool IsSoundMind() const;
    virtual bool IsSoundSpirit() const;

    bool IsSame(const Group* groupPtr) const;
    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    string GetName() const;
    string GetTitle() const;
    int GetKey() const;
    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame) = 0;
    virtual void AddFrame(RestFrame* framePtr) = 0;
    void RemoveFrame(const RestFrame* framePtr);
    bool ContainsFrame(const RestFrame* framePtr) const;
    RestFrameList* GetFrames() const;

    virtual bool AddJigsaw(Jigsaw& jigsaw) = 0;
    virtual bool AddJigsaw(Jigsaw* jigsawPtr) = 0;
    JigsawList* GetJigsaws() const;

    State* GetGroupState() const;
    bool GetState(const RestFrameList* framesPtr, StateList*& statesPtr);
    State* GetState(const RestFrame* framePtr) const;

    virtual bool InitializeAnalysis();
    virtual void ClearEvent() = 0;
    virtual bool AnalyzeEvent() = 0;

  protected:
    static int m_class_key;
    mutable bool m_Body;
    mutable bool m_Mind;
    mutable bool m_Spirit;

    string m_Name;
    string m_Title;
    GroupType m_Type;
    int m_Key;

    State* m_GroupStatePtr;
    RestFrameList m_Frames;
  
    StateList* m_StatesPtr;
    StateList* m_StatesToSplitPtr;

    JigsawList* m_JigsawsPtr;
    JigsawList* m_JigsawsToUsePtr;

    virtual State* InitializeGroupState();
    bool InitializeJigsaws();
    void InitializeJigsaw(Jigsaw* jigsawPtr);
    bool SplitState(const State* statePtr);

  private:
    void Init(const string& sname, const string& stitle);
    int GenKey();
  };

}

#endif
