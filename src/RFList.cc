/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFList.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/RFList.hh"
#include "RestFrames/ReconstructionFrame.hh"
#include "RestFrames/ResonanceGenFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/Group.hh"
#include "RestFrames/VisibleState.hh"
#include "RestFrames/InvisibleState.hh"
#include "RestFrames/CombinatoricState.hh"

using namespace std;

namespace RestFrames {

  template <class T, class Derived>
  void RFListBase<T,Derived>::Clear(){
    m_Objs.clear();
  }

  template <class T, class Derived>
  T& RFListBase<T,Derived>::Get(const RFKey& key) const{
    int N = GetN();
    for(int i = 0; i < N; i++)
      if(m_Objs[i]->IsSame(key)) return *m_Objs[i];
    
    return T::Empty();
  }

  template <class T, class Derived>
  T& RFListBase<T,Derived>::Get(int i) const { 
    if(i < 0 || i >= GetN()) return T::Empty();
    return *m_Objs[i];
  }

  template <class T, class Derived>
  bool RFListBase<T,Derived>::Add(T& obj){
    if(!obj) return false;
    int N = GetN();
    for(int i = 0; i < N; i++)
      if(m_Objs[i]->IsSame(obj)) return false;
    m_Objs.push_back(&obj);
    return true;
  }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::Add(const Derived& objs){
  //   int N = objs.GetN();
  //   double ret = true;
  //   for(int i = 0; i < N; i++) 
  //     if(!Add(objs.Get(i))) ret = false;
  //   return ret;
  // }

  template <class T, class Derived>
  int RFListBase<T,Derived>::Remove(const T& obj){
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(obj)){
	m_Objs.erase(m_Objs.begin()+i);
	return i;
      }
    }
    return -1;
  }

  // template <class T, class Derived>
  // void RFListBase<T,Derived>::Remove(const Derived& objs){
  //   int N = objs.GetN();
  //   for(int i = 0; i < N; i++) Remove(objs.Get(i));
  // }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::Contains(const RFKey& key) const {
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_Objs[i]->IsSame(key)) return true;
  //   }
  //   return false;
  // }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::Contains(const T& obj) const {
  //   int N = GetN();
  //   for(int i = 0; i < N; i++){
  //     if(m_Objs[i]->IsSame(obj)) return true;
  //   }
  //   return false;
  // }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::Contains(const Derived& objs) const {
  //   int N = objs.GetN();
  //   for(int i = 0; i < N; i++){
  //     if(!Contains(objs.Get(i))) return false;
  //   }
  //   return true;
  // }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::IsSame(const Derived& objs) const {
  //   return SizeUnion(objs) == SizeIntersection(objs);
  // }

  template <class T, class Derived>
  int RFListBase<T,Derived>::GetIndex(const RFKey& key) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(key)) return i;
    }
    return -1;
  }

  template <class T, class Derived>
  int RFListBase<T,Derived>::GetIndex(const T& obj) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsSame(obj)) return i;
    }
    return -1;
  }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::Copy() const {
  //   Derived objs;
  //   int N = GetN();
  //   for(int i = 0; i < N; i++) objs.Add(*m_Objs[i]);
  //   return objs;
  // }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::Union(const Derived& objs) const {
  //   Derived objs_this = Copy();
  //   objs_this.Add(objs);
  //   return objs_this;
  // }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::Intersection(const Derived& objs) const {
  //   Derived inter; 
  //   int N = objs.GetN();
  //   for(int i = 0; i < N; i++){
  //     if(Contains(objs.Get(i))) inter.Add(objs.Get(i));
  //   }
  //   return inter;
  // }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::Complement(const Derived& objs) const {
  //   Derived comp = Copy();
  //   int N = objs.GetN();
  //   for(int i = 0; i < N; i++)
  //     if(comp.Contains(objs.Get(i))) 
  // 	comp.Remove(objs.Get(i));
    
  //   return comp;
  // }

  // template <class T, class Derived>
  // int RFListBase<T,Derived>::SizeUnion(const Derived& objs) const {
  //   int Nthis = GetN();
  //   int Nthat = objs.GetN();
  //   for(int i = 0; i < Nthat; i++)
  //     if(!Contains(objs.Get(i))) Nthis++;
    
  //   return Nthis;
  // }

  // template <class T, class Derived>
  // int RFListBase<T,Derived>::SizeIntersection(const Derived& objs) const {
  //   int Nthis = 0;
  //   int Nthat = objs.GetN();
  //   for(int i = 0; i < Nthat; i++)
  //     if(Contains(objs.Get(i))) Nthis++;
    
  //   return Nthis;
  // }

  // template <class T, class Derived>
  // int RFListBase<T,Derived>::SizeComplement(const Derived& objs) const {
  //   int Nthis = GetN();
  //   int Nthat = objs.GetN();
  //   for(int i = 0; i < Nthat; i++)
  //     if(Contains(objs.Get(i))) Nthis--;
    
  //   return Nthis;
  // }

  // operator overload methods
  // template <class T, class Derived>
  // void RFListBase<T,Derived>::operator=(const Derived& objs){ 
  //   Clear();
  //   Add(objs);
  //   return;
  // }

  template <class T, class Derived>
  T& RFListBase<T,Derived>::operator[](int i) const {
    return Get(i);
  }

  template <class T, class Derived>
  bool RFListBase<T,Derived>::operator==(const T& obj) const {
    if(GetN() != 1) return false;
    return obj == *m_Objs[0];
  }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::operator==(const Derived& objs) const { 
  //   return IsSame(objs);
  // }

  template <class T, class Derived>
  bool RFListBase<T,Derived>::operator!=(const T& obj) const {
    if(GetN() != 1) return true;
    return !(obj == *m_Objs[0]);
  }

  // template <class T, class Derived>
  // bool RFListBase<T,Derived>::operator!=(const Derived& objs) const { 
  //   return !IsSame(objs);
  // }

  template <class T, class Derived>
  Derived RFListBase<T,Derived>::operator+(T& obj) const { 
    Derived list;
    list.Add((Derived&)*this);
    list.Add(obj);
    return list;
  }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::operator+(const Derived& objs) const {
  //   Derived list;
  //   list.Add((Derived&)*this);
  //   list.Add(objs);
  //   return list;
  // }

  template <class T, class Derived>
  Derived RFListBase<T,Derived>::operator-(const T& obj) const { 
    Derived list;
    list.Add((Derived&)*this);
    list.Remove(obj);
    return list;
  }

  // template <class T, class Derived>
  // Derived RFListBase<T,Derived>::operator-(const Derived& objs) const {
  //   Derived list;
  //   list.Add((Derived&)*this);
  //   list.Remove(objs);
  //   return list;
  // }

  template <class T, class Derived>
  Derived& RFListBase<T,Derived>::operator+=(T& obj){ 
    Add(obj);
    return (Derived&)*this;
  }

  // template <class T, class Derived>
  // Derived& RFListBase<T,Derived>::operator+=(const Derived& objs){
  //   Add(objs);
  //   return (Derived&)*this;
  // }

  template <class T, class Derived>
  Derived& RFListBase<T,Derived>::operator-=(const T& obj){ 
    Remove(obj);
    return (Derived&)*this;
  }

  // template <class T, class Derived>
  // Derived& RFListBase<T,Derived>::operator-=(const Derived& objs){
  //   Remove(objs);
  //   return (Derived&)*this;
  // }

  // RestFrame methods
  double RFList<RestFrame>::GetMass() const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetFourVector();
    return V.M();
  }

  TLorentzVector RFList<RestFrame>::GetFourVector() const {
    return GetFourVector(RestFrame::Empty());
  }

  TLorentzVector RFList<RestFrame>::GetFourVector(const RestFrame& frame) const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetFourVector(frame);
    return V;
  }

  TLorentzVector RFList<RestFrame>::GetVisibleFourVector() const {
    return GetVisibleFourVector(RestFrame::Empty());
  }

  TLorentzVector RFList<RestFrame>::GetVisibleFourVector(const RestFrame& frame) const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetVisibleFourVector(frame);
    return V;
  }

  TLorentzVector RFList<RestFrame>::GetInvisibleFourVector() const {
    return GetInvisibleFourVector(RestFrame::Empty());
  }

  TLorentzVector RFList<RestFrame>::GetInvisibleFourVector(const RestFrame& frame) const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetInvisibleFourVector(frame);
    return V;
  }

  double RFList<RestFrame>::GetEnergy(const RestFrame& frame) const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetFourVector(frame);
    return V.E();
  }

  double RFList<RestFrame>::GetMomentum(const RestFrame& frame) const {
    int N = m_Objs.size();
    TLorentzVector V(0.,0.,0.,0.);
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetFourVector(frame);
    return V.P();
  }
  
  // State methods
  State& RFList<State>::GetFrame(const RestFrame& frame) const {
    int N = GetN();
    for(int i = 0; i < N; i++)
      if(m_Objs[i]->IsFrame(frame)) return *m_Objs[i];
    
    return State::Empty();
  }

  TLorentzVector RFList<State>::GetFourVector() const {
    TLorentzVector V(0.,0.,0.,0.);
    int N = GetN();
    for(int i = 0; i < N; i++)
      V += m_Objs[i]->GetFourVector();
    
    return V;
  }

  void RFList<State>::Boost(const TVector3& B) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      m_Objs[i]->Boost(B);
    }
  }

  template class RFList<RFBase>;
  template class RFList<RestFrame>; 
  template class RFList<ReconstructionFrame>;
  template class RFList<GeneratorFrame>;
  template class RFList<ResonanceGenFrame>; 
  template class RFList<Jigsaw>;
  template class RFList<Group>;
  template class RFList<State>;
  template class RFList<VisibleState>;
  template class RFList<InvisibleState>;
  template class RFList<CombinatoricState>;

  template class RFListBase<RFBase,RFList<RFBase> >;
  template class RFListBase<RestFrame,RFList<RestFrame> >; 
  template class RFListBase<ReconstructionFrame,RFList<ReconstructionFrame> >; 
  template class RFListBase<GeneratorFrame,RFList<GeneratorFrame> >;
  template class RFListBase<ResonanceGenFrame,RFList<ResonanceGenFrame> >; 
  template class RFListBase<Jigsaw,RFList<Jigsaw> >;
  template class RFListBase<Group,RFList<Group> >;
  template class RFListBase<State,RFList<State> >;
  template class RFListBase<VisibleState,RFList<VisibleState> >;
  template class RFListBase<InvisibleState,RFList<InvisibleState> >;
  template class RFListBase<CombinatoricState,RFList<CombinatoricState> >;

}
