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

using namespace std;

namespace RestFrames {

  class RestFrame;
  class Jigsaw;

  ///////////////////////////////////////////////
  // State class
  ///////////////////////////////////////////////

  enum StateType { SVanilla, SInvisible, SCombinatoric };

  class State : public RFBase {
  public:
    State();
    virtual ~State();

    void Clear();

    virtual void AddFrame(RestFrame* framePtr);
    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrame(const RestFrames::RFList<RestFrame>* framesPtr);
    virtual void AddFrame(const RestFrames::RFList<RestFrame>& frames);
    void ClearFrames();
    RFList<RestFrame>* GetFrames() const;
    RestFrame* GetFrame() const;
    int GetNFrames() const { return m_Frames.GetN(); }

    bool IsFrame(const RestFrame* framePtr) const;
    bool IsFrames(const RestFrames::RFList<RestFrame>* framesPtr) const;

    void SetParentJigsaw(Jigsaw* jigsawPtr){ m_ParentJigsawPtr = jigsawPtr; }
    void SetChildJigsaw(Jigsaw* jigsawPtr){ m_ChildJigsawPtr = jigsawPtr; }
    Jigsaw* GetParentJigsaw() const { return m_ParentJigsawPtr; }
    Jigsaw* GetChildJigsaw() const { return m_ChildJigsawPtr; }

    virtual void Boost(const TVector3& B);
    void SetFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetFourVector() const; 

    virtual void FillGroupJigsawDependancies(RestFrames::RFList<Jigsaw>* jigsawsPtr) const;
    virtual void FillStateJigsawDependancies(RestFrames::RFList<Jigsaw>* jigsawsPtr) const;
	
  protected:
    static int m_class_key;

    StateType m_Type;

    TLorentzVector m_P;
    RestFrames::RFList<RestFrame> m_Frames;

    Jigsaw *m_ParentJigsawPtr;
    Jigsaw *m_ChildJigsawPtr;

  private:
    void Init();
    int GenKey();
  };

  typedef const State* GroupElementID;

}

#endif
