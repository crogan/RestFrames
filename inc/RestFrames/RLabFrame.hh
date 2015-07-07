/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RLabFrame.hh
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

#ifndef RLabFrame_HH
#define RLabFrame_HH

#include "RestFrames/LabFrame.hh"
#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

  class Jigsaw;

  ///////////////////////////////////////////////
  // RLabFrame class
  ///////////////////////////////////////////////
  class RLabFrame : public LabFrame, public RFrame {
  public:
    RLabFrame(const string& sname, const string& stitle);
    virtual ~RLabFrame();
  
    virtual void Clear();

    virtual bool InitializeAnalysis();
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

  protected:
    RestFrames::RFList<Group>  m_LabGroups;
    RestFrames::RFList<Jigsaw> m_LabJigsaws;
    RestFrames::RFList<State> m_LabStates;
  
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
