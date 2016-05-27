/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinatoricState.hh
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

#ifndef CombinatoricState_HH
#define CombinatoricState_HH

#include "RestFrames/State.hh"

namespace RestFrames {

  class VisibleState;
  class Jigsaw;

  ///////////////////////////////////////////////
  // CombinatoricState class
  ///////////////////////////////////////////////
  class CombinatoricState : public State {
  public:
    CombinatoricState(const std::string& sname, const std::string& stitle);
    CombinatoricState();
    virtual ~CombinatoricState();
	
    static CombinatoricState& Empty();

    virtual void Clear();

    virtual void AddFrame(RestFrame& frame);

    virtual void SetParentJigsaw(Jigsaw& jigsaw);
    virtual void SetChildJigsaw(Jigsaw& jigsaw);

    virtual void Boost(const TVector3& B);
    virtual TLorentzVector GetFourVector() const;
    virtual RFCharge GetCharge() const;

    void ClearElements();
    void AddElement(VisibleState& state);
    void AddElements(const RestFrames::RFList<VisibleState>& states);
    RestFrames::RFList<VisibleState> GetElements() const;
    int GetNElements() const;

    bool ContainsElement(const State& state) const;
    bool ContainsElement(const RFKey& key) const;
    const VisibleState& GetElement(const RFKey& key) const;

  protected:
    RestFrames::RFList<VisibleState> m_Elements;
  
  private:
    void Init();

    static CombinatoricState m_Empty;
  };

}

#endif
