/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   StateList.cc
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

#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  StateList::StateList() : RFList<State>() {}

  StateList::StateList(const RFList<State>& list){
    int N = list.GetN();
    for(int i = 0; i < N; i++)
      Add(list.Get(i));
  }

  StateList::StateList(const RFList<State>* listPtr){
    if(!listPtr) return;
    int N = listPtr->GetN();
    for(int i = 0; i < N; i++)
      Add(listPtr->Get(i));
  }

   StateList::StateList(const vector<State*>& list){
    int N = list.size();
    for(int i = 0; i < N; i++)
      Add(list[i]);
  }

  StateList::StateList(const vector<State*>* listPtr){
    if(!listPtr) return;
    int N = listPtr->size();
    for(int i = 0; i < N; i++)
      Add(listPtr->at(i));
  }

  StateList::~StateList(){}

  int StateList::GetIndexFrame(const RestFrame* framePtr) const {
    int N = GetN();
    for(int i = 0; i < N; i++){
      if(m_Objs[i]->IsFrame(framePtr)) return i;
    }
    return -1;
  }

  StateList* StateList::Copy() const {
    StateList* statesPtr = new StateList(m_Objs);
    // int N = GetN();
    // for(int i = 0; i < N; i++) statesPtr->Add(m_Objs[i]);
    return statesPtr;
  }

   TLorentzVector StateList::GetFourVector() const {
    TLorentzVector V(0.,0.,0.,0.);
    int N = GetN();
    for(int i = 0; i < N; i++){
      V += m_Objs[i]->GetFourVector();
    }
    return V;
  }

  void StateList::Boost(const TVector3& B){
    int N = GetN();
    for(int i = 0; i < N; i++){
      m_Objs[i]->Boost(B);
    }
  }

}
