/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TreePlotLink.hh
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

#ifndef TreePlotLink_HH
#define TreePlotLink_HH

#include <string>

namespace RestFrames {
  
  class TreePlotNode;
  class Jigsaw;

  class TreePlotLink {
  public:
    TreePlotLink(TreePlotNode* Node1Ptr, TreePlotNode* Node2Ptr);
    ~TreePlotLink();

    void SetLabel(const std::string& label);
    void SetWavy(bool wavy);
    void SetJigsaw(const Jigsaw& jigsaw);

    TreePlotNode* GetNode1() const;
    TreePlotNode* GetNode2() const;
    bool DoWavy() const;
    std::string GetLabel() const;
    bool DoLabel() const;
    Jigsaw const& GetJigsaw() const;

  private:
    TreePlotNode* m_Node1Ptr;
    TreePlotNode* m_Node2Ptr;
    bool m_Wavy;
    bool m_DoLabel;
    std::string m_Label;
    const Jigsaw* m_JigsawPtr;
   
  };

}

#endif
