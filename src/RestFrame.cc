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
#include "RestFrames/ReconstructionFrame.hh"

using namespace std;

namespace RestFrames {

  int RestFrame::m_class_key = 0;

  RestFrame::RestFrame() : RFBase() {}
  
  RestFrame::~RestFrame() { m_Type = kVanillaFrame; }

  RestFrame::RestFrame(const string& sname, const string& stitle)
    : RFBase(sname, stitle, RestFrame::m_class_key++) 
  {
    m_Log.SetSource("RestFrame "+GetName());
    m_Type = kVanillaFrame;
    m_ParentFramePtr = nullptr;
    m_ParentBoost.SetXYZ(0.,0.,0.);
  }

  void RestFrame::Clear(){
    SetParentFrame();
    RemoveChildren();
    RFBase::Clear();
  }
  
  RestFrame& RestFrame::Empty(){
    return ReconstructionFrame::Empty();
  }

  TVector3 RestFrame::m_Axis = TVector3(0.,0.,1.);

  void RestFrame::SetAxis(const TVector3& axis){
    RestFrame::m_Axis = axis;
  }

  TVector3 const& RestFrame::GetAxis(){
    return RestFrame::m_Axis;
  }

  RFList<RestFrame> RestFrame::operator+(RestFrame& frame){
    RFList<RestFrame> list;
    list.Add(frame);
    list.Add(*this);
    return list;
  }

  FrameType RestFrame::GetType() const { 
    return m_Type; 
  }

  bool RestFrame::IsVisibleFrame() const { 
    return m_Type == kVisibleFrame; 
  }

  bool RestFrame::IsInvisibleFrame() const { 
    return m_Type == kInvisibleFrame; 
  }

  bool RestFrame::IsDecayFrame() const { 
    return m_Type == kDecayFrame; 
  }

  bool RestFrame::IsLabFrame() const { 
    return m_Type == kLabFrame; 
  }

  bool RestFrame::IsRecoFrame() const { 
    return m_Ana == kRecoFrame; 
  }

  bool RestFrame::IsGenFrame() const { 
    return m_Ana == kGenFrame; 
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
    if(IsRecoFrame())
      output += "   Ana Type: Reconstruction \n";
    return output;
  }

  bool RestFrame::IsSoundBody() const {
    if(RFBase::IsSoundBody()) return true;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      if(!GetChildFrame(i)){
	m_Log << LogWarning << "Empty child frame:";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return SetBody(false);
      }
    return SetBody(true);
  }

  bool RestFrame::InitializeTreeRecursive() {
    if(!IsSoundBody()) return false;
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      if(!GetChildFrame(i).InitializeTreeRecursive()){
	m_Log << LogWarning;
	m_Log << "Problem with recursive tree structure from frame: ";
	m_Log << Log(GetChildFrame(i)) << m_End;
	return SetBody(false);
      }
    }
    return true;
  }

  void RestFrame::RemoveChild(RestFrame& frame){
    SetBody(false);
    bool contains = m_ChildFrames.Contains(frame);
    m_ChildBoosts.erase(&frame);
    m_ChildFrames.Remove(frame);
    if(contains)
      frame.SetParentFrame();
  }

  void RestFrame::RemoveChildren(){
    SetBody(false);
    while(GetNChildren() > 0)
      RemoveChild(m_ChildFrames[0]);
    m_ChildFrames.Clear();
    m_ChildBoosts.clear();
  }

  void RestFrame::SetParentFrame(RestFrame& frame){
    SetBody(false);
    
    RestFrame* prevPtr = m_ParentFramePtr;
    if(m_ParentFramePtr)
      if(*m_ParentFramePtr != frame){
	m_ParentFramePtr = nullptr;
	prevPtr->RemoveChild(*this);
      }
    if(!frame)
      m_ParentFramePtr = nullptr;
    else
      m_ParentFramePtr = &frame;
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
    if(!m_ChildFrames.Add(frame)){
      m_Log << LogWarning << "Unable to add child frame:";
      m_Log << Log(frame) << m_End;
      return;
    }
    frame.SetParentFrame(*this);
    m_ChildBoosts[&frame] = TVector3(0.,0.,0.);
  }

  void RestFrame::AddChildFrames(const RestFrames::RFList<RestFrame>& frames){
    int N = frames.GetN();
    for(int i = 0; i < N; i++)
      AddChildFrame(frames[i]);
  }

  int RestFrame::GetNChildren() const { 
    return m_ChildFrames.GetN();
  }

  RestFrame& RestFrame::GetChildFrame(int i) const {
    int Nchild = GetNChildren();
    if(i >= Nchild || i < 0){
      m_Log << LogWarning;
      m_Log << "Cannot GetChildFrame(" << i << "). ";
      m_Log << "No " << i << "th child" << m_End;
    }
    return m_ChildFrames[i];
  }

  RestFrame const& RestFrame::GetParentFrame() const {
    if(m_ParentFramePtr)
      return *m_ParentFramePtr;
    else 
      return RestFrame::Empty();
  }

  RestFrames::RFList<RestFrame> RestFrame::GetChildren() const {
    return m_ChildFrames;
  }

  RestFrame const& RestFrame::GetLabFrame() const {
    if(IsLabFrame()) 
      return *this;
    
    if(!GetParentFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to find LabFrame above this frame. ";
      m_Log << "No parent frame set" << m_End;
      return RestFrame::Empty();
    } 
    return m_ParentFramePtr->GetLabFrame();
  } 

  RestFrame const& RestFrame::GetSiblingFrame() const {
    if(!IsSoundBody()) 
      return RestFrame::Empty();

    if(IsLabFrame()) 
      return RestFrame::Empty();

    if(!GetParentFrame())
      return RestFrame::Empty();

    int Nsib = GetParentFrame().GetNChildren();
    for(int s = 0; s < Nsib; s++){
      if(IsSame(m_ParentFramePtr->GetChildFrame(s))) 
	continue;
      return m_ParentFramePtr->GetChildFrame(s);
    }
    return RestFrame::Empty(); 
  }

  int RestFrame::GetNDescendants() const {
    if(!IsSoundBody()) return 0.;

    int Nchild = GetNChildren();
    if(Nchild == 0) return 1;
    int Nd = 0;
    for(int i = 0; i < Nchild; i++){
      Nd += GetChildFrame(i).GetNDescendants();
    }
    return Nd;
  }

  void RestFrame::SetChildBoostVector(RestFrame& frame, const TVector3& boost) {
    if(!m_ChildFrames.Contains(frame)){
      m_Log << LogWarning;
      m_Log << "Unable to set child's boost vector. ";
      m_Log << "Frame is not among childrend:";
      m_Log << Log(frame) << m_End;
      return;
    }
    m_ChildBoosts[&frame] = boost;
    frame.SetParentBoostVector(-1.*boost);
  }

  void RestFrame::SetParentBoostVector(const TVector3& boost) {
    if(!GetParentFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to set parent boost vector. ";
      m_Log << "No parent frame set.";
      m_Log << m_End;
      return;
    }
    m_ParentBoost = boost;
  }

  TVector3 RestFrame::GetChildBoostVector(RestFrame& frame) const {
    if(!m_ChildFrames.Contains(frame)){
      m_Log << LogWarning;
      m_Log << "Unable to get child's boost vector. ";
      m_Log << "Frame is not among children:";
      m_Log << Log(frame) << m_End;
      return TVector3(0.,0.,0.);
    }
    return m_ChildBoosts[&frame];
  }

  TVector3 RestFrame::GetParentBoostVector() const {
    return m_ParentBoost;
  }

  RFList<RestFrame> RestFrame::GetListFrames(FrameType type) const {
    RFList<RestFrame> frames;
    FillListFramesRecursive(frames,type);
    return frames;
  }

  RFList<RestFrame> RestFrame::GetListVisibleFrames() const {
    return GetListFrames(kVisibleFrame);
  }

  RFList<RestFrame> RestFrame::GetListInvisibleFrames() const {
    return GetListFrames(kInvisibleFrame);
  }
  
  void RestFrame::FillListFramesRecursive(RFList<RestFrame>& frames, FrameType type) const {
    if(frames.Contains(*this)) return;
    if(type == GetType() || type == kLabFrame) frames.Add((RestFrame&)(*m_This));
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildFrame(i).FillListFramesRecursive(frames, type);
  }

  bool RestFrame::IsCircularTree(vector<RFKey>& keys) const {
    int Nkey = keys.size();
    for(int i = 0; i < Nkey; i++){
      if(keys[i] == GetKey()){
	m_Log << LogWarning;
	m_Log << "This RestFrame appears more than once in the tree:";
	m_Log << Log(*this) << m_End;
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
      try_boosts.push_back(GetChildBoostVector(GetChildFrame(i)));
    }

    int Ntry = try_frames.size();
    for(int i = 0; i < Ntry; i++){
      const RestFrame* nextPtr = try_frames[i];
      if(nextPtr->IsSame(prev_frame)) continue;
      boosts.push_back(try_boosts[i]);
      if(nextPtr->FindPathToFrame(dest_frame,*this,boosts)) 
	return true;
      boosts.pop_back();
    }
    return false;
  }

  void RestFrame::SetFourVector(const TLorentzVector& V, 
				const RestFrame& frame){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return;
    }
    if(!frame)
      m_ProdFramePtr = &GetLabFrame();
    else 
      m_ProdFramePtr = &frame; 

    m_P.SetVectM(V.Vect(),V.M()); 
  }

  //////////////////////////////
  // User Analysis functions
  //////////////////////////////

  double RestFrame::GetMass() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }
    return m_P.M();
  }

  TLorentzVector RestFrame::GetFourVector(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(!frame)
      return GetFourVector(GetLabFrame());
   
    if(!GetProductionFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to get four vector. ";
      m_Log << "Production frame is not defined." << m_End;
      return TLorentzVector(0.,0.,0.,0.);
    }

    TLorentzVector V = m_P;
    if(V.E() <= 0.)
      return TLorentzVector(0.,0.,0.,0.);
    if(frame == GetProductionFrame()) return V;

    vector<TVector3> boosts;
    if(!GetProductionFrame().
       FindPathToFrame(frame, RestFrame::Empty(), boosts)){
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

  TLorentzVector RestFrame::GetVisibleFourVector(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(!frame){
      if(!GetLabFrame())
	return TLorentzVector(0.,0.,0.,0.);
      else
	return GetVisibleFourVector(GetLabFrame());
    }
    
    if(IsVisibleFrame())
      return GetFourVector(frame);

    TLorentzVector V(0.,0.,0.,0.);
    int Nc = GetNChildren();
    for(int c = 0; c < Nc; c++){
      RFList<RestFrame> frames = GetChildFrame(c).GetListVisibleFrames();
      int Nf = frames.GetN();
      for(int f = 0; f < Nf; f++) 
	V += frames[f].GetFourVector(frame);
    }
    return V;
  }

  TLorentzVector RestFrame::GetInvisibleFourVector(const RestFrame& frame) const {
   if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
   }

   if(!frame){
     if(!GetLabFrame())
       return TLorentzVector(0.,0.,0.,0.);
     else
       return GetInvisibleFourVector(GetLabFrame());
   }

    if(IsInvisibleFrame())
      return GetFourVector(frame);
   
   TLorentzVector V(0.,0.,0.,0.);
   int Nc = GetNChildren();
   for(int c = 0; c < Nc; c++){
     RFList<RestFrame> frames = GetChildFrame(c).GetListInvisibleFrames();
     int Nf = frames.GetN();
     for(int f = 0; f < Nf; f++) 
       V += frames[f].GetFourVector(frame);
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

  double RestFrame::GetTransverseMomentum(const RestFrame& frame,
					  const TVector3& axis, 
					  const RestFrame& axis_frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(frame == axis_frame){
      TVector3 V = GetFourVector(frame).Vect();
      return (V-V.Dot(axis.Unit())*axis.Unit()).Mag();
    }

    TLorentzVector Pthis  = GetFourVector(axis_frame);

    TLorentzVector Pframe;
    if(!frame || frame.IsLabFrame()){
      Pframe = axis_frame.GetFourVector(frame);
      Pframe.SetVectM(-Pframe.Vect(),Pframe.M());
    } else {
      Pframe = frame.GetFourVector(axis_frame);
    }

    TVector3 boost_par = Pframe.BoostVector();

    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    Pframe.Boost(-boost_par);
    TVector3 boost_perp = Pframe.BoostVector();
    Pthis.Boost(-boost_perp);
   
    TVector3 V = Pthis.Vect();
    return (V-V.Dot(axis.Unit())*axis.Unit()).Mag();
  }

  TLorentzVector RestFrame::GetFourVector(const TLorentzVector& P, 
					   const RestFrame& def_frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TLorentzVector(0.,0.,0.,0.);
    }

    if(IsSame(def_frame) || (!def_frame && IsLabFrame()))
      return P;

    TLorentzVector Pret = P;
    
    vector<TVector3> boosts;
    if(!def_frame){
      if(!GetLabFrame().
	 FindPathToFrame(*this, RestFrame::Empty(), boosts)){
	m_Log << LogWarning;
	m_Log << "Unable to get four vector. ";
	m_Log << "Cannot find a path to frame " << GetName();
	m_Log << " from frame " << GetLabFrame().GetName() << m_End;
	return TLorentzVector(0.,0.,0.,0.);
      }
    } else {
       if(!def_frame.
	 FindPathToFrame(*this, RestFrame::Empty(), boosts)){
	m_Log << LogWarning;
	m_Log << "Unable to get four vector. ";
	m_Log << "Cannot find a path to frame " << GetName();
	m_Log << " from frame " << GetLabFrame().GetName() << m_End;
	return TLorentzVector(0.,0.,0.,0.);
       }
    }
    int Nboost = boosts.size();
    for(int i = 0; i < Nboost; i++)
      Pret.Boost(-1.*boosts[i]);
    return Pret;
  }

  double RestFrame::GetTransverseMomentum(const TLorentzVector& P,
					   const TVector3& axis, 
					   const RestFrame& axis_frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(IsLabFrame() && (!axis_frame || axis_frame.IsLabFrame())){
      TVector3 V = P.Vect();
      return (V-V.Dot(axis.Unit())*axis.Unit()).Mag();
    }

    TLorentzVector Pret = P;

    // move P to axis_frame
    if(!axis_frame.IsLabFrame() && !axis_frame.IsEmpty()){
      vector<TVector3> boosts;
      if(!GetLabFrame().
	 FindPathToFrame(axis_frame, RestFrame::Empty(), boosts)){
	m_Log << LogWarning;
	m_Log << "Unable to get four vector. ";
	m_Log << "Cannot find a path to frame " << axis_frame.GetName();
	m_Log << " from frame " << GetLabFrame().GetName() << m_End;
	return 0.;
      }
      int Nboost = boosts.size();
      for(int i = 0; i < Nboost; i++)
	Pret.Boost(-1.*boosts[i]);
    }

    if(*this == axis_frame){
      TVector3 V = Pret.Vect();
      return (V-V.Dot(axis.Unit())*axis.Unit()).Mag();
    }

    TLorentzVector Pthis;
    if(IsLabFrame()){
      Pthis = axis_frame.GetFourVector();
      Pthis.SetVectM(-Pthis.Vect(),Pthis.M());
    } else {
      Pthis = GetFourVector(axis_frame);
    }

    TVector3 boost_par = Pthis.BoostVector();

    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pret.Boost(-boost_par);
    Pthis.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    Pret.Boost(-boost_perp);
   
    TVector3 V = Pret.Vect();
    return (V-V.Dot(axis.Unit())*axis.Unit()).Mag();
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
      return RestFrame::Empty();
    }

    if(!frame || depth < 1) 
      return RestFrame::Empty();

    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      RestFrame& child = GetChildFrame(i);
      if(child.GetListFrames().Contains(frame)){
	if(depth == 1) return child;
	else return child.GetFrameAtDepth(depth-1,frame);
      }
    }
    return RestFrame::Empty();
  }

  double RestFrame::GetVisibleShape() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }
    double Psum = 0.;
    double Xsum = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      Psum += GetChildFrame(i).GetVisibleFourVector(*this).P();
    for(int i = 0; i < N-1; i++){
      TVector3 P1 = GetChildFrame(i).GetVisibleFourVector(*this).Vect();
      for(int j = i+1; j < N; j++){
	TVector3 P2 = GetChildFrame(j).GetVisibleFourVector(*this).Vect();
	Xsum += (P1.Mag()+P2.Mag())*(P1.Mag()+P2.Mag())-(P1-P2).Mag2();
      }
    }
    if(Psum > 0.)
      return sqrt(Xsum)/Psum;
    else 
      return 0.;
  }

  double RestFrame::GetSumVisibleMomentum() const {
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

  double RestFrame::GetSumInvisibleMomentum() const {
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

  RestFrame const& RestFrame::GetProductionFrame() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return RestFrame::Empty();
    }

    if(m_ProdFramePtr)
      return *m_ProdFramePtr;
    else 
      return RestFrame::Empty();
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

  double RestFrame::GetCosDecayAngle(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    TVector3 V1 = GetParentBoostVector().Unit();
    if(IsLabFrame())
      V1 = RestFrame::GetAxis();
    TVector3 V2;
    if(!frame.IsEmpty())
      V2 = frame.GetFourVector(*this).Vect().Unit();
    else 
      if(GetNChildren() < 1) 
	return 0.;
      else 
	V2 = GetChildFrame(0).GetFourVector(*this).Vect().Unit();
    
    return V1.Dot(V2);
  }

  TVector3 RestFrame::GetDecayPlaneNormalVector() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TVector3(0.,0.,0.);
    }

    if(GetNChildren() < 1) return TVector3(0.,0.,0.);

    TVector3 V1, V2;
    if(!IsLabFrame()){
      V1 = GetChildFrame(0).GetFourVector(GetParentFrame()).Vect();
      V2 = GetFourVector(GetParentFrame()).Vect();
    } else {
      V1 = GetChildFrame(0).GetFourVector(*this).Vect().Unit();
      V2 = RestFrame::GetAxis();
    }
    return V1.Cross(V2).Unit();
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
 
  double RestFrame::GetDeltaPhiDecayAngle(const TVector3& axis, const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(IsLabFrame())
      return 0.;

    TLorentzVector Pthis   = GetFourVector(frame);
    TLorentzVector Pchild  = GetChildFrame(0).GetFourVector(frame);

    TVector3 boost_par = Pthis.BoostVector();
    boost_par = boost_par.Dot(axis.Unit())*axis.Unit();
    Pthis.Boost(-boost_par);
    Pchild.Boost(-boost_par);
    TVector3 boost_perp = Pthis.BoostVector();
    Pchild.Boost(-boost_perp);
    
    TVector3 V1 = boost_perp;
    TVector3 V2 = Pchild.Vect();
    V1 = V1 - V1.Dot(axis.Unit())*axis.Unit();
    V2 = V2 - V2.Dot(axis.Unit())*axis.Unit();
    return V1.Angle(V2);
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

}
