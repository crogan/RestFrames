/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   State.hh
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

#ifndef State_HH
#define State_HH

#include "RestFrames/RFBase.hh"
#include "RestFrames/RFCharge.hh"
#include "RestFrames/Jigsaw.hh"

namespace RestFrames {

  class RestFrame;

  enum StateType { kVanillaState,   kVisibleState,
		   kInvisibleState, kCombinatoricState };

  class State : public RFBase {
  public:
    State(const std::string& sname, const std::string& stitle);
    State();
    virtual ~State();

    virtual void Clear();

    /// \brief Returns State (*StateType*) type 
    StateType GetType() const;
    
    /// \brief Is this a VisibleState? (true/false)
    bool IsVisibleState() const;
    
    /// \brief Is this an InvisibleState? (true/false)
    bool IsInvisibleState() const;

    /// \brief Is this a CombinatoricState? (true/false)
    bool IsCombinatoricState() const;

    virtual void AddFrame(RestFrame& frame) = 0;
    virtual void AddFrames(const RestFrameList& frames);

    RestFrameList const& GetListFrames() const;
    int GetNFrames() const;

    virtual bool IsFrame(const RestFrame& frame) const;
    virtual bool IsFrames(const RestFrameList& frames) const;

    virtual void SetParentJigsaw(Jigsaw& jigsaw = Jigsaw::Empty());
    virtual void SetChildJigsaw(Jigsaw& jigsaw = Jigsaw::Empty());
    virtual Jigsaw& GetParentJigsaw() const;
    virtual Jigsaw& GetChildJigsaw() const;

    virtual void Boost(const TVector3& B);
    virtual void SetFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetFourVector() const;
    virtual RFCharge GetCharge() const;

    static State& Empty();
    static StateList const& EmptyList();
    
  protected:
    StateType m_Type;
    RFCharge m_Charge;

    RestFrameList m_Frames;

  private:
    TLorentzVector m_P;
    
    Jigsaw* m_ParentJigsawPtr;
    Jigsaw* m_ChildJigsawPtr;
    
    static int m_class_key;

    static const StateList m_EmptyList;

  };

}

#endif
