/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleJigsaw.hh
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

#ifndef InvisibleJigsaw_HH
#define InvisibleJigsaw_HH

#include "RestFrames/Jigsaw.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/InvisibleState.hh"

namespace RestFrames {

  class InvisibleJigsaw : public Jigsaw {
  public:
    InvisibleJigsaw(const std::string& sname, const std::string& stitle, 
		    int Ninv, int Nvis);
    InvisibleJigsaw();
    virtual ~InvisibleJigsaw();

    virtual void Clear();

    virtual void SetGroup(Group& group = Group::Empty());
    virtual InvisibleGroup& GetGroup() const;

    virtual void AddVisibleFrame(RestFrame& frame, int i = 0);
    virtual void AddVisibleFrames(const RestFrames::RFList<RestFrame>& frames, int i = 0);
    virtual void AddInvisibleFrame(RestFrame& frame, int i = 0);
    virtual void AddInvisibleFrames(const RestFrames::RFList<RestFrame>& frames, int i = 0);

    virtual double GetMinimumMass() const;

    static InvisibleJigsaw& Empty();

  protected:
    virtual bool IsSoundBody() const;
    virtual InvisibleState& GetNewChildState();

    virtual void SetParentState(State& state = State::Empty());
    virtual InvisibleState& GetParentState() const;
    virtual InvisibleState& GetChildState(int i) const;

    virtual void FillInvisibleMassJigsawDependancies(RestFrames::RFList<Jigsaw>& jigsaws) const;

    virtual bool InitializeDependancyJigsaws();
    virtual bool InitializeJigsawExecutionList(RestFrames::RFList<Jigsaw>& exec_jigsaws);

  private:
    int m_Nvis;
    int m_Ninv;  
  
  };

}

#endif
