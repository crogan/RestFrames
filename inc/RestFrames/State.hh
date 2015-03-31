#ifndef State_HH
#define State_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/JigsawList.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Jigsaw;
  class JigsawList;

  ///////////////////////////////////////////////
  // State class
  ///////////////////////////////////////////////

  enum StateType { SVanilla, SInvisible, SCombinatoric };
  class State {
  public:
    State();
    State(int ikey);
    virtual ~State();

    void Clear();

    virtual State *Copy() const;

    int GetKey() const { return m_Key; }

    virtual void AddFrame(RestFrame* framePtr);
    virtual void AddFrame(RestFrameList* framesPtr);
    void ClearFrames();
    RestFrameList* GetFrames() const;
    RestFrame* GetFrame() const;
    int GetNFrames() const { return m_Frames.GetN(); }

    bool IsSame(const State* statePtr) const;
    bool IsFrame(const RestFrame* framePtr) const;
    bool IsFrames(const RestFrameList* framesPtr) const;

    void SetParentJigsaw(Jigsaw* jigsawPtr){ m_ParentJigsawPtr = jigsawPtr; }
    void SetChildJigsaw(Jigsaw* jigsawPtr){ m_ChildJigsawPtr = jigsawPtr; }
    Jigsaw* GetParentJigsaw() const { return m_ParentJigsawPtr; }
    Jigsaw* GetChildJigsaw() const { return m_ChildJigsawPtr; }

    virtual void Boost(const TVector3& B);
    void SetFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetFourVector() const; 

    virtual void FillGroupJigsawDependancies(JigsawList* jigsawsPtr) const;
    virtual void FillStateJigsawDependancies(JigsawList* jigsawsPtr) const;
	
  protected:
    static int m_class_key;

    int m_Key;
    StateType m_Type;

    TLorentzVector m_P;
    RestFrameList m_Frames;

    Jigsaw *m_ParentJigsawPtr;
    Jigsaw *m_ChildJigsawPtr;

  private:
    void Init();
    int GenKey();
  };

  typedef const State* GroupElementID;

}

#endif
