/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   SetMassInvJigsaw.hh
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

#ifndef SetMassInvJigsaw_HH
#define SetMassInvJigsaw_HH

#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  class SetMassInvJigsaw : public InvisibleJigsaw {
  public:
    SetMassInvJigsaw(const string& sname, const string& stitle);
    SetMassInvJigsaw();
    virtual ~SetMassInvJigsaw();

    virtual void Clear();
    
    static SetMassInvJigsaw& Empty();

    virtual bool AnalyzeEvent();
    
  private:
    static SetMassInvJigsaw m_Empty;
  
  };

}

#endif
