/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFPlot.cc
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

#include <TFile.h>
#include <TStyle.h>

#include "RestFrames/RFPlot.hh"

using namespace std;

namespace RestFrames {
  
  RFPlot::RFPlot(const string& sname, const string& stitle)
    : RFBase(sname, stitle, -1){
    m_Log.SetSource("RFPlot "+GetName());
  }
  
  RFPlot::~RFPlot(){
    Clear();
  }

  void RFPlot::Clear(){
    int Ncan = m_Canvases.size();
    for(int i = 0; i < Ncan; i++)
      delete m_Canvases[i];
    m_Canvases.clear();
    int Nobj = m_TObjects.size();
    for(int i = 0; i < Nobj; i++)
      delete m_TObjects[i];
    m_TObjects.clear();
    RFBase::Clear();
  }

  void RFPlot::WriteOutput(const string& filename){
    int N = m_Canvases.size();
    if(N <= 0) return;

    TFile* file = new TFile(filename.c_str(),"UPDATE");
    file->mkdir(GetName().c_str());
    file->cd(GetName().c_str());
    for(int i = 0; i < N; i++)
      m_Canvases[i]->Write("",TObject::kOverwrite);
    file->Close();
    delete file;
  }

  void RFPlot::AddCanvas(TCanvas* can){
    m_Canvases.push_back(can);
  }

  void RFPlot::AddTObject(TObject* obj){
    m_TObjects.push_back(obj);
  }

  int RFPlot::GetNCanvases(){
    return m_Canvases.size();
  }

  void RFPlot::SetStyle(bool invert_colors){
    // For the canvas:
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasColor(kWhite);
    gStyle->SetCanvasDefX(0);
    gStyle->SetCanvasDefY(0);
    
    // For the Pad:
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(kWhite);
    gStyle->SetGridColor(0);
    gStyle->SetGridStyle(3);
    gStyle->SetGridWidth(1);
    
    // For the frame:
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameBorderSize(1);
    gStyle->SetFrameFillColor(0);
    gStyle->SetFrameFillStyle(0);
    gStyle->SetFrameLineColor(1);
    gStyle->SetFrameLineStyle(1);
    gStyle->SetFrameLineWidth(1);
    
    // set the paper & margin sizes
    gStyle->SetPaperSize(20,26);
    gStyle->SetPadTopMargin(0.09);
    gStyle->SetPadRightMargin(0.25);
    gStyle->SetPadBottomMargin(0.18);
    gStyle->SetPadLeftMargin(0.15);
    
    // use large Times-Roman fonts
    gStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
    gStyle->SetTitleFont(132," ");    // set the pad title font
    gStyle->SetTitleSize(0.06,"xyz"); // set the 3 axes title size
    gStyle->SetTitleSize(0.06," ");   // set the pad title size
    gStyle->SetLabelFont(132,"xyz");
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetLabelColor(1,"xyz");
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.08);
    gStyle->SetStatFont(132);
    
    // use bold lines and markers
    gStyle->SetMarkerStyle(8);
    gStyle->SetHistLineWidth(2);
    gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
	
    //..Get rid of X error bars
    gStyle->SetErrorX(0.001);
    
    // do not display any of the standard histogram decorations
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(11111111);
    
    // put tick marks on top and RHS of plots
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    rf_iZPalette = TColor::CreateGradientColorTable(5, rf_zcolor_is, rf_zcolor_ir,
						    rf_zcolor_ig, rf_zcolor_ib,
						    rf_NZPalette);
    rf_ZPalette = TColor::CreateGradientColorTable(5, rf_zcolor_s, rf_zcolor_r,
						   rf_zcolor_g, rf_zcolor_b,
						   rf_NZPalette);
    gStyle->cd();
  }

  void RFPlot::SetZPalette(bool invert_colors){
    if(rf_ZPalette <= 0)
      SetStyle();
    
    int palette[rf_NZPalette];
    if(invert_colors){
      for(int i = 0; i < rf_NZPalette; i++)
	palette[i] = rf_iZPalette + i;
    } else {
      for(int i = 0; i < rf_NZPalette; i++)
	palette[i] = rf_ZPalette + i;
    }
    gStyle->SetPalette(rf_NZPalette, palette);
  }
  
  const int RFPlot::rf_NZPalette = 28;
  int RFPlot::rf_ZPalette  = 0;
  int RFPlot::rf_iZPalette = 0;
  double RFPlot::rf_zcolor_s[5] = { 0.00, 0.50, 0.70, 0.82, 1.00 };
  double RFPlot::rf_zcolor_r[5] = { 0.00, 0.00, 0.74, 1.00, 1.00 };
  double RFPlot::rf_zcolor_g[5] = { 0.00, 0.61, 0.82, 0.70, 1.00 };
  double RFPlot::rf_zcolor_b[5] = { 0.31, 0.73, 0.08, 0.00, 1.00 };
  double RFPlot::rf_zcolor_is[5] = { 0.00, 0.15, 0.35, 0.60, 1.00 };
  double RFPlot::rf_zcolor_ir[5] = { 1.00, 1.00, 0.74, 0.00, 0.00 };
  double RFPlot::rf_zcolor_ig[5] = { 0.90, 0.70, 0.82, 0.61, 0.00 };
  double RFPlot::rf_zcolor_ib[5] = { 0.70, 0.00, 0.08, 0.73, 0.31 };
  
  const TColor RFPlot::rf_blue0(7000,0.749,0.78,0.933);
  const TColor RFPlot::rf_blue1(7001,0.424,0.467,0.651);
  const TColor RFPlot::rf_blue2(7002,0.255,0.302,0.522);
  const TColor RFPlot::rf_blue3(7003,0.114,0.165,0.396);
  const TColor RFPlot::rf_blue4(7004,0.024,0.063,0.251);
  const TColor RFPlot::rf_green0(7010,0.737,0.949,0.784);
  const TColor RFPlot::rf_green1(7011,0.435,0.722,0.498);
  const TColor RFPlot::rf_green2(7012,0.239,0.576,0.314);
  const TColor RFPlot::rf_green3(7013,0.082,0.439,0.161);
  const TColor RFPlot::rf_green4(7014,0,0.275,0.063);
  const TColor RFPlot::rf_red0(7020,1,0.796,0.776);
  const TColor RFPlot::rf_red1(7021,0.957,0.612,0.576);
  const TColor RFPlot::rf_red2(7022,0.765,0.361,0.318);
  const TColor RFPlot::rf_red3(7023,0.58,0.157,0.11);
  const TColor RFPlot::rf_red4(7024,0.365,0.035,0);
  const TColor RFPlot::rf_yellow0(7030,1,0.933,0.776);
  const TColor RFPlot::rf_yellow1(7031,0.957,0.843,0.576);
  const TColor RFPlot::rf_yellow2(7032,0.765,0.631,0.318);
  const TColor RFPlot::rf_yellow3(7033,0.58,0.443,0.11);
  const TColor RFPlot::rf_yellow4(7034,0.365,0.259,0);
  const TColor RFPlot::rf_purple0(7040,0.937,0.729,0.898);
  const TColor RFPlot::rf_purple1(7041,0.753,0.478,0.702);
  const TColor RFPlot::rf_purple2(7042,0.6,0.286,0.541);
  const TColor RFPlot::rf_purple3(7043,0.42,0.075,0.353);
  const TColor RFPlot::rf_purple4(7044,0.196,0,0.161);
  const TColor RFPlot::rf_cyan0(7050,0.714,0.898,0.918);
  const TColor RFPlot::rf_cyan1(7051,0.424,0.639,0.659);
  const TColor RFPlot::rf_cyan2(7052,0.247,0.49,0.51);
  const TColor RFPlot::rf_cyan3(7053,0.067,0.329,0.357);
  const TColor RFPlot::rf_cyan4(7054,0,0.153,0.169);
  const TColor RFPlot::rf_orange0(7060,1,0.882,0.776);
  const TColor RFPlot::rf_orange1(7061,1,0.808,0.639);
  const TColor RFPlot::rf_orange2(7062,0.839,0.608,0.4);
  const TColor RFPlot::rf_orange3(7063,0.584,0.329,0.106);
  const TColor RFPlot::rf_orange4(7064,0.275,0.129,0);
  const TColor RFPlot::rf_lime0(7070,0.941,0.992,0.769);
  const TColor RFPlot::rf_lime1(7071,0.882,0.961,0.612);
  const TColor RFPlot::rf_lime2(7072,0.706,0.8,0.38);
  const TColor RFPlot::rf_lime3(7073,0.455,0.557,0.098);
  const TColor RFPlot::rf_lime4(7074,0.204,0.263,0);
  
}
