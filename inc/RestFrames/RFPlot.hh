/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFPlot.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 July
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

#ifndef RFPlot_HH
#define RFPlot_HH

#include <TCanvas.h>

#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  class RFPlot : public RFBase {
  public:
    RFPlot(const string& sname, const string& stitle);
    ~RFPlot();

    virtual void Clear();

    void WriteOutput(const string& filename);

  protected:
    TCanvas* m_CanvasPtr;
    void AddCanvas(TCanvas* can);
    void AddTObject(TObject* obj);
    int GetNCanvases();
    
  private:
    vector<TObject*> m_TObjects;
    vector<TCanvas*> m_Canvases;

  };

}

#endif
