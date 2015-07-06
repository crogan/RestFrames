#ifndef FramePlot_HH
#define FramePlot_HH
#include <sstream>
#include <algorithm>
#include <map>
#include <TCanvas.h>
#include "RestFrames/RFBase.hh"
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RFrame;
  class RDecayFrame;
  class Jigsaw;
  class Group;
  class State;
  class FramePlotNode;
  class FramePlotLink;

  ///////////////////////////////////////////////
  // Colors and styles
  ///////////////////////////////////////////////
  const int color_Node[4] = {kBlue+3,kGreen+3,kRed+3,kGray+3};
  const int color_fill_Node[4] = {kBlue-10,kGreen-10,kRed-10,18};

  const int color_Default = kGray+3;
  const int color_fill_Default = 18;
  const int style_Default = 1;

  //const int color_Leaf[3] = {kOrange-4,kMagenta-8,kCyan-6};
  const int color_Leaf[3] = {kOrange-4,kCyan-6,kMagenta-8};
  //const int color_Leaf[3] = {kCyan-6,kOrange-4,kMagenta-9};
  const int style_Leaf = 7;

  enum FramePlotType { PNone, PFrame, PGroup };

  ///////////////////////////////////////////////
  // FramePlot class
  ///////////////////////////////////////////////
  class FramePlot : public RFBase {
  public:
    FramePlot(const string& sname, const string& stitle);
    ~FramePlot();

    // tree drawing functions
    void AddFrameTree(const RestFrame& frame);
    void AddFrameTree(const RFrame& frame, const RestFrames::RFList<Jigsaw>& jigsaws);
    void AddFrameTree(const RFrame& frame, const Jigsaw& jigsaw);
    void AddGroupTree(const Group& group);
    void AddJigsaw(const Jigsaw& jigsaw);

    void DrawFramePlot();

    // canvas initialization and retrieval
    void SetCanvas(double xpix = 600., double ypix = 600.);
    void SetCanvas(TCanvas* canvasPtr);
    TCanvas* GetCanvas() const;

  private:
    FramePlotType m_Type;

    TCanvas* m_CanvasPtr;
    vector<TObject*> m_Objects;
  
    int m_Nrow;
    vector<int> m_Ncol;
    double m_Node_R;  

    int m_NInvJigsaw;
    int m_NCombJigsaw;
    bool m_SelfAssembling;
    map<const Jigsaw*,int> m_JigsawColorMap;
    map<FrameType,int> m_FrameColorMap;
    map<FrameType,int> m_FrameColorFillMap;

    vector<const RestFrame*> m_Frames;
    vector<const Jigsaw*> m_Jigsaws;
    const Group* m_GroupPtr;
   
    void ClearTree();
    void ClearCanvas();

    void InitTreeGrid();

    string GetStateTitle(const State& state);
    string GetSetTitle(const string& set, const string& index);

    void AddFrame(const RestFrame& frame);
    void FillFrameTree(const RestFrame& frame);
    void FillFrameTreeMap(int irow, const RestFrame& frame);
    void FillFrameTreeMap(int irow, const RDecayFrame& frame);
    void FillGroupTree(const Group& group);
    void FillGroupTreeMap(int irow, const State& state);
    void FillJigsawLink(const Jigsaw& jigsaw);

    vector<FramePlotNode*> m_TreeNodes;
    vector<FramePlotLink*> m_TreeLinks;
    void DrawTreeLinks();
    void DrawTreeNodes(bool with_rings = false);

    vector<FramePlotLink*> m_LeafLinks;
    void DrawLeafLinks();

    void DrawLink(FramePlotLink* linkPtr);
    void DrawNode(FramePlotNode* nodePtr, bool with_rings = false);

    void DrawFrameTypeLegend();
    void DrawTitle(const string& title);
    void DrawJigsawLegend();

    void ConvertNodeCoordinates(vector<FramePlotNode*>& nodesPtr);

    int GetJigsawPriority(int Nout, int Ndep) const;
  };

  ///////////////////////////////////////////////
  // FramePlotNode class
  ///////////////////////////////////////////////
  class FramePlotNode {
  public:
    FramePlotNode();
    ~FramePlotNode();
    
    void SetX(double x);
    void SetY(double y);
    void SetFrame(const RestFrame& frame);
    void AddJigsaw(const Jigsaw& jigsaw);
    void SetState(const State& state);
    void SetLabel(const string& label);
    void SetSquare(bool square);
    double GetX() const;
    double GetY() const;
    string GetLabel() const;
    RestFrame const& GetFrame() const;
    int GetNJigsaws() const;
    vector<const RestFrames::Jigsaw*> GetJigsawList() const;
    State const& GetState() const;
    bool DoLabel() const;
    bool DoSquare() const;

  private:
    double m_X;
    double m_Y;
    string m_Label;
    bool m_DoLabel;
    bool m_DoSquare;
    const RestFrame* m_FramePtr;
    vector<const Jigsaw*> m_Jigsaws;
    const State* m_StatePtr;
    void Init();
  };

  ///////////////////////////////////////////////
  // FramePlotLink class
  ///////////////////////////////////////////////
  class FramePlotLink {
  public:
    FramePlotLink(FramePlotNode* Node1Ptr, FramePlotNode* Node2Ptr);
    ~FramePlotLink();

    void SetLabel(const string& label);
    void SetWavy(bool wavy);
    void SetJigsaw(const Jigsaw& jigsaw);

    FramePlotNode* GetNode1() const;
    FramePlotNode* GetNode2() const;
    bool DoWavy() const;
    string GetLabel() const;
    bool DoLabel() const;
    Jigsaw const& GetJigsaw() const;

  private:
    FramePlotNode* m_Node1Ptr;
    FramePlotNode* m_Node2Ptr;
    bool m_Wavy;
    bool m_DoLabel;
    string m_Label;
    const Jigsaw* m_JigsawPtr;
    void Init();
  };

  ///////////////////////////////////////////////
  // Utility functions
  ///////////////////////////////////////////////
  template <typename T>
  string NumToString ( T Num )
  {
    ostringstream ss;
    ss << Num;
    return ss.str();
  }

}

#endif
