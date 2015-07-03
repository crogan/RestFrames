/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   InvisibleGroup.hh
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

#ifndef InvisibleGroup_HH
#define InvisibleGroup_HH

#include "RestFrames/Group.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleGroup class
  ///////////////////////////////////////////////
  class InvisibleGroup : public Group {
  public:
    InvisibleGroup(const string& sname, const string& stitle);
    virtual ~InvisibleGroup();
	
    virtual void Clear();

    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrame(RestFrame* framePtr);
    virtual bool AddJigsaw(Jigsaw& jigsaw);
    virtual bool AddJigsaw(Jigsaw* jigsawPtr);

    // Event analysis functions
    virtual void SetLabFrameFourVector(const TLorentzVector& V);
    virtual void SetLabFrameThreeVector(const TVector3& V);
    virtual TLorentzVector GetLabFrameFourVector();
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

  protected:
    virtual State* InitializeGroupState();

  private:
    TLorentzVector m_Lab_P;
    void Init();

  };

}

#endif
