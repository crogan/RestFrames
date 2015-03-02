#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFrame class methods
  ///////////////////////////////////////////////
  RFrame::RFrame(const string& sname, const string& stitle, int ikey) : 
    RestFrame(sname, stitle, ikey)
  {
    Init();
  }

  RFrame::RFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  RFrame::~RFrame(){
   ClearStates();
  }

  void RFrame::Init(){
    m_Ana = FReco;
    m_GroupPtr = nullptr;
  }

  void RFrame::ClearRFrame(){
    ClearStates();
  }

  void RFrame::ClearStates(){
    int N = m_ChildStates.size();
    for(int i = 0; i < N; i++){
      delete m_ChildStates[i];
    }
    m_ChildStates.clear();
  }
  
  void RFrame::SetGroup(Group* groupPtr){
    if(m_GroupPtr){
      m_GroupPtr->RemoveFrame(this);
    }
    m_GroupPtr = groupPtr;
  }

  GroupList* RFrame::GetListGroups() const {
    GroupList* groupsPtr = new GroupList();
    FillListGroupsRecursive(groupsPtr);
    return groupsPtr;
  }

  void RFrame::FillListGroupsRecursive(GroupList* groupsPtr) const{
    if(m_GroupPtr) groupsPtr->Add(m_GroupPtr);
    int Nchild = GetNChildren();
    for(int i = 0; i < Nchild; i++){
      RFrame *childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      if(childPtr) childPtr->FillListGroupsRecursive(groupsPtr);
    }
  }

  bool RFrame::InitializeNoGroupStates(const StateList* statesPtr){
    int Nchild = GetNChildren();
  
    for(int i = 0; i < Nchild; i++){
      RestFrame *childPtr = GetChildFrame(i);
      if(!childPtr) return false;

      m_ChildStates.push_back(new StateList());
    
      RestFrameList *framesPtr = childPtr->GetListFramesType(FVisible);
      int Nframe = framesPtr->GetN();
      for(int f = 0; f < Nframe; f++){
	RFrame* rframePtr = dynamic_cast<RFrame*>(framesPtr->Get(f));
	if(!rframePtr) return false;
	if(rframePtr->GetGroup()){
	  continue;
	}
	int index = statesPtr->GetIndex(framesPtr->Get(f));
	if(index >= 0)  m_ChildStates[i]->Add(statesPtr->Get(index));
      }
      delete framesPtr;
    }
    return true;
  }

  bool RFrame::InitializeGroupStates(const GroupList* groupsPtr){
    if(!groupsPtr) return false;
    int Ngroup = groupsPtr->GetN();
    int Nchild = GetNChildren();
    
    vector<FrameType> terminal_types;
    terminal_types.push_back(FVisible);
    terminal_types.push_back(FInvisible);

    for(int c = 0; c < Nchild; c++){
      RestFrame* childPtr = GetChildFrame(c);
      if(!childPtr) return false;
      RestFrameList* child_framesPtr = childPtr->GetListFramesType(terminal_types);
      int Nframe = child_framesPtr->GetN();
      for(int f = 0; f < Nframe; f++){
	RestFrame* framePtr = child_framesPtr->Get(f);
	if(!framePtr) return false;
	for(int g = 0; g < Ngroup; g++){
	  Group* groupPtr = groupsPtr->Get(g);
	  if(!groupPtr) return false;
	  if(groupPtr->ContainsFrame(framePtr)){
	    State* statePtr = groupPtr->GetState(framePtr);
	    if(!statePtr) return false;
	    m_ChildStates[c]->Add(statePtr);
	    break;
	  }
	}
      }
      delete child_framesPtr;
    }
    return true;
  }

  bool RFrame::InitializeStates(const StateList* statesPtr, const GroupList* groupsPtr){
    ClearStates();
    m_Mind = false;
    if(!m_Body){
      cout << endl << "Initialize Analysis Failure: ";
      cout << "UnSound frame " << m_Name.c_str() << " in tree" << endl;
      return false;
    }
    if(!InitializeNoGroupStates(statesPtr)) return false;
    if(!InitializeGroupStates(groupsPtr)) return false;
    
    m_Mind = true;
    return true;
  }
  
  bool RFrame::InitializeStatesRecursive(const StateList* statesPtr, const GroupList* groupsPtr){
    if(!InitializeStates(statesPtr,groupsPtr)) return false;

    int Nchild = GetNChildren();
    bool child_mind = true;
    for(int i = 0; i < Nchild; i++){
      RFrame *childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      if(!childPtr) return false;
      if(!childPtr->InitializeStatesRecursive(statesPtr,groupsPtr)) child_mind = false;;
    }
    m_Mind = child_mind;
    return m_Mind;
  }

  void RFrame::ClearEventRecursive(){ 
    m_Spirit = false;
    if(!m_Body || !m_Mind) return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i)->ClearEventRecursive();
  }

  bool RFrame::AnalyzeEventRecursive(){
    m_Spirit = false;
    if(!m_Mind){
      cout << endl << "Analyze Event Failure: ";
      cout << "UnSound frame " << m_Name.c_str() << " in tree" << endl;
      return false;
    }
    TLorentzVector Ptot(0,0,0,0);
    int Nchild = GetNChildren();
    bool child_spirit = true;
    for(int i = 0; i < Nchild; i++){
      TLorentzVector P = m_ChildStates[i]->GetFourVector();
      TVector3 B_child = P.BoostVector();
      m_ChildLinks[i]->SetBoostVector(B_child);
      Ptot += P;

      RFrame *childPtr = dynamic_cast<RFrame*>(GetChildFrame(i));
      childPtr->SetFourVector(P,this);
      if(!childPtr->IsVisibleFrame() && !childPtr->IsInvisibleFrame()){ 
	B_child *= -1.;
	m_ChildStates[i]->Boost(B_child);
      }
      if(!childPtr->AnalyzeEventRecursive()) child_spirit = false;
      if(!childPtr->IsVisibleFrame() && !childPtr->IsInvisibleFrame()){ 
	B_child *= -1.;
	m_ChildStates[i]->Boost(B_child);
      }
    }
    if(m_Type == FLab) SetFourVector(Ptot,this);
    m_Spirit = child_spirit;
    return m_Spirit;
  }

}
