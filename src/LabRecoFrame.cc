/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   LabRecoFrame.cc
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

#include "RestFrames/LabRecoFrame.hh"
#include "RestFrames/VisibleState.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/VisibleRecoFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabRecoFrame class
  ///////////////////////////////////////////////
  LabRecoFrame::LabRecoFrame(const string& sname, const string& stitle)
    : LabFrame<ReconstructionFrame>(sname, stitle)
  {
    Init();
  }

  LabRecoFrame::LabRecoFrame() : LabFrame<ReconstructionFrame>() {}

  LabRecoFrame::~LabRecoFrame(){
    ClearStates();
  }

  void LabRecoFrame::Init() {}

  void LabRecoFrame::Clear(){
    ClearStates();
    ReconstructionFrame::Clear();
  }

  void LabRecoFrame::ClearStates(){
    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      delete &m_LabStates.Get(i);
    }
    m_LabStates.Clear();
  }
  
  bool LabRecoFrame::InitializeLabGroups(){
     m_Log << LogVerbose << "Initializing Groups for analysis..." << m_End;
    
    m_LabGroups.Clear();
    m_LabGroups.Add(GetListGroups());

    int Ngroup = m_LabGroups.GetN();
    for(int i = 0; i < Ngroup; i++){
      if(!m_LabGroups[i].InitializeAnalysis()){
	m_Log << LogWarning;
	m_Log << "Unable to initialize analysis for Group ";
	m_Log << Log(m_LabGroups.Get(i)) << m_End;
	return false;
      }
    }
    m_Log << LogVerbose << "...Done" << m_End;
    return true;
  }

  void LabRecoFrame::AddVisibleState(VisibleState& state) const {
    m_LabStates += state;
  }

  void LabRecoFrame::AddVisibleStates(const RFList<VisibleState>& states) const {
    m_LabStates += states;
  }

  void LabRecoFrame::RemoveVisibleState(const VisibleState& state) const {
    m_LabStates -= state;
  }
  
  void LabRecoFrame::RemoveVisibleStates(const RFList<VisibleState>& states) const {
    m_LabStates -= states;
  }

  RFList<VisibleState> const& LabRecoFrame::GetVisibleStates() const {
    return m_LabStates;
  }

  bool LabRecoFrame::InitializeLabStates(){
    m_Log << LogVerbose << "Initializing States for analysis..." << m_End;
    
    ClearStates();
    
    // create states for visible frames not assigned to a Group
    RFList<ReconstructionFrame> frames = GetListVisibleFrames();
    int Nf = frames.GetN();
    int Ng = m_LabGroups.GetN();
    for(int f = 0; f < Nf; f++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups[g].ContainsFrame(frames[f])){
	  has_group = true;
	  break;
	}
      }
      if(!has_group) {
	VisibleState* statePtr = new VisibleState();
	statePtr->AddFrame(frames[f]);
	m_LabStates.Add(*statePtr);
      }
    }

    // check that all invisible frames are assigned to a Group
    frames = GetListInvisibleFrames();
    Nf = frames.GetN();
    for(int f = 0; f < Nf; f++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups[g].ContainsFrame(frames[f])){
	  has_group = true;
	  break;
	}
      }
      if(!has_group){
	m_Log << LogWarning;
	m_Log << "Found InvisibleRecoFrame without an assigned group: " << endl;
	m_Log << Log(frames[f]) << m_End;
	return false;
      }
    }
    m_Log << LogVerbose << "...Done initializing States" << m_End;
    return true;
  }

  bool LabRecoFrame::InitializeLabJigsaws(){
    m_Log << LogVerbose << "Initializing Jigsaws for analysis..." << m_End;
    
    m_LabJigsaws.Clear();
    
    int Ng = m_LabGroups.GetN();
    
    // Initialize Dependancy States in Jigsaws
    for(int g = 0; g < Ng; g++){
      RFList<Jigsaw> jigsaws = m_LabGroups[g].GetListJigsaws();
      int Nj = jigsaws.GetN();
      for(int j = 0; j < Nj; j++)
	if(!jigsaws[j].InitializeDependancyStates(m_LabStates, m_LabGroups)){
	  m_Log << LogWarning;
	  m_Log << "Unable to initialize dependancy states for Jigsaw:" << endl;
	  m_Log << Log(jigsaws[j]) << m_End;
	  return false;
	}
    }
    // Initialize Dependancy Jigsaw lists inside jigsaws
    for(int g = 0; g < Ng; g++){
      RFList<Jigsaw> jigsaws = m_LabGroups[g].GetListJigsaws();
      int Nj = jigsaws.GetN();
      for(int j = 0; j < Nj; j++){
	Jigsaw& jigsaw = jigsaws[j];
	if(!jigsaw.InitializeDependancyJigsaws()){
	  m_Log << LogWarning;
	  m_Log << "Unable to initialize dependancy Jigsaw list for Jigsaw:" << endl;
	  m_Log << Log(jigsaw) << m_End;
	  return false;
	}
	m_LabJigsaws.Add(jigsaw);
      }
    }
    // Initialize Jigsaw execution list
    RFList<Jigsaw> chain_jigsaws;
    int Nj = m_LabJigsaws.GetN();
    for(int j = 0; j < Nj; j++){
      if(!m_LabJigsaws[j].InitializeJigsawExecutionList(chain_jigsaws)){
	m_LabJigsaws.Clear();
	m_Log << LogWarning;
	m_Log << "Unable to initialize Jigsaw execution list in Jigsaw:" << endl;
	m_Log << Log(m_LabJigsaws[j]) << m_End;
	return false;
      }
    }  
    
    m_LabJigsaws.Clear();
    m_LabJigsaws.Add(chain_jigsaws);

    m_Log << LogVerbose << "...Done initializing Jigsaws" << m_End;
    return true;
  }

  bool LabRecoFrame::InitializeAnalysis(){
    m_Log << LogVerbose << "Initializing this tree for analysis..." << m_End;
   
    if(!IsSoundBody()){
      UnSoundBody(RF_FUNCTION);
      return SetMind(false);
    }

    if(!InitializeLabGroups()){
      m_Log << LogWarning << "Unable to intialize Groups" << m_End;
      return SetMind(false);
    }
    if(!InitializeLabStates()){
      m_Log << LogWarning << "Unable to intialize States" << m_End;
      return SetMind(false);
    }
    
    if(!InitializeLabJigsaws()){
      m_Log << LogWarning << "Unable to intialize Jigsaws" << m_End;
      return SetMind(false);
    }
    if(!InitializeAnalysisRecursive()){
      m_Log << LogWarning;
      m_Log << "Unable to recursively initialize tree for analysis";
      m_Log << m_End;
      return SetMind(false);
    }

    m_Log << LogVerbose << "...Done initializing tree for analysis" << m_End;
    SetMind(true);
    return SetMind(true);
  }

  bool LabRecoFrame::ClearEvent(){
    SetSpirit(false);
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetMind(false);
    }
    
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++)
      if(!m_LabGroups[i].ClearEvent())
	return SetMind(false);
    
    if(!ClearEventRecursive())
      return SetMind(false);

    return SetMind(true);
  }

  bool LabRecoFrame::AnalyzeEvent(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++)
      m_LabStates[i].SetLabFrameFourVector();
      
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++){
      if(!m_LabGroups[i].AnalyzeEvent()){
	m_Log << LogWarning;
	m_Log << "Unable to analyze event for Group: ";
	m_Log << Log(m_LabGroups[i]) << m_End;
	return SetSpirit(false);
      }
    }
    int Nj = m_LabJigsaws.GetN();
    for(int i = 0; i < Nj; i++){
      if(!m_LabJigsaws[i].AnalyzeEvent()){
	m_Log << LogWarning;
	m_Log << "Unable to analyze event for Jigsaw: ";
	m_Log << Log(m_LabJigsaws[i]) << m_End;
	return SetSpirit(false);
      }
    }
    if(!AnalyzeEventRecursive()){
      m_Log << LogWarning;
      m_Log << "Unable to recursively analyze event" << m_End;
      return SetSpirit(false);
    }
    return SetSpirit(true);
  }

}
