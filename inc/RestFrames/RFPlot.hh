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
#include <TColor.h>

#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  class RFPlot : public RFBase {
  public:
    RFPlot(const string& sname, const string& stitle);
    ~RFPlot();

    virtual void Clear();

    void WriteOutput(const string& filename);

    static void SetStyle(bool invert_colors = false);
    static void SetZPalette(bool invert_colors = false);
    
    static const TColor rf_blue0;
    static const TColor rf_blue1;
    static const TColor rf_blue2;
    static const TColor rf_blue3;
    static const TColor rf_blue4;
    static const TColor rf_green0;
    static const TColor rf_green1;
    static const TColor rf_green2;
    static const TColor rf_green3;
    static const TColor rf_green4;
    static const TColor rf_red0;
    static const TColor rf_red1;
    static const TColor rf_red2;
    static const TColor rf_red3;
    static const TColor rf_red4;
    static const TColor rf_yellow0;
    static const TColor rf_yellow1;
    static const TColor rf_yellow2;
    static const TColor rf_yellow3;
    static const TColor rf_yellow4;
    static const TColor rf_cyan0;
    static const TColor rf_cyan1;
    static const TColor rf_cyan2;
    static const TColor rf_cyan3;
    static const TColor rf_cyan4;
    static const TColor rf_lime0;
    static const TColor rf_lime1;
    static const TColor rf_lime2;
    static const TColor rf_lime3;
    static const TColor rf_lime4;
    static const TColor rf_purple0;
    static const TColor rf_purple1;
    static const TColor rf_purple2;
    static const TColor rf_purple3;
    static const TColor rf_purple4;
    static const TColor rf_orange0;
    static const TColor rf_orange1;
    static const TColor rf_orange2;
    static const TColor rf_orange3;
    static const TColor rf_orange4;
    
  protected:
    TCanvas* m_CanvasPtr;
    void AddCanvas(TCanvas* can);
    void AddTObject(TObject* obj);
    int GetNCanvases();
    
  private:
    vector<TObject*> m_TObjects;
    vector<TCanvas*> m_Canvases;

    static const int rf_NZPalette;
    static int rf_ZPalette;
    static int rf_iZPalette;
    static double rf_zcolor_s[5];
    static double rf_zcolor_r[5];
    static double rf_zcolor_g[5];
    static double rf_zcolor_b[5];
    static double rf_zcolor_is[5];
    static double rf_zcolor_ir[5];
    static double rf_zcolor_ig[5];
    static double rf_zcolor_ib[5];

  };

}

#endif
