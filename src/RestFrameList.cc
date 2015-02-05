#include "RestFrames/RestFrameList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RestFrameList class methods
  ///////////////////////////////////////////////
  RestFrameList::RestFrameList(){
   
  }

  RestFrameList::~RestFrameList(){
    Clear();
  }

  void RestFrameList::Clear(){
    m_Frames.clear();
  }

  RestFrame* RestFrameList::Get(int i) const { 
    if(i < 0 || i >= GetN()) return nullptr;
    return m_Frames[i];
  }

  void RestFrameList::Add(RestFrame* framePtr){
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(framePtr->GetKey() == m_Frames[i]->GetKey()) return;
    }
    m_Frames.push_back(framePtr);
  }

  void RestFrameList::Add(RestFrameList* framesPtr){
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++) Add(framesPtr->Get(i));
  }

  int RestFrameList::Remove(const RestFrame* framePtr){
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(framePtr->GetKey() == m_Frames[i]->GetKey()){
	m_Frames.erase(m_Frames.begin()+i);
	return i;
      }
    }
    return -1;
  }

  void RestFrameList::Remove(const RestFrameList* framesPtr){
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++) Remove(framesPtr->Get(i));
  }

  bool RestFrameList::Contains(const RestFrame* framePtr) const {
    if(!framePtr) return false;
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(framePtr->GetKey() == m_Frames[i]->GetKey()) return true;
    }
    return false;
  }

  bool RestFrameList::Contains(const RestFrameList* framesPtr) const {
    if(!framesPtr) return false;
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++){
      if(!Contains(framesPtr->Get(i))) return false;
    }
    return true;
  }

  bool RestFrameList::IsSame(const RestFrameList* framesPtr) const {
    if(!framesPtr) return false;
    return SizeUnion(framesPtr) == SizeIntersection(framesPtr);
  }

  int RestFrameList::GetIndex(const RestFrame* framePtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Frames[i]->IsSame(framePtr)) return i;
    }
    return -1;
  }

  RestFrameList* RestFrameList::Copy() const {
    RestFrameList* framesPtr = new RestFrameList();
    int N = GetN();
    for(int i = 0; i < N; i++) framesPtr->Add(m_Frames[i]);
    return framesPtr;
  }

  RestFrameList* RestFrameList::Union(const RestFrameList* framesPtr) const {
    if(!framesPtr) return nullptr;
    RestFrameList* union_framesPtr_this = Copy();
    RestFrameList* union_framesPtr_that = framesPtr->Copy();
    union_framesPtr_this->Add(union_framesPtr_that);
    delete union_framesPtr_that;
    return union_framesPtr_this;
  }

  RestFrameList* RestFrameList::Intersection(const RestFrameList* framesPtr) const {
    if(!framesPtr) return nullptr;
    RestFrameList* intersection_framesPtr = new RestFrameList();
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++){
      if(Contains(framesPtr->Get(i))) intersection_framesPtr->Add(framesPtr->Get(i));
    }
    return intersection_framesPtr;
  }

  RestFrameList* RestFrameList::Complement(const RestFrameList* framesPtr) const {
    if(!framesPtr) return nullptr;
    RestFrameList* complement_framesPtr = Copy();
    int N = framesPtr->GetN();
    for(int i = 0; i < N; i++){
      if(complement_framesPtr->Contains(framesPtr->Get(i))) complement_framesPtr->Remove(framesPtr->Get(i));
    }
    return complement_framesPtr;
  }

  int RestFrameList::SizeUnion(const RestFrameList* framesPtr) const {
    if(!framesPtr) return 0;
    int Nthis = GetN();
    int Nthat = framesPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(!Contains(framesPtr->Get(i))) Nthis++;
    }
    return Nthis;
  }

  int RestFrameList::SizeIntersection(const RestFrameList* framesPtr) const {
    if(!framesPtr) return 0;
    int Nthis = 0;
    int Nthat = framesPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(Contains(framesPtr->Get(i))) Nthis++;
    }
    return Nthis;
  }

  int RestFrameList::SizeComplement(const RestFrameList* framesPtr) const {
    if(!framesPtr) return 0;
    int Nthis = GetN();
    int Nthat = framesPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(Contains(framesPtr->Get(i))) Nthis--;
    }
    return Nthis;
  }

}
