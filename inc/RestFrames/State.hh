/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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
#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  enum StateType { kVanillaState,   kVisibleState,
		   kInvisibleState, kCombinatoricState };

  class State : public RFBase {
  public:
    State();
    State(const RFKey& key);
    virtual ~State();

    virtual void Clear();

    static State& Empty();

    /// \brief Returns State (*StateType*) type 
    StateType GetType() const;
    
    /// \brief Is this a VisibleState? (yes/no)
    bool IsVisibleState() const;
    
    /// \brief Is this an InvisibleState? (yes/no)
    bool IsInvisibleState() const;

    /// \brief Is this a CombinatoricState? (yes/no)
    bool IsCombinatoricState() const;

    virtual void AddFrame(RestFrame& frame) = 0;
    virtual void AddFrames(const RestFrames::RFList<RestFrame>& frames);

    RestFrames::RFList<RestFrame> const& GetListFrames() const;
    int GetNFrames() const;

    virtual bool IsFrame(const RestFrame& frame) const;
    virtual bool IsFrames(const RestFrames::RFList<RestFrame>& frames) const;

    virtual void SetParentJigsaw(Jigsaw& jigsaw = Jigsaw::Empty()) = 0;
    virtual void SetChildJigsaw(Jigsaw& jigsaw = Jigsaw::Empty()) = 0;
    virtual Jigsaw const& GetParentJigsaw() const;
    virtual Jigsaw const& GetChildJigsaw() const;

    virtual void Boost(const TVector3& B);
    virtual void SetFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetFourVector() const; 

    void FillGroupJigsawDependancies(RestFrames::RFList<Jigsaw>& jigsaws) const;
    void FillStateJigsawDependancies(RestFrames::RFList<Jigsaw>& jigsaws) const;
	
  protected:
    static int m_class_key;

    StateType m_Type;

    TLorentzVector m_P;
    RestFrames::RFList<RestFrame> m_Frames;

    Jigsaw* m_ParentJigsawPtr;
    Jigsaw* m_ChildJigsawPtr;

  private:
    void Init();
    int GenKey();
  };

}

#endif
