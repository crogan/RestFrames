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

  /// Type of RestFrame, with respect to its decays
  enum FrameType { kVanillaFrame, kVisibleFrame, kInvisibleFrame,
		   kDecayFrame,   kLabFrame};

  /// Type of RestFrame, with respect to its analysis capabilities
  enum AnaType { kRecoFrame, kGenFrame };

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

    ////////////////////////////////////////////////////////////////////
    /// \brief Set axis perpendicular to transverse plane
    ///
    /// \param axis    input axis
    /// Sets axis to define the "transverse plane", perpendicular
    /// to the axis.
    ////////////////////////////////////////////////////////////////////
    static void SetAxis(const TVector3& axis);

    /// \brief Retrieve axis which defines transverse plane
    static TVector3 const& GetAxis();

    /// \brief Returns empty instance of class
    static RestFrame& Empty();

    /// \brief Clears RestFrame of all connections to other objects
    virtual void Clear();

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame type methods
    /// \brief RestFrame type query member functions
    ///
    /// Member functions for identifying FrameType and AnaType of
    /// RestFrame. 
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Returns RestFrame (FrameType) type 
    FrameType GetType() const;
    
    /// \brief Is this a VisibleFrame ? (yes/no)
    bool IsVisibleFrame() const;
    
    /// \brief Is this an InvisibleFrame ? (yes/no)
    bool IsInvisibleFrame() const;
    
    /// \brief Is this a DecayFrame ? (yes/no)
    bool IsDecayFrame() const;
    
    /// \brief Is this a LabFrame ? (yes/no)
    bool IsLabFrame() const; 
    
    /// \brief Is this an ReconstructionFrame ? (yes/no)
    bool IsRecoFrame() const;
    
    /// \brief Is this a GeneratorFrame ? (yes/no)
    bool IsGenFrame() const;

    /// \brief String of information about RestFrame
    virtual string PrintString(LogType type = LogVerbose) const;
    
    ///@}

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame tree construction methods
    /// 
    /// Member functions for assembling/disassembling trees 
    /// of connected RestFrame
    ////////////////////////////////////////////////////////////////////
    ///@{
    
    /// \brief Add a child RestFrame to this frame
    ///
    /// \param frame    RestFrame to be added as child
    ///
    /// Method for adding a RestFrame as a child 
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
    /// Method for removing a child RestFrame from the
    /// list of children of this frame (if it is in that list).
    void RemoveChild(RestFrame& frame);

    /// \brief Remove all the children of this frame
    ///
    /// Method for removing all the children of this frame. 
    /// No child left behind.
    void RemoveChildren();
  
    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame tree structure methods
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

    /// \brief Get the RestFrame of the *i* th child
    virtual RestFrame& GetChildFrame(int i) const;

    /// \brief Returns a list of this frame's child RestFrame s
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
    /// an empty frame if there is no sibling frame.
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

    /// \brief Returns a list of VisibleFrame s inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListVisibleFrames();

    /// \brief Returns a list of InvisibleFrame s inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListInvisibleFrames();
    
    ///@}

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame event analysis functions
    /// \brief RestFrame member functions for accessing connected frames
    /// 
    /// Member functions which can be used to access RestFrames 
    /// connected to this frame through parent(s) or children.
    ////////////////////////////////////////////////////////////////////
    ///@{

    /// \brief Combines RestFrame s into \ref RFList<RestFrames::RestFrame>
    ///
    /// \param frame    additional RestFrame to add in list
    ///
    /// Returns a list of RestFrame s containing *frame* and this
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

    virtual bool IsSoundBody() const;

    void SetChildBoostVector(RestFrame& frame, const TVector3& boost);
    void SetParentBoostVector(const TVector3& boost);
    TVector3 GetChildBoostVector(RestFrame& frame) const;
    TVector3 GetParentBoostVector() const;

    void SetFourVector(const TLorentzVector& V, const RestFrame& frame);

    /// \brief Recursively initialize this frame and its children for analysis
    virtual bool InitializeAnalysisRecursive() = 0;

    /// \brief Recursively clear event information from this frame and its children
    virtual bool ClearEventRecursive() = 0;

    /// \brief Recursively analyze event in this frame and its children
    virtual bool AnalyzeEventRecursive() = 0;

    /// \brief Recursively initialize this frame's tree
    virtual bool InitializeTreeRecursive();

    /// \brief Check this RestFrame 's tree for circular connections
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
