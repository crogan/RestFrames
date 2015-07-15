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

#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {
  
  class RestFrame;

  /// Type of RestFrame, with respect to its decays
  enum FrameType { FVisible, FInvisible, FDecay, FLab};

  /// Type of RestFrame, with respect to its analysis capabilities
  enum AnaType { FReco, FGen };

  extern RestFrame g_RestFrame;

  ////////////////////////////////////////////////////////////////////
  /// \brief Base class for all *reference* *frame* objects
  ///
  /// Abstract base class from which all *reference* *frame* objects
  /// inherit. 
  ////////////////////////////////////////////////////////////////////
  class RestFrame : public RFBase {
  public:
    
    ////////////////////////////////////////////////////////////////////
    /// \brief Standard constructor
    /// 
    /// \param sname    class instance name used for log statements
    /// \param stitle   class instance title used in figures
    ////////////////////////////////////////////////////////////////////
    RestFrame(const string& sname, const string& stitle);
    
    RestFrame(const RFKey& key);

    RestFrame();
    
    virtual ~RestFrame();

    /// \brief Clears RestFrame of all connections to other objects
    virtual void Clear();

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
    bool IsRecoFrame() const;
    
    /// \brief Is this a GenFrame (for event generation) (yes/no)
    bool IsGenFrame() const;

    /// \brief String of information about RestFrame
    virtual string PrintString(LogType type = LogVerbose) const;
    
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

    /// \brief Set the parent frame for this frame
    ///
    /// \param frame     parent frame
    ///
    /// Method for connecting a child frame to its parent frame
    void SetParentFrame(RestFrame& frame);

    /// \brief Remove the parent frame setting
    void SetParentFrame();

    /// \brief Remove a child of this frame 
    ///
    /// \param frame     child frame to be removed
    ///
    /// Method for removing a child *frame* from the
    /// list of children of this frame (if it is in that list).
    void RemoveChild(const RestFrame& frame);

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

    /// \brief Get the number of child frames inheriting from this one
    int GetNChildren() const;

    /// \brief Get the number of descendents of this frame
    virtual int GetNDescendants() const;

    /// \brief Get the frame of the *i* th child
    RestFrame& GetChildFrame(int i) const;

    /// \brief Get the index of *frame* 
    ///
    /// \param frame   child frame whose index is returned
    ///
    /// Return the index of child frame *frame*. Will return -1
    /// if *frame* is not among the children
    int GetChildIndex(const RestFrame& frame) const;

    /// \brief Returns the parent of this frame
    ///
    /// Returns the parent frame of this frame.
    /// If the parent frame is not set, an empty
    /// frame is returned.
    const RestFrame& GetParentFrame() const;

    /// \brief Returns the LabFrame that this frame inherits from
    ///
    /// Returns the LabFrame frame of this frame's tree.
    /// If a LabFrame cannot be found, an empty frame 
    /// is returned.
    const RestFrame& GetLabFrame() const;

    /// \brief Returns the production frame of this frame
    virtual const RestFrame& GetProductionFrame() const;

    /// \brief Returns the sibling frame of this frame
    ///
    /// Returns the the first child frame of the parent 
    /// frame of this one that is not this frame.
    virtual const RestFrame& GetSiblingFrame() const;

    /// \brief Returns the depth of *frame* 
    ///
    /// \param frame    frame whose depth is returned
    ///
    /// Returns the depth of *frame* relative to this frame. 
    /// If *frame* is not among the descendents of this frame,
    /// -1 is returned.
    virtual int GetFrameDepth(const RestFrame& frame) const;

    /// \brief Returns the frame at depth
    ///
    /// \param depth    depth of desired frame
    /// \param frame    descendent frame that defines path
    ///
    /// Returns the frame at a depth *depth* down from this frame. The path
    /// through the descendents of this frame is defined as that proceeding 
    /// towards *frame*. If no frame exists at *depth* along this path, an
    /// empty frame is returned.
    RestFrame const& GetFrameAtDepth(int depth, const RestFrame& frame) const;

    /// \brief Returns a list of frames inheriting from this one
    ///
    /// Returns a list of frames that inherit from this one,
    /// filled recursively and including children of children (and this frame)
    RestFrames::RFList<RestFrame> GetListFrames();

    /// \brief Returns a list of frames inheriting from this one with FrameType *type*
    ///
    /// \param type    FrameType of frames to be included in list
    ///
    /// Returns a list of frames that inherit from this one,
    /// filled recursively and including children of children (and this frame), 
    /// which are of FrameType *type*
    RestFrames::RFList<RestFrame> GetListFrames(FrameType type);

    /// \brief Returns a list of frames inheriting from this one with FrameType *types*
    ///
    /// \param types    FrameType's of frames to be included in list
    ///
    /// Returns a list of frames that inherit from this one,
    /// filled recursively and including children of children (and this frame), 
    /// which have a FrameType included among *types*
    RestFrames::RFList<RestFrame> GetListFrames(const vector<RestFrames::FrameType>& types);

    /// \brief Returns a list of FVisible frames inheriting from this
    RestFrames::RFList<RestFrame> GetListVisibleFrames();

    /// \brief Returns a list of FInvisible frames inheriting from this
    RestFrames::RFList<RestFrame> GetListInvisibleFrames();
    
    ///@}

    /// \brief Recursively clear event information from this frame and its children
    virtual void ClearEventRecursive(){ }

    /// \brief Recursively analyze event in this frame and its children
    virtual bool AnalyzeEventRecursive(){ return false; }

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame frame retrieval methods
    /// \brief RestFrame member functions for accessing connected frames
    /// 
    /// Member functions which can be used to access RestFrames 
    /// connected to this frame through parent(s) or children.
    ////////////////////////////////////////////////////////////////////
    ///@{

    RestFrames::RFList<RestFrame> operator+(RestFrame& frame); 

    virtual double GetMass() const;
    virtual TLorentzVector GetFourVector() const;
    virtual TLorentzVector GetFourVector(const RestFrame& frame) const;
   
    virtual TLorentzVector GetVisibleFourVector() const; 
    virtual TLorentzVector GetVisibleFourVector(const RestFrame& frame) const; 
    
    virtual TLorentzVector GetInvisibleFourVector() const; 
    virtual TLorentzVector GetInvisibleFourVector(const RestFrame& frame) const; 
    
    virtual double GetEnergy(const RestFrame& frame) const;
    virtual double GetMomentum(const RestFrame& frame) const;

    virtual double GetCosDecayAngle(const RestFrame& frame = g_RestFrame) const;
    virtual double GetDeltaPhiDecayAngle(const TVector3& axis = TVector3(0.,0.,1.), 
					   const RestFrame& frame = g_RestFrame) const;
    virtual double GetDeltaPhiBoostVisible(const TVector3& axis = TVector3(0.,0.,1.), 
					   const RestFrame& frame = g_RestFrame) const;
    virtual double GetDeltaPhiDecayVisible(const TVector3& axis = TVector3(0.,0.,1.), 
					   const RestFrame& frame = g_RestFrame) const;
    virtual double GetDeltaPhiVisible(const TVector3& axis = TVector3(0.,0.,1.), 
				      const RestFrame& frame = g_RestFrame) const;
    virtual double GetVisibleShape() const;
    virtual double GetTransverseVisibleShape(const TVector3& axis = TVector3(0.,0.,1.), 
					     const RestFrame& frame = g_RestFrame) const;
    virtual double GetScalarVisibleMomentum() const;
    virtual double GetTransverseScalarVisibleMomentum(const TVector3& axis = TVector3(0.,0.,1.), 
						      const RestFrame& frame = g_RestFrame) const;
    virtual TVector3 GetBoostInParentFrame() const;
    virtual double GetGammaInParentFrame() const;
    virtual double GetDeltaPhiDecayPlanes(const RestFrame& frame) const;
    virtual TVector3 GetDecayPlaneNormalVector() const;

    ///@}

  protected:
    static int m_class_key;     

    virtual bool IsSoundBody() const;

    FrameType m_Type;
    AnaType m_Ana;

    // 4-vector of this state in the frame it's initialized
    TLorentzVector m_P;
    
    // the reference frame where this four-vector is initialized
    const RestFrame* m_ProdFramePtr;

    // list of child frames and boosts
    RestFrames::RFList<RestFrame> m_ChildFrames;
    vector<TVector3> m_ChildBoosts;

    // parent frame and boost
    RestFrame* m_ParentFramePtr;
    TVector3 m_ParentBoost;

    void SetChildBoostVector(int i, const TVector3& boost);
    void SetParentBoostVector(const TVector3& boost);
    TVector3 GetChildBoostVector(int i) const;
    TVector3 GetParentBoostVector() const;

    virtual void SetFourVector(const TLorentzVector& V, const RestFrame& prod_frame);
    bool FindPathToFrame(const RestFrame& dest_frame, const RestFrame& prev_frame, 
			 vector<TVector3>& boosts) const;

    // Tree construction checks
    bool IsCircularTree(vector<RFKey>& keys) const;
    bool IsConsistentAnaTree(AnaType ana) const;
    bool IsSoundBodyRecursive() const;

    // Recursively get lists of frames
    void FillListFramesRecursive(RFList<RestFrame>& frames);
    void FillListFramesRecursive(FrameType type, RFList<RestFrame>& frames);

  private:
    void Init();
    int GenKey();
  };

}

#endif
