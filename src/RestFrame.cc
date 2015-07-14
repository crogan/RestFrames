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

  RestFrame::RestFrame()
    : RFBase() 
  {
    Init();
  }

  RestFrame::RestFrame(const RFKey& key)
    : RFBase() 
  {
    Init();
    SetKey(key);
  }

  RestFrame::RestFrame(const string& sname, const string& stitle)
    : RFBase(sname, stitle) 
  {
    Init();
  }

  RestFrame g_RestFrame(g_Key);

  RestFrame::~RestFrame(){ }

  void RestFrame::Init(){
    SetKey(GenKey());
    m_ParentFramePtr = nullptr;
    m_ParentBoost.SetXYZ(0.,0.,0.);
    m_Log.SetSource("RestFrame "+GetName());
  }

  void RestFrame::Clear(){
    SetParentFrame();
    RemoveChildren();
    RFBase::Clear();
  }

  int RestFrame::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }
  
  bool RestFrame::IsSoundBody() const {
    if(RFBase::IsSoundBody()) return true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)){
	SetBody(false);
	m_Log << LogWarning << "Empty child frame:";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return false;
      }
    SetBody(true);
    return true;
  }

  bool RestFrame::IsSoundBodyRecursive() const {
    if(!IsSoundBody()) return false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!GetChildFrame(i).IsSoundBodyRecursive()){
	m_Log << LogWarning;
	m_Log << "Problem with recursive tree structure from frame: ";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return false;
      }
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

  bool RestFrame::IsGenFrame() const { 
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
    if(IsGenFrame())
      output += "   Ana Type: Generator \n";
    if(IsRFrame())
      output += "   Ana Type: Reconstruction \n";
    return output;
  }

  void RestFrame::RemoveChild(const RestFrame& frame){
    RemoveChildIndex(GetChildIndex(frame));
  }

  void RestFrame::RemoveChildIndex(int i){
    SetBody(false);
    if(i < 0 || i >= GetNChildren()) return;
    RestFrame& child = GetChildFrame(i);
    m_ChildFrames.Remove(child);
    m_ChildBoosts.erase(m_ChildBoosts.begin()+i);
    child.SetParentFrame();
  }

  void RestFrame::RemoveChildren(){
    SetBody(false);
    int N = GetNChildren();
    for(int i = N-1; i >= 0; i--)
      RemoveChildIndex(i);

    m_ChildFrames.Clear();
    m_ChildBoosts.clear();
  }

  void RestFrame::SetParentFrame(RestFrame& frame){
    SetBody(false);
    if(!frame){
      m_Log << LogWarning << "Cannot set empty frame as parent." << m_End;
    }
    if(m_Type != FLab) m_ParentFramePtr = &frame;
  }

  void RestFrame::SetParentFrame(){
    SetBody(false);
    RestFrame* framePtr = m_ParentFramePtr;
    m_ParentFramePtr = nullptr;
    if(framePtr)
      framePtr->RemoveChild(*this);
  }
  
  void RestFrame::AddChildFrame(RestFrame& frame){
    SetBody(false);
    if(!frame){
      m_Log << LogWarning << "Cannot add empty frame as child." << m_End;
      return;
    }
    if(frame.IsLabFrame()){
      m_Log << LogWarning << "Cannot add LabFrame frame as child: " << Log(frame) << m_End;
      return;
    }
    if(GetChildIndex(frame) >= 0){
      m_Log << LogWarning << "Frame is already among children: " << Log(frame) << m_End;
      return;
    }
    if(!m_ChildFrames.Add(frame)){
      m_Log << LogWarning << "Unable to add child frame:";
      m_Log << Log(frame) << m_End;
      return;
    }
    frame.SetParentFrame(*this);
    m_ChildBoosts.push_back(TVector3(0.,0.,0.));
  }

  int RestFrame::GetNChildren() const { 
    return m_ChildFrames.GetN();
  }

  int RestFrame::GetChildIndex(const RestFrame& frame) const {
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(GetChildFrame(i).IsSame(frame)) return i;
    
    return -1;
  }

  RestFrame& RestFrame::GetChildFrame(int i) const {
    int Nchild = GetNChildren();
    if(i >= Nchild || i < 0){
      m_Log << LogWarning;
      m_Log << "Cannot GetChildFrame(" << i << "). ";
      m_Log << "No " << i << "th child" << m_End;
    }
    return m_ChildFrames.Get(i);
  }

  RestFrame const& RestFrame::GetParentFrame() const {
    if(m_ParentFramePtr)
      return *m_ParentFramePtr;
    else 
      return g_RestFrame;
  }

  RestFrame const& RestFrame::GetLabFrame() const {
    if(m_Type == FLab) return *this;
  
    if(!GetParentFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to find LabFrame above this frame. ";
      m_Log << "No parent frame set" << m_End;
      return g_RestFrame;
    } 
    return GetParentFrame().GetLabFrame();
  } 

  RestFrame const& RestFrame::GetSiblingFrame() const {
    if(!IsSoundBody()) return g_RestFrame;

    if(IsLabFrame()) return g_RestFrame;

    if(!GetParentFrame())
      return g_RestFrame;

    int Nsib = GetParentFrame().GetNChildren();
    for(int s = 0; s < Nsib; s++){
      if(IsSame(GetParentFrame().GetChildFrame(s))) continue;
      return GetParentFrame().GetChildFrame(s);
    }
    return g_RestFrame; 
  }

  int RestFrame::GetNDescendants() const {
    if(!IsSoundBody()) return 0.;

    int Nchild = GetNChildren();
    if(Nchild == 0) return 0;
    int Nd = 0;
    for(int i = 0; i < Nchild; i++){
      Nd += GetChildFrame(i).GetNDescendants();
    }
    return Nd;
  }

  void RestFrame::SetChildBoostVector(int i, const TVector3& boost) {
    int N = GetNChildren();
    if(i < 0 || i >= N){
      m_Log << LogWarning;
      m_Log << "Unable to set child " << i;
      m_Log << "'s boost vector. No entry " << i;
      m_Log << m_End;
      return;
    }
    m_ChildBoosts[i] = boost;
    GetChildFrame(i).SetParentBoostVector(-1.*boost);
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
    if(i < 0 || i >= N){
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
    frames.Add(*this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
	GetChildFrame(i).FillListFramesRecursive(frames);
    return frames;
  }

  RFList<RestFrame> RestFrame::GetListFrames(FrameType type){
    RFList<RestFrame> frames;
    if(m_Type == type) frames.Add(*this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
	GetChildFrame(i).FillListFramesRecursive(type, frames);
    
    return frames;
  }

  RFList<RestFrame> RestFrame::GetListFrames(const vector<FrameType>& types){
    RFList<RestFrame> frames;
    int Ntype = types.size();
    for(int i = 0; i < Ntype; i++)
      FillListFramesRecursive(types[i], frames);

    return frames;
  }

  RFList<RestFrame> RestFrame::GetListVisibleFrames(){
    return GetListFrames(FVisible);
  }

  RFList<RestFrame> RestFrame::GetListInvisibleFrames(){
    return GetListFrames(FInvisible);
  }

  void RestFrame::FillListFramesRecursive(RFList<RestFrame>& frames){
    frames.Add(*this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i).FillListFramesRecursive(frames);
  }
  
  void RestFrame::FillListFramesRecursive(FrameType type, RFList<RestFrame>& frames){
    if(m_Type == type) frames.Add(*this);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i).FillListFramesRecursive(type, frames);
  }

  bool RestFrame::IsCircularTree(vector<RFKey>& keys) const {
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
      if(GetChildFrame(i).IsCircularTree(keys))
	return true;
       
    return false;
  }

  bool RestFrame::IsConsistentAnaTree(AnaType ana) const {
    if(ana != m_Ana) return false;
    
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i).IsConsistentAnaTree(ana)) return false;
    
    return true;
  }

  bool RestFrame::FindPathToFrame(const RestFrame& dest_frame, const RestFrame& prev_frame, 
				  vector<TVector3>& boosts) const {
    if(IsSame(dest_frame)) return true;
  
    vector<const RestFrame*> try_frames;
    vector<TVector3> try_boosts;

    if(!GetParentFrame().IsEmpty()){
      try_frames.push_back(&GetParentFrame());
      try_boosts.push_back(GetParentBoostVector());
    }
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      try_frames.push_back(&GetChildFrame(i));
      try_boosts.push_back(GetChildBoostVector(i));
    }

    int Ntry = try_frames.size();
    for(int i = 0; i < Ntry; i++){
      const RestFrame* nextPtr = try_frames[i];
      if(nextPtr->IsSame(prev_frame)) continue;
      boosts.push_back(try_boosts[i]);
      if(nextPtr->FindPathToFrame(dest_frame,*this,boosts)) return true;
      boosts.pop_back();
    }
    return false;
  }

  void RestFrame::SetFourVector(const TLorentzVector& V, const RestFrame& prod_frame){
    m_P.SetVectM(V.Vect(),V.M());
    m_ProdFramePtr = &prod_frame;  
  }

  //////////////////////////////
  // User Analysis functions
  //////////////////////////////

  RFList<RestFrame> RestFrame::operator+(RestFrame& frame){
    RFList<RestFrame> list;
    list.Add(frame);
    list.Add(*this);
    return list;
  }

  double RestFrame::GetMass() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }
    return m_P.M();
  }

  double RestFrame::GetCosDecayAngle() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }
    if(!GetParentFrame()) return 0.;
    if(GetNChildren() < 1) return 0.;
    TVector3 V1 = GetParentBoostVector().Unit();
    TVector3 V2 = GetChildFrame(0).GetFourVector(*this).Vect().Unit();
    return V1.Dot(V2);
  }

  double RestFrame::GetCosDecayAngle(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(!frame)
      return GetCosDecayAngle();

    if(!GetParentFrame()) return 0.;
    TVector3 V1 = GetParentBoostVector().Unit();
    TVector3 V2 = frame.GetFourVector(*this).Vect().Unit();
    return V1.Dot(V2);
  }
  
  // Get decay angle in plane perpendicular to 3-vector 'axis', where axis is defined
  // in the production frame of 'this'. Decay angle is relative to frame 'framePtr'
  // unless framePtr == nullptr (default), in which case is it first child of 'this'
  double RestFrame::GetDeltaPhiDecayAngle(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    const RestFrame& prod_frame = GetParentFrame();
    TLorentzVector Pthis = GetFourVector(prod_frame);
    TLorentzVector Pchild;
    if(!frame){
      if(GetNChildren() < 1) return 0.;
      Pchild = GetChildFrame(0).GetFourVector(prod_frame);
    } else {
      Pchild = frame.GetFourVector(prod_frame);
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
  double RestFrame::GetDeltaPhiBoostVisible(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    TLorentzVector Pvis = GetVisibleFourVector(frame);
    TLorentzVector Pthis = GetFourVector(frame);

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
  double RestFrame::GetDeltaPhiDecayVisible(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(GetNChildren() < 1) return 0.;

    TLorentzVector Pvis   = GetVisibleFourVector(frame);
    TLorentzVector Pchild = GetChildFrame(0).GetFourVector(frame);
    TLorentzVector Pthis  = GetFourVector(frame);

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
  double RestFrame::GetDeltaPhiVisible(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(GetNChildren() != 2) return 0.;
 
    TLorentzVector Pthis = GetFourVector(frame);
    TLorentzVector P1 = GetChildFrame(0).GetVisibleFourVector(frame);
    TLorentzVector P2 = GetChildFrame(1).GetVisibleFourVector(frame);

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
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(GetNChildren() != 2) return 0.;
    TVector3 P1 = GetChildFrame(0).GetVisibleFourVector(*this).Vect();
    TVector3 P2 = GetChildFrame(1).GetVisibleFourVector(*this).Vect();
    if(P1.Mag()+P2.Mag() > 0.) return sqrt(pow(P1.Mag()+P2.Mag(),2.)-(P1-P2).Mag2())/(P1.Mag()+P2.Mag());
    else return 0.;
  }
 
  double RestFrame::GetTransverseVisibleShape(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(GetNChildren() != 2) return 0.;
   
    TLorentzVector Pthis = GetFourVector(frame);
    TLorentzVector P1 = GetChildFrame(0).GetVisibleFourVector(frame);
    TLorentzVector P2 = GetChildFrame(1).GetVisibleFourVector(frame);

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
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    double ret = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      ret += GetChildFrame(i).GetVisibleFourVector(*this).P();
  
    return ret;
  }
  double RestFrame::GetTransverseScalarVisibleMomentum(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    double ret = 0.;
    int N = GetNChildren();
    vector<TLorentzVector> P;
   
    for(int i = 0; i < N; i++)
      P.push_back(GetChildFrame(i).GetVisibleFourVector(frame));
    TLorentzVector Pthis = GetFourVector(frame);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    for(int i = 0; i < N; i++)
      P[i].Boost(-boost_par);
    Pthis.Boost(-boost_par);
   
    TVector3 boost_perp = Pthis.BoostVector();
    for(int i = 0; i < N; i++)
      P[i].Boost(-boost_perp);
    
    for(int i = 0; i < N; i++){
      TVector3 V = P[i].Vect();
      V = V - V.Dot(axis.Unit())*axis.Unit();
      ret += V.Mag();
    }

    return ret;
  }

  TLorentzVector RestFrame::GetFourVector() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    TLorentzVector V = m_P;
    
    if(IsLabFrame()) return V;

    const RestFrame& lab = GetLabFrame();
    if(!lab){
      m_Log << LogWarning;
      m_Log << "Cannot find lab frame to evaluate four vector";
      m_Log << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }
    return GetFourVector(lab);

  }

  TLorentzVector RestFrame::GetFourVector(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(!frame) return GetFourVector();
 
    if(!GetProductionFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to get four vector. ";
      m_Log << "Production frame is not defined." << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }

    TLorentzVector V = m_P;
    if(frame == GetProductionFrame()) return V;

    vector<TVector3> boosts;
    if(!GetProductionFrame().FindPathToFrame(frame, g_RestFrame, boosts)){
      m_Log << LogWarning;
      m_Log << "Unable to get four vector. ";
      m_Log << "Cannot find a path to frame " << frame.GetName();
      m_Log << " from frame " << GetProductionFrame().GetName() << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }
  
    int Nboost = boosts.size();
    for(int i = 0; i < Nboost; i++)
      V.Boost(-1.*boosts[i]);
    return V;
  }

  TLorentzVector RestFrame::GetVisibleFourVector() const {   
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }
 
    const RestFrame& lab = GetLabFrame();
    if(!lab){
      m_Log << LogWarning;
      m_Log << "Cannot find lab frame to evaluate four vector";
      m_Log << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }
    return GetVisibleFourVector(lab);
  }
  TLorentzVector RestFrame::GetVisibleFourVector(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(!frame) return GetVisibleFourVector();

    TLorentzVector V(0.,0.,0.,0.);
    if(!m_Spirit) return V;
    int Nc = GetNChildren();
    for(int c = 0; c < Nc; c++){
      RFList<RestFrame> frames = GetChildFrame(c).GetListVisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++) 
	V += frames.Get(f).GetFourVector(frame);
    }
    return V;
  }

  TLorentzVector RestFrame::GetInvisibleFourVector() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    const RestFrame& lab = GetLabFrame();
    if(!lab){
      m_Log << LogWarning;
      m_Log << "Cannot find lab frame to evaluate four vector";
      m_Log << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }
    return GetInvisibleFourVector(lab);
  }
  TLorentzVector RestFrame::GetInvisibleFourVector(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(!frame) return GetVisibleFourVector();

    TLorentzVector V(0.,0.,0.,0.);
    if(!m_Spirit) return V;
    int Nc = GetNChildren();
    for(int c = 0; c < Nc; c++){
      RFList<RestFrame> frames = GetChildFrame(c).GetListInvisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++) 
	V += frames.Get(f).GetFourVector(frame);
    }
    return V;
  }

  double RestFrame::GetEnergy(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    return GetFourVector(frame).E();
  }

  double RestFrame::GetMomentum(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    return GetFourVector(frame).P();
  }

  int RestFrame::GetFrameDepth(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(!frame) return -1;
    if(IsSame(frame)) return 0.;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      int depth = GetChildFrame(i).GetFrameDepth(frame);
      if(depth >= 0) return depth+1;
    }
    return -1;
  }

  RestFrame const& RestFrame::GetFrameAtDepth(int depth, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return g_RestFrame;
    }

    if(!frame || depth < 1) return m_ChildFrames.Get(-1);
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      RestFrame& child = GetChildFrame(i);
      if(child.GetListFrames().Contains(frame)){
	if(depth == 1) return child;
	else return child.GetFrameAtDepth(depth-1,frame);
      }
    }
    return g_RestFrame;
  }

  double RestFrame::GetDeltaPhiDecayPlanes(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(!frame) return 0.;
    if(GetNChildren() < 1) return 0.;

    TVector3 vNorm_frame = frame.GetDecayPlaneNormalVector();
    TVector3 vNorm_this = GetDecayPlaneNormalVector();
    double dphi = vNorm_this.Angle(vNorm_frame);

    if(frame.GetFourVector(*this).Vect().Cross(vNorm_frame).Dot(vNorm_this) < 0.){
      dphi = TMath::Pi()*2. - dphi;
    }

    return dphi;
  }
  
  TVector3 RestFrame::GetDecayPlaneNormalVector() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TVector3(0.,0.,0.);
    }

    if(GetNChildren() < 1) return TVector3(0.,0.,0.);

    TVector3 V1 = GetChildFrame(0).GetFourVector(GetParentFrame()).Vect();
    TVector3 V2 = GetFourVector(GetParentFrame()).Vect();

    return V1.Cross(V2).Unit();
  }

  RestFrame const& RestFrame::GetProductionFrame() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return g_RestFrame;
    }

    if(m_ProdFramePtr)
      return *m_ProdFramePtr;
    else 
      return g_RestFrame;
  }

  TVector3 RestFrame::GetBoostInParentFrame() const {
    TVector3 V(0.,0.,0.);

    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return V;
    }

    if(!GetParentFrame()) return V;
    return -1.*GetParentBoostVector();
  }

  double RestFrame::GetGammaInParentFrame() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    TVector3 vbeta = GetBoostInParentFrame();
    double beta = min(1.,vbeta.Mag());
    return 1./sqrt(1.-beta*beta);
  }

}
