/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   SetRapidityInvJigsaw.hh
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

#ifndef SetRapidityInvJigsaw_HH
#define SetRapidityInvJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  class SetRapidityInvJigsaw : public InvisibleJigsaw {
  public:
    SetRapidityInvJigsaw(const string& sname, const string& stitle);
    SetRapidityInvJigsaw();
    virtual ~SetRapidityInvJigsaw();

    virtual string GetLabel() const { return "Set Invisible Rapidity"; }

    virtual void SetAxis(const TVector3& axis);

    virtual bool AnalyzeEvent();

  protected:
    TVector3 m_Axis;
  
  };

}

#endif
