/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   DecayFrame.cc
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

#include "RestFrames/DecayFrame.hh"
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayFrame class
  ///////////////////////////////////////////////
  template <class T> DecayFrame<T>::DecayFrame(const string& sname,const string& stitle)
    : T(sname, stitle)
  {
    Init();
  }

  template <class T> 
  DecayFrame<T>::~DecayFrame(){
  
  }

  template <class T> 
  void DecayFrame<T>::Init(){
    T::m_Type = kDecayFrame;
  }

  template <class T> 
  bool DecayFrame<T>::IsSoundBody() const {
    if(RFBase::IsSoundBody()) return true;
    if(!T::IsSoundBody()){
      return T::SetBody(false);
    }
    int Nchild = T::GetNChildren();
    if(Nchild < 1 || T::GetParentFrame().IsEmpty()){
      return T::SetBody(false);
    } 
    return true;
  }

  template class DecayFrame<ReconstructionFrame>;
  template class DecayFrame<GeneratorFrame>;
}
