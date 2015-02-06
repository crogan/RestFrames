#ifndef RestFrame_HH
#define RestFrame_HH
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/FrameLink.hh"
#include "RestFrames/RestFrameList.hh"

using namespace std;

namespace RestFrames {

  class FrameLink;
  class RestFrameList;

  enum FrameType { FVisible, FInvisible, FDecay, FLab};
  enum AnaType { FReco, FGen };

  ///////////////////////////////////////////////
  // RestFrame class
  ///////////////////////////////////////////////
  class RestFrame {
  public:
    RestFrame(const string& sname, const string& stitle, int key);
    RestFrame(const string& sname, const string& stitle);
    virtual ~RestFrame();
  
    virtual void ClearFrame();

    // Frame identity functions
    int GetKey() const;
    string GetName() const;
    string GetTitle() const;
    bool IsSame(const RestFrame& frame) const;
    bool IsSame(const RestFrame* framePtr) const;

    // Frame type queries
    FrameType GetType() const;
    bool IsVisibleFrame() const;
    bool IsInvisibleFrame() const;
    bool IsDecayFrame() const;
    bool IsLabFrame() const;
    bool IsRFrame() const;
    bool IsGFrame() const;

    // Tree construction functions
    void AddChildFrame(RestFrame& frame);
    void AddChildFrame(RestFrame* framePtr);
    void SetParentLink(FrameLink* linkPtr);
    void RemoveChild(const RestFrame* framePtr);
    void RemoveChildIndex(int i);
    virtual void RemoveChildren();
  
    //Frame retrieval
    int GetNChildren() const;
    RestFrame* GetChildFrame(int i) const;
    int GetChildIndex(const RestFrame* framePtr) const;
    const RestFrame* GetLabFrame() const;
    RestFrameList* GetListFrames();
    RestFrameList* GetListFramesType(FrameType type);
    RestFrameList* GetListFramesType(const vector<FrameType>& types);
    RestFrameList* GetListVisibleFrames();
    RestFrameList* GetListInvisibleFrames();

    virtual void ClearEventRecursive() = 0;
    virtual bool AnalyzeEventRecursive() = 0;

    // Analysis functions
    virtual double GetMass() const;
    virtual double GetCosDecayAngle(const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiDecayAngle(const TVector3& axis = TVector3(0.,0.,1.), 
					 const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiBoostVisible(const TVector3& axis = TVector3(0.,0.,1.), 
					   const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiVisible(const TVector3& axis = TVector3(0.,0.,1.), 
				      const RestFrame* framePtr = nullptr) const;
    virtual double GetVisibleShape() const;
    virtual double GetTransverseVisibleShape(const TVector3& axis = TVector3(0.,0.,1.), 
					     const RestFrame* framePtr = nullptr) const;
    virtual double GetScalarVisibleMomentum() const;
    virtual double GetTransverseScalarVisibleMomentum(const TVector3& axis = TVector3(0.,0.,1.), 
						      const RestFrame* framePtr = nullptr) const;
    virtual TLorentzVector GetFourVector(const RestFrame& frame) const;
    virtual TLorentzVector GetFourVector(const RestFrame* framePtr) const;
    virtual TLorentzVector GetVisibleFourVector(const RestFrame& frame) const; 
    virtual TLorentzVector GetVisibleFourVector(const RestFrame* framePtr) const; 
    virtual TLorentzVector GetInvisibleFourVector(const RestFrame& frame) const; 
    virtual TLorentzVector GetInvisibleFourVector(const RestFrame* framePtr) const; 
    virtual double GetEnergy(const RestFrame& frame) const;
    virtual double GetEnergy(const RestFrame* framePtr) const;
    virtual double GetMomentum(const RestFrame& frame) const;
    virtual double GetMomentum(const RestFrame* framePtr) const;
    virtual int GetFrameDepth(const RestFrame& frame) const;
    virtual int GetFrameDepth(const RestFrame* framePtr) const;
    virtual const RestFrame* GetFrameAtDepth(int depth, const RestFrame& frame) const;
    virtual const RestFrame* GetFrameAtDepth(int depth, const RestFrame* framePtr) const;
    virtual const RestFrame* GetProductionFrame() const;
    virtual const RestFrame* GetSiblingFrame() const;
    virtual TVector3 GetBoostInParentFrame() const;
    virtual double GetGammaInParentFrame() const;
    virtual double GetDeltaPhiDecayPlanes(const RestFrame& frame) const;
    virtual double GetDeltaPhiDecayPlanes(const RestFrame* framePtr) const;
    

  protected:
    static int m_class_key;
    mutable bool m_Body;
    mutable bool m_Mind;
    mutable bool m_Spirit;

    virtual bool IsSoundBody() const;
    bool IsSoundBodyRecursive() const;
    virtual bool IsSoundMind() const;
    bool IsSoundMindRecursive() const;
    virtual bool IsSoundSpirit() const;
    bool IsSoundSpiritRecursive() const;

    string m_Name;
    string m_Title;
    int m_Key;
    FrameType m_Type;
    AnaType m_Ana;

    FrameLink* m_ParentLinkPtr;
    vector<FrameLink*> m_ChildLinks;

    // 4-vector of this state in the frame it's initialized
    TLorentzVector m_P;
    // the reference frame where this four-vector is defined
    const RestFrame* m_ProdFramePtr;

    // Tree construction checks
    bool IsCircularTree(vector<int>* KEYS) const;
    bool IsConsistentAnaTree(AnaType ana) const;
    void FillListFramesRecursive(RestFrameList* framesPtr);
    void FillListFramesTypeRecursive(FrameType type, RestFrameList* framesPtr);
    virtual void SetFourVector(const TLorentzVector& V, const RestFrame* framePtr);
    bool FindPathToFrame(const RestFrame* framePtr, vector<FrameLink*>* linksPtr, 
			 vector<int>* linkSignsPtr, const RestFrame* prevPtr) const;
    TVector3 GetDecayPlaneNormalVector() const;

  private:
    void Init(const string& sname, const string& stitle);
    int GenKey();
  };

  ///////////////////////////////////////////////
  // Utility functions
  ///////////////////////////////////////////////
  template <typename T>
  string NumToString ( T Num )
  {
    ostringstream ss;
    ss << Num;
    return ss.str();
  }

}

#endif
