#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleJigsaw class
  ///////////////////////////////////////////////

  InvisibleJigsaw::InvisibleJigsaw(const string& sname, const string& stitle, int ikey) : 
    Jigsaw(sname, stitle, ikey)
  {
    Init();
  }

  InvisibleJigsaw::InvisibleJigsaw(const string& sname, const string& stitle) : 
    Jigsaw(sname, stitle)
  {
    Init();
  }

  InvisibleJigsaw::~InvisibleJigsaw(){
   
  }

  void InvisibleJigsaw::Init(){
    m_Type = JInvisible;
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame& frame, int i){
    AddVisibleFrame(&frame,i);
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    RestFrameList* framesPtr = framePtr->GetListVisibleFrames();
    AddDependancyFrame(framesPtr, i);
    delete framesPtr;
  }

  void InvisibleJigsaw::AddVisibleFrame(RestFrameList* framesPtr, int i){
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddVisibleFrame(framesPtr->Get(f), i);
    }
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame& frame, int i){
    AddInvisibleFrame(&frame,i);
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrame* framePtr, int i){
    if(!m_GroupPtr) return;
  
    RestFrameList* framesPtr = framePtr->GetListInvisibleFrames();
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      if(m_GroupPtr->ContainsFrame(framesPtr->Get(f))) 
	AddOutputFrame(framesPtr->Get(f), i);
    }  
    delete framesPtr;
  }

  void InvisibleJigsaw::AddInvisibleFrame(RestFrameList* framesPtr, int i){
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddInvisibleFrame(framesPtr->Get(f), i);
    }
  }

  State* InvisibleJigsaw::NewOutputState(){
    return new InvisibleState();
  }

  double InvisibleJigsaw::GetMinimumMass(){
    if(!m_Mind) return 0.;
    int N = GetNChildStates();
    double M = 0.;
    for(int i = 0; i < N; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(!statePtr) return 0.;
      M += statePtr->GetMinimumMass();
    }

    return M;
  }

  void InvisibleJigsaw::FillInvisibleMassJigsawDependancies(JigsawList* jigsawsPtr){ 
    if(!jigsawsPtr) return;
   
    int Nchild = GetNChildStates();
    for(int i = 0; i < Nchild; i++){
      InvisibleState* statePtr = dynamic_cast<InvisibleState*>(GetChildState(i));
      if(statePtr) statePtr->FillInvisibleMassJigsawDependancies(jigsawsPtr);
    }
  }

  bool InvisibleJigsaw::InitializeDependancyJigsaws(){
    if(!m_Mind) return false;
    m_DependancyJigsawsPtr->Clear();

    JigsawList* jigsawsPtr = new JigsawList();
    FillStateJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    jigsawsPtr->Clear();
    FillInvisibleMassJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    jigsawsPtr->Clear();
    FillGroupJigsawDependancies(jigsawsPtr);
    jigsawsPtr->Remove(this);
    m_DependancyJigsawsPtr->Add(jigsawsPtr);

    delete jigsawsPtr;
    return m_Mind;
  }

  bool InvisibleJigsaw::InitializeJigsawExecutionList(JigsawList* chain_jigsawPtr){
    if(!m_Mind) return false;
    if(chain_jigsawPtr->Contains(this)) return true;

    // Add group dependancy jigsaws first
    JigsawList* group_jigsawsPtr = new JigsawList();
    FillGroupJigsawDependancies(group_jigsawsPtr);
    group_jigsawsPtr->Remove(this);
    int Ngroup = group_jigsawsPtr->GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw* jigsawPtr = group_jigsawsPtr->Get(i);
      if(!chain_jigsawPtr->Contains(jigsawPtr)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
      }
      m_DependancyJigsawsPtr->Remove(jigsawPtr);
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsawsPtr->GetN() > 0){
      Jigsaw* jigsawPtr = m_DependancyJigsawsPtr->Get(m_DependancyJigsawsPtr->GetN()-1);
      if(chain_jigsawPtr->Contains(jigsawPtr)){
	m_DependancyJigsawsPtr->Remove(jigsawPtr);
	continue;
      }
      if(jigsawPtr->DependsOnJigsaw(this)) m_DependancyJigsawsPtr->Remove(jigsawPtr);
      if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	m_Mind = false;
	return false;
      }
      m_DependancyJigsawsPtr->Remove(jigsawPtr);
      continue;
    }
    chain_jigsawPtr->Add(this);
    
    return true;
  }

}
