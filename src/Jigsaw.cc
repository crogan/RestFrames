#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // Jigsaw class methods
  ///////////////////////////////////////////////
  int Jigsaw::m_class_key = 0;

  Jigsaw::Jigsaw(const string& sname, const string& stitle){
    Init(sname, stitle);
    m_Key = GenKey();
  }

  Jigsaw::Jigsaw(const string& sname, const string& stitle, int ikey){
    Init(sname, stitle);
    m_Key = ikey;
  }

  Jigsaw::~Jigsaw(){
    Clear();
    delete m_DependancyJigsawsPtr;
    delete m_OutputStatesPtr;
  }

  void Jigsaw::Init(const string &sname, const string &stitle){
    m_Name = sname;
    m_Title = stitle;
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;

    m_Priority = 0.;
    m_GroupPtr = nullptr;
    m_InputStatePtr = nullptr;
    m_DependancyJigsawsPtr = new JigsawList();
    m_OutputStatesPtr = new StateList();
  }

  void Jigsaw::Clear(){
    int Nof = m_OutputFrames.size();
    for(int i = 0; i < Nof; i++){
      delete m_OutputFrames[i];
    }
    m_OutputFrames.clear();

    int Ndf = m_DependancyFrames.size();
    for(int i = 0; i < Ndf; i++){
      delete m_DependancyFrames[i];
    }
    m_DependancyFrames.clear();

    ClearOutputStates();
    ClearDependancyStates();
    m_DependancyJigsawsPtr->Clear();
  }
  
  void Jigsaw::ClearOutputStates(){
    int Nos = m_OutputStatesPtr->GetN();
    for(int i = 0; i < Nos; i++){
      delete m_OutputStatesPtr->Get(i);
    }
    m_OutputStatesPtr->Clear();
  }

  void Jigsaw::ClearDependancyStates(){
    int Nds = m_DependancyStates.size();
    for(int i = 0; i < Nds; i++){
      delete m_DependancyStates[i];
    }
    m_DependancyStates.clear();
  }

  int Jigsaw::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  int Jigsaw::GetKey() const {
    return m_Key;
  }

  string Jigsaw::GetName() const {
    return m_Name;
  }

  string Jigsaw::GetTitle() const {
    return m_Title;
  }

  bool Jigsaw::IsSame(const Jigsaw* jigsawPtr) const {
    if(!jigsawPtr) return false;
    return (m_Key == jigsawPtr->GetKey());
  }

  bool Jigsaw::IsInvisibleJigsaw() const {
    return m_Type == JInvisible;
  }

  bool Jigsaw::IsCombinatoricJigsaw() const {
    return m_Type == JCombinatoric;
  }

  void Jigsaw::SetGroup(Group* groupPtr){
    m_GroupPtr = groupPtr;
  }

  Group* Jigsaw::GetGroup() const{
    return m_GroupPtr;
  }

  bool Jigsaw::CanSplit(const RestFrameList* framesPtr){
    if(!framesPtr) return false;

    int NoF = m_OutputFrames.size();
    RestFrameList frames;
    for(int i = 0; i < NoF; i++){
      frames.Add(m_OutputFrames[i]);
    }
    return frames.IsSame(framesPtr);
  }

  bool Jigsaw::CanSplit(const State* statePtr){
    if(!statePtr) return false;
    RestFrameList* framesPtr = statePtr->GetFrames();
    bool ret = CanSplit(framesPtr);
    delete framesPtr;
    return ret;
  }

  State* Jigsaw::NewOutputState(){
    return new State();
  }

  StateList* Jigsaw::InitializeOutputStates(State* statePtr){
    if(!statePtr) return nullptr;
    if(!CanSplit(statePtr)) return nullptr;
    ClearOutputStates();

    m_InputStatePtr = statePtr;
    statePtr->SetChildJigsaw(this);

    int NoF = m_OutputFrames.size();
    for(int i = 0; i < NoF; i++){
      State* new_statePtr = NewOutputState();
      new_statePtr->AddFrame(m_OutputFrames[i]);
      new_statePtr->SetParentJigsaw(this);
      m_OutputStatesPtr->Add(new_statePtr);
    }
    return m_OutputStatesPtr->Copy();
  }

  bool Jigsaw::InitializeDependancyStates(const StateList* statesPtr, const GroupList* groupsPtr){
    if(!statesPtr) return false;
    ClearDependancyStates();
    if(!m_Body){
      m_Mind = false;
      return m_Mind;
    }
    m_Mind = true;

    int Ngroup;
    if(groupsPtr){
      Ngroup = groupsPtr->GetN();
    } else {
      Ngroup = 0;
    }
    int Ndep = m_DependancyFrames.size();
    for(int d = 0; d < Ndep; d++){
      m_DependancyStates.push_back(new StateList());
      vector<RestFrameList*>* group_framesPtr = new vector<RestFrameList*>;
      for(int i = 0; i < Ngroup; i++) group_framesPtr->push_back(new RestFrameList());
      RestFrameList* framesPtr = m_DependancyFrames[d];
      int Nf = framesPtr->GetN();
      for(int f = 0; f < Nf; f++){
	RestFrame* framePtr = framesPtr->Get(f);
	bool no_group = true;
	for(int g = 0; g < Ngroup; g++){
	  if(groupsPtr->Get(g)->ContainsFrame(framePtr)){
	    group_framesPtr->at(g)->Add(framePtr);
	    no_group = false;
	    break;
	  }
	}
	if(no_group){
	  int index = statesPtr->GetIndex(framePtr);
	  if(index < 0) m_Mind = false;
	  m_DependancyStates[d]->Add(statesPtr->Get(index));
	}
      }
      for(int g = 0; g < Ngroup; g++){
	StateList* group_statesPtr = nullptr;
	if(group_framesPtr->at(g)->GetN() > 0){
	  if(!groupsPtr->Get(g)->GetState(group_framesPtr->at(g),group_statesPtr)) m_Mind = false;
	  m_DependancyStates[d]->Add(group_statesPtr);
	  delete group_statesPtr;
	}
	delete group_framesPtr->at(g);
      }
      delete group_framesPtr;
    }
    return m_Mind;
  } 

  bool Jigsaw::InitializeDependancyJigsaws(){
    if(!m_Mind) return false;
    m_DependancyJigsawsPtr->Clear();

    JigsawList* jigsawsPtr = new JigsawList();
    FillStateJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    jigsawsPtr->Clear();
    FillGroupJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    delete jigsawsPtr;
    return m_Mind;
  }

  bool Jigsaw::DependsOnJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return false;
    return m_DependancyJigsawsPtr->Contains(jigsawPtr);
  }

  void Jigsaw::FillGroupJigsawDependancies(JigsawList* jigsawsPtr){
    if(!jigsawsPtr) return;
    if(jigsawsPtr->Contains(this)) return;
    jigsawsPtr->Add(this);
    if(m_InputStatePtr) m_InputStatePtr->FillGroupJigsawDependancies(jigsawsPtr);
  }

  void Jigsaw::FillStateJigsawDependancies(JigsawList* jigsawsPtr){
    if(!jigsawsPtr) return;
    if(jigsawsPtr->Contains(this)) return;
    jigsawsPtr->Add(this);

    int N = m_DependancyStates.size();
    for(int i = 0; i < N; i++){
      StateList* statesPtr = m_DependancyStates[i];
      int M = statesPtr->GetN();
      for(int j = 0; j < M; j++){
	statesPtr->Get(j)->FillStateJigsawDependancies(jigsawsPtr);
      }
    }
  }

  void Jigsaw::AddOutputFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    if(!m_GroupPtr) return;
    if(!m_GroupPtr->ContainsFrame(framePtr)) return;
    while(i >= int(m_OutputFrames.size())){
      m_OutputFrames.push_back(new RestFrameList());
    }
    m_OutputFrames[i]->Add(framePtr);
  }

  void Jigsaw::AddOutputFrame(RestFrameList* framesPtr, int i){
    if(!framesPtr) return;
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++) AddOutputFrame(framesPtr->Get(f), i);
  }

  void Jigsaw::AddDependancyFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    while(i >= int(m_DependancyFrames.size())){
      m_DependancyFrames.push_back(new RestFrameList());
    }
    m_DependancyFrames[i]->Add(framePtr);
  }

  void Jigsaw::AddDependancyFrame(RestFrameList* framesPtr, int i){
    if(!framesPtr) return;
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++) AddDependancyFrame(framesPtr->Get(f), i);
  }

  bool Jigsaw::IsSoundBody() const {
    m_Body = true;
    int Nout = m_OutputFrames.size();
    for(int i = 0; i < Nout-1; i++)
      for(int j = i+1; j < Nout; j++)
	if(m_OutputFrames[i]->SizeIntersection(m_OutputFrames[j]) > 0) m_Body = false;
    for(int i = 0; i < Nout; i++)
      if(m_OutputFrames[i]->GetN() == 0) m_Body = false;
    return m_Body;
  }

  bool Jigsaw::IsSoundMind() const {
    return m_Mind;
  }

  bool Jigsaw::IsSoundSpirit() const {
    return m_Spirit;
  }


  int Jigsaw::GetNChildStates() const {
    return m_OutputFrames.size();
  }
  State* Jigsaw::GetChildState(int i) const {
    if(i < 0 || i >= GetNChildStates()) return nullptr;
    return m_OutputStatesPtr->Get(i);
  }

  RestFrameList* Jigsaw::GetChildFrames(int i) const {
    if(i < 0 || i >= GetNChildStates()) return nullptr;
    RestFrameList* framesPtr = m_OutputFrames[i]->Copy();
    if(i < int(m_DependancyFrames.size())) framesPtr->Add(m_DependancyFrames[i]);
    return framesPtr;
  }
  
  RestFrameList* Jigsaw::GetChildFrames() const {
    RestFrameList* child_framesPtr = new RestFrameList();
    int N = GetNChildStates();
    for(int i = 0; i < N; i++){
      RestFrameList* framesPtr = GetChildFrames(i);
      child_framesPtr->Add(framesPtr);
      delete framesPtr;
    }
    return child_framesPtr;
  }

}
