#include "TArc.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "RestFrames/TreePlotLink.hh"
#include "RestFrames/Jigsaw.hh"

using namespace std;

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

  void TreePlotLink::SetLabel(const string& label){ 
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

  string TreePlotLink::GetLabel() const { 
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
