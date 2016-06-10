/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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

namespace RestFrames {

  ///////////////////////////////////////////////
  // GeneratorFrame class methods
  ///////////////////////////////////////////////
  GeneratorFrame::GeneratorFrame(const std::string& sname, 
				 const std::string& stitle) 
    : RestFrame(sname, stitle)
  {
    m_Ana = kGenFrame;
    m_VarMassMCMC = false;
    m_Mass = 0.;
  
    TDatime now;
    int today = now.GetDate();
    int clock = now.GetTime();
    int key   = GetKey().GetKey();
    int seed = today+clock+key;
    m_Random = new TRandom3(seed);
  }

  GeneratorFrame::GeneratorFrame() 
    : RestFrame() { }

  GeneratorFrame::~GeneratorFrame(){
    if(m_Random) delete m_Random;
  }

  void GeneratorFrame::Clear(){
    RestFrame::Clear();
  }

  /// \brief Returns empty instance of class
  GeneratorFrame& GeneratorFrame::Empty(){
    return VisibleGenFrame::Empty();
  }

  double GeneratorFrame::GetMass() const {
    return std::max(m_Mass, 0.);
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
      m_Log << LogEnd;
      return SetSpirit(false);
    }

    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      if(!GetChildFrame(i).AnalyzeEventRecursive()){
	return SetSpirit(false);
      }

    return SetSpirit(true);
  }

  void GeneratorFrame::SetChildren(const std::vector<TLorentzVector>& P_children){
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
      if(!GetChildFrame(i).InitializeAnalysisRecursive())
	return SetMind(false);

    return SetMind(true);
  }

  double GeneratorFrame::GetRandom() const {
    return m_Random->Rndm();
  }

  double GeneratorFrame::GetGaus(double mu, double sig) const {
    return m_Random->Gaus(mu,sig);
  }

  bool GeneratorFrame::IterateMCMC(){
    return true;
  }

  bool GeneratorFrame::IterateRecursiveMCMC(){
     if(!IsSoundMind()){
       UnSoundMind(RF_FUNCTION);
       return SetMind(false);
     }
    
     if(!IterateMCMC())
       return SetMind(false);

     int N = GetNChildren();
     for(int i = 0; i < N; i++)
       if(!GetChildFrame(i).IterateRecursiveMCMC())
	 return SetMind(false);

     return SetMind(true);
  }

  void GeneratorFrame::SetVariableMassMCMC(bool var){ 
    m_VarMassMCMC = var; 
  }

  bool GeneratorFrame::IsVariableMassMCMC() const { 
    return m_VarMassMCMC; 
  }

  double GeneratorFrame::GetMinimumMassMCMC() const {
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetBody(false);
    }
    
    double mass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++)
      mass += GetChildFrame(i).GetMinimumMassMCMC();

    if(!IsVariableMassMCMC())
      mass = std::max(GetMass(),mass);

    return mass;
  }

  void GeneratorFrame::GenerateMassMCMC(double& mass, double& prob, 
					double max) const {
    mass = 0.;
    prob = 1.;
  }

  void GeneratorFrame::SetMassMCMC(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set mass to negative value ";
      m_Log << val << ". Setting to zero." << LogEnd;
      m_Mass = 0.;
    } else {
      m_Mass = val;
    }
  }

  void GeneratorFrame::SetMassMCMC(double mass, 
				   GeneratorFrame& frame) const {
    frame.SetMassMCMC(mass);
  }

  double GeneratorFrame::GetProbMCMC(double mass) const {
    return 1.;
  }

}
