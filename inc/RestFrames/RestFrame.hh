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
  enum FrameType { kVanillaFrame, kVisibleFrame, kInvisibleFrame,
		   kDecayFrame,   kLabFrame};

  /// Type of RestFrame, with respect to its analysis capabilities
  enum AnaType { kRecoFrame, kGenFrame };

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
    
    /// \brief Empty constructor
    RestFrame();
    
    virtual ~RestFrame();

    /// \brief Clears RestFrame of all connections to other objects
    virtual void Clear();

    /// \brief Returns empty instance of class
    static RestFrame& Empty();

    /// \brief Set axis perpendicular to transverse plane
    static void SetAxis(const TVector3& axis);

    /// \brief Retrieve axis perpendicular to transverse plane
    static TVector3 const& GetAxis();

    ////////////////////////////////////////////////////////////////////
    /// \name FrameBase type methods
    /// \brief FrameBase type query member functions
    ///
    /// Member functions for identifying *FrameType* and *AnaType* of
    /// reference frame
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Returns RestFrame (*FrameType*) type 
    FrameType GetType() const;
    
    /// \brief Is this a Visible type frame? (yes/no)
    bool IsVisibleFrame() const;
    
    /// \brief Is this an Invisible type frame? (yes/no)
    bool IsInvisibleFrame() const;
    
    /// \brief Is this a Decay type frame? (yes/no)
    bool IsDecayFrame() const;
    
    /// \brief Is this a Lab type frame? (yes/no)
    bool IsLabFrame() const;
    
    /// \brief Is this an RecoFrame (for event reconstruction) (yes/no)
    bool IsRecoFrame() const;
    
    /// \brief Is this a GenFrame (for event generation) (yes/no)
    bool IsGenFrame() const;

    /// \brief String of information about FrameBase
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
    virtual void AddChildFrame(RestFrame& frame);

    /// \brief Add a list of children to this frame
    ///
    /// \param frames    RestFrames to be added as children
    void AddChildFrames(const RestFrames::RFList<RestFrame>& frames);

    /// \brief Set the parent frame for this frame
    ///
    /// \param frame     parent frame
    ///
    /// Method for connecting a child frame to its parent frame
    /// Empty default sets parent frame to none
    virtual void SetParentFrame(RestFrame& frame = 
				RestFrame::Empty());

    /// \brief Remove a child of this frame 
    ///
    /// \param frame     child frame to be removed
    ///
    /// Method for removing a child *frame* from the
    /// list of children of this frame (if it is in that list).
    void RemoveChild(RestFrame& frame);

    /// \brief Remove all the children of this frame
    ///
    /// Method for removing all the children of this frame. 
    /// No child left behind.
    void RemoveChildren();
  
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
    int GetNDescendants() const;

    /// \brief Returns the parent of this frame
    ///
    /// Returns the parent frame of this frame.
    /// If the parent frame is not set, an empty
    /// frame is returned.
    virtual RestFrame const& GetParentFrame() const;

    /// \brief Get the frame of the *i* th child
    virtual RestFrame& GetChildFrame(int i) const;

    /// \brief Returns the parent of this frame
    ///
    /// Returns the parent frame of this frame.
    /// If the parent frame is not set, an empty
    /// frame is returned.

    /// \brief Returns a list of this frame's children
    RestFrames::RFList<RestFrame> GetChildren() const;

    /// \brief Returns the LabFrame that this frame inherits from
    ///
    /// Returns the LabFrame frame of this frame's tree.
    /// If a LabFrame cannot be found, an empty frame 
    /// is returned.
    virtual RestFrame const& GetLabFrame() const;

    /// \brief Returns the production frame of this frame
    virtual RestFrame const& GetProductionFrame() const;

    /// \brief Returns the sibling frame of this frame
    ///
    /// Returns the the first child frame of the parent 
    /// frame of this one that is not this frame. Returns
    /// an empty frame if there is no sibling frame
    virtual RestFrame const& GetSiblingFrame() const;

    /// \brief Returns the depth of *frame* 
    ///
    /// \param frame    frame whose depth is returned
    ///
    /// Returns the depth of *frame* relative to this frame. 
    /// If *frame* is not among the descendents of this frame,
    /// -1 is returned.
    int GetFrameDepth(const RestFrame& frame) const;

    /// \brief Returns the frame at depth
    ///
    /// \param depth    depth of desired frame
    /// \param frame    descendent frame that defines path
    ///
    /// Returns the frame at a depth *depth* down from this frame. The path
    /// through the descendents of this frame is defined as that proceeding 
    /// towards *frame*. If no frame exists at *depth* along this path, an
    /// empty frame is returned.
    virtual RestFrame const& GetFrameAtDepth(int depth, const RestFrame& frame) const;
    
    /// \brief Returns a list of frames inheriting from this one
    ///
    /// Returns a list of frames that inherit from this one,
    /// filled recursively and including children of children
    /// which are of FrameType *type*. If *type* is LabFrame
    /// (default) then *type* is ignored and all frames are included.
    virtual RestFrames::RFList<RestFrame> GetListFrames(FrameType type = kLabFrame);

    /// \brief Returns a list of VisibleFrames inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListVisibleFrames();

    /// \brief Returns a list of InvisibleFrames inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListInvisibleFrames();
    
    ///@}

    /// \brief Recursively initialize this frame's tree
    virtual bool InitializeTreeRecursive();

    /// \brief Recursively initialize this frame and its children for analysis
    virtual bool InitializeAnalysisRecursive() = 0;

    /// \brief Recursively clear event information from this frame and its children
    virtual bool ClearEventRecursive() = 0;

    /// \brief Recursively analyze event in this frame and its children
    virtual bool AnalyzeEventRecursive() = 0;

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame event analysis functions
    /// \brief RestFrame member functions for accessing connected frames
    /// 
    /// Member functions which can be used to access RestFrames 
    /// connected to this frame through parent(s) or children.
    ////////////////////////////////////////////////////////////////////
    ///@{

    RestFrames::RFList<RestFrame> operator+(RestFrame& frame); 

    virtual double GetMass() const;
 
    TLorentzVector GetFourVector(const RestFrame& frame = 
				 RestFrame::Empty()) const;
   
    TLorentzVector GetVisibleFourVector(const RestFrame& frame = 
					RestFrame::Empty()) const; 
    
    TLorentzVector GetInvisibleFourVector(const RestFrame& frame = 
					  RestFrame::Empty()) const; 
    
    double GetEnergy(const RestFrame& frame) const;
    double GetMomentum(const RestFrame& frame) const;


    double GetVisibleShape() const;
    double GetSumVisibleMomentum() const;
    double GetSumInvisibleMomentum() const;
    
    TVector3 GetBoostInParentFrame() const;
    double GetGammaInParentFrame() const;

    TVector3 GetDecayPlaneNormalVector() const;

    double GetDeltaPhiDecayPlanes(const RestFrame& frame) const;

    double GetCosDecayAngle(const RestFrame& frame = 
			    RestFrame::Empty()) const;

    double GetDeltaPhiDecayAngle(const TVector3& axis = 
				 RestFrame::GetAxis(), 
				 const RestFrame& frame = 
				 RestFrame::Empty()) const;
    double GetDeltaPhiBoostVisible(const TVector3& axis = 
				   RestFrame::GetAxis(), 
				   const RestFrame& frame = 
				   RestFrame::Empty()) const;
    double GetDeltaPhiDecayVisible(const TVector3& axis = 
				   RestFrame::GetAxis(), 
				   const RestFrame& frame = 
				   RestFrame::Empty()) const;
    double GetDeltaPhiVisible(const TVector3& axis = 
			      RestFrame::GetAxis(), 
			      const RestFrame& frame = 
			      RestFrame::Empty()) const;
    ///@}

  protected:   
    FrameType m_Type;
    AnaType m_Ana;

    void SetChildBoostVector(RestFrame& frame, const TVector3& boost);
    void SetParentBoostVector(const TVector3& boost);
    TVector3 GetChildBoostVector(RestFrame& frame) const;
    TVector3 GetParentBoostVector() const;

    void SetFourVector(const TLorentzVector& V, const RestFrame& frame);

    // Tree construction checks
    virtual bool IsSoundBody() const;
    bool IsCircularTree(vector<RFKey>& keys) const;

  private:
    /// \brief RestFrame ID key
    static int m_class_key;

    /// \brief Axis perpendicular to transverse plane in the lab frame
    static TVector3 m_Axis;

    // 4-vector of this state in the frame it's initialized
    TLorentzVector m_P;
    
    // the reference frame where this four-vector is initialized
    const RestFrame* m_ProdFramePtr;

    // list of child frames and boosts
    RestFrames::RFList<RestFrame> m_ChildFrames;
    mutable map<const RestFrame*, TVector3> m_ChildBoosts;

    // parent frame and boost
    RestFrame* m_ParentFramePtr;
    TVector3 m_ParentBoost;

    // Recursively get lists of frames
    void FillListFramesRecursive(RFList<RestFrame>& frames, FrameType type = kLabFrame);

    bool FindPathToFrame(const RestFrame& dest_frame, const RestFrame& prev_frame, 
			 vector<TVector3>& boosts) const;

  };

}

#endif
