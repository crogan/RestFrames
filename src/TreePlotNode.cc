/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TreePlotNode.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
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

#include "RestFrames/TreePlotNode.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/State.hh"

namespace RestFrames {
  
  TreePlotNode::TreePlotNode(){ 
    m_X = 0;
    m_Y = 0;
    m_Label = "";
    m_DoLabel = false;
    m_DoSquare = false;
    m_FramePtr = nullptr;
    m_StatePtr = nullptr;
  }
  
  TreePlotNode::~TreePlotNode() {}
    
  void TreePlotNode::SetX(double x){ 
    m_X = x; 
  }
  
  void TreePlotNode::SetY(double y){ 
    m_Y = y; 
  }
  
  void TreePlotNode::SetLabel(const std::string& label){ 
    m_Label = label; 
    m_DoLabel = true; 
  }

  void TreePlotNode::SetSquare(bool square){ 
    m_DoSquare = square; 
  }
  
  void TreePlotNode::SetFrame(const RestFrame& frame){ 
    m_FramePtr = &frame; 
  }

  void TreePlotNode::SetState(const State& state){ 
    m_StatePtr = &state; 
  }

  void TreePlotNode::AddJigsaw(const Jigsaw& jigsaw){ 
    m_Jigsaws.Add(jigsaw);
  }
  
  double TreePlotNode::GetX() const { 
    return m_X; 
  }

  double TreePlotNode::GetY() const { 
    return m_Y; 
  }

  std::string TreePlotNode::GetLabel() const { 
    return m_Label; 
  }

  const RestFrame& TreePlotNode::GetFrame() const { 
    if(m_FramePtr)
      return *m_FramePtr;
    else 
      return RestFrame::Empty();
  }

  const State& TreePlotNode::GetState() const { 
    if(m_StatePtr)
      return *m_StatePtr;
    else
      return State::Empty();
  }

  bool TreePlotNode::DoLabel() const { 
    return m_DoLabel; 
  }

  bool TreePlotNode::DoSquare() const { 
    return m_DoSquare; 
  }

  int TreePlotNode::GetNJigsaws() const { 
    return m_Jigsaws.GetN(); 
  }
  
  ConstJigsawList TreePlotNode::GetJigsawList() const {
    return m_Jigsaws;
  }

}
