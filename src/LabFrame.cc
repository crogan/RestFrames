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
    if(!RestFrame::IsSoundBody()){
      SetBody(false);
      return false;
    }
    int Nchild = GetNChildren();
    if(Nchild != 1 || GetParentFrame()){
      m_Log << LogWarning << "Problem with parent or child frames" << m_End;
      SetBody(false);
      return false;
    }
    SetBody(true);
    return true;
  }

  void LabFrame::SetChildFrame(RestFrame& frame){
    SetChildFrame(&frame);
  }

  void LabFrame::SetChildFrame(RestFrame* framePtr){ 
    if(!framePtr) return;
    AddChildFrame(framePtr); 
  }

  bool LabFrame::InitializeTree() const {
     vector<int> keys;
    if(IsCircularTree(keys)){
      cout << endl << "Consistent Topology Failure: ";
      cout << "Tree is circular in construction" << endl;
      return false;
    }

    if(!IsSoundBodyRecursive()){
      cout << endl << "Consistent Topology Failure: ";
      cout << "UnSound frame in tree" << endl;
      return false;
    }
 
    if(!IsConsistentAnaTree(m_Ana)){
      cout << endl << "Consistent Topology Failure: ";
      cout << "Tree contains mixture of node types (Reco, Gen)" << endl;
      return false;
    }
    
    return true;
  }

  TVector3 LabFrame::GetInvisibleMomentum() const {
    return GetInvisibleFourVector().Vect();
  }

  double LabFrame::GetCosDecayAngle(const RestFrame* framePtr) const {
    if(GetNChildren() < 1) return 0.;
    TVector3 V1(0.,0.,1.);
    TVector3 V2;
    if(framePtr){
      V2 = framePtr->GetFourVector(this).Vect().Unit();
    } else {
      V2 = GetChildFrame(0)->GetFourVector(this).Vect().Unit();
    }
    return V1.Dot(V2);
  }

  TVector3 LabFrame::GetDecayPlaneNormalVector() const {
    TVector3 V(0.,0.,0.);
    if(GetNChildren() < 1) return V;
   
    TVector3 V1 = GetChildFrame(0)->GetFourVector(this).Vect().Unit();
    TVector3 V2(0.,0.,1.);
    return V1.Cross(V2).Unit();
  }

}
