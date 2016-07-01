/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TreePlot.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 July
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

#ifndef TreePlot_HH
#define TreePlot_HH

#include <TCanvas.h>

#include "RestFrames/RFPlot.hh"
#include "RestFrames/RestFrame.hh"

namespace RestFrames {

  class ReconstructionFrame;
  class DecayRecoFrame;
  class Jigsaw;
  class Group;
  class State;
  class TreePlotNode;
  class TreePlotLink;

  enum TreeType { kVanillaTree, kFrameTree, kGroupTree };

  class TreePlot : public RFPlot {
  public:
    TreePlot(const std::string& sname, const std::string& stitle);
    ~TreePlot();

    virtual void Clear();

    void SetTree(const RestFrame& frame);
    void SetTree(const Group& group);
    void SetTree(const Jigsaw& jigsaw);
    
    void AddJigsaw(const Jigsaw& jigsaw);

    void Draw(const std::string& name = "",
	      const std::string& title = "",
	      bool invert_colors  = false,
	      bool invert_node_colors = false);
    
  private:
    TreeType m_Type;
  
    int m_Nrow;
    std::vector<int> m_Ncol;
    double m_Node_R;  

    bool m_SelfAssembling;
    std::map<const Jigsaw*,int> m_JigsawColorMap;
    std::map<FrameType,int> m_FrameColorMap;

    ConstRestFrameList m_Frames;
    ConstJigsawList    m_Jigsaws;
    const Group* m_GroupPtr;

    int m_color_Node_text[4];
    int m_color_Node_line[4];
    int m_color_Node_fill[4];
    int m_color_Default_text;
    int m_color_Default_line;
    int m_color_Default_fill;
    int m_color_Text;
    int m_color_Bkg;
    std::vector<int> m_color_Leaf;
    int m_style_Default;
    int m_style_Leaf;
    
    void ClearTree();

    void InitTreeGrid();

    std::string GetStateTitle(const State& state);
    std::string GetSetTitle(const std::string& set, 
			    const std::string& index);

    void AddFrame(const RestFrame& frame);
    void FillFrameTree(const RestFrame& frame);
    void FillFrameTreeMap(int irow, const RestFrame& frame);
    void FillFrameTreeMap(int irow, const DecayRecoFrame& frame);
    void FillGroupTree(const Group& group);
    void FillJigsawTree(const Jigsaw& jigsaw);
    void FillStateTreeMap(int irow, const State& state);
    void FillJigsawLink(const Jigsaw& jigsaw);

    void SetColors(bool invert_bkg_color,
		   bool invert_node_color);
    
    std::vector<TreePlotNode*> m_TreeNodes;
    std::vector<TreePlotLink*> m_TreeLinks;
    void DrawTreeLinks();
    void DrawTreeNodes(bool with_rings = false);

    std::vector<TreePlotLink*> m_LeafLinks;
    void DrawLeafLinks();

    void DrawLink(TreePlotLink* linkPtr);
    void DrawNode(TreePlotNode* nodePtr, bool with_rings = false);

    void DrawFrameTypeLegend();
    void DrawTitle(const std::string& title);
    void DrawJigsawLegend();

    void ConvertNodeCoordinates(std::vector<TreePlotNode*>& nodesPtr);

    int GetJigsawPriority(int Nout, int Ndep) const;
    TCanvas* GetNewCanvas(const std::string& name, 
			  const std::string& title);
  };

}

#endif
