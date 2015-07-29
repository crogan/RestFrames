/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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

using namespace std;

namespace RestFrames {

  class ReconstructionFrame;
  class DecayRecoFrame;
  class Jigsaw;
  class Group;
  class State;
  class TreePlotNode;
  class TreePlotLink;

  ///////////////////////////////////////////////
  // Colors and styles
  ///////////////////////////////////////////////
  const int color_Node[4] = {kBlue+3,kGreen+3,kRed+3,kGray+3};
  const int color_fill_Node[4] = {kBlue-10,kGreen-10,kRed-10,18};

  const int color_Default = kGray+3;
  const int color_fill_Default = 18;
  const int style_Default = 1;

  const int color_Leaf[6] = {kCyan-6,kOrange-4,kMagenta-8,
			     kCyan-7,kOrange+6,kMagenta-2};
  const int style_Leaf = 7;

  enum TreeType { kVanillaTree, kFrameTree, kGroupTree };

  class TreePlot : public RFPlot {
  public:
    TreePlot(const string& sname, const string& stitle);
    ~TreePlot();

    virtual void Clear();

    void SetFrameTree(const RestFrame& frame);
    void AddJigsaws(const RestFrames::RFList<Jigsaw>& jigsaws);
    void AddJigsaw(const Jigsaw& jigsaw);
    
    void SetGroupTree(const Group& group);

    void Draw(const string& name = "", const string& title = "");
    
  private:
    TreeType m_Type;
  
    int m_Nrow;
    vector<int> m_Ncol;
    double m_Node_R;  

    bool m_SelfAssembling;
    map<const Jigsaw*,int> m_JigsawColorMap;
    map<FrameType,int> m_FrameColorMap;
    map<FrameType,int> m_FrameColorFillMap;

    RestFrames::RFList<const RestFrame> m_Frames;
    RestFrames::RFList<const Jigsaw>    m_Jigsaws;
    const Group* m_GroupPtr;
   
    void ClearTree();

    void InitTreeGrid();

    string GetStateTitle(const State& state);
    string GetSetTitle(const string& set, const string& index);

    void AddFrame(const RestFrame& frame);
    void FillFrameTree(const RestFrame& frame);
    void FillFrameTreeMap(int irow, const RestFrame& frame);
    void FillFrameTreeMap(int irow, const DecayRecoFrame& frame);
    void FillGroupTree(const Group& group);
    void FillGroupTreeMap(int irow, const State& state);
    void FillJigsawLink(const Jigsaw& jigsaw);

    vector<TreePlotNode*> m_TreeNodes;
    vector<TreePlotLink*> m_TreeLinks;
    void DrawTreeLinks();
    void DrawTreeNodes(bool with_rings = false);

    vector<TreePlotLink*> m_LeafLinks;
    void DrawLeafLinks();

    void DrawLink(TreePlotLink* linkPtr);
    void DrawNode(TreePlotNode* nodePtr, bool with_rings = false);

    void DrawFrameTypeLegend();
    void DrawTitle(const string& title);
    void DrawJigsawLegend();

    void ConvertNodeCoordinates(vector<TreePlotNode*>& nodesPtr);

    int GetJigsawPriority(int Nout, int Ndep) const;
    TCanvas* GetNewCanvas(const string& name, const string& title);
  };

}

#endif
