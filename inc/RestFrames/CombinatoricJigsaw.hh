/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
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

using namespace std;

namespace RestFrames {

  class CombinatoricState;

  ///////////////////////////////////////////////
  // CombinatoricJigsaw class
  ///////////////////////////////////////////////

  class CombinatoricJigsaw : public Jigsaw {
  public:
    //constructor and destructor
    CombinatoricJigsaw(const string& sname, const string& stitle);
    virtual ~CombinatoricJigsaw();

    void Clear();

    virtual void AddFrame(RestFrame& frame, int i = 0);
    virtual void AddFrame(RestFrame* framePtr, int i = 0);
    virtual void AddFrame(RFList<RestFrame>* framesPtr, int i = 0);

    virtual bool InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr);
  
  protected:
    virtual State* NewOutputState();

    RFList<Jigsaw> m_ExecuteJigsaws;
    bool ExecuteDependancyJigsaws();

    vector<State*> m_Inputs;
    vector<CombinatoricState*> m_Outputs;
    vector<int> m_NForOutput;
    vector<bool> m_NExclusive;
    virtual bool InitializeEvent();
    
  private:
    void Init();
  };

}

#endif
