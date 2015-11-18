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
    
    /// \brief Standard constructor
    /// 
    /// \param sname    class instance name used for log statements
    /// \param stitle   class instance title used in figures
    RestFrame(const string& sname, const string& stitle);
    
    /// \brief Empty constructor
    RestFrame();
    
    virtual ~RestFrame();

    /// \brief Set axis perpendicular to transverse plane
    ///
    /// \param axis    input axis
    /// Sets axis to define the "transverse plane", perpendicular
    /// to the axis.
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
    /// of this frame. **frame** will not be added as a child
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

    /// \brief Returns the depth of **frame** 
    ///
    /// \param frame    frame whose depth is returned
    ///
    /// Returns the depth of **frame** relative to this frame. 
    /// If **frame** is not among the descendents of this frame,
    /// -1 is returned.
    int GetFrameDepth(const RestFrame& frame) const;

    /// \brief Returns the frame at depth
    ///
    /// \param depth    depth of desired frame
    /// \param frame    descendent frame that defines path
    ///
    /// Returns the frame at a depth *depth* down from this frame. The path
    /// through the descendents of this frame is defined as that proceeding 
    /// towards **frame**. If no frame exists at *depth* along this path, an
    /// empty frame is returned.
    virtual RestFrame const& GetFrameAtDepth(int depth, const RestFrame& frame) const;
    
    /// \brief Returns a list of frames inheriting from this one
    ///
    /// Returns a list of frames that inherit from this one,
    /// filled recursively and including children of children
    /// which are of FrameType **type**. If **type** is LabFrame
    /// (default) then all frames, regardless of type, are included.
    virtual RestFrames::RFList<RestFrame> GetListFrames(FrameType type = kLabFrame) const;

    /// \brief Returns a list of **VisibleFrame** s inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListVisibleFrames() const;

    /// \brief Returns a list of **InvisibleFrame** s inheriting from this
    virtual RestFrames::RFList<RestFrame> GetListInvisibleFrames() const;
    
    ///@}

    ////////////////////////////////////////////////////////////////////
    /// \name RestFrame event analysis functions
    /// 
    /// Member functions which can be used to analyze an event. Each
    /// these functions generally requires that the method 
    /// "AnalyzeEvent()" be successfully called from a LabFrame which is
    /// connected to this frame. Otherwise, a trivial value will be 
    /// returned and a warning message printed.
    ////////////////////////////////////////////////////////////////////
    ///@{

    /// \brief Combines RestFrame s into \ref RFList<RestFrames::RestFrame>
    ///
    /// \param frame    additional RestFrame to add in list
    ///
    /// Returns a list of RestFrame s containing __frame__ and this
    RestFrames::RFList<RestFrame> operator+(RestFrame& frame); 

    /// \brief Returns the mass of this frame.
    virtual double GetMass() const;
 
    /// \brief Returns this frames four-vector in a specified frame
    ///
    /// \param frame    rest frame in which to evaluate four-vector
    ///
    /// Returns the four-vector of this frame evaluated in the 
    /// rest frame of __frame__. If __frame__ is empty, then 
    /// the four-vector will be evaluated in the LabFrame 
    /// associated with this frame.
    TLorentzVector GetFourVector(const RestFrame& frame = 
				 RestFrame::Empty()) const;
   
    /// \brief Returns four-vector of visible descendants
    ///
    /// \param frame    rest frame in which to evaluate four-vector
    ///
    /// Returns the four-vector all of the visible descendants of 
    /// this frame, evaluated in the rest frame of __frame__. If 
    /// __frame__ is empty, then the four-vector will be evaluated 
    /// in the LabFrame associated with this frame.
    TLorentzVector GetVisibleFourVector(const RestFrame& frame = 
					RestFrame::Empty()) const; 
    
    /// \brief Returns four-vector of invisible descendants
    ///
    /// \param frame    rest frame in which to evaluate four-vector
    ///
    /// Returns the four-vector all of the invisible descendants of 
    /// this frame, evaluated in the rest frame of __frame__. If 
    /// __frame__ is empty, then the four-vector will be evaluated 
    /// in the LabFrame associated with this frame.
    TLorentzVector GetInvisibleFourVector(const RestFrame& frame = 
					  RestFrame::Empty()) const; 
    
    /// \brief Returns energy of this frame in specified reference frame
    ///
    /// \param frame    rest frame in which to evaluate energy
    ///
    /// Returns the energy of this frame, evaluated in the rest frame 
    /// of __frame__. If __frame__ is empty, then the four-vector will
    /// be evaluated in the LabFrame associated with this frame.
    double GetEnergy(const RestFrame& frame) const;

    /// \brief Returns magnitude of momentum
    ///
    /// \param frame    rest frame in which to evaluate momentum
    ///
    /// Returns the momentum of this frame, evaluated in the rest frame 
    /// of __frame__. If __frame__ is empty, then the four-vector will
    /// be evaluated in the LabFrame associated with this frame.
    double GetMomentum(const RestFrame& frame) const;

    /// \brief Returns four vector boosted to different frame
    ///
    TLorentzVector GetFourVector(const TLorentzVector& P,
				 const RestFrame& def_frame = 
				 RestFrame::Empty()) const;

    /// \brief Returns magnitude of transverse momentum
    ///
    /// \param frame       rest frame in which to evaluate momentum
    /// \param axis        longitudinal axis
    /// \param axis_frame  rest frame in which axis is defined
    ///
    /// Returns the transverse momentum of this frame, evaluated in the rest frame 
    /// of __frame__. If __frame__ is empty, then the four-vector will
    /// be evaluated in the LabFrame associated with this frame.
    double GetTransverseMomentum(const RestFrame& frame = 
				 RestFrame::Empty(),
				 const TVector3& axis = 
				 RestFrame::GetAxis(), 
				 const RestFrame& axis_frame = 
				 RestFrame::Empty()) const;

    /// \brief Returns magnitude of transverse momentum in this frame
    ///
    double GetTransverseMomentum(const TLorentzVector& P,
				 const TVector3& axis = 
				 RestFrame::GetAxis(), 
				 const RestFrame& axis_frame = 
				 RestFrame::Empty()) const;

    /// \brief Returns visible shape of frame
    ///
    /// Returns the visible shape of the frame, calculated from 
    /// visible frames associated with the children of this frame.
    /// For each of the _N_ child frames, the four-vector of all
    /// the visible frames associated with the frame is evaluated in
    /// this rest frame. The visible shape is then defined as:
    /// \f[ \mathrm{visible~shape} \equiv
    ///     \frac{ \sqrt{ \sum_{i}^{N}\sum_{j < i}^{N} 
    ///          2\left(\left|\vec{p}_{i}\right|\left|\vec{p}_{j}\right|+ 
    ///          \vec{p}_{i}\cdot\vec{p}_{j}\right) } }
    ///          { \sum_{i}^{N}\left|\vec{p}_{i}\right| }~. \f]
    /// If this frame has no children then zero is returned.
    double GetVisibleShape() const;

    /// \brief Returns visible shape of frame
    ///
    /// Returns the visible shape of the frame, calculated from 
    /// the sum four vectors of each frame list in the input vector,
    /// evaluated in this frame.
    /// For _N_ four vectors the visible shape is then defined as:
    /// \f[ \mathrm{visible~shape} \equiv
    ///     \frac{ \sqrt{ \sum_{i}^{N}\sum_{j < i}^{N} 
    ///          2\left(\left|\vec{p}_{i}\right|\left|\vec{p}_{j}\right|+ 
    ///          \vec{p}_{i}\cdot\vec{p}_{j}\right) } }
    ///          { \sum_{i}^{N}\left|\vec{p}_{i}\right| }~. \f]
    /// If the input vector contains no lists then zero is returned.
    /// double GetVisibleShape(const vector<RFList<RestFrame> >& frames) const;

    /// \brief Returns scalar sum of visible child momenta
    ///
    /// Returns the scalar sum of the momenta of the visible 
    /// frames associated with this frame's children. The
    /// four-vectors of all the visible frames associated with
    /// with each child frame are evaluated in this rest frame.
    double GetSumVisibleMomentum() const;

    /// \brief Returns scalar sum of invisible child momenta
    ///
    /// Returns the scalar sum of the momenta of the invisible 
    /// frames associated with this frame's children. The
    /// four-vectors of all the invisible frames associated with
    /// with each child frame are evaluated in this rest frame.
    double GetSumInvisibleMomentum() const;

    /// \brief Returns the boost of this frame in it's parent's frame
    ///
    /// Returns the three-vector of this frame's momenta, evaluated 
    /// in it's parent's rest frame. The orientation of the parent 
    /// frame is defined as that which occurs if one boosts from the 
    /// LabFrame to this frame via each intermediate frame in the tree
    TVector3 GetBoostInParentFrame() const;

    /// \brief Returns the gamma of this frame in it's parent's frame
    double GetGammaInParentFrame() const;


    /// \brief Returns the vector normal to the decay plane of this frame
    ///
    /// Returns the vector normal to the decay plane of this frame. The
    /// normal vector, \f$\hat{n}_{\perp}\f$, is defined as:
    /// \f[ \hat{n}_{\perp}\ = \frac{ 
    ///     \vec{p}_{C}^{~P} \times \vec{p}_{F}^{~P} }
    ///   {\left|\vec{p}_{C}^{~P} \times \vec{p}_{F}^{~P}\right|}~, \f]
    /// where \f$\vec{p}_{F}^{~P}\f$ is the momentum of this frame
    /// evaluated in it's parent's rest frame and \f$\vec{p}_{C}^{~P}\f$
    /// is the momentum of it's first child frame evaluated in the same 
    /// frame. If this frame is of type LabFrame then \f$\hat{n}_{\perp}\f$
    /// is defined alternatively as:
    /// \f[ \hat{n}_{\perp}\ = \frac{ 
    ///     \vec{p}_{C}^{~F} \times \vec{n}_{\parallel} }
    ///   {\left|\vec{p}_{C}^{~F} \times \vec{n}_{\parallel}\right|}~, \f]
    /// where \f$\vec{p}_{C}^{~F}\f$ is the momentum of the child frame 
    /// evaluated in this frame and \f$\vec{n}_{\parallel}\f$ is the vector
    /// defining the transverse plane as returned by RestFrame::GetAxis().
    /// If this frame has no children then an empty vector is returned.
    TVector3 GetDecayPlaneNormalVector() const;

    /// \brief Returns the azimuthal angle between decay planes
    ///
    /// \param frame    frame corresponding to other decay plane
    ///
    /// Returns the azimuthal angle between the decay planes of 
    /// this frame and **frame**. The decay planes are defined by
    /// their normal vectors, as returned by GetDecayPlaneNormalVector().
    double GetDeltaPhiDecayPlanes(const RestFrame& frame) const;

    /// \brief Returns the cosine of this frame's decay angle
    ///
    /// \param frame    frame defining child axis
    ///
    /// Returns the cosine of this frame's decay angle, 
    /// \f$\cos\theta_{F}\f$, which is defined as:
    /// \f[ \cos\theta_{F} = 
    ///     
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
    void FillListFramesRecursive(RFList<RestFrame>& frames, FrameType type = kLabFrame) const;

    bool FindPathToFrame(const RestFrame& dest_frame, const RestFrame& prev_frame, 
			 vector<TVector3>& boosts) const;
  };

}

#endif
