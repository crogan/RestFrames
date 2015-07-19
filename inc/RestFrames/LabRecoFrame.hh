/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabRecoFrame.hh
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

#ifndef LabRecoFrame_HH
#define LabRecoFrame_HH

#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/LabFrame.hh"

using namespace std;

namespace RestFrames {

  class Jigsaw;
  class VisibleState;

  ///////////////////////////////////////////////
  // LabRecoFrame class
  ///////////////////////////////////////////////
  class LabRecoFrame : public LabFrame<ReconstructionFrame> {
  public:
    LabRecoFrame(const string& sname, const string& stitle);
    LabRecoFrame();
    virtual ~LabRecoFrame();
    
    virtual void Clear();

    virtual bool InitializeAnalysis();
    virtual bool ClearEvent();
    virtual bool AnalyzeEvent();

    void AddVisibleState(VisibleState& state) const;
    void AddVisibleStates(const RestFrames::RFList<VisibleState>& states) const;
    void RemoveVisibleState(const VisibleState& state) const;
    void RemoveVisibleStates(const RestFrames::RFList<VisibleState>& states) const;
    RestFrames::RFList<VisibleState> const& GetVisibleStates() const;

  protected:
    RestFrames::RFList<Group>  m_LabGroups;
    RestFrames::RFList<Jigsaw> m_LabJigsaws;
    mutable RestFrames::RFList<VisibleState>  m_LabStates;
  
    bool InitializeLabStates();
    bool InitializeLabGroups();
    bool InitializeLabJigsaws();

    bool ExecuteJigsaws();

  private:
    void Init();
    void ClearStates();
  };

}

#endif
