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
  template <class T> 
  LabFrame<T>::LabFrame(const string& sname, const string& stitle)
    : T(sname, stitle)
  {
    T::m_Type = kLabFrame;
  }

  template <class T> 
  LabFrame<T>::LabFrame() : T() {}
 

  template <class T> 
  LabFrame<T>::~LabFrame() {}

  template <class T>  
  bool LabFrame<T>::IsSoundBody() const {
    if(RFBase::IsSoundBody()) 
      return true;
    if(!T::IsSoundBody()){
      return T::SetBody(false);
    }
    int Nchild = T::GetNChildren();
    if(Nchild != 1){
      T::m_Log << LogWarning << "Number of LabFrame children != 1: ";
      T::m_Log << Nchild << m_End;
      return T::SetBody(false);
    }
    if(!T::GetParentFrame().IsEmpty()){
      T::m_Log << LogWarning << "Parent frame of LabFrame is not empty: ";
      T::m_Log << Log(T::GetParentFrame()) << m_End;
      return T::SetBody(false);
    }
    return T::SetBody(true);;
  }

  template <class T>
  void LabFrame<T>::SetChildFrame(RestFrame& frame){ 
    T::AddChildFrame(frame); 
  }

  template <class T> 
  bool LabFrame<T>::InitializeTree() {
    T::m_Log << LogVerbose;
    T::m_Log << "Initializing tree skeleton...";
    T::m_Log << m_End;

    vector<RFKey> keys;
    if(T::IsCircularTree(keys)){
      T::m_Log << LogWarning;
      T::m_Log << "   Tree is circular in construction";
      T::m_Log << m_End;
      return false;
    }

    if(!T::InitializeTreeRecursive()){
      T::m_Log << LogWarning;
      T::m_Log << "   Problem with structure of tree";
      T::m_Log << m_End;
      return false;
    }

    T::m_Log << LogVerbose;
    T::m_Log << "...Done";
    T::m_Log << m_End;
    
    return true;
  }

  template <class T> 
  TVector3 LabFrame<T>::GetInvisibleMomentum() const {
    return T::GetInvisibleFourVector().Vect();
  }

  template class LabFrame<ReconstructionFrame>;
  template class LabFrame<GeneratorFrame>;
}
