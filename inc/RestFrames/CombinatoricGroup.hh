/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricGroup.hh
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

#ifndef CombinatoricGroup_HH
#define CombinatoricGroup_HH

#include "RestFrames/Group.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class Group;

  ///////////////////////////////////////////////
  // CombinatoricGroup class
  ///////////////////////////////////////////////
  class CombinatoricGroup : public Group {
  public:
    CombinatoricGroup(const string& sname, const string& stitle);
    virtual ~CombinatoricGroup();

    virtual void Clear();

    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrame(RestFrame* framePtr);
    virtual void SetNElementsForFrame(const RestFrame& frame, 
				      int N, bool exclusive_N = false);
    virtual void SetNElementsForFrame(const RestFrame* framePtr, 
				      int N, bool exclusive_N = false);
    virtual void GetNElementsForFrame(const RestFrame* framePtr, 
				      int& N, bool& exclusive_N);

    virtual bool AddJigsaw(Jigsaw& jigsaw);
    virtual bool AddJigsaw(Jigsaw* jigsawPtr);

    // Event analysis functions
    void ClearFourVectors();
    RFKey AddLabFrameFourVector(const TLorentzVector& V);
    int GetNFourVectors() const;

    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

    const RestFrame* GetFrame(const RFKey& key);
    TLorentzVector GetLabFrameFourVector(const RFKey& key);
    int GetNElementsInFrame(const RestFrame& frame);
    int GetNElementsInFrame(const RestFrame* framePtr);
	
  protected:
    StateList m_StateElements;
    vector<int> m_NElementsForFrame;
    vector<bool> m_NExclusiveElementsForFrame; 

    virtual State* InitializeGroupState();
    void ClearElements();
    void AddElement(State* statePtr);
    void AddElement(State& state);
    int GetNElements() const;

  private:
    void Init();
    State* GetNewState();
    StateList m_InitStates;
  };

}

#endif
