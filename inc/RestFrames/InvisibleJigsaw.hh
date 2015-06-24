/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleJigsaw class
  ///////////////////////////////////////////////

  class InvisibleJigsaw : public Jigsaw {
  public:
    InvisibleJigsaw(const string& sname, const string& stitle, int Ninv, int Nvis);
    virtual ~InvisibleJigsaw();

    void AddVisibleFrame(RestFrame& framePtr, int i = 0);
    void AddVisibleFrame(RestFrame* framePtr, int i = 0);
    void AddVisibleFrame(RFList<RestFrame>* framesPtr, int i = 0);
    void AddInvisibleFrame(RestFrame& framePtr, int i = 0);
    void AddInvisibleFrame(RestFrame* framePtr, int i = 0);
    void AddInvisibleFrame(RFList<RestFrame>* framesPtr, int i = 0);

    virtual double GetMinimumMass();
    virtual bool InitializeDependancyJigsaws();
    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsaws);

    virtual bool InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr);

    virtual bool IsSoundBody() const ;
    virtual void SetGroup(Group* groupPtr);

  protected:
    virtual State* NewOutputState();

  private:
    void Init(int Ninv, int Nvis);
    int m_Nvis;
    int m_Ninv;  
  
  };

}

#endif
