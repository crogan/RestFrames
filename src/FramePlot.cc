#include "TArc.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "RestFrames/FramePlot.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/State.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/DecayRecoFrame.hh"
#include "RestFrames/VisibleRecoFrame.hh"
#include "RestFrames/InvisibleRecoFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // FramePlot class methods
  // class which can plot RestFrame trees
  ///////////////////////////////////////////////
  FramePlot::FramePlot(const string& sname, const string& stitle)
    : RFBase(sname, stitle){
    m_Type = PNone;
    m_GroupPtr = nullptr;
    m_CanvasPtr = nullptr;
    m_Log.SetSource("FramePlot "+GetName());
  }
  
  FramePlot::~FramePlot(){
    ClearTree();
    ClearCanvas();
  }

  void FramePlot::ClearTree(){
    int Ntreenode = m_TreeNodes.size();
    for(int i = 0; i < Ntreenode; i++){
      delete m_TreeNodes[i];
    }
    m_TreeNodes.clear();
    
    int Ntreelink = m_TreeLinks.size();
    for(int i = 0; i < Ntreelink; i++){
      delete m_TreeLinks[i];
    }
    m_TreeLinks.clear();
    
    int Nleaflink = m_LeafLinks.size();
    for(int i = 0; i < Nleaflink; i++){
      delete m_LeafLinks[i];
    }
    m_LeafLinks.clear();

    m_Frames.clear();
    m_Jigsaws.clear();
    m_GroupPtr = nullptr;
    m_Type = PNone;
    m_NInvJigsaw = 0;
    m_NCombJigsaw = 0;
    m_SelfAssembling = false;
    m_JigsawColorMap.clear();
    m_FrameColorMap.clear();
    m_FrameColorFillMap.clear();
  }

  void FramePlot::ClearCanvas(){
     if(m_CanvasPtr){
       delete m_CanvasPtr;
       m_CanvasPtr = nullptr;
     }
     int Nobj = m_Objects.size();
     for(int i = 0; i < Nobj; i++){
       delete m_Objects[i];
     }
     m_Objects.clear();
  }

  void FramePlot::SetCanvas(double xpix, double ypix){
    ClearCanvas();
    m_CanvasPtr = (TCanvas*) new TCanvas(GetName().c_str(),GetTitle().c_str(),xpix,ypix);
    m_CanvasPtr->Range(0.,0.,1.,1.);
  }

  void FramePlot::SetCanvas(TCanvas* canvasPtr){
    if(!canvasPtr) return;
    ClearCanvas();
    m_CanvasPtr = (TCanvas*) canvasPtr;
    m_CanvasPtr->Range(0.,0.,1.,1.);
  }

  TCanvas* FramePlot::GetCanvas() const {
    TCanvas* canvasPtr = (TCanvas*) m_CanvasPtr->DrawClone();
    canvasPtr->SetName((GetName()+"_copy").c_str());
    canvasPtr->SetTitle(GetTitle().c_str());
    return canvasPtr;
  }

  void FramePlot::DrawFramePlot(){
    if(m_Type == PNone) return;
    SetCanvas();

    if(m_Type == PFrame){
      bool do_jigsaws = (m_Jigsaws.size() > 0);
      DrawTreeLinks();
      if(do_jigsaws) DrawTreeNodes(true);
      if(do_jigsaws) DrawLeafLinks();
      DrawTreeNodes();
      DrawFrameTypeLegend();
      if(do_jigsaws) DrawJigsawLegend();
    }
    
    if(m_Type == PGroup){
      DrawTreeLinks();
      DrawTreeNodes();
      DrawTitle(m_GroupPtr->GetTitle());
    }
 
    m_CanvasPtr->Draw();
  }

  void FramePlot::AddFrameTree(const RestFrame& frame){
    if(frame.IsEmpty()) return;
    ClearTree();
    m_Type = PFrame;

    FillFrameTree(frame);
    InitTreeGrid();
    ConvertNodeCoordinates(m_TreeNodes);
  }

  void FramePlot::AddFrameTree(const ReconstructionFrame& frame, const Jigsaw& jigsaw){
    if(frame.IsEmpty()) return;

    AddFrameTree(frame);
    AddJigsaw(jigsaw);
  }

  void FramePlot::AddFrameTree(const ReconstructionFrame& frame, const RFList<Jigsaw>& jigsaws){
    if(frame.IsEmpty()) return;

    AddFrameTree(frame);

    int N = jigsaws.GetN();
    for(int j = 0; j < N; j++) AddJigsaw(jigsaws.Get(j));
  }

  void FramePlot::AddJigsaw(const Jigsaw& jigsaw){
    if(jigsaw.IsEmpty()) return;
    
    int Nj = m_Jigsaws.size();
    for(int i = 0; i < Nj; i++)
      if(*m_Jigsaws[i] == jigsaw)
	return;

    RFList<RestFrame> frames = jigsaw.GetParentFrames();
    int N = frames.GetN();
    int M = m_Frames.size();
    for(int i = 0; i < N; i++){
      bool found = false;
      for(int j = 0; j < M; j++){
	if(frames.Get(i) == *m_Frames[j]){
	  found = true;
	  break;
	}
      }
      if(!found) return;
    }
    
    FillJigsawLink(jigsaw);
    if(jigsaw.IsInvisibleJigsaw()){
      m_JigsawColorMap[&jigsaw] = color_Leaf[0]+m_NInvJigsaw;
      m_NInvJigsaw++;
    }
    if(jigsaw.IsCombinatoricJigsaw()){
      m_JigsawColorMap[&jigsaw] = color_Leaf[1]+m_NCombJigsaw;
      m_NCombJigsaw++;
    }
  }

  void FramePlot::AddGroupTree(const Group& group){
    ClearTree();
    if(!group) return;
    m_GroupPtr = &group;
    m_Type = PGroup;

    FillGroupTree(group);
    InitTreeGrid();
    ConvertNodeCoordinates(m_TreeNodes);
  }

  void FramePlot::InitTreeGrid(){
    int NcolMAX = 0;
    for(int irow = 0; irow < m_Nrow; irow++){
      if(m_Ncol[irow] > NcolMAX) NcolMAX = m_Ncol[irow];
    }
    m_Node_R = min(min(0.85/double(2*NcolMAX+1),0.85*0.85/double(2*m_Nrow+1)),0.12);
    if(m_Type == PGroup) m_Node_R = min(min(0.65/double(2*NcolMAX+1),0.85/double(2*m_Nrow+1)),0.12);
  }

  void FramePlot::ConvertNodeCoordinates(vector<FramePlotNode*>& nodes){
    double xmin = 0.;
    double xmax = 1.;
    double ymin = 0.;
    double ymax = 1.;
    if(m_Type == PFrame) ymax = 0.8;
    int Nnode = nodes.size();
    for(int i = 0; i < Nnode; i++){
      double new_x = nodes[i]->GetX();
      double new_y = nodes[i]->GetY();
      new_x = xmin + (xmax-xmin)*(new_x+0.5)/double(m_Ncol[int(new_y)]);
      new_y = ymin + (ymax-ymin)*(1.-(new_y+0.5)/double(m_Nrow));
      nodes[i]->SetX(new_x);
      nodes[i]->SetY(new_y);
    }
  }

  void FramePlot::AddFrame(const RestFrame& frame){
    int N = m_Frames.size();
    for(int i = 0; i < N; i++)
      if(*m_Frames[i] == frame)
	return;
    if(m_FrameColorMap[frame.GetType()] <= 0){
      m_FrameColorMap[frame.GetType()] = color_Node[int(frame.GetType())-1];
      m_FrameColorFillMap[frame.GetType()] = color_fill_Node[int(frame.GetType())-1];
    }
  }

  void FramePlot::FillFrameTree(const RestFrame& frame){
    m_Nrow = 0;
    m_Ncol.clear();

    FramePlotNode* top_nodePtr = new FramePlotNode();
    top_nodePtr->SetX(0.);
    top_nodePtr->SetY(0.);
    top_nodePtr->SetFrame(frame);
    top_nodePtr->SetLabel(frame.GetTitle());
    m_TreeNodes.push_back(top_nodePtr);
    AddFrame(frame);
    m_Ncol.push_back(1);
    FillFrameTreeMap(0, frame);
    m_Nrow = m_Ncol.size();
  }

  void FramePlot::FillFrameTreeMap(int irow, const RestFrame& frame){
    if(frame.IsDecayFrame() && frame.IsRecoFrame()){
      const DecayRecoFrame* rframePtr = dynamic_cast<const DecayRecoFrame*>(&frame);
      if(rframePtr){
	if(rframePtr->IsSelfAssemblingFrame()){
	  FillFrameTreeMap(irow, *rframePtr);
	  return;
	}
      }
    }
    FramePlotNode* frame_nodePtr = m_TreeNodes[m_TreeNodes.size()-1];

    int Nchild = frame.GetNChildren();
    for(int i = 0; i < Nchild; i++){
      const RestFrame& child = frame.GetChildFrame(i);
      if(irow+1 >= int(m_Ncol.size())){
  	m_Ncol.push_back(0);
      }
      FramePlotNode* child_nodePtr = new FramePlotNode();
      child_nodePtr->SetX(m_Ncol[irow+1]);
      child_nodePtr->SetY(irow+1);
      child_nodePtr->SetFrame(child);
      child_nodePtr->SetLabel(child.GetTitle());
      m_TreeNodes.push_back(child_nodePtr);
      AddFrame(child);
      FramePlotLink* linkPtr = new FramePlotLink(frame_nodePtr,child_nodePtr);
      m_TreeLinks.push_back(linkPtr);
      m_Ncol[irow+1]++;
      FillFrameTreeMap(irow+1,child);
    }
  }

  void FramePlot::FillFrameTreeMap(int irow, const DecayRecoFrame& frame){
    FramePlotNode* frame_nodePtr = m_TreeNodes[m_TreeNodes.size()-1];
    frame_nodePtr->SetSquare(true);
    m_SelfAssembling = true;

    int Nchild = frame.GetNChildren();
    for(int i = 0; i < Nchild; i++){
      const RestFrame& child = frame.GetChildFrame(i);
      bool expand_child = false;
       int N = -1;
       bool excl = false;
      if(child.IsVisibleFrame() && child.IsRecoFrame()){
	const VisibleRecoFrame* rvframePtr = dynamic_cast<const VisibleRecoFrame*>(&child);
	if(!rvframePtr) continue;
	CombinatoricGroup* groupPtr = dynamic_cast<CombinatoricGroup*>(&rvframePtr->GetGroup());
	if(groupPtr){
	  groupPtr->GetNElementsForFrame(*rvframePtr, N, excl);
	  if( (N >= 2 && excl) || (N >= 1 && !excl) ) expand_child = true;
	}
      }
      if(irow+1 >= int(m_Ncol.size())){
	m_Ncol.push_back(0);
      }
      if(expand_child){
	for(int j = 0; j < 3; j++){
	  FramePlotNode* child_nodePtr = new FramePlotNode();
	  child_nodePtr->SetX(m_Ncol[irow+1]+ double(j-1)*0.08);
	  child_nodePtr->SetY(irow+1);
	  child_nodePtr->SetFrame(child);
	  if(j == 2) child_nodePtr->SetLabel(GetSetTitle(child.GetTitle(),"i"));
	  m_TreeNodes.push_back(child_nodePtr);
	  FramePlotLink* linkPtr = new FramePlotLink(frame_nodePtr,child_nodePtr);
	  m_TreeLinks.push_back(linkPtr);
	}
	m_Ncol[irow+1]++;
	AddFrame(child);
      } else {
	FramePlotNode* child_nodePtr = new FramePlotNode();
	child_nodePtr->SetX(m_Ncol[irow+1]);
	child_nodePtr->SetY(irow+1);
	child_nodePtr->SetFrame(child);
	child_nodePtr->SetLabel(child.GetTitle());
	m_TreeNodes.push_back(child_nodePtr);
	AddFrame(child);
	FramePlotLink* linkPtr = new FramePlotLink(frame_nodePtr,child_nodePtr);
	m_TreeLinks.push_back(linkPtr);
	m_Ncol[irow+1]++;
	FillFrameTreeMap(irow+1,child);
      }
    }
  }

  void FramePlot::FillGroupTree(const Group& group){
    m_Nrow = 0;
    m_Ncol.clear();
  
    const State& top_state = group.GetParentState();
    if(!top_state) return;

    FramePlotNode* top_nodePtr = new FramePlotNode();
    top_nodePtr->SetX(0.);
    top_nodePtr->SetY(0.);
    top_nodePtr->SetState(top_state);
    top_nodePtr->SetLabel(GetStateTitle(top_state));
    m_TreeNodes.push_back(top_nodePtr);
    m_Ncol.push_back(1);
    FillGroupTreeMap(0, top_state);

    m_Nrow = m_Ncol.size();
  }

  void FramePlot::FillGroupTreeMap(int irow, const State& state){
    const Jigsaw& jigsaw = state.GetChildJigsaw();
    if(!jigsaw)
      return;
    
    FramePlotNode* state_nodePtr = m_TreeNodes[int(m_TreeNodes.size())-1];
    int Nchild = jigsaw.GetNChildren();

    for(int i = 0; i < Nchild; i++){
      State& child = jigsaw.GetChildState(i);
      if(child.IsEmpty()) continue;
      if(irow+1 >= int(m_Ncol.size())){
	m_Ncol.push_back(0);
      }
      FramePlotNode *child_nodePtr = new FramePlotNode();
      child_nodePtr->SetX(m_Ncol[irow+1]);
      child_nodePtr->SetY(irow+1);
      child_nodePtr->SetState(child);
      child_nodePtr->SetLabel(GetStateTitle(child));
      m_TreeNodes.push_back(child_nodePtr);
      FramePlotLink* linkPtr = new FramePlotLink(state_nodePtr,child_nodePtr);
      linkPtr->SetJigsaw(jigsaw);
      linkPtr->SetLabel(jigsaw.GetTitle());
      m_TreeLinks.push_back(linkPtr);
      m_Ncol[irow+1]++;
      FillGroupTreeMap(irow+1, child);
    }
  }

  void FramePlot::FillJigsawLink(const Jigsaw& jigsaw){
    int Nsplit = jigsaw.GetNChildren();
    Group& group = jigsaw.GetGroup();
    if(group.IsEmpty()) return;
    FramePlotNode* high_old = nullptr;
    FramePlotNode* high_new = nullptr;
    //vector<RFList<RestFrame> > child_frames;
    for(int s = 0; s < Nsplit; s++){
      RFList<RestFrame> frames = jigsaw.GetChildFrames(s);
      int Nnode = m_TreeNodes.size();
      FramePlotNode* last_nodePtr = nullptr;
      double high = -1.;
      for(int n = 0; n < Nnode; n++){
	FramePlotNode* nodePtr = m_TreeNodes[n];
	const RestFrame& frame = nodePtr->GetFrame();
	if(frames.Contains(frame)){
	  nodePtr->AddJigsaw(jigsaw);
	  if(group.ContainsFrame(frame)){
	    if(nodePtr->GetY() > high){
	      high_new = nodePtr;
	      high = nodePtr->GetY();
	    }
	  }
	  if(last_nodePtr){
	    if(!(last_nodePtr->GetFrame() == nodePtr->GetFrame())){
	      FramePlotLink* linkPtr = new FramePlotLink(last_nodePtr,nodePtr);
	      linkPtr->SetJigsaw(jigsaw);
	      m_LeafLinks.push_back(linkPtr);
	    }
	  }
	  last_nodePtr = nodePtr;
	}
      }
      
      if(s != 0){
	FramePlotLink* linkPtr = new FramePlotLink(high_old,high_new);
	linkPtr->SetWavy(true);
	linkPtr->SetJigsaw(jigsaw);
	m_LeafLinks.push_back(linkPtr);
      }
      high_old = high_new;
    }
  }

  void FramePlot::DrawTreeNodes(bool with_rings){
    int Nnode = m_TreeNodes.size();
    for(int i = 0; i < Nnode; i++)
      DrawNode(m_TreeNodes[i], with_rings);
  }

  void FramePlot::DrawTreeLinks(){
    int Nlink = m_TreeLinks.size();
    for(int i = 0; i < Nlink; i++)
      DrawLink(m_TreeLinks[i]);
  }

  void FramePlot::DrawLeafLinks(){
    int Nlink = m_LeafLinks.size();
    for(int i = 0; i < Nlink; i++)
      DrawLink(m_LeafLinks[i]);
  }

  void FramePlot::DrawLink(FramePlotLink* linkPtr){
    double x0 = linkPtr->GetNode1()->GetX();
    double y0 = linkPtr->GetNode1()->GetY();
    double x1 = linkPtr->GetNode2()->GetX();
    double y1 = linkPtr->GetNode2()->GetY();

    int icolor = color_Default;
    int icolor_fill = -1;
    int istyle = style_Default;
    int isize = int(m_Node_R*70.);
    bool DoWavy = linkPtr->DoWavy();

    const Jigsaw& jigsaw = linkPtr->GetJigsaw();

    if(!jigsaw.IsEmpty()){
      if(linkPtr->DoLabel()){
	int priority = GetJigsawPriority(jigsaw.GetNChildren(), jigsaw.GetNDependancyStates());
	icolor = color_Node[priority];
	icolor_fill = color_fill_Node[priority];
      } else {
	int Nj = m_Jigsaws.size();
	int index = -1;
	for(int i = 0; i < Nj; i++){
	  if(*m_Jigsaws[i] == jigsaw){
	    index = i;
	    break;
	  }
	}
	x0 += (double(index+1)-double(Nj+1)/2.)*m_Node_R*1./max(4.,double(Nj));
	x1 += (double(index+1)-double(Nj+1)/2.)*m_Node_R*1./max(4.,double(Nj));
	icolor = m_JigsawColorMap[&jigsaw];
	if(DoWavy){
	  istyle = style_Leaf;
	  isize++;
	}
      }
    }
      
    if(fabs(y0-y1) > 1e-10){ // nodes are at different heights - draw line
      TLine *line = new TLine(x0,y0,x1,y1);
      line->SetLineColor(icolor);
      line->SetLineWidth(isize);
      line->SetLineStyle(istyle);
      m_CanvasPtr->cd();
      line->Draw();
      m_Objects.push_back(line);
    } else { // nodes are at same height - draw arc
      TArc *arc;
      double c = fabs(x0-x1);
      double h = 1./double(2*m_Nrow);
      if(h > c/2.){
	double R = c/2.;
	if(h > R+m_Node_R) arc = new TArc((x0+x1)/2.,y0+m_Node_R,R, 0., 180.);
	else arc = new TArc((x0+x1)/2.,y0+h-R,R, 0., 180.);
      } else {
	double R = h/2. + c*c/(8.*h);
	arc = new TArc((x0+x1)/2.,y0+0.5/double(m_Nrow)-R,R,
		       -asin(c/(2.*R))*180/TMath::Pi()+90., 
		       asin(c/(2.*R))*180/TMath::Pi()+90.);
      }
      arc->SetLineColor(icolor);
      arc->SetLineStyle(istyle);
      arc->SetFillStyle(0);
      arc->SetLineWidth(isize);
      m_CanvasPtr->cd();
      arc->Draw("only");
      m_Objects.push_back(arc);
    }
    
    if(linkPtr->DoLabel()){
      double x = x0;
      double y = (y0+y1)/2.;
      
      char* slabel = new char[500];
      sprintf(slabel,"#bf{%s}",linkPtr->GetLabel().c_str());
      TLatex* lat = new TLatex(x,y,slabel);
      lat->SetTextAlign(22);
      lat->SetNDC();
      lat->SetTextSize(1.);
      lat->SetTextFont(132);
      double Xsize = lat->GetXsize();
      double Ysize = lat->GetYsize();
      double scale = max( Xsize/(6.*m_Node_R), 1.25*Ysize/(fabs(y0-y1)-2.*m_Node_R) );
      lat->SetTextSize(1./scale);
      lat->SetTextColor(icolor);
      lat->SetLineColor(icolor);
      
      Xsize = lat->GetXsize()*1.03;
      Ysize = lat->GetYsize();
      TBox *border = new TBox(x-Xsize/2., y-Ysize/2., x+Xsize/2.,y+Ysize/2.);
      border->SetLineColor(icolor);
      border->SetLineWidth(2);
      border->SetLineStyle(1);
      border->SetFillColor(icolor_fill);
      m_CanvasPtr->cd();
      border->Draw("l");
      m_CanvasPtr->cd();
      lat->DrawLatex(x,y,slabel);
      delete[] slabel;
      m_Objects.push_back(lat);
      m_Objects.push_back(border);
    }
  }

  void FramePlot::DrawNode(FramePlotNode* nodePtr, bool with_rings){
    double x = nodePtr->GetX();
    double y = nodePtr->GetY();

    int icolor = color_Default;
    int icolor_fill = color_fill_Default;
    int istyle = 1;
    int iwidth = int(m_Node_R*50.);;
    bool square = nodePtr->DoSquare();

    const RestFrame& frame = nodePtr->GetFrame();

    if(!frame.IsEmpty()){
      icolor = m_FrameColorMap[frame.GetType()];
      icolor_fill = m_FrameColorFillMap[frame.GetType()]; 
    }

    if(with_rings){
      vector<const Jigsaw*> jigsaws = nodePtr->GetJigsawList();
      int Njigsaw = jigsaws.size();
      for(int i = 0; i < Njigsaw; i++){
	double R = 1.03 + double(Njigsaw-i)*0.08;
	const Jigsaw& jigsaw = *jigsaws[i];
	if(square){
	  TBox* ring = new TBox(x-m_Node_R*R*0.88,y-m_Node_R*R*0.88,
				x+m_Node_R*R*0.88,y+m_Node_R*R*0.88);
	  ring->SetLineColor(m_JigsawColorMap[&jigsaw]);
	  ring->SetFillColor(m_JigsawColorMap[&jigsaw]);
	  m_CanvasPtr->cd();
	  ring->Draw();
	  m_Objects.push_back(ring);
	} else {
	  TArc* ring = new TArc(x,y,m_Node_R*R);
	  ring->SetLineColor(m_JigsawColorMap[&jigsaw]);
	  ring->SetFillColor(m_JigsawColorMap[&jigsaw]);
	  m_CanvasPtr->cd();
	  ring->Draw();
	  m_Objects.push_back(ring);
	}
      }
    }
    if(square){
      TBox *box = new TBox(x-m_Node_R*0.88,y-m_Node_R*0.88,
			   x+m_Node_R*0.88,y+m_Node_R*0.88);
      box->SetLineColor(icolor);
      box->SetFillColor(icolor_fill);
      box->SetLineStyle(istyle);
      box->SetLineWidth(iwidth);
      m_CanvasPtr->cd();
      box->Draw("l");
      m_Objects.push_back(box);
    } else {
      TArc *circle = new TArc(x,y,m_Node_R);
      circle->SetLineColor(icolor);
      circle->SetFillColor(icolor_fill);
      circle->SetLineStyle(istyle);
      circle->SetLineWidth(iwidth);
      m_CanvasPtr->cd();
      circle->Draw();
      m_Objects.push_back(circle);
    }

    if(nodePtr->DoLabel()){
      char* nodetitle = new char[200];
      sprintf(nodetitle,"#bf{%s}",nodePtr->GetLabel().c_str());
      TLatex* lat = new TLatex(x,y,nodetitle);
      lat->SetTextAlign(22);
      lat->SetNDC();
      lat->SetTextSize(1.);
      lat->SetTextFont(132);
      double Rnorm = sqrt(lat->GetXsize()*lat->GetXsize()+lat->GetYsize()*lat->GetYsize());
      lat->SetTextSize(1.8*m_Node_R/Rnorm);
      lat->SetTextColor(icolor);
      m_CanvasPtr->cd();
      lat->DrawLatex(x,y,nodetitle);
      delete[] nodetitle;
      m_Objects.push_back(lat);
    }
  }

  string FramePlot::GetStateTitle(const State& state){
    RFList<RestFrame> frames = state.GetListFrames();
    int Nf = frames.GetN();
    string title = "";
    if(Nf > 2) title.append("#splitline{");
    title.append(frames.Get(0).GetTitle());
    for(int f = 1; f < Nf; f++){
      if(f%((Nf+1)/2) == 0 && Nf > 2) title.append("}{");
      title.append("+ ");
      title.append(frames.Get(f).GetTitle());
    }
    if(Nf > 2) title.append("}");
   
    return title;
  }

  string FramePlot::GetSetTitle(const string& set, const string& index){
    string title = "#left{#left(";
    title.append(set);
    title.append("#right)_{");
    title.append(index);
    title.append("}#right}");
    return title;
  }

  void FramePlot::DrawFrameTypeLegend(){
    vector<string> frame_title;
    frame_title.push_back("Lab State");
    frame_title.push_back("Decay States");
    frame_title.push_back("Visible States");
    frame_title.push_back("Invisible States");
    vector<FrameType> frame_type;
    frame_type.push_back(kLabFrame);
    frame_type.push_back(kDecayFrame);
    frame_type.push_back(kVisibleFrame);
    frame_type.push_back(kInvisibleFrame);
   
    TLatex* lat = new TLatex(0.,0.,"");
    lat->SetNDC();
    lat->SetTextAlign(12);
    lat->SetTextSize(0.045);
    lat->SetTextFont(132);
    lat->SetTextColor(color_Default);
    double X = 0.045;
    double Y = 0.955;
    double R = 0.035;
    double step = 2.2*R;
    for(int i = 0; i < 4; i++){
      TBox* white = new TBox(X-step/2.,Y-step/2.,0.36,Y+step/2.);
      white->SetFillColor(kWhite);
      white->SetLineColor(kBlack);
      white->Draw();
      m_Objects.push_back(white);
      if(m_FrameColorMap[frame_type[i]] <= 0) continue;
      TArc* circle = new TArc(X,Y,R);
      circle->SetLineColor(m_FrameColorMap[frame_type[i]]);
      circle->SetFillColor(m_FrameColorFillMap[frame_type[i]]);
      circle->SetLineWidth(2);
      circle->Draw();
      m_Objects.push_back(circle);
      lat->DrawLatex(X+R*1.3,Y,frame_title[i].c_str());
      Y -= step;
    }
    if(m_SelfAssembling){
      TBox* white = new TBox(X-step/2.,Y-step/2.,0.36,Y+step/2.);
      white->SetFillColor(kWhite);
      white->SetLineColor(kBlack);
      white->Draw();
      m_Objects.push_back(white);
      TBox* box = new TBox(X-R*0.88,Y-R*0.88,X+R*0.88,Y+R*0.88);
      box->SetLineColor(m_FrameColorMap[kDecayFrame]);
      box->SetFillColor(m_FrameColorFillMap[kDecayFrame]);
      box->Draw("l");
      m_Objects.push_back(box);
      lat->DrawLatex(X+R*1.3,Y,"Self Assembling");
    }
    m_Objects.push_back(lat);
  }

  void FramePlot::DrawJigsawLegend(){
    vector<string> jigsaw_title;
    jigsaw_title.push_back("Invisible Jigsaw");
    jigsaw_title.push_back("Combinatoric Jigsaw");
    vector<JigsawType> jigsaw_type;
    jigsaw_type.push_back(kInvisibleJigsaw);
    jigsaw_type.push_back(kCombinatoricJigsaw);

    vector<string> ititle;
    vector<int> icolor;
    int Nj = m_Jigsaws.size();
    for(int i = 0; i < Nj; i++){
      icolor.push_back(m_JigsawColorMap[m_Jigsaws[i]]);
      for(int j = 0; j < 2; j++)
	if(m_Jigsaws[i]->GetType() == jigsaw_type[j]) ititle.push_back(jigsaw_title[j]);
    }

    TLine* dum[Nj];
    for(int i = 0; i < Nj; i++){
      dum[i] = (TLine*) new TLine(0.0,0.0,0.001,0.001);
      dum[i]->SetLineWidth(int(m_Node_R*70.));
      dum[i]->SetLineColor(icolor[i]);
      m_Objects.push_back(dum[i]);
    }
  
    TLegend* leg = new TLegend(0.60235,0.997-Nj*.078,0.8087,0.997);
    m_Objects.push_back(leg);
    TLegendEntry *entry[Nj];
    for(int i = 0; i < Nj; i++){
      entry[i] = leg->AddEntry(dum[i], ititle[i].c_str());
      entry[i]->SetMarkerSize(0);
      entry[i]->SetMarkerColor(icolor[i]);
    }
    leg->SetLineColor(kWhite);
    leg->SetFillColor(kWhite);
    leg->SetShadowColor(kWhite);
    leg->SetTextFont(132);
    leg->SetTextSize(0.042);
    leg->Draw();
  }

  void FramePlot::DrawTitle(const string& title){
    TLatex* lat = new TLatex(0.0,0.0,title.c_str());
    lat->SetTextAlign(22);
    lat->SetNDC();
    lat->SetTextSize(0.045);
    lat->SetTextFont(132);
    double x = lat->GetXsize()/2. + 0.01;
    double y = 1. - lat->GetYsize()/2. - 0.01;
    m_CanvasPtr->cd();
    lat->DrawLatex(x,y,title.c_str());
    m_Objects.push_back(lat);
  }

  int FramePlot::GetJigsawPriority(int Nout, int Ndep) const {
    if(Nout == 1 && Ndep == 0) return 0;
    if(Nout == 1 && Ndep == 1) return 1;
    return 2;
  }
  
  ///////////////////////////////////////////////
  // FramePlotNode class methods
  ///////////////////////////////////////////////
  FramePlotNode::FramePlotNode(){ 
    Init();
  }
  
  FramePlotNode::~FramePlotNode(){ }

  void FramePlotNode::Init(){
    m_X = 0;
    m_Y = 0;
    m_Label = "";
    m_DoLabel = false;
    m_DoSquare = false;
    m_FramePtr = nullptr;
    m_StatePtr = nullptr;
  }
    
  void FramePlotNode::SetX(double x){ m_X = x; }
  void FramePlotNode::SetY(double y){ m_Y = y; }
  void FramePlotNode::SetLabel(const string& label){ m_Label = label; m_DoLabel = true; }
  void FramePlotNode::SetSquare(bool square){ m_DoSquare = square; }
  void FramePlotNode::SetFrame(const RestFrame& frame){ m_FramePtr = &frame; }
  void FramePlotNode::SetState(const State& state){ m_StatePtr = &state; }
  void FramePlotNode::AddJigsaw(const Jigsaw& jigsaw){ 
    int N = m_Jigsaws.size();
    for(int i = 0; i < N; i++)
      if(*m_Jigsaws[i] == jigsaw)
	return;
    m_Jigsaws.push_back(&jigsaw);
  }
  
  double FramePlotNode::GetX() const { return m_X; }
  double FramePlotNode::GetY() const { return m_Y; }
  string FramePlotNode::GetLabel() const { return m_Label; }
  const RestFrame& FramePlotNode::GetFrame() const { 
    if(m_FramePtr)
      return *m_FramePtr;
    else 
      return RestFrame::Empty();
  }

  const State& FramePlotNode::GetState() const { 
    if(m_StatePtr)
      return *m_StatePtr;
    else
      return State::Empty();
  }
  bool FramePlotNode::DoLabel() const { return m_DoLabel; }
  bool FramePlotNode::DoSquare() const { return m_DoSquare; }
  int FramePlotNode::GetNJigsaws() const { return m_Jigsaws.size(); }
  vector<const Jigsaw*> FramePlotNode::GetJigsawList() const {
    vector<const Jigsaw*> list;
    int N = m_Jigsaws.size();
    for(int i = 0; i < N; i++)
      list.push_back(m_Jigsaws[i]);
    return list;
  }
    

  ///////////////////////////////////////////////
  // FramePlotLink class methods
  ///////////////////////////////////////////////
  FramePlotLink::FramePlotLink(FramePlotNode* Node1Ptr, FramePlotNode* Node2Ptr){
    m_Node1Ptr = Node1Ptr;
    m_Node2Ptr = Node2Ptr;
    Init();
  }
  
  FramePlotLink::~FramePlotLink(){ }

  void FramePlotLink::Init(){ 
    m_Wavy = false; 
    m_DoLabel = false; 
    m_Label = ""; 
    m_JigsawPtr = nullptr;
  }

  void FramePlotLink::SetWavy(bool wavy){ m_Wavy = wavy; }
  void FramePlotLink::SetLabel(const string& label){ m_Label = label; m_DoLabel = true; }
  void FramePlotLink::SetJigsaw(const Jigsaw& jigsaw){ m_JigsawPtr = &jigsaw; }
  
  FramePlotNode* FramePlotLink::GetNode1() const { return m_Node1Ptr; }
  FramePlotNode* FramePlotLink::GetNode2() const { return m_Node2Ptr; }
  bool FramePlotLink::DoWavy() const { return m_Wavy; }
  string FramePlotLink::GetLabel() const { return m_Label; }
  bool FramePlotLink::DoLabel() const { return m_DoLabel; }
  Jigsaw const& FramePlotLink::GetJigsaw() const { 
    if(m_JigsawPtr)
      return *m_JigsawPtr;
    else 
      return Jigsaw::Empty();
  }

}
