/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ReconstructionFrame.hh
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

#ifndef ReconstructionFrame_HH
#define ReconstructionFrame_HH

#include "RestFrames/RestFrame.hh"
#include "RestFrames/Group.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  // ReconstructionFrame class
  ///////////////////////////////////////////////
  class ReconstructionFrame : public RestFrame {
  public:
    ReconstructionFrame(const std::string& sname, const std::string& stitle);
    ReconstructionFrame();
    virtual ~ReconstructionFrame();

    /// \brief Clears ReconstructionFrame of all connections to other objects
    virtual void Clear();

    static ReconstructionFrame& Empty();

    /// \brief Add a child RestFrame to this frame
    ///
    /// \param frame    RestFrame to be added as child
    ///
    /// Method for adding a RestFrame *frame* as a child 
    /// of this frame. *frame* will not be added as a child
    /// if it is already listed as a child.
    virtual void AddChildFrame(RestFrame& frame);

    /// \brief Set the parent frame for this frame
    ///
    /// \param frame     parent frame
    ///
    /// Method for connecting a child frame to its parent frame
    /// Empty default sets parent frame to none
    virtual void SetParentFrame(RestFrame& frame = 
				RestFrame::Empty());

    /// \brief Returns the parent of this frame
    ///
    /// Returns the parent frame of this frame.
    /// If the parent frame is not set, an empty
    /// frame is returned.
    virtual ReconstructionFrame const& GetParentFrame() const;

    /// \brief Get the frame of the *i* th child
    virtual ReconstructionFrame& GetChildFrame(int i) const;

    virtual void SetGroup(Group& group = Group::Empty());
    Group& GetGroup() const;
    RestFrames::RFList<Group> GetListGroups() const;

    virtual bool InitializeAnalysisRecursive();
    virtual bool ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    Group* m_GroupPtr;
    mutable std::map<const RestFrame*, RestFrames::RFList<State> > m_ChildStates;

  private:
    void Init();
    bool InitializeVisibleStates();
    bool InitializeGroupStates();
    void FillListGroupsRecursive(RestFrames::RFList<Group>& groups) const;

  };

}

#endif
