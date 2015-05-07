#include <iostream>
#include "RestFrames/RestFrames_config.h"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/FrameLink.hh"
#include "RestFrames/FrameLog.hh"


using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RestFrame class methods
  ///////////////////////////////////////////////
  int RestFrame::m_class_key = 0;

  RestFrame::RestFrame(const string& sname, const string& stitle, int ikey){
    Init(sname, stitle);
    m_Key = ikey;
  }

  RestFrame::RestFrame(const string& sname, const string& stitle){
    Init(sname, stitle);
    m_Key = GenKey();
  }

  RestFrame::~RestFrame(){
    ClearFrame();
  }

  void RestFrame::Init(const string& sname, const string& stitle){
    m_Name = sname;
    m_Title = stitle;
    m_Body   = false;
    m_Mind   = false;
    m_Spirit = false;
    m_ParentLinkPtr = nullptr;
  }

  int RestFrame::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  void RestFrame::ClearFrame(){
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
    //m_ParentLinkPtr = nullptr;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(m_ChildLinks[i]){
	delete m_ChildLinks[i];
	m_ChildLinks[i] = nullptr;
      }
    }
    m_ChildLinks.clear();
  }
  
  bool RestFrame::IsSoundBody() const {
    m_Body = true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!m_ChildLinks[i]){
	m_Body = false;
	break;
      }
      if(!m_ChildLinks[i]->GetChildFrame()){
	m_Body = false;
	break;
      }
    }
    return m_Body;
  }

  bool RestFrame::IsSoundBodyRecursive() const {
    bool child_body = true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!m_ChildLinks[i]) continue;
      if(!m_ChildLinks[i]->GetChildFrame()) continue;
      child_body = m_ChildLinks[i]->GetChildFrame()->IsSoundBodyRecursive();
    }
    return IsSoundBody() && child_body;
  }

  bool RestFrame::IsSoundMind() const {
    return m_Mind;
  }

  bool RestFrame::IsSoundMindRecursive() const {
    bool child_mind = true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!m_ChildLinks[i]) continue;
      if(!m_ChildLinks[i]->GetChildFrame()) continue;
      child_mind = m_ChildLinks[i]->GetChildFrame()->IsSoundMindRecursive();
    }
    return IsSoundMind() && child_mind;
  }

  bool RestFrame::IsSoundSpirit() const {
    return m_Spirit;
  }

  bool RestFrame::IsSoundSpiritRecursive() const {
    bool child_spirit = true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!m_ChildLinks[i]) continue;
      if(!m_ChildLinks[i]->GetChildFrame()) continue;
      child_spirit = m_ChildLinks[i]->GetChildFrame()->IsSoundSpiritRecursive();
    }
    return IsSoundSpirit() && child_spirit;
  }

  bool RestFrame::IsSame(const RestFrame& frame) const {
    return IsSame(&frame);
  }
     
  bool RestFrame::IsSame(const RestFrame* framePtr) const {
    if(!framePtr) return false;
    if(m_Key == framePtr->GetKey()) return true;
    return false;
  }

  int RestFrame::GetKey() const {
    return m_Key;
  }

  string RestFrame::GetName() const {
    return m_Name;
  }

  string RestFrame::GetTitle() const {
    return m_Title;
  }

  FrameType RestFrame::GetType() const { 
    return m_Type; 
  }

  bool RestFrame::IsVisibleFrame() const { 
    return m_Type == FVisible; 
  }

  bool RestFrame::IsInvisibleFrame() const { 
    return m_Type == FInvisible; 
  }

  bool RestFrame::IsDecayFrame() const { 
    return m_Type == FDecay; 
  }

  bool RestFrame::IsLabFrame() const { 
    return m_Type == FLab; 
  }

  bool RestFrame::IsRFrame() const { 
    return m_Ana == FReco; 
  }

  bool RestFrame::IsGFrame() const { 
    return m_Ana == FGen; 
  }

  void RestFrame::RemoveChild(const RestFrame* framePtr){
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
    RemoveChildIndex(GetChildIndex(framePtr));
  }

  void RestFrame::RemoveChildIndex(int i){
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
    if(i < 0 || i >= GetNChildren()) return;
    if(m_ChildLinks[i]){
      delete m_ChildLinks[i];
      m_ChildLinks[i] = nullptr;
    }
    m_ChildLinks.erase(m_ChildLinks.begin()+i);
  }

  void RestFrame::RemoveChildren(){
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++) RemoveChildIndex(i);
    m_ChildLinks.clear();
  }

  //////////////////////////////
  //Tree construction functions
  //////////////////////////////

  void RestFrame::SetParentLink(FrameLink* linkPtr){
    if(m_Type != FLab) m_ParentLinkPtr = linkPtr;
  }

  void RestFrame::AddChildFrame(RestFrame& frame){
    AddChildFrame(&frame);
  }
  
  void RestFrame::AddChildFrame(RestFrame* framePtr){
    if(!framePtr) return;
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
    if(framePtr->IsLabFrame()) return;
    if(GetChildIndex(framePtr) >= 0) return;
  
    FrameLink* linkPtr = new FrameLink();
    linkPtr->SetParentFrame(this);
    linkPtr->SetChildFrame(framePtr);
    framePtr->SetParentLink(linkPtr);
    m_ChildLinks.push_back(linkPtr);
  }

  int RestFrame::GetNChildren() const { 
    return int(m_ChildLinks.size()); 
  }

  int RestFrame::GetChildIndex(const RestFrame* framePtr) const {
    if(!framePtr) return -1;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(GetChildFrame(i)->IsSame(framePtr)) return i;
    }
    return -1;
  }

  RestFrame* RestFrame::GetChildFrame(int i) const {
    int Nchild = GetNChildren();
    if(i >= Nchild || i < 0) return nullptr;
    return m_ChildLinks[i]->GetChildFrame();
  }

  const RestFrame* RestFrame::GetParentFrame() const {
    if(!m_ParentLinkPtr) return nullptr;
    return m_ParentLinkPtr->GetParentFrame();
  }

  const RestFrame* RestFrame::GetLabFrame() const {
    if(m_Type == FLab) return this;
    if(!m_ParentLinkPtr) return nullptr;
    const RestFrame* parentPtr = GetParentFrame();
    if(parentPtr) return parentPtr->GetLabFrame();
    return nullptr;
  } 

  void RestFrame::SetChildBoostVector(int i, TVector3 boost) const {
    m_ChildLinks[i]->SetBoostVector(boost);
  }

  TVector3 RestFrame::GetChildBoostVector(int i) const {
    return m_ChildLinks[i]->GetBoostVector();
  }

  TVector3 RestFrame::GetParentBoostVector() const {
    return m_ParentLinkPtr->GetBoostVector();
  }

  RestFrameList* RestFrame::GetListFrames(){
    RestFrameList* framesPtr = new RestFrameList();
    framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesRecursive(framesPtr);
    
    return framesPtr;
  }

  RestFrameList* RestFrame::GetListFramesType(FrameType type){
    RestFrameList* framesPtr = new RestFrameList();
    if(m_Type == type) framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesTypeRecursive(type, framesPtr);
   
    return framesPtr;
  }

  RestFrameList* RestFrame::GetListFramesType(const vector<FrameType>& types){
    RestFrameList* framesPtr = new RestFrameList();
    int Ntype = types.size();
    for(int i = 0; i < Ntype; i++)
      FillListFramesTypeRecursive(types[i], framesPtr);

    return framesPtr;
  }

  RestFrameList* RestFrame::GetListVisibleFrames(){
    return GetListFramesType(FVisible);
  }

  RestFrameList* RestFrame::GetListInvisibleFrames(){
    return GetListFramesType(FInvisible);
  }

  void RestFrame::FillListFramesRecursive(RestFrameList* framesPtr){
    framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesRecursive(framesPtr);
  }
  
  void RestFrame::FillListFramesTypeRecursive(FrameType type, RestFrameList* framesPtr){
    if(m_Type == type) framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesTypeRecursive(type, framesPtr);
  }

  ///////////////////////////////////////////////
  // Recursive checks of tree integrity
  ///////////////////////////////////////////////
  bool RestFrame::IsCircularTree(vector<int>* KEYS) const {
    int Nkey = KEYS->size();
  
    for(int i = 0; i < Nkey; i++){
      if((*KEYS)[i] == m_Key){
	cout << endl << "Warning: reference frame ";
	cout << m_Name.c_str() << " appears more than once in tree" << endl;
	return true;
      }
    }
    KEYS->push_back(m_Key);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i)->IsCircularTree(KEYS)) return true;
     
    return false;
  }

  bool RestFrame::IsConsistentAnaTree(AnaType ana) const {
    if(ana != m_Ana) return false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)->IsConsistentAnaTree(ana)) return false;
    
    return true;
  }

  bool RestFrame::FindPathToFrame(const RestFrame* framePtr, vector<FrameLink*>* linksPtr, 
				  vector<int>* linkSignsPtr, const RestFrame *prevPtr) const {
    if(IsSame(framePtr)) return true;
  
    vector<FrameLink*> try_links;
    vector<int> try_signs;

    try_links.push_back(m_ParentLinkPtr);
    try_signs.push_back(1);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      try_links.push_back(m_ChildLinks[i]);
      try_signs.push_back(-1);
    }
    for(int i = 0; i < Nchild+1; i++){
      if(!try_links[i]) continue;
      RestFrame* nextPtr;
      if(try_signs[i] > 0) nextPtr = try_links[i]->GetParentFrame();
      else nextPtr = try_links[i]->GetChildFrame();
      if(nextPtr->IsSame(prevPtr)) continue;
      linksPtr->push_back(try_links[i]);
      linkSignsPtr->push_back(try_signs[i]);
      if(nextPtr->FindPathToFrame(framePtr,linksPtr,linkSignsPtr,this)) return true;
      linksPtr->pop_back();
      linkSignsPtr->pop_back();
    }
    return false;
  }

  void RestFrame::SetFourVector(const TLorentzVector& V, const RestFrame* framePtr){
    m_P.SetVectM(V.Vect(),V.M());
    m_ProdFramePtr = framePtr;  
  }

  //////////////////////////////
  // User Analysis functions
  //////////////////////////////

  double RestFrame::GetMass() const {
    return m_P.M();
  }

  double RestFrame::GetCosDecayAngle(const RestFrame* framePtr) const {
    if(m_ChildLinks.size() <= 0) return 0.;
    if(!m_ParentLinkPtr) return 0.;
    TVector3 V1 = GetParentBoostVector().Unit();
    TVector3 V2;
    if(framePtr){
      V2 = framePtr->GetFourVector(this).Vect().Unit();
    } else {
      V2 = GetChildFrame(0)->GetFourVector(this).Vect().Unit();
    }
    return V1.Dot(V2);
  }
  
  // Get decay angle in plane perpendicular to 3-vector 'axis', where axis is defined
  // in the production frame of 'this'. Decay angle is relative to frame 'framePtr'
  // unless framePtr == nullptr (default), in which case is it first child of 'this'
  double RestFrame::GetDeltaPhiDecayAngle(const TVector3& axis, const RestFrame* framePtr) const {
    const RestFrame* prod_framePtr = GetProductionFrame();
    if(!prod_framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(prod_framePtr);
    TLorentzVector Pchild;
    if(framePtr){
      Pchild = framePtr->GetFourVector(prod_framePtr);
    } else {
      if(m_ChildLinks.size() <= 0) return 0.;
      Pchild = m_ChildLinks[0]->GetChildFrame()->GetFourVector(prod_framePtr);
    }

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    Pchild.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    Pchild.Boost(-boost_perp);

    TVector3 V = Pchild.Vect();
    V = V - V.Dot(axis.Unit())*axis.Unit();
    return V.Angle(boost_perp);
  }

  // Get angle between 'this' boost and visible children in plane 
  // perpendicular to 3-vector 'axis', where axis is defined
  // in 'framePtr' (default gives lab frame). 
  double RestFrame::GetDeltaPhiBoostVisible(const TVector3& axis, const RestFrame* framePtr) const {
    if(!framePtr) framePtr = GetLabFrame();
    TLorentzVector Pvis = GetVisibleFourVector(framePtr);
    TLorentzVector Pthis = GetFourVector(framePtr);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    Pvis.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    Pvis.Boost(-boost_perp);

    TVector3 V = Pvis.Vect();
    V = V - V.Dot(axis.Unit())*axis.Unit();
   
    return V.Angle(boost_perp);
  }

  // Get angle between 'this' decay axis (defined by first child)
  // and visible children in plane 
  // perpendicular to 3-vector 'axis', where axis is defined
  // in 'framePtr' (default gives lab frame). 
  double RestFrame::GetDeltaPhiDecayVisible(const TVector3& axis, const RestFrame* framePtr) const {
    if(GetNChildren() < 1) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    TLorentzVector Pvis   = GetVisibleFourVector(framePtr);
    TLorentzVector Pchild = GetChildFrame(0)->GetFourVector(framePtr);
    TLorentzVector Pthis  = GetFourVector(framePtr);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    Pvis.Boost(-boost_par);
    Pchild.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    Pvis.Boost(-boost_perp);
    Pchild.Boost(-boost_perp);

    TVector3 Vv = Pvis.Vect();
    Vv = Vv - Vv.Dot(axis.Unit())*axis.Unit();
    TVector3 Vc = Pchild.Vect();
    Vc = Vc - Vc.Dot(axis.Unit())*axis.Unit();
   
    return Vv.Angle(Vc);
  }
 
  // Get angle between the visible portions of children 1 and 2
  // in the plane perpendicular to 3-vector 'axis', where
  // axis is defined in 'framePtr' (default gives lab frame). 
  double RestFrame::GetDeltaPhiVisible(const TVector3& axis, const RestFrame* framePtr) const {
    if(GetNChildren() != 2) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    if(!framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(framePtr);
    TLorentzVector P1 = m_ChildLinks[0]->GetChildFrame()->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = m_ChildLinks[1]->GetChildFrame()->GetVisibleFourVector(framePtr);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    P1.Boost(-boost_par);
    P2.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    P1.Boost(-boost_perp);
    P2.Boost(-boost_perp);

    TVector3 V1 = P1.Vect();
    TVector3 V2 = P2.Vect();
    V1 = V1 - V1.Dot(axis.Unit())*axis.Unit();
    V2 = V2 - V2.Dot(axis.Unit())*axis.Unit();
   
    return V1.Angle(V2);
  }

  double RestFrame::GetVisibleShape() const {
    if(GetNChildren() != 2) return 0.;
    TVector3 P1 = m_ChildLinks[0]->GetChildFrame()->GetVisibleFourVector(this).Vect();
    TVector3 P2 = m_ChildLinks[1]->GetChildFrame()->GetVisibleFourVector(this).Vect();
    if(P1.Mag()+P2.Mag() > 0.) return sqrt(pow(P1.Mag()+P2.Mag(),2.)-(P1-P2).Mag2())/(P1.Mag()+P2.Mag());
    else return 0.;
  }
 
  double RestFrame::GetTransverseVisibleShape(const TVector3& axis, const RestFrame* framePtr) const {
    if(GetNChildren() != 2) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    if(!framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(framePtr);
    TLorentzVector P1 = m_ChildLinks[0]->GetChildFrame()->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = m_ChildLinks[1]->GetChildFrame()->GetVisibleFourVector(framePtr);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    P1.Boost(-boost_par);
    P2.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    P1.Boost(-boost_perp);
    P2.Boost(-boost_perp);

    TVector3 V1 = P1.Vect();
    TVector3 V2 = P2.Vect();
    V1 = V1 - V1.Dot(axis.Unit())*axis.Unit();
    V2 = V2 - V2.Dot(axis.Unit())*axis.Unit();

    if(V1.Mag()+V2.Mag() > 0.) return sqrt(pow(V1.Mag()+V2.Mag(),2.)-(V1-V2).Mag2())/(V1.Mag()+V2.Mag());
    else return 0.;
  }

  double RestFrame::GetScalarVisibleMomentum() const {
    if(GetNChildren() != 2) return 0.;
    TLorentzVector P1 = m_ChildLinks[0]->GetChildFrame()->GetVisibleFourVector(this);
    TLorentzVector P2 = m_ChildLinks[1]->GetChildFrame()->GetVisibleFourVector(this);
    return P1.P() + P2.P();
  }
  double RestFrame::GetTransverseScalarVisibleMomentum(const TVector3& axis, 
						       const RestFrame* framePtr) const {
    if(GetNChildren() != 2) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    if(!framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(framePtr);
    TLorentzVector P1 = m_ChildLinks[0]->GetChildFrame()->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = m_ChildLinks[1]->GetChildFrame()->GetVisibleFourVector(framePtr);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    P1.Boost(-boost_par);
    P2.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    P1.Boost(-boost_perp);
    P2.Boost(-boost_perp);

    TVector3 V1 = P1.Vect();
    TVector3 V2 = P2.Vect();
    V1 = V1 - V1.Dot(axis.Unit())*axis.Unit();
    V2 = V2 - V2.Dot(axis.Unit())*axis.Unit();

    return V1.Mag()+V2.Mag();
  }

  TLorentzVector RestFrame::GetFourVector(const RestFrame& frame) const {
    return GetFourVector(&frame);
  }
  TLorentzVector RestFrame::GetFourVector(const RestFrame* framePtr) const {
    TLorentzVector V(0.,0.,0.,0.);
    if(!framePtr) framePtr = GetLabFrame();
 
    V.SetVectM(m_P.Vect(),m_P.M());
    if(framePtr->IsSame(m_ProdFramePtr)) return V;

    vector<FrameLink*>* linksPtr = new vector<FrameLink*>;
    vector<int>* linkSignsPtr = new vector<int>;
    if(!m_ProdFramePtr->FindPathToFrame(framePtr,linksPtr,linkSignsPtr,nullptr)) return V;
  
    int Nlink = linksPtr->size();
    for(int i = 0; i < Nlink; i++){
      V.Boost((*linkSignsPtr)[i]*(*linksPtr)[i]->GetBoostVector());
    }
    delete linksPtr;
    delete linkSignsPtr;
    return V;
  }

  TLorentzVector RestFrame::GetVisibleFourVector(const RestFrame& frame) const {
    return GetVisibleFourVector(&frame);
  }
  TLorentzVector RestFrame::GetVisibleFourVector(const RestFrame* framePtr) const {
    TLorentzVector V(0.,0.,0.,0.);
    if(!framePtr || !m_Spirit) return V;
    int Nc = GetNChildren();
    for(int c = 0; c < Nc; c++){
      RestFrameList* framesPtr = m_ChildLinks[c]->GetChildFrame()->GetListVisibleFrames();
      int Nf = framesPtr->GetN();
      for(int f = 0; f < Nf; f++) V += framesPtr->Get(f)->GetFourVector(framePtr);
      delete framesPtr;
    }
    return V;
  }
  TLorentzVector RestFrame::GetInvisibleFourVector(const RestFrame& frame) const {
    return GetInvisibleFourVector(&frame);
  }
  TLorentzVector RestFrame::GetInvisibleFourVector(const RestFrame* framePtr) const {
    TLorentzVector V(0.,0.,0.,0.);
    if(!m_Spirit) return V;
    if(!framePtr) framePtr = this;
    int Nc = GetNChildren();
    for(int c = 0; c < Nc; c++){
      RestFrameList* framesPtr = GetChildFrame(c)->GetListInvisibleFrames();
      int Nf = framesPtr->GetN();
      for(int f = 0; f < Nf; f++) V += framesPtr->Get(f)->GetFourVector(framePtr);
      delete framesPtr;
    }
    return V;
  }
  double RestFrame::GetEnergy(const RestFrame& frame) const {
    return GetFourVector(&frame).E();
  }
  double RestFrame::GetEnergy(const RestFrame* framePtr) const {
    return GetFourVector(framePtr).E();
  }
  double RestFrame::GetMomentum(const RestFrame& frame) const {
    return GetFourVector(&frame).P();
  }
  double RestFrame::GetMomentum(const RestFrame *framePtr) const {
    return GetFourVector(framePtr).P();
  }

  int RestFrame::GetFrameDepth(const RestFrame& frame) const {
    return GetFrameDepth(&frame);
  }
  int RestFrame::GetFrameDepth(const RestFrame* framePtr) const {
    if(!framePtr) return -1;
    if(IsSame(framePtr)) return 0.;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      int depth = GetChildFrame(i)->GetFrameDepth(framePtr);
      if(depth >= 0) return depth+1;
    }
    return -1;
  }

  const RestFrame* RestFrame::GetFrameAtDepth(int depth, const RestFrame& frame) const {
    return GetFrameAtDepth(depth, &frame);
  }
  const RestFrame* RestFrame::GetFrameAtDepth(int depth, const RestFrame* framePtr) const {
    if(!framePtr || depth < 1) return nullptr;
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      RestFrame* childPtr = GetChildFrame(i);
      RestFrameList* framesPtr = childPtr->GetListFrames();
      if(framesPtr->Contains(framePtr)){
	delete framesPtr;
	if(depth == 1) return childPtr;
	else return childPtr->GetFrameAtDepth(depth-1,framePtr);
      }
      delete framesPtr;
    }
    return nullptr;
  }

  double RestFrame::GetDeltaPhiDecayPlanes(const RestFrame& frame) const {
    return GetDeltaPhiDecayPlanes(&frame);
  }
  double RestFrame::GetDeltaPhiDecayPlanes(const RestFrame* framePtr) const {
    if(!framePtr) return 0.;
    if(m_ChildLinks.size() < 1) return 0.;
   
    TVector3 vNorm_frame = framePtr->GetDecayPlaneNormalVector();
    TVector3 vNorm_this = GetDecayPlaneNormalVector();
    double dphi = vNorm_this.Angle(vNorm_frame);

    if(framePtr->GetFourVector(this).Vect().Cross(vNorm_frame).Dot(vNorm_this) < 0.){
      dphi = TMath::Pi()*2. - dphi;
    }
    return dphi;
  }
  
  TVector3 RestFrame::GetDecayPlaneNormalVector() const {
    TVector3 V(0.,0.,0.);
    if(GetNChildren() < 2) return V;
    TVector3 V1 = GetChildFrame(0)->GetFourVector(GetParentFrame()).Vect();
    TVector3 V2 = GetChildFrame(1)->GetFourVector(GetParentFrame()).Vect();
    return V1.Cross(V2).Unit();
  }

  const RestFrame* RestFrame::GetProductionFrame() const {
    return m_ProdFramePtr;
  }

  const RestFrame* RestFrame::GetSiblingFrame() const {
    if(IsLabFrame()) return nullptr;
    int Nsib = m_ProdFramePtr->GetNChildren();
    for(int s = 0; s < Nsib; s++){
      if(IsSame(m_ProdFramePtr->GetChildFrame(s))) continue;
      return m_ProdFramePtr->GetChildFrame(s);
    }
    return nullptr; 
  }

  int RestFrame::GetNDescendants() const {
    int Nchild = GetNChildren();
    if(Nchild == 0) return 1;
    int Nd = 0;
    for(int i = 0; i < Nchild; i++){
      Nd += GetChildFrame(i)->GetNDescendants();
    }
    return Nd;
  }

  TVector3 RestFrame::GetBoostInParentFrame() const{
    TVector3 V(0.,0.,0.);
    if(!m_ParentLinkPtr) return V;
    return m_ParentLinkPtr->GetBoostVector();
  }

  double RestFrame::GetGammaInParentFrame() const{
    TVector3 vbeta = GetBoostInParentFrame();
    double beta = min(1.,vbeta.Mag());
    return 1./sqrt(1.-beta*beta);
  }

  double RestFrame::GetProdPoM(int& NDecay) const{
    if(IsVisibleFrame() || IsInvisibleFrame()) return 1.;
    NDecay++;
    int Nc = GetNChildren();
    double prod = 2.*GetChildFrame(0)->GetMomentum(this) / GetMass();
    for(int i = 0; i < Nc; i++){
      prod *= GetChildFrame(i)->GetProdPoM(NDecay);
    }
    return prod;
  }

  double RestFrame::GetProdSinDecayAngle(int& NDecay) const{
    if(IsVisibleFrame() || IsInvisibleFrame()) return 1.;
    NDecay++;
    int Nc = GetNChildren();
    double c = GetCosDecayAngle();
    double prod = sqrt(1.-c*c);
    for(int i = 0; i < Nc; i++){
      prod *= GetChildFrame(i)->GetProdSinDecayAngle(NDecay);
    }
    return prod;
  }

  // Initializer.
  __attribute__((constructor))
  static void initializer(void){
    printf("\n" "\x1b[36m");
    printf(PACKAGE_NAME);
    printf(" v");
    printf(PACKAGE_VERSION);
    printf(" -- Developed by Christopher Rogan (crogan@cern.ch)\n");
    printf("                     ");
    printf("Copyright (c) 2014-2015, Christopher Rogan\n");
    printf("                     ");
    printf("http://RestFrames.com\n");
    printf("\x1b[0m" "\n");
  }

}
