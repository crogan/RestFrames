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

  void RLabFrame::ClearStates(){
    m_Log << LogVerbose << "Clearing States associated with this frame" << m_End;
    int Ns = m_LabStates.GetN();
    for(int i = 0; i < Ns; i++){
      delete m_LabStates.Get(i);
    }
    m_LabStates.Clear();
  }
  
  bool RLabFrame::InitializeLabGroups(){
    m_Log << LogVerbose << "Initializing Groups associated with this tree" << m_End;
    m_LabGroups.Clear();
    RFList<Group> *groupsPtr = GetListGroups();
    m_LabGroups.Add(groupsPtr);
    delete groupsPtr;
    int Ngroup = m_LabGroups.GetN();
    for(int i = 0; i < Ngroup; i++){
      if(!m_LabGroups.Get(i)->InitializeAnalysis()) return false;
      
    }
    return true;
  }

  bool RLabFrame::InitializeLabStates(){
    m_Log << LogVerbose << "Initializing States associated with this tree" << m_End;
    ClearStates();
    RFList<RestFrame>* framesPtr = GetListFramesType(FVisible);
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
    m_Log << LogVerbose << "Initializing Jigsaws associated with this tree" << m_End;
    m_LabJigsaws.Clear();
    int Ng = m_LabGroups.GetN();
    // Initialize Dependancy States in jigsaws
    for(int g = 0; g < Ng; g++){
      Group* groupPtr = m_LabGroups.Get(g);
      RFList<Jigsaw>* jigsawsPtr = groupPtr->GetJigsaws();
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
      RFList<Jigsaw>* jigsawsPtr = groupPtr->GetJigsaws();
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
    RFList<Jigsaw>* chain_jigsawsPtr = new RFList<Jigsaw>();
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
    m_Log << LogVerbose << "Initializing this tree for analysis" << m_End;
   
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
    if(!InitializeStatesRecursive(&m_LabStates,&m_LabGroups)){
      m_Log << LogWarning << "Unable to recursively initialize tree" << m_End;
      SetMind(false);
      return false;
    }

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
      m_LabGroups.Get(i)->ClearEvent();
    
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
      State* statePtr = m_LabStates.Get(i);
      RFList<RestFrame>* framesPtr = statePtr->GetFrames();
      VisibleFrame* vframePtr = dynamic_cast<VisibleFrame*>(framesPtr->Get(0));
      if(vframePtr) statePtr->SetFourVector(vframePtr->GetLabFrameFourVector());
      delete framesPtr;
    }
  
    int Ng = m_LabGroups.GetN();
    for(int i = 0; i < Ng; i++){
      if(!m_LabGroups.Get(i)->AnalyzeEvent()){
	m_Log << LogWarning;
	m_Log << "Group failed in event analysis: ";
	m_Log << Log(m_LabGroups.Get(i)) << m_End;
	SetSpirit(false);
	return false;
      }
    }

    int Nj = m_LabJigsaws.GetN();
    for(int i = 0; i < Nj; i++){
      if(!m_LabJigsaws.Get(i)->AnalyzeEvent()){
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
