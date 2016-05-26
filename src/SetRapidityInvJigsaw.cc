/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   SetRapidityInvJigsaw.cc
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

#include "RestFrames/SetRapidityInvJigsaw.hh"
#include "RestFrames/RestFrame.hh"

namespace RestFrames {

  SetRapidityInvJigsaw::SetRapidityInvJigsaw(const std::string& sname, 
					     const std::string& stitle) : 
    InvisibleJigsaw(sname, stitle, 1, 1)
  {
    m_Axis = RestFrame::GetAxis();
  }

  SetRapidityInvJigsaw::SetRapidityInvJigsaw() : InvisibleJigsaw() {}
 
  SetRapidityInvJigsaw::~SetRapidityInvJigsaw() {}

  void SetRapidityInvJigsaw::SetAxis(const TVector3& axis){
    m_Axis = axis;
  }

  bool SetRapidityInvJigsaw::AnalyzeEvent(){
    if(!IsSoundMind()) 
      return SetSpirit(false);
    
    TLorentzVector inv_P = GetParentState().GetFourVector();
    TLorentzVector vis_P = m_DependancyStates[0].GetFourVector();

    TVector3 boostZ = vis_P.BoostVector();
    boostZ = boostZ.Dot(m_Axis.Unit())*m_Axis.Unit();

    inv_P.SetZ(0.0);
    inv_P.Boost(boostZ);

    GetChildState(0).SetFourVector(inv_P);
    
    return SetSpirit(true);
  }

}
