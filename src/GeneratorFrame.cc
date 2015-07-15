/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   GeneratorFrame.cc
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
#include "RestFrames/GeneratorFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GeneratorFrame class methods
  ///////////////////////////////////////////////
  GeneratorFrame::GeneratorFrame(const string& sname, const string& stitle) 
    : RestFrame(sname, stitle)
  {
    Init();
  }

  GeneratorFrame::GeneratorFrame() 
    : RestFrame()
  {
    Init();
  }

  GeneratorFrame::~GeneratorFrame(){
    delete m_Random;
  }

  void GeneratorFrame::Init(){
    m_Ana = FGen;
  
    TDatime *now = new TDatime();
    int today = now->GetDate();
    int clock = now->GetTime();
    delete now;
    int seed = today+clock+m_Key.GetKey();
    m_Random = new TRandom3(seed);
    
  }

  void GeneratorFrame::Clear(){
    RestFrame::Clear();
  }

  void GeneratorFrame::ClearEventRecursive(){ 
    ResetFrame();
    if(!IsSoundMind())
      return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i).ClearEventRecursive();
    SetSpirit(false);
  }

  bool GeneratorFrame::AnalyzeEventRecursive(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      SetSpirit(false);
      return false;
    }
    if(!GenerateFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to generate event for this frame.";
      m_Log << m_End;
      SetSpirit(false);
      return false;
    }

    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++){
      if(!GetChildFrame(i).AnalyzeEventRecursive()){
	SetSpirit(false);
	return false;
      }
    }
    SetSpirit(true);
    return m_Spirit;
  }

  void GeneratorFrame::SetChildren(const vector<TLorentzVector>& P_children){
    int N = P_children.size();
    for(int i = 0; i < N; i++){
      TLorentzVector P = P_children[i];
      TVector3 B_child = P.BoostVector();

      SetChildBoostVector(i, B_child);
      dynamic_cast<GeneratorFrame*>(&GetChildFrame(i))->SetFourVector(P,*this);
    }
  }

  bool GeneratorFrame::InitializeGenAnalysis(){
    SetMind(true);
    return true;
  }

  bool GeneratorFrame::InitializeAnalysisRecursive(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      SetMind(false);
      return false;
    }

    if(!InitializeGenAnalysis()){
      SetMind(false);
      return false;
    }

    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      if(!dynamic_cast<GeneratorFrame*>(&GetChildFrame(i))->InitializeAnalysisRecursive()){
	SetMind(false);
	return false;
      }
    }
    SetMind(true);
    return true;
  }

  double GeneratorFrame::GetRandom() const {
    return m_Random->Rndm();
  }

  double GeneratorFrame::GetGaus(double mu, double sig) const {
    return m_Random->Gaus(mu,sig);
  }

}
