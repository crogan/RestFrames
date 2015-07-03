/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RSelfAssemblingFrame.hh
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

#ifndef RSelfAssemblingFrame_HH
#define RSelfAssemblingFrame_HH

#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/State.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RSelfAssemblingFrame class
  ///////////////////////////////////////////////
  class RSelfAssemblingFrame : public RDecayFrame {
  public:
    RSelfAssemblingFrame(const string& sname, const string& stitle);
    virtual ~RSelfAssemblingFrame();

    virtual void Clear();

    virtual void ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

    const RestFrame* GetFrame(const RFKey& key) const;

  private:
    void Init();
    
    bool m_Body_UnAssembled;
    bool m_Mind_UnAssembled; 
    RestFrames::RFList<RestFrame> m_ChildFrames_UnAssembled;
    vector<StateList> m_ChildStates_UnAssembled;

    RestFrames::RFList<RFrame> m_VisibleFrames;
    RestFrames::RFList<RFrame> m_DecayFrames;
    int m_Nvisible;
    int m_Ndecay;

    RFrame* GetNewDecayFrame(const string& sname, const string& stitle);
    RFrame* GetNewVisibleFrame(const string& sname, const string& stitle);
    void ClearNewFrames();

    bool m_IsAssembled;
    bool m_IsBackedUp;
    void Disassemble();
    void Assemble();
    void AssembleRecursive(RestFrame* framePtr, vector<RestFrame*>& frames, vector<TLorentzVector>& Ps); 

    //const RestFrame* GetFrame(const State* statePtr) const;
  };

}

#endif
