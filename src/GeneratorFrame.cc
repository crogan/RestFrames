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
#include "RestFrames/VisibleGenFrame.hh"

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
    : RestFrame() { }

  GeneratorFrame::~GeneratorFrame(){
    if(m_Random) delete m_Random;
  }

  void GeneratorFrame::Init(){
    m_Ana = kGenFrame;
  
    TDatime now;
    int today = now.GetDate();
    int clock = now.GetTime();
    int key   = GetKey().GetKey();
    int seed = today+clock+key;
    m_Random = new TRandom3(seed);
  }

  void GeneratorFrame::Clear(){
    RestFrame::Clear();
  }

  /// \brief Returns empty instance of class
  GeneratorFrame& GeneratorFrame::Empty(){
    return VisibleGenFrame::Empty();
  }

  
  void GeneratorFrame::AddChildFrame(RestFrame& frame){
    if(!frame.IsEmpty())
      if(!dynamic_cast<GeneratorFrame*>(&frame))
	return;
    RestFrame::AddChildFrame(frame);
  }

  void GeneratorFrame::SetParentFrame(RestFrame& frame){
    if(!frame.IsEmpty())
      if(!dynamic_cast<GeneratorFrame*>(&frame))
	return;
    RestFrame::SetParentFrame(frame);
  }

  GeneratorFrame const& GeneratorFrame::GetParentFrame() const {
    const RestFrame& frame = RestFrame::GetParentFrame();
    if(!frame.IsEmpty())
      return static_cast<const GeneratorFrame&>(frame);
    else 
      return GeneratorFrame::Empty();
  }

  GeneratorFrame& GeneratorFrame::GetChildFrame(int i) const {
    RestFrame& frame = RestFrame::GetChildFrame(i);
    if(!frame.IsEmpty())
      return static_cast<GeneratorFrame&>(frame);
    else 
      return GeneratorFrame::Empty();
  }

  bool GeneratorFrame::ClearEventRecursive(){ 
    ResetFrame();
    if(!IsSoundMind())
      return false;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      if(!GetChildFrame(i).ClearEventRecursive())
	return false;
    return true;
  }

  bool GeneratorFrame::AnalyzeEventRecursive(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    if(!GenerateFrame()){
      m_Log << LogWarning;
      m_Log << "Unable to generate event for this frame.";
      m_Log << m_End;
      return SetSpirit(false);
    }

    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      if(!GetChildFrame(i).AnalyzeEventRecursive())
	return SetSpirit(false);
   
    return SetSpirit(true);
  }

  void GeneratorFrame::SetChildren(const vector<TLorentzVector>& P_children){
    int N = P_children.size();
    for(int i = 0; i < N; i++){
      TLorentzVector P = P_children[i];
      TVector3 B_child = P.BoostVector();

      SetChildBoostVector(GetChildFrame(i), B_child);
      GetChildFrame(i).SetFourVector(P,*this);
    }
  }

  bool GeneratorFrame::InitializeGenAnalysis(){
    return SetMind(true);
  }

  bool GeneratorFrame::InitializeAnalysisRecursive(){
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    }

    if(!InitializeGenAnalysis())
      return SetMind(false);

    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      if(!dynamic_cast<GeneratorFrame*>(&GetChildFrame(i))->InitializeAnalysisRecursive())
	return SetMind(false);
 
    return SetMind(true);
  }

  double GeneratorFrame::GetRandom() const {
    return m_Random->Rndm();
  }

  double GeneratorFrame::GetGaus(double mu, double sig) const {
    return m_Random->Gaus(mu,sig);
  }

}
