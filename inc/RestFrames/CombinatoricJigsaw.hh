/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricJigsaw.hh
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

#ifndef CombinatoricJigsaw_HH
#define CombinatoricJigsaw_HH

#include "RestFrames/Jigsaw.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/CombinatoricState.hh"

namespace RestFrames {

  class CombinatoricJigsaw : public Jigsaw {
  public:
    //constructor and destructor
    CombinatoricJigsaw(const std::string& sname, const std::string& stitle);
    CombinatoricJigsaw();
    virtual ~CombinatoricJigsaw();

    virtual void Clear();

    static CombinatoricJigsaw& Empty();

    virtual void SetGroup(Group& group = Group::Empty());
    virtual CombinatoricGroup& GetGroup() const;

    virtual void AddFrame(RestFrame& frame, int i = 0);
    virtual void AddFrames(const RestFrameList& frames, int i = 0);

  protected:
    virtual bool InitializeCombinatoric();
    virtual bool LoopCombinatoric();
    virtual double EvaluateMetric() const = 0;

    virtual bool AnalyzeEvent();

    VisibleStateList m_InputStates;
    std::map<const State*, int>  m_NForChild;
    std::map<const State*, bool> m_NExclusive;

    virtual void SetParentState(State& state = State::Empty());
    virtual CombinatoricState& GetParentState() const;
    
    virtual CombinatoricState& GetChildState(int i) const;

    virtual CombinatoricState& GetNewChildState();
 
    virtual bool InitializeJigsawExecutionList(JigsawList& exec_jigsaws);

    JigsawList m_ExecuteJigsaws;
    bool ExecuteDependancyJigsaws();
    
  };

}

#endif
