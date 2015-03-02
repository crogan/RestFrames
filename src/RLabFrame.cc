#include "RestFrames/RLabFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RLabFrame class
  ///////////////////////////////////////////////
  RLabFrame::RLabFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    LabFrame(sname, stitle, key), 
    RFrame(sname, stitle, key)
  {
    Init();
  }

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

  void RLabFrame::ClearStates(){
    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      delete m_LabStates.Get(i);
    }
    m_LabStates.Clear();
  }
  
  bool RLabFrame::InitializeLabGroups(){
    m_LabGroups.Clear();
    GroupList *groupsPtr = GetListGroups();
    m_LabGroups.Add(groupsPtr);
    delete groupsPtr;
    int Ngroup = m_LabGroups.GetN();
    for(int i = 0; i < Ngroup; i++){
      if(!m_LabGroups.Get(i)->InitializeAnalysis()) return false;
    }
    return true;
  }

  bool RLabFrame::InitializeLabStates(){
    ClearStates();
    RestFrameList* framesPtr = GetListFramesType(FVisible);
    int Nf = framesPtr->GetN();
    int Ng = m_LabGroups.GetN();
    for(int f = 0; f < Nf; f++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups.Get(g)->ContainsFrame(framesPtr->Get(f))){
	  has_group = true;
	  break;
	}
      }
      if(!has_group) {
	State* statePtr = new State();
	statePtr->AddFrame(framesPtr->Get(f));
	m_LabStates.Add(statePtr);
      }
    }
    delete framesPtr;

    framesPtr = GetListFramesType(FInvisible);
    Nf = framesPtr->GetN();
    for(int i = 0; i < Nf; i++){
      bool has_group = false;
      for(int g = 0; g < Ng; g++){
	if(m_LabGroups.Get(g)->ContainsFrame(framesPtr->Get(i))){
	  has_group = true;
	  break;
	}
      }
      if(!has_group){
	return false;
      }
    }
    return true;
  }

  bool RLabFrame::InitializeLabJigsaws(){
     m_LabJigsaws.Clear();
    int Ng = m_LabGroups.GetN();
    // Initialize Dependancy States in jigsaws
    for(int g = 0; g < Ng; g++){
      Group* groupPtr = m_LabGroups.Get(g);
      JigsawList* jigsawsPtr = groupPtr->GetJigsaws();
      int Nj = jigsawsPtr->GetN();
      for(int j = 0; j < Nj; j++){
	Jigsaw* jigsawPtr = jigsawsPtr->Get(j);
	if(!jigsawPtr->InitializeDependancyStates(&m_LabStates,&m_LabGroups)){
	  delete jigsawsPtr;
	  return false;
	}
      }
      delete jigsawsPtr;
    }
    // Initialize Dependancy Jigsaw lists in jigsaws
    for(int g = 0; g < Ng; g++){
      Group* groupPtr = m_LabGroups.Get(g);
      JigsawList* jigsawsPtr = groupPtr->GetJigsaws();
      int Nj = jigsawsPtr->GetN();
      for(int j = 0; j < Nj; j++){
	Jigsaw* jigsawPtr = jigsawsPtr->Get(j);
	if(!jigsawPtr->InitializeDependancyJigsaws()){
	  delete jigsawsPtr;
	  return false;
	}
	m_LabJigsaws.Add(jigsawPtr);
      }
      delete jigsawsPtr;
    }
    // Initialize Jigsaw execution list
    JigsawList* chain_jigsawsPtr = new JigsawList();
    int Nj = m_LabJigsaws.GetN();
    for(int i = 0; i < Nj; i++){
      Jigsaw* jigsawPtr = m_LabJigsaws.Get(i);
      if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawsPtr)){
	m_LabJigsaws.Clear();
	delete chain_jigsawsPtr;
	return false;
      }
    }  
    
    m_LabJigsaws.Clear();
    m_LabJigsaws.Add(chain_jigsawsPtr);
    delete chain_jigsawsPtr;

    return true;
  }

  bool RLabFrame::InitializeAnalysis(){
    m_Mind = false;
   
    for(;;){
      if(!InitializeLabGroups())  break;
      if(!InitializeLabStates())  break;
      if(!InitializeLabJigsaws()) break;
      if(!InitializeStatesRecursive(&m_LabStates,&m_LabGroups)) break;
      m_Mind = true;
      break;
    }
    if(!m_Mind){
      cout << endl << "Initialize Analysis Failure: ";
      cout << "UnSound frame in tree" << endl;
    }
    return m_Mind;
  }

  void RLabFrame::ClearEvent(){
    m_Spirit = false;
    if(!m_Body || !m_Mind) return;
    
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++)
      m_LabGroups.Get(i)->ClearEvent();
    
    ClearEventRecursive();
  }

  bool RLabFrame::AnalyzeEvent(){
    m_Spirit = false;
    if(!m_Mind) return false;

    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      State* statePtr = m_LabStates.Get(i);
      RestFrameList* framesPtr = statePtr->GetFrames();
      VisibleFrame* vframePtr = dynamic_cast<VisibleFrame*>(framesPtr->Get(0));
      if(vframePtr) statePtr->SetFourVector(vframePtr->GetLabFrameFourVector());
      delete framesPtr;
    }

    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++){
      if(!m_LabGroups.Get(i)->AnalyzeEvent()) return false;
    }

    int Nj = m_LabJigsaws.GetN();
    for(int i = 0; i < Nj; i++){
      if(!m_LabJigsaws.Get(i)->AnalyzeEvent()) return false;
    }

    if(!AnalyzeEventRecursive()) return false;
    m_Spirit = true;
    return true;
  }

}
