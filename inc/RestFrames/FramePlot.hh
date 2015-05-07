#ifndef FramePlot_HH
#define FramePlot_HH
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/FrameLink.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/CombinatoricGroup.hh"

using namespace std;

namespace RestFrames {

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
  //const int color_Leaf[3] = {kOrange-4,kCyan-6,kMagenta-8};
  const int color_Leaf[3] = {kCyan-6,kOrange-4,kMagenta-9};
  const int style_Leaf = 7;

  enum FramePlotType { PNone, PFrame, PGroup };

  ///////////////////////////////////////////////
  // FramePlot class
  ///////////////////////////////////////////////
  class FramePlot {
  public:
    FramePlot(const string& sname, const string& stitle);
    ~FramePlot();

    // tree drawing functions
    void AddFrameTree(const RestFrame* framePtr);
    void AddFrameTree(const RFrame* framePtr, const JigsawList* jigsawsPtr);
    void AddFrameTree(const RFrame* framePtr, Jigsaw* jigsawPtr);
    void AddGroupTree(const Group* groupPtr);
    void AddJigsaw(Jigsaw* jigsawPtr);

    void AddFrameTree(const RestFrame& frame);
    void AddFrameTree(const RFrame& frame, const JigsawList& jigsaws);
    void AddFrameTree(const RFrame& frame, Jigsaw& jigsaw);
    void AddGroupTree(const Group& group);
    void AddJigsaw(Jigsaw& jigsaw);

    void DrawFramePlot();

    // canvas initialization and retrieval
    void SetCanvas(double xpix = 600., double ypix = 600.);
    void SetCanvas(TCanvas* canvasPtr);
    TCanvas* GetCanvas() const;

  private:
    string m_Name;
    string m_Title;
    FramePlotType m_Type;

    TCanvas* m_CanvasPtr;
    vector<TObject*> m_Objects;
  
    int m_Nrow;
    vector<int> m_Ncol;
    double m_Node_R;  

    int m_NInvJigsaw;
    int m_NCombJigsaw;
    bool m_SelfAssembling;
    map<Jigsaw*,int> m_JigsawColorMap;
    map<FrameType,int> m_FrameColorMap;
    map<FrameType,int> m_FrameColorFillMap;

    RestFrameList m_Frames;
    JigsawList m_Jigsaws;
    const Group* m_GroupPtr;
   
    void ClearTree();
    void ClearCanvas();

    void InitTreeGrid();

    string GetStateTitle(State* statePtr);
    string GetSetTitle(const string& set, const string& index);

    void AddFrame(const RestFrame* framePtr);
    void AddFrame(RestFrame* framePtr);
    void FillFrameTree(const RestFrame* framePtr);
    void FillFrameTreeMap(int irow, const RestFrame* framePtr);
    void FillFrameTreeMap(int irow, const RDecayFrame* framePtr);
    void FillGroupTree(const Group* groupPtr);
    void FillGroupTreeMap(int irow, State* statePtr);
    void FillJigsawLink(Jigsaw* jigsawPtr);

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

    void ConvertNodeCoordinates(vector<FramePlotNode*>* nodesPtr);
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
    void SetFrame(const RestFrame* framePtr);
    void AddJigsaw(Jigsaw* jigsawPtr);
    void SetState(State* statePtr);
    void SetLabel(const string& label);
    void SetSquare(bool square);
    double GetX() const;
    double GetY() const;
    string GetLabel() const;
    const RestFrame* GetFrame() const;
    int GetNJigsaws() const;
    JigsawList* GetJigsawList() const;
    State* GetState() const;
    bool DoLabel() const;
    bool DoSquare() const;

  private:
    double m_X;
    double m_Y;
    string m_Label;
    bool m_DoLabel;
    bool m_DoSquare;
    const RestFrame* m_FramePtr;
    JigsawList* m_JigsawsPtr;
    State* m_StatePtr;
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
    void SetJigsaw(Jigsaw* jigsawPtr);

    FramePlotNode* GetNode1() const;
    FramePlotNode* GetNode2() const;
    bool DoWavy() const;
    string GetLabel() const;
    bool DoLabel() const;
    Jigsaw* GetJigsaw() const;

  private:
    FramePlotNode* m_Node1Ptr;
    FramePlotNode* m_Node2Ptr;
    bool m_Wavy;
    bool m_DoLabel;
    string m_Label;
    Jigsaw* m_JigsawPtr;
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
