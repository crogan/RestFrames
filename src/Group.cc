#include "RestFrames/Group.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // Group class methods
  ///////////////////////////////////////////////
  int Group::m_class_key = 0;

  Group::Group(const string& sname, const string& stitle){
    Init(sname,stitle);
    m_Key = GenKey();
  }

  Group::Group(const string& sname, const string& stitle, int ikey){
    Init(sname,stitle);
    m_Key = ikey;
  }

  Group::~Group(){
    Clear();
    delete m_JigsawsPtr;
    delete m_JigsawsToUsePtr;
    delete m_StatesPtr;
    delete m_StatesToSplitPtr;
  }

  void Group::Init(const string& sname, const string& stitle){
    m_Name = sname;
    m_Title = stitle;
    m_Body = false;
    m_Mind = false;
    m_Spirit = false;

    m_GroupStatePtr = nullptr;
    m_JigsawsPtr = new JigsawList();
    m_JigsawsToUsePtr = new JigsawList();
    m_StatesPtr = new StateList();
    m_StatesToSplitPtr = new StateList();
  }

  bool Group::IsSoundBody() const {
    return m_Body;
  }

  bool Group::IsSoundMind() const {
    return m_Mind;
  }

  bool Group::IsSoundSpirit() const {
    return m_Spirit;
  }

  void Group::Clear(){
    if(m_GroupStatePtr) delete m_GroupStatePtr;
    m_Frames.Clear();
    m_StatesPtr->Clear();
    m_JigsawsPtr->Clear();
    m_StatesToSplitPtr->Clear();
    m_JigsawsToUsePtr->Clear(); 
  }

  int Group::GenKey(){
    int newkey = m_class_key;
    m_class_key++;
    return newkey;
  }

  string Group::GetName() const {
    return m_Name;
  }

  string Group::GetTitle() const {
    return m_Title;
  }

  bool Group::IsSame(const Group* groupPtr) const{
    if(!groupPtr) return false;
    if(m_Key == groupPtr->GetKey()) return true;
    return false;
  }

  bool Group::IsInvisibleGroup() const{
    return m_Type == GInvisible;
  }
  
  bool Group::IsCombinatoricGroup() const{
    return m_Type == GCombinatoric;
  }

  int Group::GetKey() const {
    return m_Key;
  }

  void Group::RemoveFrame(const RestFrame* framePtr){
    if(!framePtr) return;
    m_Frames.Remove(framePtr);
  }

  bool Group::ContainsFrame(const RestFrame* framePtr) const {
    return m_Frames.Contains(framePtr);
  }

  RestFrameList* Group::GetFrames() const {
    return m_Frames.Copy();
  }

  JigsawList* Group::GetJigsaws() const {
    return m_JigsawsPtr->Copy();
  }

  State* Group::GetGroupState() const {
    return m_GroupStatePtr;
  }

  bool Group::InitializeAnalysis(){
    m_StatesPtr->Clear();
    m_StatesToSplitPtr->Clear();
    if(m_GroupStatePtr) delete m_GroupStatePtr;

    m_GroupStatePtr = InitializeGroupState();
    m_GroupStatePtr->AddFrame(&m_Frames);
    m_StatesPtr->Add(m_GroupStatePtr);
    m_StatesToSplitPtr->Add(m_GroupStatePtr);
 
    m_Body = false;
    m_Mind = false;
    if(!InitializeJigsaws()) return false;
    m_Body = true;
    m_Mind = true;
 
    return true;
  }

  State* Group::InitializeGroupState(){
    return new State();
  }

  bool Group::InitializeJigsaws(){
    while(m_StatesToSplitPtr->GetN() > 0){
      State* statePtr = m_StatesToSplitPtr->Get(0);
      if(!SplitState(statePtr)){
	if(statePtr->GetNFrames() != 1) return false;
	m_StatesToSplitPtr->Remove(statePtr);
      }
    }
    return true;
  }

  bool Group::SplitState(const State* statePtr){
    int N = m_JigsawsToUsePtr->GetN();
    Jigsaw *jigsawForSplitPtr = nullptr;
    for(int i = 0; i < N; i++){
      Jigsaw *jigsawPtr = m_JigsawsToUsePtr->Get(i);
      if(jigsawPtr->CanSplit(statePtr)){
	if(!jigsawForSplitPtr) jigsawForSplitPtr = jigsawPtr;
	if(jigsawPtr->GetPriority() < jigsawForSplitPtr->GetPriority()) jigsawForSplitPtr = jigsawPtr;
      }
    }
    if(!jigsawForSplitPtr) return false;
    InitializeJigsaw(jigsawForSplitPtr);
    m_JigsawsToUsePtr->Remove(jigsawForSplitPtr);
    return true;
  }

  void Group::InitializeJigsaw(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return;
    if(!jigsawPtr->IsSoundBody()) return;

    State* statePtr = m_StatesToSplitPtr->Get(0);
    StateList* statesPtr = jigsawPtr->InitializeOutputStates(statePtr);
    m_StatesToSplitPtr->Remove(statePtr);
    m_StatesToSplitPtr->Add(statesPtr);
    m_StatesPtr->Add(statesPtr);
    m_JigsawsPtr->Add(jigsawPtr);
    if(statesPtr) delete statesPtr;
    return;
  }

  State* Group::GetState(const RestFrame* framePtr) const{
    if(!framePtr) return nullptr;
    int Ns = m_StatesPtr->GetN();
    for(int i = Ns-1; i >= 0; i--){
      State* statePtr = m_StatesPtr->Get(i);
      if(statePtr->IsFrame(framePtr)){
	return statePtr;
      }
    }
    m_Mind = false;
    return nullptr;
  }
  
  bool Group::GetState(const RestFrameList* framesPtr, StateList*& statesPtr){
    if(statesPtr){
      delete statesPtr;
      statesPtr = nullptr;
    }
    if(!framesPtr) return false;

    statesPtr = new StateList();
    int Ns = m_StatesPtr->GetN();
    for(int i = 0; i < Ns; i++){
      State* istatePtr = m_StatesPtr->Get(i);
      RestFrameList* iframesPtr = istatePtr->GetFrames();
      if(framesPtr->Contains(iframesPtr)){
	int Nsol = statesPtr->GetN();
	bool isnew = true;
	for(int j = 0; j < Nsol; j++){
	  State* jstatePtr = statesPtr->Get(j);
	  RestFrameList* jframesPtr = jstatePtr->GetFrames();
	  if(iframesPtr->Contains(jframesPtr)){
	    statesPtr->Remove(jstatePtr);
	    delete jframesPtr;
	    break;
	  }
	  if(jframesPtr->Contains(iframesPtr)){
	    isnew = false;
	    delete jframesPtr;
	    break;
	  }
	  delete jframesPtr;
	}
	if(isnew) statesPtr->Add(istatePtr);
      }
      delete iframesPtr;
    }
    RestFrameList* match_framesPtr = new RestFrameList();
    Ns = statesPtr->GetN();
    for(int i = 0; i < Ns; i++){
      match_framesPtr->Add(statesPtr->Get(i)->GetFrames());
    }
    if(!framesPtr->IsSame(match_framesPtr)){
      delete match_framesPtr;
      delete statesPtr;
      statesPtr = nullptr;
      m_Mind = false;
      return false;
    }
    delete match_framesPtr;
    return true;
  }

}
