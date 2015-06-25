/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFrame.hh
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

#ifndef RFrame_HH
#define RFrame_HH

#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class Group;
  class StateList;

  ///////////////////////////////////////////////
  // RFrame class
  ///////////////////////////////////////////////
  class RFrame : public virtual RestFrame {
  public:
    RFrame(const string& sname, const string& stitle);
    virtual ~RFrame();

    void RemoveChildStates();

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const { return m_GroupPtr; }

    RFList<Group>* GetListGroups() const;

    virtual bool InitializeStates(const StateList* statesPtr, const RFList<Group>* groupsPtr);
    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

  protected:
    vector<StateList> m_ChildStates;
    Group* m_GroupPtr;

    virtual bool InitializeStatesRecursive(const StateList* statesPtr, const RFList<Group>* groupsPtr);
    virtual bool InitializeNoGroupStates(const StateList* statesPtr);
    virtual bool InitializeGroupStates(const RFList<Group>* groupsPtr);

    void FillListGroupsRecursive(RFList<Group>* groupsPtr) const;

  private:
    void Init();

  };

}

#endif
