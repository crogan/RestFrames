#ifndef State_HH
#define State_HH

#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class Jigsaw;

  ///////////////////////////////////////////////
  // State class
  ///////////////////////////////////////////////

  enum StateType { SVanilla, SInvisible, SCombinatoric };

  class State : public RFBase {
  public:
    State();
    virtual ~State();

    void Clear();

    //virtual State *Copy() const;

    virtual void AddFrame(RestFrame* framePtr);
    virtual void AddFrame(RFList<RestFrame>* framesPtr);
    void ClearFrames();
    RFList<RestFrame>* GetFrames() const;
    RestFrame* GetFrame() const;
    int GetNFrames() const { return m_Frames.GetN(); }

    bool IsFrame(const RestFrame* framePtr) const;
    bool IsFrames(const RFList<RestFrame>* framesPtr) const;

    void SetParentJigsaw(Jigsaw* jigsawPtr){ m_ParentJigsawPtr = jigsawPtr; }
    void SetChildJigsaw(Jigsaw* jigsawPtr){ m_ChildJigsawPtr = jigsawPtr; }
    Jigsaw* GetParentJigsaw() const { return m_ParentJigsawPtr; }
    Jigsaw* GetChildJigsaw() const { return m_ChildJigsawPtr; }

    virtual void Boost(const TVector3& B);
    void SetFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetFourVector() const; 

    virtual void FillGroupJigsawDependancies(RFList<Jigsaw>* jigsawsPtr) const;
    virtual void FillStateJigsawDependancies(RFList<Jigsaw>* jigsawsPtr) const;
	
  protected:
    static int m_class_key;

    StateType m_Type;

    TLorentzVector m_P;
    RFList<RestFrame> m_Frames;

    Jigsaw *m_ParentJigsawPtr;
    Jigsaw *m_ChildJigsawPtr;

  private:
    void Init();
    int GenKey();
  };

  typedef const State* GroupElementID;

}

#endif
