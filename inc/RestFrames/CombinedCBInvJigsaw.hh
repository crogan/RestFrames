/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   CombinedCBInvJigsaw.hh
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

#ifndef CombinedCBInvJigsaw_HH
#define CombinedCBInvJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

namespace RestFrames {

  class CombinedCBInvJigsaw : public InvisibleJigsaw {
  public:
    CombinedCBInvJigsaw(const std::string& sname, 
			const std::string& stitle,
			int N_CBjigsaw);
    CombinedCBInvJigsaw();
    ~CombinedCBInvJigsaw();

    virtual std::string GetLabel() const {
      return "Combined Contra-boost Inv.";
    }

    void AddVisibleFrame(const RestFrame& frame,
			 int ijigsaw, int j);
    void AddVisibleFrames(const ConstRestFrameList& frames,
			  int ijigsaw, int j);
    void AddInvisibleFrame(const RestFrame& frame,
			   int ijigsaw, int j);
    void AddInvisibleFrames(const ConstRestFrameList& frames,
			    int ijigsaw, int j);

    void AddJigsaw(const ContraBoostInvJigsaw& jigsaw, int ijigsaw);
    
    virtual double GetMinimumMass() const;
    
    virtual bool AnalyzeEvent();

  private:
    const int m_NCB;
    double ContraBoostInvJigsaw::GetCBMinimumMass(int i) const;

  };

}

#endif
