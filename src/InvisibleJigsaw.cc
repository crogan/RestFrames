/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleJigsaw.cc
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

#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/SetMassInvJigsaw.hh"
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleJigsaw class
  ///////////////////////////////////////////////

  InvisibleJigsaw::InvisibleJigsaw(const string& sname, const string& stitle, int Ninv, int Nvis) : 
    Jigsaw(sname, stitle)
  {
    Init(Ninv, Nvis);
  }

  InvisibleJigsaw::InvisibleJigsaw() : Jigsaw() {}

  InvisibleJigsaw::~InvisibleJigsaw() {}

  void InvisibleJigsaw::Clear(){
    Jigsaw::Clear();
  }

  void InvisibleJigsaw::Init(int Ninv, int Nvis){
    m_Type = kInvisibleJigsaw;
    m_Ninv = Ninv;
    m_Nvis = Nvis;
  }

  InvisibleJigsaw& InvisibleJigsaw::Empty(){
    return SetMassInvJigsaw::Empty();
  }

  void InvisibleJigsaw::SetGroup(Group& group){
    if(!group) return;
    if(!group.IsInvisibleGroup()) return;
    Jigsaw::SetGroup(group);
    if(m_Ninv == 1)
      AddInvisibleFrames(group.GetListFrames());
  }

  InvisibleGroup& InvisibleJigsaw::GetGroup() const {
    if(!Jigsaw::GetGroup())
      return InvisibleGroup::Empty();
    else
      return static_cast<InvisibleGroup&>(Jigsaw::GetGroup());
  }

  void InvisibleJigsaw::SetParentState(State& state){
    if(!state) return;
    if(!state.IsInvisibleState()) return;
    Jigsaw::SetParentState(state);
  }

  InvisibleState& InvisibleJigsaw::GetParentState() const {
    if(!Jigsaw::GetParentState())
      return InvisibleState::Empty();
    else
      return static_cast<InvisibleState&>(Jigsaw::GetParentState());
  }

  InvisibleState& InvisibleJigsaw::GetChildState(int i) const{
    if(!Jigsaw::GetChildState(i)) 
      return InvisibleState::Empty();
    else
      return static_cast<InvisibleState&>(Jigsaw::GetChildState(i));
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame& frame, int i){
    if(!frame) return;
    RFList<RestFrame> frames = frame.GetListVisibleFrames();
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      if(frames[f].IsRecoFrame())
	AddDependancyFrame(frames[f], i);
  }

  void InvisibleJigsaw::AddVisibleFrames(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      AddVisibleFrame(frames[f], i);
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame& frame, int i){
    if(!GetGroup()) return;
    RFList<RestFrame> frames = frame.GetListInvisibleFrames();
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      if(GetGroup().ContainsFrame(frames[f]))
	AddChildFrame(frames[f], i);
  }

  void InvisibleJigsaw::AddInvisibleFrames(const RFList<RestFrame>& frames, int i){
    int N = frames.GetN();
    for(int f = 0; f < N; f++)
      AddInvisibleFrame(frames[f], i);
  }

  InvisibleState& InvisibleJigsaw::GetNewChildState(){
    string name = GetName()+"_"+std::to_string(m_ChildStates.GetN());
    InvisibleState* statePtr = new InvisibleState(name, name);
    AddDependent(statePtr);
    m_ChildStates += *statePtr;
    return *statePtr;
  }

  double InvisibleJigsaw::GetMinimumMass() const {
    if(!IsSoundMind()) return 0.;
    int N = GetNChildren();
    double M = 0.;
    for(int i = 0; i < N; i++)
      M += GetChildState(i).GetMinimumMass();
    return M;
  }

  void InvisibleJigsaw::FillInvisibleMassJigsawDependancies(RFList<Jigsaw>& jigsaws) const{ 
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++)
      GetChildState(i).FillInvisibleMassJigsawDependancies(jigsaws);
  }

  bool InvisibleJigsaw::InitializeDependancyJigsaws(){
    if(!IsSoundMind()) return false;
    
    m_DependancyJigsaws.Clear();

    RFList<Jigsaw> jigsaws; 
    FillStateJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillInvisibleMassJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    jigsaws.Clear();
    FillGroupJigsawDependancies(jigsaws);
    jigsaws.Remove(*this);
    m_DependancyJigsaws.Add(jigsaws);

    return true;
  }

  bool InvisibleJigsaw::InitializeJigsawExecutionList(RFList<Jigsaw>& exec_jigsaws){
    if(!IsSoundMind()) return false;
    if(exec_jigsaws.Contains(*this)) return true;

    // Add group dependancy jigsaws first
    RFList<Jigsaw> group_jigsaws;
    FillGroupJigsawDependancies(group_jigsaws);
    group_jigsaws -= *this;

    int Ngroup = group_jigsaws.GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw& jigsaw = group_jigsaws[i];
      if(!exec_jigsaws.Contains(jigsaw))
	if(!jigsaw.InitializeJigsawExecutionList(exec_jigsaws))
	  return SetMind(false);
      m_DependancyJigsaws -= jigsaw;
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsaws.GetN() > 0){
      Jigsaw& jigsaw = m_DependancyJigsaws[m_DependancyJigsaws.GetN()-1];
      if(exec_jigsaws.Contains(jigsaw)){
	m_DependancyJigsaws -= jigsaw;
	continue;
      }
      if(jigsaw.DependsOnJigsaw(*this)) 
	m_DependancyJigsaws -= jigsaw;
      if(!jigsaw.InitializeJigsawExecutionList(exec_jigsaws))
	return SetMind(false);
      m_DependancyJigsaws -= jigsaw;
    }

    exec_jigsaws += *this;
    return true;
  }

  bool InvisibleJigsaw::IsSoundBody() const {
    if(RFBase::IsSoundBody())
      return true;

    if(!Jigsaw::IsSoundBody())
      return SetBody(false);
    
    int Ndep = m_DependancyFrames.size();
    int Nout = m_ChildFrames.size();

    if(Ndep != m_Nvis || Nout != m_Ninv){
      m_Log << LogWarning;
      m_Log << "Incorrect number of input/output frames for jigsaw";
      m_Log<< m_End;
      return SetBody(false);
    }
    for(int i = 0; i < Ndep; i++)
      if(m_DependancyFrames[i].GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of visible frames: " << i;
	m_Log << m_End;
	return SetBody(false);
      }
    for(int i = 0; i < Nout; i++)
      if(m_ChildFrames[i].GetN() == 0){
	m_Log << LogWarning;
	m_Log << "Empty collection of invisible frames: " << i;
	m_Log << m_End;
	return SetBody(false);
      }
    return SetBody(true);
  }

}
