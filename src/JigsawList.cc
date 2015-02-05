#include "RestFrames/JigsawList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // JigsawList class methods
  ///////////////////////////////////////////////
  JigsawList::JigsawList(){ }

  JigsawList::~JigsawList(){
    Clear();
  }

  void JigsawList::Clear(){
    m_Jigsaws.clear();
  }

  Jigsaw* JigsawList::Get(int i) const { 
    if(i < 0 || i >= GetN()) return nullptr;
    return m_Jigsaws[i];
  }

  bool JigsawList::Add(Jigsaw& jigsaw){
    return Add(&jigsaw);
  }
  bool JigsawList::Add(Jigsaw* jigsawPtr){
    if(!jigsawPtr) return false;
    int N = GetN();
    for(int i = 0; i < N; i++)
      if(jigsawPtr->IsSame(m_Jigsaws[i])) return false;
    m_Jigsaws.push_back(jigsawPtr);
    return true;
  }

  bool JigsawList::Add(JigsawList* jigsawsPtr){
    if(!jigsawsPtr) return false;
    bool ret = true;
    int N = jigsawsPtr->GetN();
    for(int i = 0; i < N; i++){
      if(!Add(jigsawsPtr->Get(i))) ret = false;
    }
    return ret;
  }

  void JigsawList::Remove(const Jigsaw* jigsawPtr){
    if(!jigsawPtr) return;
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(jigsawPtr->GetKey() == m_Jigsaws[i]->GetKey()){
	m_Jigsaws.erase(m_Jigsaws.begin()+i);
	return;
      }
    }
  }

  void JigsawList::Remove(const JigsawList* jigsawsPtr){
    if(!jigsawsPtr) return;
    int N = jigsawsPtr->GetN();
    for(int i = 0; i < N; i++) Remove(jigsawsPtr->Get(i));
  }

  bool JigsawList::Contains(const Jigsaw *jigsawPtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(jigsawPtr->GetKey() == m_Jigsaws[i]->GetKey()) return true;
    }
    return false;
  }

  int JigsawList::GetIndex(const Jigsaw* jigsawPtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Jigsaws[i]->IsSame(jigsawPtr)) return i;
    }
    return -1;
  }

  JigsawList* JigsawList::Copy() const {
    JigsawList* jigsawsPtr = new JigsawList();
    int N = GetN();
    for(int i = 0; i < N; i++) jigsawsPtr->Add(m_Jigsaws[i]);
    return jigsawsPtr;
  }

}
