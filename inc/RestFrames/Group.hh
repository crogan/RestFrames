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

  enum GroupType { GInvisible, GCombinatoric };

  ///////////////////////////////////////////////
  // Group class
  ///////////////////////////////////////////////
  class Group : public RFBase {
  public:
    Group(const string& sname, const string& stitle);
    Group(const RFKey& key);
    Group();
    virtual ~Group();

    /// \brief Clears Group of all connections to other objects
    virtual void Clear();

    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame){ }
    void RemoveFrame(const RestFrame& frame);
    bool ContainsFrame(const RestFrame& frame) const;
    RestFrames::RFList<RestFrame> GetFrames() const;

    virtual bool AddJigsaw(Jigsaw& jigsaw){ return false; }
    void RemoveJigsaw(const Jigsaw& jigsaw);
    RestFrames::RFList<Jigsaw> GetJigsaws() const;

    State const& GetGroupState() const;
    StateList GetStates(const RestFrames::RFList<RestFrame>& frames) const;
    State& GetState(const RestFrame& frame) const;

    virtual bool InitializeAnalysis();
    virtual void ClearEvent(){ };
    virtual bool AnalyzeEvent(){ return false; };

  protected:
    static int m_class_key;

    GroupType m_Type;

    State* m_GroupStatePtr;
    RestFrames::RFList<RestFrame> m_Frames;
  
    StateList m_States;
    StateList m_StatesToSplit;

    RestFrames::RFList<Jigsaw> m_Jigsaws;
    RestFrames::RFList<Jigsaw> m_JigsawsToUse;

    virtual State& InitializeGroupState();
    bool InitializeJigsaws();
    void InitializeJigsaw(Jigsaw& jigsaw);

    bool SplitState(const State& state);

  private:
    void Init();
    int GenKey();
  };

  extern Group g_Group;
  
}

#endif
