/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleMassJigsaw.hh
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

#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleState.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  //InvisibleMassJigsaw class methods
  ///////////////////////////////////////////////
  InvisibleMassJigsaw::InvisibleMassJigsaw(const string& sname, const string& stitle) : 
    InvisibleJigsaw(sname, stitle, 1, 0)
  {
    Init();
  }
 
  InvisibleMassJigsaw::~InvisibleMassJigsaw(){
  
  }

  void InvisibleMassJigsaw::Init(){
    m_Priority = -2;
  }

  bool InvisibleMassJigsaw::AnalyzeEvent(){
    if(!IsSoundMind() || !m_GroupPtr){
      m_Log << LogWarning;
      m_Log << "Unable to analyze event. ";
      m_Log << "Requires successfull call to \"InitializeAnalysis()\" ";
      m_Log << "from LabFrame" << m_End;
      SetSpirit(false);
      return false;
    }

    TLorentzVector inv_P = m_InputStatePtr->GetFourVector();
    double M = dynamic_cast<InvisibleState*>(m_OutputStatesPtr->Get(0))->GetMinimumMass();

    inv_P.SetVectM(inv_P.Vect(),M);
    m_OutputStatesPtr->Get(0)->SetFourVector(inv_P);

    SetSpirit(true);
    return true;
  }

}
