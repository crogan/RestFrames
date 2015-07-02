/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RestFrame.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2014 Jan
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

#include "RestFrames/RestFrame.hh"

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
    m_ParentFramePtr = nullptr;
    m_ParentBoost.SetXYZ(0.,0.,0.);
    m_Log.SetSource("RestFrame "+GetName());
  }

  int RestFrame::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }
  
  bool RestFrame::IsSoundBody() const {
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)){
	SetBody(false);
	m_Log << LogWarning << "NULL child frame pointer" << m_End;
	return false;
      }
    SetBody(true);
    return true;
  }

  bool RestFrame::IsSoundBodyRecursive() const {
    if(!IsSoundBody()) return false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)->IsSoundBodyRecursive()){
	m_Log << LogWarning;
	m_Log << "Problem with recursive tree structure from frame: ";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return false;
      }
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

  string RestFrame::PrintString(LogType type) const {
    string output = RFBase::PrintString(type);
    if(IsLabFrame())
      output += "   Frame Type: Lab \n";
    if(IsDecayFrame())
      output += "   Frame Type: Decay \n";
    if(IsVisibleFrame())
      output += "   Frame Type: Visible \n";
    if(IsInvisibleFrame())
      output += "   Frame Type: Invisible \n";
    if(IsGFrame())
      output += "   Ana Type: Generator \n";
    if(IsRFrame())
      output += "   Ana Type: Reconstruction \n";
    return output;
  }

  void RestFrame::RemoveChild(const RestFrame* framePtr){
    RemoveChildIndex(GetChildIndex(framePtr));
  }

  void RestFrame::RemoveChildIndex(int i){
    SetBody(false);
    if(i < 0 || i >= GetNChildren()) return;
    
    GetChildFrame(i)->SetParentFrame(nullptr);
    m_ChildFrames.Remove(GetChildFrame(i));
    m_ChildBoosts.erase(m_ChildBoosts.begin()+i);
  }

  void RestFrame::RemoveChildren(){
    SetBody(false);
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      if(GetChildFrame(i)) 
	GetChildFrame(i)->SetParentFrame(nullptr);
    m_ChildFrames.Clear();
    m_ChildBoosts.clear();
  }

  void RestFrame::SetParentFrame(const RestFrame* framePtr){
    if(m_Type != FLab) m_ParentFramePtr = framePtr;
  }

  void RestFrame::AddChildFrame(RestFrame& frame){
    AddChildFrame(&frame);
  }
  
  void RestFrame::AddChildFrame(RestFrame* framePtr){
    if(!framePtr) return;
    SetBody(false);
    if(framePtr->IsLabFrame()){
      m_Log << LogWarning << "Cannot add LabFrame frame as child: " << Log(framePtr) << m_End;
      return;
    }
    if(GetChildIndex(framePtr) >= 0){
      m_Log << LogWarning << "Frame is already among children: " << Log(framePtr) << m_End;
      return;
    }
    
    if(!m_ChildFrames.Add(framePtr)){
      m_Log << LogWarning << "Unable to add child frame:";
      m_Log << Log(framePtr) << m_End;
      return;
    }
    framePtr->SetParentFrame(this);
    m_ChildBoosts.push_back(TVector3(0.,0.,0.));
  }

  int RestFrame::GetNChildren() const { 
    return m_ChildFrames.GetN();
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
    if(i >= Nchild || i < 0 || !m_ChildFrames.Get(i)) return nullptr;
    return m_ChildFrames.Get(i);
  }

  const RestFrame* RestFrame::GetParentFrame() const {
    return m_ParentFramePtr;
  }

  const RestFrame* RestFrame::GetLabFrame() const {
    if(m_Type == FLab) return this;
    const RestFrame* parentPtr = GetParentFrame();
    if(!parentPtr){
      m_Log << LogWarning;
      m_Log << "Unable to find LabFrame above this frame. ";
      m_Log << "No pointer to parent frame." << m_End;
      return nullptr;
    } 
    return parentPtr->GetLabFrame();
  } 

  void RestFrame::SetChildBoostVector(int i, const TVector3& boost) {
    int N = GetNChildren();
    if(i < 0 || i >= N || !GetChildFrame(i)){
      m_Log << LogWarning;
      m_Log << "Unable to set child " << i;
      m_Log << "'s boost vector. No entry " << i;
      m_Log << m_End;
      return;
    }
    m_ChildBoosts[i] = boost;
    GetChildFrame(i)->SetParentBoostVector(-1.*boost);
  }

  void RestFrame::SetParentBoostVector(const TVector3& boost) {
    if(!GetParentFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to set parent boost vector. ";
      m_Log << "No parent frame linked.";
      m_Log << m_End;
      return;
    }
    m_ParentBoost = boost;
  }


  TVector3 RestFrame::GetChildBoostVector(int i) const {
    int N = GetNChildren();
    if(i < 0 || i >= N || !GetChildFrame(i)){
      m_Log << LogWarning;
      m_Log << "Unable to get child " << i;
      m_Log << "'s boost vector. No entry " << i;
      m_Log << m_End;
      return TVector3(0.,0.,0.);
    }
    return m_ChildBoosts[i];
  }

  TVector3 RestFrame::GetParentBoostVector() const {
    return m_ParentBoost;
  }

  RFList<RestFrame> RestFrame::GetListFrames(){
    RFList<RestFrame> frames;
    frames.Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i))
	GetChildFrame(i)->FillListFramesRecursive(frames);
    
    return frames;
  }

  RFList<RestFrame> RestFrame::GetListFramesType(FrameType type){
    RFList<RestFrame> frames;
    if(m_Type == type) frames.Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i))
	GetChildFrame(i)->FillListFramesTypeRecursive(type, frames);
   
    return frames;
  }

  RFList<RestFrame> RestFrame::GetListFramesType(const vector<FrameType>& types){
    RFList<RestFrame> frames;

    int Ntype = types.size();
    for(int i = 0; i < Ntype; i++)
      FillListFramesTypeRecursive(types[i], frames);

    return frames;
  }

  RFList<RestFrame> RestFrame::GetListVisibleFrames(){
    return GetListFramesType(FVisible);
  }

  RFList<RestFrame> RestFrame::GetListInvisibleFrames(){
    return GetListFramesType(FInvisible);
  }

  void RestFrame::FillListFramesRecursive(RFList<RestFrame>& frames){
    frames.Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i))
	GetChildFrame(i)->FillListFramesRecursive(frames);
  }
  
  void RestFrame::FillListFramesTypeRecursive(FrameType type, RFList<RestFrame>& frames){
    if(m_Type == type) frames.Add(this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i))
	GetChildFrame(i)->FillListFramesTypeRecursive(type, frames);
  }

  bool RestFrame::IsCircularTree(vector<int>& keys) const {
    int Nkey = keys.size();

    for(int i = 0; i < Nkey; i++){
      if(keys[i] == GetKey()){
	m_Log << LogWarning << "This RestFrame appears more than once in the tree" << m_End;
	return true;
      }
    }
    keys.push_back(GetKey());
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i))
	if(GetChildFrame(i)->IsCircularTree(keys))
	  return true;
       
    return false;
  }

  bool RestFrame::IsConsistentAnaTree(AnaType ana) const {
    if(ana != m_Ana) return false;
    
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)->IsConsistentAnaTree(ana)) return false;
    
    return true;
  }

  bool RestFrame::FindPathToFrame(const RestFrame* framePtr, const RestFrame *prevPtr, 
				  vector<TVector3>& boosts) const {
    if(IsSame(framePtr)) return true;
  
    vector<const RestFrame*> try_frames;
    vector<TVector3> try_boosts;

    if(GetParentFrame()){
      try_frames.push_back(GetParentFrame());
      try_boosts.push_back(GetParentBoostVector());
    }
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i)){
	try_frames.push_back(GetChildFrame(i));
	try_boosts.push_back(GetChildBoostVector(i));
      }

    int Ntry = try_frames.size();
    for(int i = 0; i < Ntry; i++){
      const RestFrame* nextPtr = try_frames[i];
      if(nextPtr->IsSame(prevPtr)) continue;
      boosts.push_back(try_boosts[i]);
      if(nextPtr->FindPathToFrame(framePtr,this,boosts)) return true;
      boosts.pop_back();
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

  double RestFrame::GetCosDecayAngle(const RestFrame& frame) const{
    return GetCosDecayAngle(&frame);
  }

  double RestFrame::GetCosDecayAngle(const RestFrame* framePtr) const {
    if(!m_Spirit){
      //m_Log << LogInfo << "Can't call analysis function CosDecayAngle" << sendl;
      return 0.;
    }
    if(!GetParentFrame()) return 0.;
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
      if(GetNChildren() <= 0) return 0.;
      Pchild = GetChildFrame(0)->GetFourVector(prod_framePtr);
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
    TLorentzVector P1 = GetChildFrame(0)->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = GetChildFrame(1)->GetVisibleFourVector(framePtr);

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
    TVector3 P1 = GetChildFrame(0)->GetVisibleFourVector(this).Vect();
    TVector3 P2 = GetChildFrame(1)->GetVisibleFourVector(this).Vect();
    if(P1.Mag()+P2.Mag() > 0.) return sqrt(pow(P1.Mag()+P2.Mag(),2.)-(P1-P2).Mag2())/(P1.Mag()+P2.Mag());
    else return 0.;
  }
 
  double RestFrame::GetTransverseVisibleShape(const TVector3& axis, const RestFrame* framePtr) const {
    if(GetNChildren() != 2) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    if(!framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(framePtr);
    TLorentzVector P1 = GetChildFrame(0)->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = GetChildFrame(1)->GetVisibleFourVector(framePtr);

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
    TLorentzVector P1 = GetChildFrame(0)->GetVisibleFourVector(this);
    TLorentzVector P2 = GetChildFrame(1)->GetVisibleFourVector(this);
    return P1.P() + P2.P();
  }
  double RestFrame::GetTransverseScalarVisibleMomentum(const TVector3& axis, 
						       const RestFrame* framePtr) const {
    if(GetNChildren() != 2) return 0.;
    if(!framePtr) framePtr = GetLabFrame();
    if(!framePtr) return 0.;
    TLorentzVector Pthis = GetFourVector(framePtr);
    TLorentzVector P1 = GetChildFrame(0)->GetVisibleFourVector(framePtr);
    TLorentzVector P2 = GetChildFrame(1)->GetVisibleFourVector(framePtr);

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

    vector<TVector3> boosts;
    if(!m_ProdFramePtr->FindPathToFrame(framePtr, nullptr, boosts)) return V;
  
    int Nboost = boosts.size();
    for(int i = 0; i < Nboost; i++){
      V.Boost(-1.*boosts[i]);
    }
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
      RFList<RestFrame> frames = GetChildFrame(c)->GetListVisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++) 
	V += frames.Get(f)->GetFourVector(framePtr);
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
      RFList<RestFrame> frames = GetChildFrame(c)->GetListInvisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++) V += frames.Get(f)->GetFourVector(framePtr);
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
      if(childPtr->GetListFrames().Contains(framePtr)){
	if(depth == 1) return childPtr;
	else return childPtr->GetFrameAtDepth(depth-1,framePtr);
      }
    }
    return nullptr;
  }

  double RestFrame::GetDeltaPhiDecayPlanes(const RestFrame& frame) const {
    return GetDeltaPhiDecayPlanes(&frame);
  }
  double RestFrame::GetDeltaPhiDecayPlanes(const RestFrame* framePtr) const {
    if(!framePtr) return 0.;
    if(GetNChildren() < 1) return 0.;

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
    if(!GetParentFrame()) return V;
    return -1.*GetParentBoostVector();
  }

  double RestFrame::GetGammaInParentFrame() const{
    TVector3 vbeta = GetBoostInParentFrame();
    double beta = min(1.,vbeta.Mag());
    return 1./sqrt(1.-beta*beta);
  }

}
