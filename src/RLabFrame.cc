/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RLabFrame.cc
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

#include "RestFrames/RLabFrame.hh"
#include "RestFrames/State.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/VisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RLabFrame class
  ///////////////////////////////////////////////
  RLabFrame::RLabFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle), 
    LabFrame(sname, stitle), 
    RFrame(sname, stitle)
  {
    Init();
  }

  RLabFrame::~RLabFrame(){
    ClearStates();
  }

  void RLabFrame::Init(){
  
  }

  void RLabFrame::Clear(){
    ClearStates();
    RFrame::Clear();
  }

  void RLabFrame::ClearStates(){
    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      delete &m_LabStates.Get(i);
    }
    m_LabStates.Clear();
  }
  
  bool RLabFrame::InitializeLabGroups(){
    m_Log << LogVerbose << "Initializing Groups associated with this tree" << m_End;
    
    m_LabGroups.Clear();
    m_LabGroups.Add(GetListGroups());

    int Ngroup = m_LabGroups.GetN();
    for(int i = 0; i < Ngroup; i++){
      if(!m_LabGroups.Get(i).InitializeAnalysis()){
	m_Log << LogWarning;
	m_Log << "Unable to initialize analysis for Group ";
	m_Log << Log(m_LabGroups.Get(i)) << m_End;
	return false;
      }
    }
    return true;
  }

  bool RLabFrame::InitializeLabStates(){
    m_Log << LogVerbose << "Initializing States associated with this tree" << m_End;
    
    ClearStates();
    
    // create states for visible frames not assigned to a Group
    RFList<RestFrame> frames = GetListFrames(FVisible);
    int Nf = frames.GetN();
    int Ng = m_LabGroups.GetN();
    for(int f = 0; f < Nf; f++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups.Get(g).ContainsFrame(frames.Get(f))){
	  has_group = true;
	  break;
	}
      }
      if(!has_group) {
	State* statePtr = new State();
	statePtr->AddFrame(frames.Get(f));
	m_LabStates.Add(*statePtr);
      }
    }

    // check that all invisible frames are assigned to a Group
    frames = GetListFrames(FInvisible);
    Nf = frames.GetN();
    for(int i = 0; i < Nf; i++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups.Get(g).ContainsFrame(frames.Get(i))){
	  has_group = true;
	  break;
	}
      }
      if(!has_group){
	m_Log << LogWarning;
	m_Log << "Found InvisibleFrame without an assigned group: " << endl;
	m_Log << Log(frames.Get(i)) << m_End;
	return false;
      }
    }
    return true;
  }

  bool RLabFrame::InitializeLabJigsaws(){
    m_Log << LogVerbose << "Initializing Jigsaws associated with this tree" << m_End;
    
    m_LabJigsaws.Clear();
    
    int Ng = m_LabGroups.GetN();
    
    // Initialize Dependancy States in Jigsaws
    for(int g = 0; g < Ng; g++){
      RFList<Jigsaw> jigsaws = m_LabGroups.Get(g).GetJigsaws();
      int Nj = jigsaws.GetN();
      for(int j = 0; j < Nj; j++)
	if(!jigsaws.Get(j).InitializeDependancyStates(m_LabStates, m_LabGroups)){
	  m_Log << LogWarning;
	  m_Log << "Unable to initialize dependancy states for Jigsaw:" << endl;
	  m_Log << Log(jigsaws.Get(j)) << m_End;
	  return false;
	}
    }
    // Initialize Dependancy Jigsaw lists inside jigsaws
    for(int g = 0; g < Ng; g++){
      RFList<Jigsaw> jigsaws = m_LabGroups.Get(g).GetJigsaws();
      int Nj = jigsaws.GetN();
      for(int j = 0; j < Nj; j++){
	Jigsaw& jigsaw = jigsaws.Get(j);
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
    for(int i = 0; i < Nj; i++){
      if(!m_LabJigsaws.Get(i).InitializeJigsawExecutionList(chain_jigsaws)){
	m_LabJigsaws.Clear();
	m_Log << LogWarning;
	m_Log << "Unable to initialize Jigsaw execution list in Jigsaw:" << endl;
	m_Log << Log(m_LabJigsaws.Get(i)) << m_End;
	return false;
      }
    }  
    
    m_LabJigsaws.Clear();
    m_LabJigsaws.Add(chain_jigsaws);
    return true;
  }

  bool RLabFrame::InitializeAnalysis(){
    m_Log << LogVerbose << "Initializing this tree for analysis..." << m_End;
   
    if(!IsSoundBody()){
      m_Log << LogWarning;
      m_Log << "Unable to initialize tree for analysis before ";
      m_Log << "successful call of \"InitializeTree()\"" << endl << m_End;
      SetMind(false);
      return false;
    }

    if(!InitializeLabGroups()){
      m_Log << LogWarning << "Unable to intialize Groups" << m_End;
      SetMind(false);
      return false;
    }
    if(!InitializeLabStates()){
      m_Log << LogWarning << "Unable to intialize States" << m_End;
      SetMind(false);
      return false;
    }
    
    if(!InitializeLabJigsaws()){
      m_Log << LogWarning << "Unable to intialize Jigsaws" << m_End;
      SetMind(false);
      return false;
    }
    if(!InitializeStatesRecursive(m_LabStates, m_LabGroups)){
      m_Log << LogWarning << "Unable to recursively initialize tree" << m_End;
      SetMind(false);
      return false;
    }

    m_Log << LogVerbose << "...Done" << m_End;
    SetMind(true);
    return true;
  }

  void RLabFrame::ClearEvent(){
    SetSpirit(false);
    if(!IsSoundBody() || !IsSoundMind()){
      m_Log << LogWarning << "Unable to ClearEvent. ";
      m_Log << "Requires a successfull call to \"InitializeAnalysis()\" ";
      m_Log << "from the LabFrame" << m_End;
      return;
    }
    
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++)
      m_LabGroups.Get(i).ClearEvent();
    
    ClearEventRecursive();
  }

  bool RLabFrame::AnalyzeEvent(){
    if(!IsSoundMind()){
      m_Log << LogWarning << "Unable to analyze event. ";
      m_Log << "Requires a successfull call to \"InitializeAnalysis\" ";
      m_Log << "from the LabFrame" << m_End;
      SetSpirit(false);
      return false;
    }

    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      State& state = m_LabStates.Get(i);
      RFList<RestFrame> frames = state.GetFrames();
      VisibleFrame* vframePtr = dynamic_cast<VisibleFrame*>(&frames.Get(0));
      if(vframePtr) state.SetFourVector(vframePtr->GetLabFrameFourVector());
    }
  
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++){
      if(!m_LabGroups.Get(i).AnalyzeEvent()){
	m_Log << LogWarning;
	m_Log << "Group failed in event analysis: ";
	m_Log << Log(m_LabGroups.Get(i)) << m_End;
	SetSpirit(false);
	return false;
      }
    }

    int Nj = m_LabJigsaws.GetN();
    for(int i = 0; i < Nj; i++){
      if(!m_LabJigsaws.Get(i).AnalyzeEvent()){
	m_Log << LogWarning;
	m_Log << "Jigsaw failed in event analysis: ";
	m_Log << Log(m_LabJigsaws.Get(i)) << m_End;
	SetSpirit(false);
	return false;
      }
    }
    
    if(!AnalyzeEventRecursive()){
      m_Log << LogWarning;
      m_Log << "Recursive event analysis failed" << m_End;
      SetSpirit(false);
      return false;
    }

    SetSpirit(true);
    return true;
  }

}
