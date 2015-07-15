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
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabFrame class
  ///////////////////////////////////////////////
  template <class T> LabFrame<T>::LabFrame(const string& sname, const string& stitle)
    : T(sname, stitle)
  {
    Init();
  }

  template <class T> 
  LabFrame<T>::~LabFrame(){
  
  }

  template <class T> 
  void LabFrame<T>::Init(){
    T::m_Type = FLab;
  }

  template <class T>  
  bool LabFrame<T>::IsSoundBody() const {
    if(RFBase::IsSoundBody()) return true;
    if(!T::IsSoundBody()){
      return T::SetBody(false);;
    }
    int Nchild = T::GetNChildren();
    if(Nchild != 1 || !T::GetParentFrame().IsEmpty()){
      *T::m_LogPtr << LogWarning << "Problem with parent or child frames" << m_End;
      return T::SetBody(false);
    }
    return T::SetBody(true);;
  }

  template <class T>
  void LabFrame<T>::SetChildFrame(RestFrame& frame){ 
    T::AddChildFrame(frame); 
  }

  template <class T> 
  bool LabFrame<T>::InitializeTree() const {
    *T::m_LogPtr << LogVerbose;
    *T::m_LogPtr << "Initializing tree skeleton...";
    *T::m_LogPtr << m_End;

    vector<RFKey> keys;
    if(T::IsCircularTree(keys)){
      *T::m_LogPtr << LogWarning;
      *T::m_LogPtr << "Tree is circular in construction";
      *T::m_LogPtr << m_End;
      return false;
    }

    if(!T::IsSoundBodyRecursive()){
      *T::m_LogPtr << LogWarning;
      *T::m_LogPtr << "Illegally constructed tree";
      *T::m_LogPtr << m_End;
      return false;
    }
 
    if(!T::IsConsistentAnaTree(T::m_Ana)){
      *T::m_LogPtr << LogWarning;
      *T::m_LogPtr << "Tree is not homogenous in node types (Reco, Gen)";
      *T::m_LogPtr << m_End;
      return false;
    }

    *T::m_LogPtr << LogVerbose;
    *T::m_LogPtr << "...Done initializing tree skeleton";
    *T::m_LogPtr << m_End;
    
    return true;
  }

  template <class T> 
  TVector3 LabFrame<T>::GetInvisibleMomentum() const {
    return T::GetInvisibleFourVector().Vect();
  }

  template <class T> 
  double LabFrame<T>::GetCosDecayAngle(const RestFrame& frame) const {
    if(!T::IsSoundSpirit()){
      T::UnSoundSpirit(RF_FUNCTION);
      return 0.;
    }

    if(T::GetNChildren() < 1) return 0.;
    TVector3 V1(0.,0.,1.);
    TVector3 V2;
    if(!frame.IsEmpty()){
      V2 = frame.T::GetFourVector(*this).Vect().Unit();
    } else {
      V2 = T::GetChildFrame(0).T::GetFourVector(*this).Vect().Unit();
    }
    return V1.Dot(V2);
  }

  template <class T> 
  TVector3 LabFrame<T>::GetDecayPlaneNormalVector() const {
    if(!T::IsSoundSpirit()){
      T::UnSoundSpirit(RF_FUNCTION);
      return TVector3(0.,0.,0.);
    }

    if(T::GetNChildren() < 1) return TVector3(0.,0.,0.);
   
    TVector3 V1 = T::GetChildFrame(0).T::GetFourVector(*this).Vect().Unit();
    TVector3 V2(0.,0.,1.);
    return V1.Cross(V2).Unit();
  }

  template class LabFrame<ReconstructionFrame>;
  template class LabFrame<GeneratorFrame>;
}
