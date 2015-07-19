/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabFrame.hh
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

#ifndef LabFrame_HH
#define LabFrame_HH

#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabFrame class
  ///////////////////////////////////////////////
  template <class T>
  class LabFrame : public T {
  public:
    LabFrame(const string& sname, const string& stitle);
    LabFrame();
    virtual ~LabFrame();

    void SetChildFrame(RestFrame& frame);
    void SetChildFrame(RestFrame* framePtr);
  
    virtual bool InitializeTree();
    virtual bool InitializeAnalysis() = 0;
    virtual bool ClearEvent() = 0;
    virtual bool AnalyzeEvent() = 0;

    // Analysis functions
    TVector3 GetInvisibleMomentum() const;

  protected:
    virtual bool IsSoundBody() const;

  private:
    void Init();
  };

}

#endif
