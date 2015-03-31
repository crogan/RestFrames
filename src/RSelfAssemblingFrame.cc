#include <sstream>
#include "RestFrames/RSelfAssemblingFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RSelfAssemblingFrame class
  ///////////////////////////////////////////////
  RSelfAssemblingFrame::RSelfAssemblingFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname,stitle,key),
    RDecayFrame(sname,stitle,key)
  {
    Init();
  }
  RSelfAssemblingFrame::RSelfAssemblingFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle),
    RDecayFrame(sname,stitle)
  {
    Init();
  }
  
  RSelfAssemblingFrame::~RSelfAssemblingFrame(){
    Disassemble();
    int Nv = m_VisibleFrames.GetN();
    for(int i = 0; i < Nv; i++) delete m_VisibleFrames.Get(i);
    m_VisibleFrames.Clear();
    int Nd = m_DecayFrames.GetN();
    for(int i = 0; i < Nd; i++) delete m_DecayFrames.Get(i);
    m_DecayFrames.Clear();
  }

  void RSelfAssemblingFrame::Init(){
    m_RType = RDSelfAssembling;
    m_IsAssembled = false;
    m_IsBackedUp = false;
    m_Body_UnAssembled = false;
    m_Mind_UnAssembled = false;
    m_Nvisible = 0;
    m_Ndecay = 0;
  }

  void RSelfAssemblingFrame::ClearEventRecursive(){
    m_Spirit = false;
   
    if(!m_Body || !m_Mind) return;
   
    Disassemble();
    RFrame::ClearEventRecursive();
  }

  void RSelfAssemblingFrame::Disassemble(){
    if(!m_IsAssembled) return;

    m_Nvisible = 0;
    m_Ndecay = 0;
  
    ClearFrame();
    int Ncf = m_ChildFrames_UnAssembled.GetN();
    for(int i = 0; i < Ncf; i++)
      AddChildFrame(m_ChildFrames_UnAssembled.Get(i));
    //m_ChildFrames_UnAssembled.Clear();
    
    m_Body = IsSoundBodyRecursive();

    ClearRFrame();
    int Ncs = m_ChildStates_UnAssembled.size();
    for(int i = 0; i < Ncs; i++){
      m_ChildStates.push_back(m_ChildStates_UnAssembled[i]->Copy());
      //delete m_ChildStates_UnAssembled[i];
    }
    // m_ChildStates_UnAssembled.clear();

    m_IsAssembled = false;
    m_Body = m_Body_UnAssembled;
    m_Mind = m_Mind_UnAssembled;
    m_Spirit = false;
  }

  void RSelfAssemblingFrame::Assemble(){
    if(m_IsAssembled) Disassemble();
    if(!m_Body || !m_Mind) return;

    vector<RestFrame*> frames;
    vector<TLorentzVector> Ps; 
    StateList states;
    
    GroupList* groupsPtr = GetListGroups();
    GroupList groups;
    groups.Add(groupsPtr);
    delete groupsPtr;

    //m_ChildFrames_UnAssembled.Clear();
    //m_ChildStates_UnAssembled.clear();
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      RestFrame* framePtr = m_ChildLinks[i]->GetChildFrame();
      if(!m_IsBackedUp){
	m_ChildFrames_UnAssembled.Add(framePtr);
	m_ChildStates_UnAssembled.push_back(m_ChildStates[i]->Copy());
      }
      bool expand = false;
      if(m_ChildStates[i]->GetN() == 1){
	CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(m_ChildStates[i]->Get(0));
	if(statePtr){
	  const StateList* elementsPtr = statePtr->GetElements();
	  int Nelement = elementsPtr->GetN();
	  for(int e = 0; e < Nelement; e++){
	    State* elementPtr = elementsPtr->Get(e);
	    RestFrame* new_framePtr = GetNewVisibleFrame(framePtr->GetName(),framePtr->GetTitle());
	    elementPtr->ClearFrames();
	    elementPtr->AddFrame(new_framePtr);
	    frames.push_back(new_framePtr);
	    TLorentzVector V = elementPtr->GetFourVector();
	    if(V.M() < 0.) V.SetVectM(V.Vect(),0.);
	    Ps.push_back(V);
	    states.Add(elementPtr);
	  }
	  delete elementsPtr;
	  expand = true;
	}
      }
      if(!expand){
	frames.push_back(framePtr);
	TLorentzVector V = m_ChildStates[i]->GetFourVector();
	if(V.M() < 0.) V.SetVectM(V.Vect(),0.);
	Ps.push_back(V);
	states.Add(m_ChildStates[i]);
      }
    }
    m_Body_UnAssembled = m_Body;
    m_Mind_UnAssembled = m_Mind;
    m_IsBackedUp = true;
    ClearFrame();
    ClearRFrame();
   
    AssembleRecursive(this, frames, Ps); 

    m_Body = IsSoundBodyRecursive();
    m_Mind = InitializeStates(&states, &groups);

    for(int i = 0; i < m_Ndecay; i++){
      RFrame* framePtr = dynamic_cast<RFrame*>(m_DecayFrames.Get(i));
      m_Mind = m_Mind && framePtr->InitializeStates(&states, &groups);
    }
    for(int i = 0; i < m_Nvisible; i++){
      RFrame* framePtr = dynamic_cast<RFrame*>(m_VisibleFrames.Get(i));
      m_Mind = m_Mind && framePtr->InitializeStates(&states, &groups);
    }
    m_IsAssembled = true;
  }

  void RSelfAssemblingFrame::AssembleRecursive(RestFrame* framePtr, vector<RestFrame*>& frames, vector<TLorentzVector>& Ps){
    int Ninput = frames.size();
    if(Ninput <= 1){
      for(int i = 0; i < Ninput; i++) framePtr->AddChildFrame(frames[i]);
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
	framePtr->AddChildFrame(child_frames[j][0]);
      } else {
	RestFrame* new_framePtr = GetNewDecayFrame(GetName(),GetTitle());
	framePtr->AddChildFrame(new_framePtr);
	AssembleRecursive(new_framePtr, child_frames[j], child_Ps[j]);
      }
    }
  }

  bool RSelfAssemblingFrame::AnalyzeEventRecursive(){
    if(m_Spirit || m_IsAssembled) Disassemble();
    if(!RFrame::AnalyzeEventRecursive()) return false;
    Assemble();
    return RFrame::AnalyzeEventRecursive();
  }

  RestFrame* RSelfAssemblingFrame::GetNewDecayFrame(const string& sname, const string& stitle){
    if(m_Ndecay < m_DecayFrames.GetN()){
      m_DecayFrames.Get(m_Ndecay)->ClearFrame();
      dynamic_cast<RFrame*>(m_DecayFrames.Get(m_Ndecay))->ClearRFrame();
      m_Ndecay++;
      return m_DecayFrames.Get(m_Ndecay-1);
    }

    ostringstream name;
    name << sname << "_" << m_Ndecay+1;
    ostringstream title; 
    title << "#left(" << stitle << "#right)_{" << m_Ndecay+1 << "}";
    RDecayFrame* framePtr = new RDecayFrame(name.str(),title.str());
    
    m_DecayFrames.Add(framePtr);
    m_Ndecay++;
    return framePtr;
  }

  RestFrame* RSelfAssemblingFrame::GetNewVisibleFrame(const string& sname, const string& stitle){
    if(m_Nvisible < m_VisibleFrames.GetN()){
      m_VisibleFrames.Get(m_Nvisible)->ClearFrame();
      dynamic_cast<RFrame*>(m_VisibleFrames.Get(m_Nvisible))->ClearRFrame();
      m_Nvisible++;
      return m_VisibleFrames.Get(m_Nvisible-1);
    }

    ostringstream name;
    name << sname << "_" << m_Nvisible+1;
    ostringstream title; 
    title << "#left(" << stitle << "#right)_{" << m_Nvisible+1 << "}";
    RVisibleFrame* framePtr = new RVisibleFrame(name.str(),title.str());
    
    m_VisibleFrames.Add(framePtr);
    m_Nvisible++;
    return framePtr;
  }

  // const RestFrame* RSelfAssemblingFrame::GetFrame(GroupElementID obj) const {
  //   return GetFrame(obj);
  // }
  const RestFrame* RSelfAssemblingFrame::GetFrame(GroupElementID obj) const {
    if(!m_IsAssembled) return nullptr;

    const State* statePtr = obj;

    for(int i = 0; i < m_ChildStates.size(); i++){
      int index = m_ChildStates[i]->GetIndex(statePtr);
      if(index >= 0) return m_ChildStates[i]->Get(index)->GetFrame();
    }
    return nullptr;
  }

}
