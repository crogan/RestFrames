/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   StateList.hh
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

#ifndef StateList_HH
#define StateList_HH

#include "RestFrames/RFList.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class State;

  class StateList : public RFList<State> {
  public:
    StateList();
    StateList(const RFList<State>& list);
    StateList(const RFList<State>* listPtr);
    StateList(const vector<State*>& list);
    StateList(const vector<State*>* listPtr);
    ~StateList();

    // bool Add(const StateList* objsPtr);
    // bool Add(const StateList& objs);

    StateList* Copy() const;

    int GetIndexFrame(const RestFrame* framePtr) const;
    TLorentzVector GetFourVector() const;
    void Boost(const TVector3& B);

  };

}

#endif
