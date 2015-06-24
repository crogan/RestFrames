/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   GFrame.cc
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

#include <math.h>
#include <TRandom3.h>
#include <TDatime.h>
#include "RestFrames/GFrame.hh"
#include "RestFrames/FrameLink.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GFrame class methods
  ///////////////////////////////////////////////
  GFrame::GFrame(const string& sname, const string& stitle) 
    : RestFrame(sname, stitle)
  {
    Init();
  }

  GFrame::~GFrame(){
    delete m_Random;
  }

  void GFrame::Init(){
    m_Ana = FGen;
  
    TDatime *now = new TDatime();
    int today = now->GetDate();
    int clock = now->GetTime();
    delete now;
    int seed = today+clock+m_Key;
    m_Random = new TRandom3(seed);
    
  }

  void GFrame::ClearGFrame(){
   
  }

  void GFrame::ClearEventRecursive(){ 
    ResetFrame();
    if(!m_Body) return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i)->ClearEventRecursive();
  }

  bool GFrame::AnalyzeEventRecursive(){
    m_Spirit = false;
    if(!GenerateFrame()) return false;

    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++){
      if(!GetChildFrame(i)->AnalyzeEventRecursive()){
	return false;
      }
    }

    m_Spirit = true;
    return m_Spirit;
  }

  void GFrame::SetChildren(const vector<TLorentzVector>& P_children){
    int N = P_children.size();
    for(int i = 0; i < N; i++){
      TLorentzVector P = P_children[i];
      TVector3 B_child = P.BoostVector();

      m_ChildLinks[i]->SetBoostVector(B_child);
      dynamic_cast<GFrame*>(GetChildFrame(i))->SetFourVector(P,this);
    }
  }

  double GFrame::GetRandom(){
    return m_Random->Rndm();
  }

}
