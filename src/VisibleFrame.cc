/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   VisibleFrame.cc
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

#include "RestFrames/VisibleFrame.hh"

using namespace std;

namespace RestFrames {
  ///////////////////////////////////////////////
  // VisibleFrame class methods
  ///////////////////////////////////////////////
  VisibleFrame::VisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  VisibleFrame::~VisibleFrame(){
    
  }

  void VisibleFrame::Init(){
    m_Type = FVisible;
  }

  bool VisibleFrame::IsSoundBody() const {
    RestFrame::IsSoundBody();
   
    int Nchild = GetNChildren();
    if(Nchild > 0 || !m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

  void VisibleFrame::SetLabFrameFourVector(const TLorentzVector& V){
    m_Lab_P.SetVectM(V.Vect(),V.M());
  }

  TLorentzVector VisibleFrame::GetLabFrameFourVector() const{
    TLorentzVector V;
    V.SetVectM(m_Lab_P.Vect(),m_Lab_P.M());
    return V;
  }

}
