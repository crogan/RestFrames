/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleRapidityJigsaw.cc
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

#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //InvisibleRapidityJigsaw class methods
  ///////////////////////////////////////////////
  InvisibleRapidityJigsaw::InvisibleRapidityJigsaw(const string& sname, const string& stitle) : 
    InvisibleJigsaw(sname, stitle, 1, 1)
  {
    Init();
  }
 
  InvisibleRapidityJigsaw::~InvisibleRapidityJigsaw(){
  
  }

  void InvisibleRapidityJigsaw::Init(){}

  bool InvisibleRapidityJigsaw::AnalyzeEvent(){
    m_Spirit = false;
    if(!m_Mind || !m_GroupPtr) return m_Spirit;
    
    TLorentzVector inv_P = m_InputStatePtr->GetFourVector();
    TLorentzVector vis_P = m_DependancyStates[0].GetFourVector();

    // double Minv = inv_P.M();
    // TVector3 Pinv = inv_P.Vect();
    // Pinv.SetZ(0.0);
    // double Pz = vis_P.Pz()*sqrt(Minv*Minv+Pinv.Mag2())/sqrt(vis_P.E()*vis_P.E()-vis_P.Pz()*vis_P.Pz());
    // Pinv.SetXYZ(Pinv.X(),Pinv.Y(),Pz);
    // inv_P.SetVectM(Pinv,Minv);
    // //inv_P.SetVectM(inv_P.Vect(),M);

    inv_P.SetZ(0.0);
    TVector3 boostZ = vis_P.BoostVector();
    boostZ.SetX(0.);
    boostZ.SetY(0.);
    inv_P.Boost(boostZ);

    m_OutputStates.Get(0)->SetFourVector(inv_P);
    
    m_Spirit = true;
    return m_Spirit;
  }

}
