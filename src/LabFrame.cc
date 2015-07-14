/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabFrame.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
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

#include "RestFrames/LabFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabFrame class
  ///////////////////////////////////////////////
  LabFrame::LabFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }
  LabFrame::~LabFrame(){
  
  }
  void LabFrame::Init(){
    m_Type = FLab;
  }

  bool LabFrame::IsSoundBody() const {
    if(RFBase::IsSoundBody()) return true;
    if(!RestFrame::IsSoundBody()){
      SetBody(false);
      return false;
    }
    int Nchild = GetNChildren();
    if(Nchild != 1 || !GetParentFrame().IsEmpty()){
      m_Log << LogWarning << "Problem with parent or child frames" << m_End;
      SetBody(false);
      return false;
    }
    SetBody(true);
    return true;
  }

  void LabFrame::SetChildFrame(RestFrame& frame){ 
    AddChildFrame(frame); 
  }

  bool LabFrame::InitializeTree() const {
    m_Log << LogVerbose;
    m_Log << "Initializing tree skeleton...";
    m_Log << m_End;

    vector<RFKey> keys;
    if(IsCircularTree(keys)){
      m_Log << LogWarning;
      m_Log << "Tree is circular in construction";
      m_Log << m_End;
      return false;
    }

    if(!IsSoundBodyRecursive()){
      m_Log << LogWarning;
      m_Log << "Illegally constructed tree";
      m_Log << m_End;
      return false;
    }
 
    if(!IsConsistentAnaTree(m_Ana)){
      m_Log << LogWarning;
      m_Log << "Tree is not homogenous in node types (Reco, Gen)";
      m_Log << m_End;
      return false;
    }

    m_Log << LogVerbose;
    m_Log << "...Done initializing tree skeleton";
    m_Log << m_End;
    
    return true;
  }

  TVector3 LabFrame::GetInvisibleMomentum() const {
    return GetInvisibleFourVector().Vect();
  }

  double LabFrame::GetCosDecayAngle(const RestFrame& frame) const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(GetNChildren() < 1) return 0.;
    TVector3 V1(0.,0.,1.);
    TVector3 V2;
    if(!frame.IsEmpty()){
      V2 = frame.GetFourVector(*this).Vect().Unit();
    } else {
      V2 = GetChildFrame(0).GetFourVector(*this).Vect().Unit();
    }
    return V1.Dot(V2);
  }

  TVector3 LabFrame::GetDecayPlaneNormalVector() const {
    if(!IsSoundSpirit()){
      UnSoundSpirit(RF_FUNCTION);
      return TVector3(0.,0.,0.);
    }

    if(GetNChildren() < 1) return TVector3(0.,0.,0.);
   
    TVector3 V1 = GetChildFrame(0).GetFourVector(*this).Vect().Unit();
    TVector3 V2(0.,0.,1.);
    return V1.Cross(V2).Unit();
  }

}
