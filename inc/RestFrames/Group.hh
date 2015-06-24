#ifndef Group_HH
#define Group_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {
 
  class Jigsaw;
  class State;
  class StateList;

  enum GroupType { GInvisible, GCombinatoric };

  ///////////////////////////////////////////////
  // Group class
  ///////////////////////////////////////////////
  class Group : public RFBase {
  public:
    Group(const string& sname, const string& stitle);
    virtual ~Group();

    virtual void Clear();

    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame) = 0;
    virtual void AddFrame(RestFrame* framePtr) = 0;
    void RemoveFrame(const RestFrame* framePtr);
    bool ContainsFrame(const RestFrame* framePtr) const;
    RFList<RestFrame>* GetFrames() const;

    virtual bool AddJigsaw(Jigsaw& jigsaw) = 0;
    virtual bool AddJigsaw(Jigsaw* jigsawPtr) = 0;
    RFList<Jigsaw>* GetJigsaws() const;

    State* GetGroupState() const;
    bool GetState(const RFList<RestFrame>* framesPtr, StateList*& statesPtr);
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
    RFList<RestFrame> m_Frames;
  
    StateList* m_StatesPtr;
    StateList* m_StatesToSplitPtr;

    //RFList<Jigsaw>* m_JigsawsPtr;
    RFList<Jigsaw>* m_JigsawsPtr;
    RFList<Jigsaw>* m_JigsawsToUsePtr;

    virtual State* InitializeGroupState();
    bool InitializeJigsaws();
    void InitializeJigsaw(Jigsaw* jigsawPtr);
    bool SplitState(const State* statePtr);

  private:
    void Init();
    int GenKey();
  };

}

#endif
