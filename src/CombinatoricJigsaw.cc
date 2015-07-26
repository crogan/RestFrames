/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricJigsaw.cc
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

#include "RestFrames/CombinatoricJigsaw.hh"
#include "RestFrames/MinMassesCombJigsaw.hh"
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  CombinatoricJigsaw::CombinatoricJigsaw(const string& sname,const string& stitle)
    : Jigsaw(sname, stitle)
  {
    Init();
  }

  CombinatoricJigsaw::CombinatoricJigsaw() : Jigsaw() {}

  CombinatoricJigsaw::~CombinatoricJigsaw() {}

  void CombinatoricJigsaw::Init(){
    m_Type = kCombinatoricJigsaw;
  }

  void CombinatoricJigsaw::Clear(){
    Jigsaw::Clear();
  }

  CombinatoricJigsaw& CombinatoricJigsaw::Empty(){
    return MinMassesCombJigsaw::Empty();
  }

  void CombinatoricJigsaw::SetGroup(Group& group){
    if(!group) return;
    if(!group.IsCombinatoricGroup()) return;
    Jigsaw::SetGroup(group);
  }

  CombinatoricGroup& CombinatoricJigsaw::GetGroup() const {
    if(!Jigsaw::GetGroup())
      return CombinatoricGroup::Empty();
    else
      return static_cast<CombinatoricGroup&>(Jigsaw::GetGroup());
  }

  void CombinatoricJigsaw::SetParentState(State& state){
    if(!state) return;
    if(!state.IsCombinatoricState()) return;
    Jigsaw::SetParentState(state);
  }

  CombinatoricState& CombinatoricJigsaw::GetParentState() const {
    if(!Jigsaw::GetParentState())
      return CombinatoricState::Empty();
    else
      return static_cast<CombinatoricState&>(Jigsaw::GetParentState());
  }

  CombinatoricState& CombinatoricJigsaw::GetChildState(int i) const{
    if(!Jigsaw::GetChildState(i)) 
      return CombinatoricState::Empty();
    else
      return static_cast<CombinatoricState&>(Jigsaw::GetChildState(i));
  }

  void CombinatoricJigsaw::AddFrame(RestFrame& frame, int i){
    if(!frame) return;
    if(!GetGroup()) return;
    
    RFList<RestFrame> frames = 
      frame.GetListVisibleFrames()+frame.GetListInvisibleFrames();
    int N = frames.GetN();
    for(int f = 0; f < N; f++){
      if(GetGroup().ContainsFrame(frames[f]))
	AddChildFrame(frames[f], i);
      else 
	AddDependancyFrame(frames[f], i);
    }
  }

  void CombinatoricJigsaw::AddFrames(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      AddFrame(frames[f],i);
  }

  bool CombinatoricJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>& exec_jigsaws){
    if(!IsSoundMind()) return false;
    if(exec_jigsaws.Contains(*this)) return true;

    m_ExecuteJigsaws.Clear();

    // Add group dependancy jigsaws first
    RFList<Jigsaw> group_jigsaws; 
    FillGroupJigsawDependancies(group_jigsaws);
    group_jigsaws -= *this;

    int Ngroup = group_jigsaws.GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw& jigsaw = group_jigsaws[i];
      m_DependancyJigsaws -= jigsaw;
      if(!exec_jigsaws.Contains(jigsaw))
	if(!jigsaw.InitializeJigsawExecutionList(exec_jigsaws))
	  return SetMind(false);
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsaws.GetN() > 0){
      Jigsaw& jigsaw = m_DependancyJigsaws[m_DependancyJigsaws.GetN()-1];
      if(exec_jigsaws.Contains(jigsaw)){
	m_DependancyJigsaws -= jigsaw;
	continue;
      }
      if(!jigsaw.DependsOnJigsaw(*this)){
	if(!jigsaw.InitializeJigsawExecutionList(exec_jigsaws)){
	  return SetMind(false);
	}
	m_DependancyJigsaws -= jigsaw;
	continue;
      }
      RFList<Jigsaw> temp_exec_jigsaws = exec_jigsaws;
      temp_exec_jigsaws += m_ExecuteJigsaws;
      temp_exec_jigsaws += *this;
      if(!jigsaw.InitializeJigsawExecutionList(temp_exec_jigsaws))
	return SetMind(false);
      temp_exec_jigsaws -= *this;
      temp_exec_jigsaws -= exec_jigsaws;

      m_DependancyJigsaws -= temp_exec_jigsaws;
      m_ExecuteJigsaws    += temp_exec_jigsaws;
    }

    exec_jigsaws += *this;
    exec_jigsaws += m_ExecuteJigsaws;
    return true;
  }

  CombinatoricState& CombinatoricJigsaw::GetNewChildState(){
    string name = GetName()+"_"+std::to_string(GetNChildren());
    CombinatoricState* statePtr = new CombinatoricState(name, name);
    AddDependent(statePtr);
    return *statePtr;
  }

  bool CombinatoricJigsaw::ExecuteDependancyJigsaws(){
    int N = m_ExecuteJigsaws.GetN();
    for(int i = 0; i < N; i++)
      if(!m_ExecuteJigsaws[i].AnalyzeEvent()) 
	return false;
    return true;
  }

  bool CombinatoricJigsaw::AnalyzeEvent(){
    if(!IsSoundMind() || !GetGroup())
      return SetSpirit(false);

    if(!InitializeCombinatoric()){
      m_Log << LogWarning;
      m_Log << "Problem initializing event info" << m_End;
      return SetSpirit(false);
    }

    if(!LoopCombinatoric()){
      m_Log << LogWarning;
      m_Log << "Problem looping over combinatorics" << m_End;
      return SetSpirit(false);
    }

    return SetSpirit(true);
  }

  bool CombinatoricJigsaw::InitializeCombinatoric(){
    if(!IsSoundMind()) 
      return SetSpirit(false);

    if(!GetGroup())
      return SetSpirit(false);

    if(!GetParentState())
      return SetSpirit(false);

    m_InputStates.Clear();
    m_InputStates = GetParentState().GetElements();

    m_NForChild.clear();
    m_NExclusive.clear();

    int NTOTOT = 0;
    bool exclTOTOT = true;
    int Nc = GetNChildren();
    for(int i = 0; i < Nc; i++){
      RFList<RestFrame> frames = GetChildState(i).GetListFrames();
      int Nf = frames.GetN();
      int NTOT = 0;
      bool exclTOT = true;
      for(int f = 0; f < Nf; f++){
	int N = -1;
	bool excl = false;
	GetGroup().GetNElementsForFrame(frames[f], N, excl);
	if(N < 0) return SetSpirit(false);
	NTOT += N;
	exclTOT = exclTOT && excl;
      }
      m_NForChild[&GetChildState(i)] = NTOT;
      m_NExclusive[&GetChildState(i)] = exclTOT;
      NTOTOT += NTOT;
      exclTOTOT = exclTOTOT && exclTOT;
    }

    if(m_InputStates.GetN() < NTOTOT){
      m_Log << LogWarning;
      m_Log << "Unable to execute Jigsaw. ";
      m_Log << "Insufficienct number of inputs.";
      m_Log << m_End;
      return SetSpirit(false);
    }

    if(exclTOTOT &&
       (m_InputStates.GetN() != NTOTOT)){
      m_Log << LogWarning;
      m_Log << "Unable to execute Jigsaw. ";
      m_Log << "Incorrect number of exclusive inputs: ";
      m_Log << NTOTOT << " (required) != ";
      m_Log << m_InputStates.GetN() << " (provided)";
      m_Log << m_End;
      return SetSpirit(false);
    }

    return SetSpirit(true);
  }

  bool CombinatoricJigsaw::LoopCombinatoric(){
    int Ninput = m_InputStates.GetN();
    int Nout   = GetNChildren();	

    int N_comb = 1;
    for(int i = 0; i < Ninput; i++) N_comb *= Nout;
   
    vector<int> Nhem;
    for(int i = 0; i < Nout; i++)
      Nhem.push_back(0);
    
    int c_max = -1;
    double metric_max = -1; 
   
    for(int c = 0; c < N_comb; c++){
      int key = c;
      for(int i = 0; i < Nout; i++){
	Nhem[i] = 0;
	GetChildState(i).ClearElements();
      } 

      // set output states for combinatoric;
      for(int i = 0; i < Ninput; i++){
	int ihem = key%Nout;
	key /= Nout;
	Nhem[ihem]++;
	GetChildState(ihem).AddElement(m_InputStates[i]);
      }
      
      // check validity of combinatoric
      bool valid = true;
      for(int i = 0; i < Nout; i++){
	if(m_NExclusive[&GetChildState(i)]){
	  if(Nhem[i] != m_NForChild[&GetChildState(i)]) 
	    valid = false;
	} else {
	  if(Nhem[i] < m_NForChild[&GetChildState(i)]) 
	    valid = false;
	}
      }
      if(!valid)
	continue;

      // Execute depedancy Jigsawsfor this combintoric
      ExecuteDependancyJigsaws();

      // Evaluate metric for this combinatoric
      double metric = EvaluateMetric();
     
      if(metric >= metric_max){
	metric_max = metric;
	c_max = c;
      }
    }
   
    if(c_max < 0){
      m_Log << LogWarning;
      m_Log << "Unable to find combinatoric with positive metric";
      m_Log << m_End;
      return SetSpirit(false);
    }
     
    // Set outputs to best combinatoric
    for(int i = 0; i < 2; i++) 
      GetChildState(i).ClearElements();
    int key = c_max;
    for(int i = 0; i < Ninput; i++){
      int ihem = key%Nout;
      key /= Nout;
      GetChildState(ihem).AddElement(m_InputStates[i]);
    }
  
    // Execute depedancy Jigsaws
    ExecuteDependancyJigsaws();

    return SetSpirit(true);
  }

}
