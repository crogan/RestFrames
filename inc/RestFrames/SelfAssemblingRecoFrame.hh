/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   SelfAssemblingRecoFrame.hh
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

#include "RestFrames/DecayRecoFrame.hh"
#include "RestFrames/State.hh"

namespace RestFrames {

  ///////////////////////////////////////////////
  // SelfAssemblingRecoFrame class
  ///////////////////////////////////////////////
  class SelfAssemblingRecoFrame : public DecayRecoFrame {
  public:
    SelfAssemblingRecoFrame(const std::string& sname, 
			    const std::string& stitle);
    virtual ~SelfAssemblingRecoFrame();

    virtual void Clear();

    virtual bool ClearEventRecursive();
    virtual bool AnalyzeEventRecursive();

    const RestFrame& GetFrame(const RFKey& key) const;

  private:
    bool m_Body_UnAssembled;
    bool m_Mind_UnAssembled; 
    RestFrames::RFList<RestFrame> m_ChildFrames_UnAssembled;
    std::vector<RestFrames::RFList<State> > m_ChildStates_UnAssembled;

    RestFrames::RFList<State> m_VisibleStates;
    RestFrames::RFList<ReconstructionFrame> m_VisibleFrames;
    RestFrames::RFList<ReconstructionFrame> m_DecayFrames;
    int m_Nvisible;
    int m_Ndecay;

    ReconstructionFrame& GetNewDecayFrame(const std::string& sname, 
					  const std::string& stitle);
    ReconstructionFrame& GetNewVisibleFrame(const std::string& sname, 
					    const std::string& stitle);
    void ClearNewFrames();

    bool m_IsAssembled;
    bool m_IsBackedUp;
    void Disassemble();
    void Assemble();
    void AssembleRecursive(RestFrame& frame, 
			   std::vector<RestFrame*>& frames, 
			   std::vector<TLorentzVector>& Ps); 
  };

}

#endif
