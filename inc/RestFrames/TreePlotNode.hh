/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TreePlotNode.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
//
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

#ifndef TreePlotNode_HH
#define TreePlotNode_HH

#include <string>
#include "RestFrames/RFList.hh"

namespace RestFrames {

  class RestFrame;
  class Jigsaw;
  class State;

  class TreePlotNode {
  public:
    TreePlotNode();
    ~TreePlotNode();
    
    void SetX(double x);
    void SetY(double y);
    void SetLabel(const std::string& label);
    void SetSquare(bool square);
    void SetFrame(const RestFrame& frame);
    void SetState(const State& state);
    void AddJigsaw(const Jigsaw& jigsaw);
  
    double GetX() const;
    double GetY() const;
    bool DoLabel() const;
    bool DoSquare() const;
    std::string GetLabel() const;
    RestFrame const& GetFrame() const;
    int GetNJigsaws() const;
    ConstJigsawList GetJigsawList() const;
    State const& GetState() const;

  private:
    double m_X;
    double m_Y;
    std::string m_Label;
    bool m_DoLabel;
    bool m_DoSquare;
    const RestFrame* m_FramePtr;
    ConstJigsawList m_Jigsaws;
    const State* m_StatePtr;
    void Init();
  };

}

#endif
