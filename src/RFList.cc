#include "RestFrames/RFList.hh"
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFList class methods
  ///////////////////////////////////////////////
  template <class T>
  RFList<T>::RFList(){
   
  }
  template <class T>
  RFList<T>::~RFList<T>(){
    Clear();
  }

  template <class T>
  void RFList<T>::Clear(){
    m_Objs.clear();
  }

  template <class T>
  T* RFList<T>::Get(int i) const { 
    if(i < 0 || i >= GetN()) return nullptr;
    return m_Objs[i];
  }

  template <class T>
  bool RFList<T>::Add(T obj){
    return Add(&obj);
  }

  template <class T>
  bool RFList<T>::Add(T* objPtr){
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(objPtr)) return false;
    }
    m_Objs.push_back(objPtr);
    return true;
  }

  template <class T>
  bool RFList<T>::Add(RFList<T>* objsPtr){
    int N = objsPtr->GetN();
    double ret = true;
    for(int i = 0; i < N; i++) 
      if(!Add(objsPtr->Get(i))) ret = false;
    return ret;
  }

  template <class T>
  int RFList<T>::Remove(const T* objPtr){
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(objPtr)){
	m_Objs.erase(m_Objs.begin()+i);
	return i;
      }
    }
    return -1;
  }

  template <class T>
  void RFList<T>::Remove(const RFList<T>* objsPtr){
    int N = objsPtr->GetN();
    for(int i = 0; i < N; i++) Remove(objsPtr->Get(i));
  }

  template <class T>
  bool RFList<T>::Contains(const T* objPtr) const {
    if(!objPtr) return false;
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(objPtr)) return true;
    }
    return false;
  }

  template <class T>
  bool RFList<T>::Contains(const RFList<T>* objsPtr) const {
    if(!objsPtr) return false;
    int N = objsPtr->GetN();
    for(int i = 0; i < N; i++){
      if(!Contains(objsPtr->Get(i))) return false;
    }
    return true;
  }

  template <class T>
  bool RFList<T>::IsSame(const RFList<T>* objsPtr) const {
    if(!objsPtr) return false;
    return SizeUnion(objsPtr) == SizeIntersection(objsPtr);
  }

  template <class T>
  int RFList<T>::GetIndex(const T* objPtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(objPtr)) return i;
    }
    return -1;
  }

  template <class T>
  RFList<T>* RFList<T>::Copy() const {
    RFList<T>* objsPtr = new RFList<T>();
    int N = GetN();
    for(int i = 0; i < N; i++) objsPtr->Add(m_Objs[i]);
    return objsPtr;
  }

  template <class T>
  RFList<T>* RFList<T>::Union(const RFList<T>* objsPtr) const {
    if(!objsPtr) return nullptr;
    RFList<T>* union_objsPtr_this = Copy();
    RFList<T>* union_objsPtr_that = objsPtr->Copy();
    union_objsPtr_this->Add(union_objsPtr_that);
    delete union_objsPtr_that;
    return union_objsPtr_this;
  }

  template <class T>
  RFList<T>* RFList<T>::Intersection(const RFList<T>* objsPtr) const {
    if(!objsPtr) return nullptr;
    RFList<T>* intersection_objsPtr = new RFList<T>();
    int N = objsPtr->GetN();
    for(int i = 0; i < N; i++){
      if(Contains(objsPtr->Get(i))) intersection_objsPtr->Add(objsPtr->Get(i));
    }
    return intersection_objsPtr;
  }

  template <class T>
  RFList<T>* RFList<T>::Complement(const RFList<T>* objsPtr) const {
    if(!objsPtr) return nullptr;
    RFList<T>* complement_objsPtr = Copy();
    int N = objsPtr->GetN();
    for(int i = 0; i < N; i++){
      if(complement_objsPtr->Contains(objsPtr->Get(i))) complement_objsPtr->Remove(objsPtr->Get(i));
    }
    return complement_objsPtr;
  }

  template <class T>
  int RFList<T>::SizeUnion(const RFList<T>* objsPtr) const {
    if(!objsPtr) return 0;
    int Nthis = GetN();
    int Nthat = objsPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(!Contains(objsPtr->Get(i))) Nthis++;
    }
    return Nthis;
  }

  template <class T>
  int RFList<T>::SizeIntersection(const RFList<T>* objsPtr) const {
    if(!objsPtr) return 0;
    int Nthis = 0;
    int Nthat = objsPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(Contains(objsPtr->Get(i))) Nthis++;
    }
    return Nthis;
  }

  template <class T>
  int RFList<T>::SizeComplement(const RFList<T>* objsPtr) const {
    if(!objsPtr) return 0;
    int Nthis = GetN();
    int Nthat = objsPtr->GetN();
    for(int i = 0; i < Nthat; i++){
      if(Contains(objsPtr->Get(i))) Nthis--;
    }
    return Nthis;
  }

  template class RFList<RFBase>;
  template class RFList<RestFrame>; 
  template class RFList<Jigsaw>;
  template class RFList<Group>;
  template class RFList<State>;

}
