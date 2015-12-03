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

using namespace std;

namespace RestFrames {
 
  class Jigsaw;
  class RestFrame;

  enum GroupType { kVanillaGroup, kInvisibleGroup, 
		   kCombinatoricGroup };

  ///////////////////////////////////////////////
  // Group class
  ///////////////////////////////////////////////
  class Group : public RFBase {
  public:
    Group(const string& sname, const string& stitle);
    Group();
    virtual ~Group();

    /// \brief Clears Group of all connections to other objects
    virtual void Clear();

    static Group& Empty();

    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrames(RestFrames::RFList<RestFrame> frames);
    virtual void AddJigsaw(Jigsaw& jigsaw);

    void RemoveFrame(const RestFrame& frame);
    void RemoveJigsaw(const Jigsaw& jigsaw);

    bool ContainsFrame(const RestFrame& frame) const;
    
    int GetNFrames() const;
    const RestFrames::RFList<RestFrame>& GetListFrames() const;
    const RestFrames::RFList<Jigsaw>& GetListJigsaws() const;

    virtual State& GetParentState() const;
    State& GetChildState(const RestFrame& frame) const;
    RestFrames::RFList<State> GetChildStates(const RestFrames::RFList<RestFrame>& frames) const;

    RestFrame const& GetLabFrame() const;

    virtual bool InitializeAnalysis();
    virtual bool ClearEvent() = 0;
    virtual bool AnalyzeEvent() = 0;

  protected:
    GroupType m_Type;
    State* m_GroupStatePtr;

    int GetNChildStates() const;
    virtual State& GetChildState(int i) const;

    virtual State& InitializeParentState() = 0;

    bool ResolveUnknowns();
    bool ResolveState(const State& state);
    void InitializeJigsaw(Jigsaw& jigsaw);

  private:
    RestFrames::RFList<RestFrame> m_Frames;
    RestFrames::RFList<Jigsaw> m_Jigsaws;
    RestFrames::RFList<State> m_States;

    RestFrames::RFList<State> m_StatesToResolve;
    RestFrames::RFList<Jigsaw> m_JigsawsToUse;

    static int m_class_key;

  };
  
}

#endif
