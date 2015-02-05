#include "RestFrames/CombinatoricJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  CombinatoricJigsaw::CombinatoricJigsaw(const string& sname,const string& stitle,int ikey) : 
    Jigsaw(sname, stitle, ikey)
  {
    Init();
  }

  CombinatoricJigsaw::CombinatoricJigsaw(const string& sname,const string& stitle) : 
    Jigsaw(sname, stitle)
  {
    Init();
  }

  CombinatoricJigsaw::~CombinatoricJigsaw(){
    
  }

  void CombinatoricJigsaw::Init(){
    m_Type = JCombinatoric;
  }

  void CombinatoricJigsaw::Clear(){
    Jigsaw::Clear();
  }

  void CombinatoricJigsaw::AddFrame(RestFrame& frame, int i){
    AddFrame(&frame,i);
  }

  void CombinatoricJigsaw::AddFrame(RestFrame* framePtr, int i){
    if(!framePtr) return;
    if(!m_GroupPtr) return;
    vector<FrameType> terminals;
    terminals.push_back(FVisible);
    terminals.push_back(FInvisible);
    RestFrameList *framesPtr = framePtr->GetListFramesType(terminals);
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      if(m_GroupPtr->ContainsFrame(framesPtr->Get(f))){
	AddOutputFrame(framesPtr->Get(f), i);
      } else {
	AddDependancyFrame(framesPtr->Get(f), i);
      }
    }
    delete framesPtr;
  
  }

  void CombinatoricJigsaw::AddFrame(RestFrameList* framesPtr, int i){
    if(!framesPtr) return;
    int N = framesPtr->GetN();
    for(int f = 0; f < N; f++){
      AddFrame(framesPtr->Get(f),i);
    }
  }

  State* CombinatoricJigsaw::NewOutputState(){
    return new CombinatoricState();
  }

  bool CombinatoricJigsaw::InitializeJigsawExecutionList(JigsawList* chain_jigsawPtr){
    if(!m_Mind) return false;
    if(chain_jigsawPtr->Contains(this)) return true;
    m_ExecuteJigsaws.Clear();

    // Add group dependancy jigsaws first
    JigsawList* group_jigsawsPtr = new JigsawList();
    FillGroupJigsawDependancies(group_jigsawsPtr);
    group_jigsawsPtr->Remove(this);
    int Ngroup = group_jigsawsPtr->GetN();
    for(int i = Ngroup-1; i >= 0; i--){
      Jigsaw* jigsawPtr = group_jigsawsPtr->Get(i);
      m_DependancyJigsawsPtr->Remove(jigsawPtr);
      if(!chain_jigsawPtr->Contains(jigsawPtr)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
      }
    }
    // Satisfy dependancy jigsaws
    while(m_DependancyJigsawsPtr->GetN() > 0){
      Jigsaw* jigsawPtr = m_DependancyJigsawsPtr->Get(m_DependancyJigsawsPtr->GetN()-1);
      // Remove dependancy if already in chain
      if(chain_jigsawPtr->Contains(jigsawPtr)){
	m_DependancyJigsawsPtr->Remove(jigsawPtr);
	continue;
      }
      
      if(!jigsawPtr->DependsOnJigsaw(this)){
	if(!jigsawPtr->InitializeJigsawExecutionList(chain_jigsawPtr)){
	  m_Mind = false;
	  return false;
	}
	m_DependancyJigsawsPtr->Remove(jigsawPtr);
	continue;
      }
      JigsawList* temp_chainPtr = chain_jigsawPtr->Copy();
      temp_chainPtr->Add(&m_ExecuteJigsaws);
      temp_chainPtr->Add(this);
      if(!jigsawPtr->InitializeJigsawExecutionList(temp_chainPtr)){
	m_Mind = false;
	return false;
      }
      temp_chainPtr->Remove(this);
      temp_chainPtr->Remove(chain_jigsawPtr);
      m_DependancyJigsawsPtr->Remove(temp_chainPtr);
      m_ExecuteJigsaws.Add(temp_chainPtr);
      
      delete temp_chainPtr;
    }
    chain_jigsawPtr->Add(this);
    chain_jigsawPtr->Add(&m_ExecuteJigsaws);
    return true;
  }

  bool CombinatoricJigsaw::ExecuteDependancyJigsaws(){
    int N = m_ExecuteJigsaws.GetN();
    for(int i = 0; i < N; i++){
      if(!m_ExecuteJigsaws.Get(i)->AnalyzeEvent()) return false;
    }
    return true;
  }

  bool CombinatoricJigsaw::InitializeEvent(){
    if(!m_Mind) return false;

    CombinatoricState* input_statePtr = dynamic_cast<CombinatoricState*>(m_InputStatePtr);
    if(!input_statePtr) return false;

    CombinatoricGroup* groupPtr = dynamic_cast<CombinatoricGroup*>(m_GroupPtr);
    if(!groupPtr) return false;

    m_Inputs.clear();
    const StateList* elementsPtr = input_statePtr->GetElements();
    int Ninput = elementsPtr->GetN();
    for(int i = 0; i < Ninput; i++){
      m_Inputs.push_back(elementsPtr->Get(i));
    }

    m_Outputs.clear();
    m_NForOutput.clear();
    m_NExclusive.clear();
    int Noutput = m_OutputStatesPtr->GetN();
    for(int i = 0; i < Noutput; i++){
      CombinatoricState* statePtr = dynamic_cast<CombinatoricState*>(m_OutputStatesPtr->Get(i));
      if(!statePtr) return false;
      m_Outputs.push_back(statePtr);
      RestFrameList* framesPtr = statePtr->GetFrames();
      int Nf = framesPtr->GetN();
      int NTOT = 0;
      bool exclTOT = true;
      for(int f = 0; f < Nf; f++){
	int N = -1;
	bool excl = false;
	groupPtr->GetNElementsForFrame(framesPtr->Get(f), N, excl);
	if(N < 0) return false;
	NTOT += N;
	exclTOT = exclTOT && excl;
      }
      m_NForOutput.push_back(NTOT);
      m_NExclusive.push_back(exclTOT);
    }

    return true;
  }

}
