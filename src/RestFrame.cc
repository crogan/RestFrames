#include <iostream>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/FrameLink.hh"


using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RestFrame class methods
  ///////////////////////////////////////////////
  int RestFrame::m_class_key = 0;

  RestFrame::RestFrame(const string& sname, const string& stitle)
    : RFBase(sname, stitle) 
  {
    Init();
  }

  RestFrame::~RestFrame(){
    RemoveChildren();
  }

  void RestFrame::Init(){
    SetKey(GenKey());
    m_ParentLinkPtr = nullptr;
    m_Log.SetSource("RestFrame "+GetName());
  }

  int RestFrame::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }
  
  bool RestFrame::IsSoundBody() const {
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!m_ChildLinks[i]){
	SetBody(false);
	m_Log << LogWarning << "Broken child link" << m_End;
	return false;
      }
      if(!m_ChildLinks[i]->GetChildFrame()){
	SetBody(false);
	m_Log << LogWarning << "NULL child pointer" << m_End;
	break;
      }
    }
    SetBody(true);
    return true;
  }

  bool RestFrame::IsSoundBodyRecursive() const {
    if(!IsSoundBody()) return false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!m_ChildLinks[i]->GetChildFrame()->IsSoundBodyRecursive())
	return false;
      
    return true;
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
    for(int i = 0; i < Nchild; i++){
      if(m_ChildLinks[i]){
	delete m_ChildLinks[i];
	m_ChildLinks[i] = nullptr;
      }
    }
    m_ChildLinks.clear();
  }

  void RestFrame::SetParentLink(FrameLink* linkPtr){
    if(m_Type != FLab) m_ParentLinkPtr = linkPtr;
  }

  void RestFrame::AddChildFrame(RestFrame& frame){
    AddChildFrame(&frame);
  }
  
  void RestFrame::AddChildFrame(RestFrame* framePtr){
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;
   
    if(!framePtr) return;
    if(framePtr->IsLabFrame()){
      m_Log << LogWarning << "Cannot add LabFrame frame as child: " << Log(framePtr) << m_End;
      return;
    }
    if(GetChildIndex(framePtr) >= 0){
      m_Log << LogWarning << "Frame is already among children: " << Log(framePtr) << m_End;
      return;
    }

    m_Log << LogVerbose << "Adding child frame " << framePtr->GetName() << m_End;
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
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i)->IsSame(framePtr)) return i;
    
    return -1;
  }

  RestFrame* RestFrame::GetChildFrame(int i) const {
    int Nchild = GetNChildren();
    if(i >= Nchild || i < 0 || !m_ChildLinks[i]) return nullptr;
    return GetChildLink(i)->GetChildFrame();
  }

  const RestFrame* RestFrame::GetParentFrame() const {
    if(!m_ParentLinkPtr) return nullptr;
    return m_ParentLinkPtr->GetParentFrame();
  }

  const RestFrame* RestFrame::GetLabFrame() const {
    if(m_Type == FLab) return this;
    const RestFrame* parentPtr = GetParentFrame();
    if(parentPtr) return parentPtr->GetLabFrame();
    return nullptr;
  } 

  void RestFrame::SetChildBoostVector(int i, const TVector3& boost) const {
    FrameLink* linkPtr = GetChildLink(i);
    if(linkPtr) linkPtr->SetBoostVector(boost);
  }

  FrameLink* RestFrame::GetParentLink() const {
    return m_ParentLinkPtr;
  }
  
  FrameLink* RestFrame::GetChildLink(int i) const {
    int Nchild =  m_ChildLinks.size();
    if(i >= Nchild || i < 0 || !m_ChildLinks[i]) return nullptr;
    return m_ChildLinks[i];
  }

  TVector3 RestFrame::GetChildBoostVector(int i) const {
    FrameLink* linkPtr = GetChildLink(i);
    if(linkPtr) return linkPtr->GetBoostVector();
    return TVector3(0.,0.,0.);
  }

  TVector3 RestFrame::GetParentBoostVector() const {
    return GetParentLink()->GetBoostVector();
  }

  RFList<RestFrame>* RestFrame::GetListFrames(){
    RFList<RestFrame>* framesPtr = new RFList<RestFrame>();

    framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesRecursive(framesPtr);
    
    return framesPtr;
  }

  RFList<RestFrame>* RestFrame::GetListFramesType(FrameType type){
    RFList<RestFrame>* framesPtr = new RFList<RestFrame>();
    
    if(m_Type == type) framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesTypeRecursive(type, framesPtr);
   
    return framesPtr;
  }

  RFList<RestFrame>* RestFrame::GetListFramesType(const vector<FrameType>& types){
    RFList<RestFrame>* framesPtr = new RFList<RestFrame>();

    int Ntype = types.size();
    for(int i = 0; i < Ntype; i++)
      FillListFramesTypeRecursive(types[i], framesPtr);

    return framesPtr;
  }

  RFList<RestFrame>* RestFrame::GetListVisibleFrames(){
    return GetListFramesType(FVisible);
  }

  RFList<RestFrame>* RestFrame::GetListInvisibleFrames(){
    return GetListFramesType(FInvisible);
  }

  void RestFrame::FillListFramesRecursive(RFList<RestFrame>* framesPtr){
    framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesRecursive(framesPtr);
  }
  
  void RestFrame::FillListFramesTypeRecursive(FrameType type, RFList<RestFrame>* framesPtr){
    if(m_Type == type) framesPtr->Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i)->FillListFramesTypeRecursive(type, framesPtr);
  }

  bool RestFrame::IsCircularTree(vector<int>* KEYS) const {
    int Nkey = KEYS->size();
  
    for(int i = 0; i < Nkey; i++){
      if((*KEYS)[i] == m_Key){
	m_Log << LogWarning << "This RestFrame appears more than once in the tree" << m_End;
	return true;
      }
    }
    KEYS->push_back(m_Key);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i)->IsCircularTree(KEYS)){
	m_Log << LogWarning << "Tree is circular" << m_End;
	return true;
      }
     
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

    try_links.push_back(GetParentLink());
    try_signs.push_back(1);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      try_links.push_back(GetChildLink(i));
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
    if(!m_Spirit){
      //m_Log << LogInfo << "Can't call analysis function CosDecayAngle" << sendl;
      return 0.;
    }
    if(!GetParentLink()) return 0.;
    TVector3 V1 = GetParentBoostVector().Unit();
    TVector3 V2(0.,0.,0.);
    if(framePtr){
      V2 = framePtr->GetFourVector(this).Vect().Unit();
    } else {
      RestFrame* childPtr = GetChildFrame(0);
      if(childPtr) V2 = childPtr->GetFourVector(this).Vect().Unit();
    }
    return V1.Dot(V2);
  }
  
  // Get decay angle in plane perpendicular to 3-vector 'axis', where axis is defined
  // in the production frame of 'this'. Decay angle is relative to frame 'framePtr'
  // unless framePtr == nullptr (default), in which case is it first child of 'this'
  double RestFrame::GetDeltaPhiDecayAngle(const TVector3& axis, const RestFrame* framePtr) const {
    if(!m_Spirit) return 0.;
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
      RFList<RestFrame>* framesPtr = m_ChildLinks[c]->GetChildFrame()->GetListVisibleFrames();
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
      RFList<RestFrame>* framesPtr = GetChildFrame(c)->GetListInvisibleFrames();
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
      RFList<RestFrame>* framesPtr = childPtr->GetListFrames();
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

}
