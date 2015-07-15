/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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

using namespace std;

namespace RestFrames {

  class Group;
  class State;

  ///////////////////////////////////////////////
  // ReconstructionFrame class
  ///////////////////////////////////////////////
  class ReconstructionFrame : public RestFrame {
  public:
    ReconstructionFrame(const string& sname, const string& stitle);
    ReconstructionFrame();
    virtual ~ReconstructionFrame();

    /// \brief Clears ReconstructionFrame of all connections to other objects
    virtual void Clear();

    virtual void SetGroup(Group& group);
    Group& GetGroup() const;

    RestFrames::RFList<Group> GetListGroups() const;

    virtual bool InitializeStates(const RestFrames::RFList<State>& states, 
				  const RestFrames::RFList<Group>& groups);
    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    vector<RestFrames::RFList<State> > m_ChildStates;
    Group* m_GroupPtr;

    virtual bool InitializeStatesRecursive(const RestFrames::RFList<State>& states, 
					   const RestFrames::RFList<Group>& groups);
    virtual bool InitializeNoGroupStates(const RestFrames::RFList<State>& states);
    virtual bool InitializeGroupStates(const RestFrames::RFList<Group>& groups);

    void FillListGroupsRecursive(RestFrames::RFList<Group>& groups) const;

  private:
    void Init();

  };

}

#endif
