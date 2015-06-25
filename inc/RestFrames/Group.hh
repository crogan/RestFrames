/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   Group.hh
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

#ifndef Group_HH
#define Group_HH

#include "RestFrames/RFBase.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {
 
  class Jigsaw;
  class State;

  enum GroupType { GInvisible, GCombinatoric };

  ///////////////////////////////////////////////
  // Group class
  ///////////////////////////////////////////////
  class Group : public RFBase {
  public:
    Group(const string& sname, const string& stitle);
    virtual ~Group();

    virtual void Clear();

    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame) = 0;
    virtual void AddFrame(RestFrame* framePtr) = 0;
    void RemoveFrame(const RestFrame* framePtr);
    bool ContainsFrame(const RestFrame* framePtr) const;
    RFList<RestFrame>* GetFrames() const;

    virtual bool AddJigsaw(Jigsaw& jigsaw) = 0;
    virtual bool AddJigsaw(Jigsaw* jigsawPtr) = 0;
    RFList<Jigsaw>* GetJigsaws() const;

    State* GetGroupState() const;
    bool GetState(const RFList<RestFrame>* framesPtr, StateList*& statesPtr);
    State* GetState(const RestFrame* framePtr) const;

    virtual bool InitializeAnalysis();
    virtual void ClearEvent() = 0;
    virtual bool AnalyzeEvent() = 0;

  protected:
    static int m_class_key;

    GroupType m_Type;

    State* m_GroupStatePtr;
    RFList<RestFrame> m_Frames;
  
    StateList m_States;
    StateList m_StatesToSplit;

    RFList<Jigsaw> m_Jigsaws;
    RFList<Jigsaw> m_JigsawsToUse;

    virtual State* InitializeGroupState();
    bool InitializeJigsaws();
    void InitializeJigsaw(Jigsaw* jigsawPtr);
    bool SplitState(const State* statePtr);

  private:
    void Init();
    int GenKey();
  };

}

#endif
