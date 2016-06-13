/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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
    Group(const std::string& sname, const std::string& stitle);
    Group();
    virtual ~Group();

    /// \brief Clears Group of all connections to other objects
    virtual void Clear();

    bool IsInvisibleGroup() const;
    bool IsCombinatoricGroup() const;

    GroupType GetType() const { return m_Type; }

    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrames(RestFrames::RFList<RestFrame> frames);
    virtual void AddJigsaw(Jigsaw& jigsaw);

    void RemoveFrame(RestFrame& frame);
    void RemoveFrames();
    void RemoveJigsaw(Jigsaw& jigsaw);
    void RemoveJigsaws();

    bool ContainsFrame(const RestFrame& frame) const;
    
    int GetNFrames() const;
    const RestFrames::RFList<RestFrame>& GetListFrames() const;
    const RestFrames::RFList<Jigsaw>& GetListJigsaws() const;

    static Group& Empty();
    
  protected:
    GroupType m_Type;
    State* m_GroupStatePtr;

    virtual bool InitializeAnalysis();
    virtual bool ClearEvent() = 0;
    virtual bool AnalyzeEvent() = 0;

    virtual State& InitializeParentState() = 0;
    virtual State& GetParentState() const;

    int GetNChildStates() const;
    virtual State& GetChildState(int i) const;

    State& GetChildState(const RestFrame& frame) const;
    RestFrames::RFList<State> GetChildStates(const RestFrames::RFList<RestFrame>& frames) const;

    RestFrame const& GetLabFrame() const;

  private:
    RestFrames::RFList<RestFrame> m_Frames;
    RestFrames::RFList<Jigsaw> m_Jigsaws;
    RestFrames::RFList<State> m_States;

    RestFrames::RFList<State> m_StatesToResolve;
    RestFrames::RFList<Jigsaw> m_JigsawsToUse;

    bool ResolveUnknowns();
    bool ResolveState(const State& state);
    bool InitializeJigsaw(Jigsaw& jigsaw);

    static int m_class_key;

    friend class TreePlot;
    friend class ReconstructionFrame;
    friend class LabRecoFrame;
    friend class Jigsaw;

  };
  
}

#endif
