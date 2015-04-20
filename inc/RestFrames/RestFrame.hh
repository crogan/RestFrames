/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RestFrame.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#ifndef RestFrame_HH
#define RestFrame_HH
#include <TVector3.h>
#include <string>
#include <TLorentzVector.h>
#include <TVector3.h>

using namespace std;

namespace RestFrames {

  class FrameLink;
  class RestFrameList;

  /// Type of RestFrame, with respect to its decays
  enum FrameType { FVisible, FInvisible, FDecay, FLab};

  /// Type of RestFrame, with respect to its analysis capabilities
  enum AnaType { FReco, FGen };

  ////////////////////////////////////////////////////////////////////
  /// \brief Base class for all *reference* *frame* objects
  ///
  /// Abstract base class from which all *reference* *frame* objects
  /// inherit. 
  ////////////////////////////////////////////////////////////////////
  class RestFrame {
  public:
    
    ////////////////////////////////////////////////////////////////////
    /// \brief Standard constructor
    /// 
    /// \param sname    class instance name used for print statements
    /// \param stitle   class instance title used in figures
    ////////////////////////////////////////////////////////////////////
    RestFrame(const string& sname, const string& stitle);

    ////////////////////////////////////////////////////////////////////
    /// \brief Constructor with user-provided key
    ///
    /// \param sname    class instance name used for print statements
    /// \param stitle   class instance title used in figures
    /// \param key      class instance identification key
    ///
    /// Standard constructor auto-assigns identification key, ensuring
    /// it is not replicated in another class instance. Care should be 
    /// taken to prevent replication when using this non-standard 
    /// constructor.
    ////////////////////////////////////////////////////////////////////
    RestFrame(const string& sname, const string& stitle, int key);
    
    virtual ~RestFrame();
  
    virtual void ClearFrame();

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame identity/comparison methods
    /// \brief RestFrame identity query member functions
    /// 
    /// Member functions for identifying/comparing class instances
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Returns RestFrame identification key
    int GetKey() const;
    
    /// \brief Returns RestFrame name 
    string GetName() const;
    
    /// \brief Returns RestFrame title 
    string GetTitle() const;
    
    /// \brief Tests whether *frame* is the same as this RestFrame
    bool IsSame(const RestFrame& frame) const;
    
    /// \brief Tests whether *framePtr* points to this RestFrame
    bool IsSame(const RestFrame* framePtr) const;
    
    ///@} // end identity/comparison methods

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame type methods
    /// \brief RestFrame type query member functions
    ///
    /// Member functions for identifying *FrameType* and *AnaType* of
    /// reference frame
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Returns RestFrame (*FrameType*) type 
    FrameType GetType() const;
    
    /// \brief Is this an FVisible type frame? (yes/no)
    bool IsVisibleFrame() const;
    
    /// \brief Is this an FInvisible type frame? (yes/no)
    bool IsInvisibleFrame() const;
    
    /// \brief Is this an FDecay type frame? (yes/no)
    bool IsDecayFrame() const;
    
    /// \brief Is this an FLab type frame? (yes/no)
    bool IsLabFrame() const;
    
    /// \brief Is this an RFrame (for event reconstruction) (yes/no)
    bool IsRFrame() const;
    
    /// \brief Is this a GFrame (for event generation) (yes/no)
    bool IsGFrame() const;
    
    ///@}

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame tree construction methods
    /// \brief RestFrame tree assembly member functions
    /// 
    /// Member functions for assembling trees of connected RestFrames
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Add a child RestFrame to this frame
    ///
    /// \param frame    RestFrame to be added as child
    ///
    /// Method for adding a RestFrame *frame* as a child 
    /// of this frame. *frame* will not be added as a child
    /// if it is already listed as a child.
    void AddChildFrame(RestFrame& frame);

    /// \brief Add a child RestFrame to this frame
    ///
    /// \param framePtr    pointer to RestFrame to be added as child
    ///
    /// Method for adding a RestFrame pointed to but *framePtr* as a 
    /// child of this frame. &(*framePtr*) will not be added as a child
    /// if it is already listed as a child.
    void AddChildFrame(RestFrame* framePtr);

    /// \brief Set the parent link for this frame
    ///
    /// \param linkPtr     pointer to FrameLink of parent frame
    ///
    /// Method for connecting a child frame to its parent frame, 
    /// via a pointer to a FrameLink *linkPtr*.
    void SetParentLink(FrameLink* linkPtr);

    /// \brief Remove a child of this frame by pointer
    ///
    /// \param framePtr     pointer to child frame to be removed
    ///
    /// Method for removing a child, pointed to by *framePtr*, from the
    /// list of children of this frame (if it is in that list).
    void RemoveChild(const RestFrame* framePtr);

    /// \brief Remove a child of this frame by index
    ///
    /// \param index     index of child to be removed
    ///
    /// Method for removing the *index* th child of the frame.
    /// If *index* does not correspond to a valid child then 
    /// no children are removed.
    void RemoveChildIndex(int index);

    /// \brief Remove all the children of this frame
    ///
    /// Method for removing all the children of this frame. 
    /// No child is left behind.
    virtual void RemoveChildren();
  
    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame frame retrieval methods
    /// \brief RestFrame member functions for accessing connected frames
    /// 
    /// Member functions which can be used to access RestFrames 
    /// connected to this frame through parent(s) or children.
    ////////////////////////////////////////////////////////////////////
    ///@{
    int GetNChildren() const;
    RestFrame* GetChildFrame(int i) const;
    int GetChildIndex(const RestFrame* framePtr) const;
    const RestFrame* GetParentFrame() const;
    const RestFrame* GetLabFrame() const;
    RestFrameList* GetListFrames();
    RestFrameList* GetListFramesType(FrameType type);
    RestFrameList* GetListFramesType(const vector<RestFrames::FrameType>& types);
    RestFrameList* GetListVisibleFrames();
    RestFrameList* GetListInvisibleFrames();
    
    ///@}

    virtual void ClearEventRecursive() = 0;
    virtual bool AnalyzeEventRecursive() = 0;

    //////////////////////////
    // Analysis functions
    //////////////////////////
    virtual double GetMass() const;
    virtual double GetCosDecayAngle(const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiDecayAngle(const TVector3& axis = TVector3(0.,0.,1.), 
					 const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiBoostVisible(const TVector3& axis = TVector3(0.,0.,1.), 
					   const RestFrame* framePtr = nullptr) const;
    virtual double GetDeltaPhiDecayVisible(const TVector3& axis = TVector3(0.,0.,1.), 
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
    virtual TLorentzVector GetFourVector(const RestFrame* framePtr = nullptr) const;
    virtual TLorentzVector GetVisibleFourVector(const RestFrame& frame) const; 
    virtual TLorentzVector GetVisibleFourVector(const RestFrame* framePtr) const; 
    virtual TLorentzVector GetInvisibleFourVector(const RestFrame& frame) const; 
    virtual TLorentzVector GetInvisibleFourVector(const RestFrame* framePtr = nullptr) const; 
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
    virtual int GetNDescendants() const;
    virtual TVector3 GetBoostInParentFrame() const;
    virtual double GetGammaInParentFrame() const;
    virtual double GetDeltaPhiDecayPlanes(const RestFrame& frame) const;
    virtual double GetDeltaPhiDecayPlanes(const RestFrame* framePtr) const;
    virtual TVector3 GetDecayPlaneNormalVector() const;
    virtual double GetProdPoM(int& NDecay) const;
    virtual double GetProdSinDecayAngle(int& NDecay) const;

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

    void SetChildBoostVector(int i, TVector3 boost) const;
    TVector3 GetChildBoostVector(int i) const;
    TVector3 GetParentBoostVector() const;

    // 4-vector of this state in the frame it's initialized
    TLorentzVector m_P;
    // the reference frame where this four-vector is defined
    const RestFrame* m_ProdFramePtr;

    virtual void SetFourVector(const TLorentzVector& V, const RestFrame* framePtr);
    bool FindPathToFrame(const RestFrame* framePtr, vector<FrameLink*>* linksPtr, 
			 vector<int>* linkSignsPtr, const RestFrame* prevPtr) const;

    // Tree construction checks
    bool IsCircularTree(vector<int>* KEYS) const;
    bool IsConsistentAnaTree(AnaType ana) const;

    // Recursively get lists of frames
    void FillListFramesRecursive(RestFrameList* framesPtr);
    void FillListFramesTypeRecursive(FrameType type, RestFrameList* framesPtr);

  private:
    void Init(const string& sname, const string& stitle);
    int GenKey();
  };

}

#endif
