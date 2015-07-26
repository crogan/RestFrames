
#include "RestFrames/TreePlotNode.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/State.hh"

using namespace std;

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
  
  void TreePlotNode::SetLabel(const string& label){ 
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

  string TreePlotNode::GetLabel() const { 
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
  
  RFList<const Jigsaw> TreePlotNode::GetJigsawList() const {
    return m_Jigsaws;
  }

}
