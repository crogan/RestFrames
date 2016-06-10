/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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
#include "RestFrames/VisibleState.hh"
#include "RestFrames/CombinatoricState.hh"

namespace RestFrames { 

  class CombinatoricGroup : public Group {
  public:
    CombinatoricGroup(const std::string& sname, const std::string& stitle);
    CombinatoricGroup();
    virtual ~CombinatoricGroup();

    virtual void Clear();

    static CombinatoricGroup& Empty();

    virtual void AddFrame(RestFrame& frame);
    virtual void AddJigsaw(Jigsaw& jigsaw);

    virtual void SetNElementsForFrame(const RestFrame& frame, 
				      int N, bool exclusive_N = false);
    virtual void GetNElementsForFrame(const RestFrame& frame, 
				      int& N, bool& exclusive_N) const;

    // Event analysis functions
    RFKey AddLabFrameFourVector(const TLorentzVector& V,
				const RFCharge& charge = RFCharge());

    RestFrame const& GetFrame(const RFKey& key) const;

    TLorentzVector GetLabFrameFourVector(const RFKey& key) const;

    int GetNElementsInFrame(const RestFrame& frame) const;
	
  protected:
    RestFrames::RFList<VisibleState>         m_Elements;
    mutable std::map<const RestFrame*, int>  m_NElementsForFrame;
    mutable std::map<const RestFrame*, bool> m_NExclusiveElementsForFrame; 

    virtual bool ClearEvent();
    virtual bool AnalyzeEvent();

    virtual CombinatoricState& InitializeParentState();
    virtual CombinatoricState& GetChildState(int i) const;

    virtual CombinatoricState& GetParentState() const;

  private:
    VisibleState& GetNewElement();
    RestFrames::RFList<VisibleState> m_InitStates;

    static CombinatoricGroup m_Empty;

  };

}

#endif
