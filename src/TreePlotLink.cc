/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TreePlotLink.cc
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

#include "RestFrames/TreePlotLink.hh"
#include "RestFrames/Jigsaw.hh"

namespace RestFrames {

  TreePlotLink::TreePlotLink(TreePlotNode* Node1Ptr, TreePlotNode* Node2Ptr){
    m_Node1Ptr = Node1Ptr;
    m_Node2Ptr = Node2Ptr;
    m_Wavy = false; 
    m_DoLabel = false; 
    m_Label = ""; 
    m_JigsawPtr = nullptr;
  }
  
  TreePlotLink::~TreePlotLink() {}

  void TreePlotLink::SetWavy(bool wavy){ 
    m_Wavy = wavy; 
  }

  void TreePlotLink::SetLabel(const std::string& label){ 
    m_Label = label; 
    m_DoLabel = true; 
  }

  void TreePlotLink::SetJigsaw(const Jigsaw& jigsaw){ 
    m_JigsawPtr = &jigsaw; 
  }
  
  TreePlotNode* TreePlotLink::GetNode1() const { 
    return m_Node1Ptr; 
  }

  TreePlotNode* TreePlotLink::GetNode2() const { 
    return m_Node2Ptr; 
  }

  bool TreePlotLink::DoWavy() const { 
    return m_Wavy; 
  }

  std::string TreePlotLink::GetLabel() const { 
    return m_Label; 
  }

  bool TreePlotLink::DoLabel() const { 
    return m_DoLabel; 
  }
  
  Jigsaw const& TreePlotLink::GetJigsaw() const { 
    if(m_JigsawPtr)
      return *m_JigsawPtr;
    else 
      return Jigsaw::Empty();
  }

}
