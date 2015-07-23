/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   Jigsaw.hh
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

#ifndef Jigsaw_HH
#define Jigsaw_HH

#include "RestFrames/RFBase.hh"
#include "RestFrames/Group.hh"

using namespace std;

namespace RestFrames {

  enum JigsawType { kVanillaJigsaw, kInvisibleJigsaw, 
		    kCombinatoricJigsaw };

  ///////////////////////////////////////////////
  // Jigsaw class
  ///////////////////////////////////////////////
  class Jigsaw : public RFBase {
  public:
    Jigsaw(const string& sname, const string& stitle);
    Jigsaw();
    
    virtual ~Jigsaw();

    virtual void Clear();

    static Jigsaw& Empty();

    JigsawType GetType() const { return m_Type; }
  
    /// \brief String of information associated with Jigsaw
    virtual string PrintString(LogType type = LogVerbose) const;

    bool IsInvisibleJigsaw() const;
    bool IsCombinatoricJigsaw() const;

     virtual bool AnalyzeEvent() = 0;

    virtual void SetGroup(Group& group = Group::Empty());
    virtual Group& GetGroup() const;

    virtual void SetParentState(State& state);
    virtual State& GetParentState() const;

    int GetNChildStates() const;
    int GetNDependancyStates() const;

    bool CanResolve(const State& state) const;
    bool CanResolve(const RestFrames::RFList<RestFrame>& frames) const;
  
    virtual bool InitializeTree();
    virtual bool InitializeAnalysis();

    virtual bool InitializeDependancyJigsaws();
    virtual bool InitializeJigsawExecutionList(RestFrames::RFList<Jigsaw>& exec_jigsaws) = 0;

    virtual int GetNChildren() const;
    virtual State& GetChildState(int i) const;
    virtual RestFrames::RFList<State> GetChildStates() const;

    virtual RestFrames::RFList<RestFrame> GetParentFrames() const;
    virtual RestFrames::RFList<RestFrame> GetChildFrames(int i) const;

    bool DependsOnJigsaw(const Jigsaw& jigsaw) const;
    virtual void FillGroupJigsawDependancies(RestFrames::RFList<Jigsaw>& jigsaws);
    virtual void FillStateJigsawDependancies(RestFrames::RFList<Jigsaw>& jigsaws);
  
  protected:
    JigsawType m_Type;
    virtual bool IsSoundBody() const;

    virtual State& GetNewChildState() = 0;

    void AddChildFrame(RestFrame& frame, int i = 0);
    void AddDependancyFrame(RestFrame& frame, int i = 0);

    vector<RestFrames::RFList<RestFrame> > m_ChildFrames;

    vector<RestFrames::RFList<State> > m_DependancyStates;
    vector<RestFrames::RFList<RestFrame> > m_DependancyFrames;
    RestFrames::RFList<Jigsaw> m_DependancyJigsaws;

  private:
    Group* m_GroupPtr;
    State* m_ParentStatePtr;

    RestFrames::RFList<State> m_ChildStates;

    static int m_class_key;

  };

}

#endif
