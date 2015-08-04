/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   SelfAssemblingRecoFrame.cc
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

#include "RestFrames/SelfAssemblingRecoFrame.hh"
#include "RestFrames/VisibleRecoFrame.hh"
#include "RestFrames/LabRecoFrame.hh"
#include "RestFrames/VisibleState.hh"
#include "RestFrames/CombinatoricState.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // SelfAssemblingRecoFrame class
  ///////////////////////////////////////////////
  SelfAssemblingRecoFrame::SelfAssemblingRecoFrame(const string& sname, const string& stitle)
    : DecayRecoFrame(sname,stitle)
  {
    m_RType = RDSelfAssembling;
    m_IsAssembled = false;
    m_IsBackedUp = false;
    m_Body_UnAssembled = false;
    m_Mind_UnAssembled = false;
    m_Nvisible = 0;
    m_Ndecay = 0;
  }
  
  SelfAssemblingRecoFrame::~SelfAssemblingRecoFrame() {}

  void SelfAssemblingRecoFrame::Clear(){
    m_VisibleFrames.Clear();
    m_DecayFrames.Clear();
    ReconstructionFrame::Clear();
  }

  bool SelfAssemblingRecoFrame::ClearEventRecursive(){
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return SetSpirit(false);
    }
    Disassemble();
    return ReconstructionFrame::ClearEventRecursive();
  }

  void SelfAssemblingRecoFrame::Disassemble(){
    if(!m_IsAssembled) return;

    m_Nvisible = 0;
    m_Ndecay = 0;
  
    // replace frames with unassembled ones
    RemoveChildren();
    ClearNewFrames();
    AddChildFrames(m_ChildFrames_UnAssembled);
    
    if(!InitializeTreeRecursive()){
      m_Log << LogWarning;
      m_Log << "Problem with recursive tree after disassembly";
      m_Log << m_End;
      SetBody(false);
      return;
    } else 
      SetBody(true);

    if(!InitializeAnalysisRecursive()){
      m_Log << LogWarning;
      m_Log << "Problem connecting states after disassembly";
      m_Log << m_End;
      SetMind(false);
      return;
    } else 
      SetMind(true);

    SetSpirit(false);
    m_IsAssembled = false;
  }

  void SelfAssemblingRecoFrame::Assemble(){
    if(m_IsAssembled) Disassemble();
    if(!IsSoundMind()){
      UnSoundMind(RF_FUNCTION);
      return;
    }

    // new Visible States
    RFList<VisibleState> states;
    // new Frames associated with States
    vector<RestFrame*> frames;
    // States' four-vector
    vector<TLorentzVector> Ps; 

    // clear unassembled lists
    m_ChildFrames_UnAssembled.Clear();
    m_ChildFrames_UnAssembled += GetChildren();
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      ReconstructionFrame& frame = GetChildFrame(i);
      bool expand_frame = false;
      if(m_ChildStates[&frame].GetN() == 1)
	if(m_ChildStates[&frame][0].IsCombinatoricState()){
	  expand_frame = true;
	  RFList<VisibleState> elements = 
	    static_cast<CombinatoricState&>(m_ChildStates[&frame][0]).GetElements();
	  int Nelement = elements.GetN();
	  for(int e = 0; e < Nelement; e++){
	    VisibleState& element = elements[e];
	    RestFrame& new_frame = GetNewVisibleFrame(frame.GetName(),frame.GetTitle());
	    element.AddFrame(new_frame);
	    frames.push_back(&new_frame);
	    TLorentzVector V = element.GetFourVector();
	    if(V.M() < 0.) V.SetVectM(V.Vect(),0.);
	    Ps.push_back(V);
	    states.Add(element);
	  }
	}
      if(!expand_frame){
	TLorentzVector V = m_ChildStates[&frame].GetFourVector();
	if(V.M() < 0.) V.SetVectM(V.Vect(),0.);
	Ps.push_back(V);
	frames.push_back(&frame);
      }
    }
    RemoveChildren();
    m_ChildStates.clear();
    AssembleRecursive(*this, frames, Ps); 
    
    if(!InitializeTreeRecursive()){
      m_Log << LogWarning;
      m_Log << "Problem with recursive tree after assembly";
      m_Log << m_End;
      SetBody(false);
      return;
    }
    SetMind(true);
    const LabRecoFrame& lab_frame = static_cast<const LabRecoFrame&>(GetLabFrame());
    lab_frame.AddTreeStates(states);
    if(!InitializeAnalysisRecursive()){
      m_Log << LogWarning;
      m_Log << "Problem connecting states after assembly";
      m_Log << m_End;
      SetMind(false);
      return;
    }
    lab_frame.RemoveTreeStates(states);

    m_IsAssembled = true;
  }

  void SelfAssemblingRecoFrame::AssembleRecursive(RestFrame& frame, vector<RestFrame*>& frames, vector<TLorentzVector>& Ps){

    int Ninput = frames.size();
    if(Ninput <= 1){
      for(int i = 0; i < Ninput; i++) frame.AddChildFrame(*frames[i]);
      return;
    }

    TLorentzVector TOT(0.,0.,0.,0.);
    for(int i = 0; i < Ninput; i++) TOT += Ps[i];
    TVector3 boost = TOT.BoostVector();
    for(int i = 0; i < Ninput; i++){
      Ps[i].Boost(-boost);
    }

    int ip_max[2];
    int jp_max[2];
    for(int i = 0; i < 2; i++) ip_max[i] = -1;
    for(int i = 0; i < 2; i++) jp_max[i] = -1;
    double val_max = -1.;
    // Loop over all 2-jet seed probes
    int ip[2], jp[2];
    for(ip[0] = 0; ip[0] < Ninput-1; ip[0]++){
      for(ip[1] = ip[0]+1; ip[1] < Ninput; ip[1]++){
	TVector3 nRef = Ps[ip[0]].Vect().Cross(Ps[ip[1]].Vect());
	int Nhem[2];
	TLorentzVector hem[2];
	for(int i = 0; i < 2; i++){
	  Nhem[i] = 0;
	  hem[i].SetPxPyPzE(0.,0.,0.,0.);
	}
	// Loop over all jets
	for(int i = 0; i < Ninput; i++){
	  if((i == ip[0]) || (i ==ip[1])) continue;
	  int ihem = int(Ps[i].Vect().Dot(nRef) > 0.);
	  Nhem[ihem]++;
	  hem[ihem] += Ps[i];
	}
	// assign 2 probes
	for(jp[0] = 0; jp[0] < 2; jp[0]++){
	  for(jp[1] = 0; jp[1] < 2; jp[1]++){
	    if(jp[0] == jp[1] && Nhem[(jp[0]+1)%2] == 0) continue;
	    TLorentzVector hem_probes[2];
	    for(int i = 0; i < 2; i++) hem_probes[i] = hem[i];
	    for(int i = 0; i < 2; i++) hem_probes[jp[i]] += Ps[ip[i]];
	    double val = hem_probes[0].P() + hem_probes[1].P();
	    if(val > val_max){
	      val_max = val;
	      for(int i = 0; i < 2; i++) ip_max[i] = ip[i];
	      for(int i = 0; i < 2; i++) jp_max[i] = jp[i];
	    }
	  }
	}
      }
    }
    vector<RestFrame*> child_frames[2];
    vector<TLorentzVector> child_Ps[2];
    TLorentzVector hem[2];
    for(int i = 0; i < 2; i++){
      hem[i].SetPxPyPzE(0.,0.,0.,0.);
    }
    for(int i = 0; i < 2; i++){
      child_frames[jp_max[i]].push_back(frames[ip_max[i]]);
      child_Ps[jp_max[i]].push_back(Ps[ip_max[i]]);
      hem[jp_max[i]] += Ps[ip_max[i]];
    }
    TVector3 nRef = Ps[ip_max[0]].Vect().Cross(Ps[ip_max[1]].Vect());
    for(int i = 0; i < Ninput; i++){
      if((i == ip_max[0]) || (i == ip_max[1])) continue;
      int ihem = int(Ps[i].Vect().Dot(nRef) > 0.);
      child_frames[ihem].push_back(frames[i]);
      child_Ps[ihem].push_back(Ps[i]);
      hem[ihem] += Ps[i];
    }

    int flip = int(hem[1].M() > hem[0].M());
    for(int i = 0; i < 2; i++){
      int j = (i+flip)%2;
      if(child_frames[j].size() == 1){
	frame.AddChildFrame(*child_frames[j][0]);
      } else {
	RestFrame& new_frame = GetNewDecayFrame(GetName(),GetTitle());
	frame.AddChildFrame(new_frame);
	AssembleRecursive(new_frame, child_frames[j], child_Ps[j]);
      }
    }
  }

  bool SelfAssemblingRecoFrame::AnalyzeEventRecursive(){
    // Disassemble Frame tree if it assembled
    if(m_IsAssembled) Disassemble();
    if(!ReconstructionFrame::AnalyzeEventRecursive()){
      m_Log << LogWarning;
      m_Log << "Unable to recursively analyze event with ";
      m_Log << "disassembled SelfAssemblingRecoFrame" << m_End;
      return SetSpirit(false);
    }
    // Assemble Frame tree
    Assemble();
    if(!ReconstructionFrame::AnalyzeEventRecursive()){
      m_Log << LogWarning;
      m_Log << "Unable to recursively analyze event with ";
      m_Log << "assembled SelfAssemblingRecoFrame" << m_End;
      return SetSpirit(false);
    }
    return SetSpirit(true);
  }

  void SelfAssemblingRecoFrame::ClearNewFrames(){
    int N = m_DecayFrames.GetN();
    for(int i = 0; i < N; i++) m_DecayFrames[i].Clear();
    N = m_VisibleFrames.GetN();
    for(int i = 0; i < N; i++) m_VisibleFrames[i].Clear();
  }

  ReconstructionFrame& SelfAssemblingRecoFrame::GetNewDecayFrame(const string& sname, const string& stitle){
    if(m_Ndecay < m_DecayFrames.GetN()){
      m_Ndecay++;
      return m_DecayFrames.Get(m_Ndecay-1);
    }
    string name  = sname+"_"+std::to_string(m_Ndecay+1);
    string title = "#left("+stitle+"#right)_{"+std::to_string(m_Ndecay+1)+"}";
    DecayRecoFrame* framePtr = new DecayRecoFrame(name, title);
    
    m_DecayFrames.Add(*framePtr);
    AddDependent(framePtr);
    m_Ndecay++;
    return *framePtr;
  }

  ReconstructionFrame& SelfAssemblingRecoFrame::GetNewVisibleFrame(const string& sname, const string& stitle){
    if(m_Nvisible < m_VisibleFrames.GetN()){
      m_Nvisible++;
      return m_VisibleFrames.Get(m_Nvisible-1);
    }
    string name  = sname+"_"+std::to_string(m_Nvisible+1);
    string title = "#left("+stitle+"#right)_{"+std::to_string(m_Nvisible+1)+"}";
    VisibleRecoFrame* framePtr = new VisibleRecoFrame(name, title);
    
    m_VisibleFrames.Add(*framePtr);
    AddDependent(framePtr);
    m_Nvisible++;
    return *framePtr;
  }

  RestFrame const& SelfAssemblingRecoFrame::GetFrame(const RFKey& key) const {
    if(!m_IsAssembled)
      return RestFrame::Empty();
    int N = m_ChildStates.size();
    for(int i = 0; i < N; i++){
      if(m_ChildStates[&GetChildFrame(i)].Contains(key))
	return m_ChildStates[&GetChildFrame(i)].Get(key).GetListFrames()[0];
    }

    return RestFrame::Empty();
  }

}
