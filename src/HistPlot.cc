/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlot.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jul
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

#include "RestFrames/HistPlot.hh"
#include "RestFrames/HistPlotVar.hh"

namespace RestFrames {

  HistPlot::HistPlot(const string& sname, const string& stitle)
    : RFPlot(sname, stitle)
  {
    SetPlotLabel("#bf{#it{RestFrames}} Toy Event Generation");
    SetPlotTitle(GetTitle());
    SetPlotCategory("");
    SetScaleLabel("a. u.");
    m_Scale = 1.;
    m_SetScale = false;
    m_Log.SetSource("HistPlot "+GetName());
    HistPlot::SetStyle();
  }

  HistPlot::~HistPlot(){
    int N = m_Vars.size();
    for(int i = 0; i < N; i++)
      delete m_Vars[i];
    ClearHist();
  }

  void HistPlot::Clear(){
    ClearHist();
    RFPlot::Clear();
  }

  void HistPlot::ClearHist(){
    int N = m_1DHists.size();
    for(int i = 0; i < N; i++)
      delete m_1DHists[i];
    m_1DHists.clear();
    N = m_2DHists.size();
    for(int i = 0; i < N; i++)
      delete m_2DHists[i];
    m_2DHists.clear();
    m_HistToVar.clear();
    m_HistToVars.clear();
  }

  HistPlotVar const& HistPlot::GetNewVar(const string& name,
					 const string& title, 
					 double minval, double maxval){
    HistPlotVar* var = new HistPlotVar(name,title,minval,maxval);
    m_Vars.push_back(var);
    return *var;
  }
    
  void HistPlot::AddHist(const HistPlotVar& var){
    string name = var.GetName()+"_"+GetName();
    TH1D* hist = new TH1D(name.c_str(),name.c_str(),
			  128,var.GetMin(),var.GetMax());
    m_HistToVar[hist] = &var;
    m_1DHists.push_back(hist);
  }

  void HistPlot::AddHist(const HistPlotVar& varX,
			 const HistPlotVar& varY){
    string name = varX.GetName()+"_v_"+varY.GetName()+"_"+GetName();
    TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			  32,varX.GetMin(),varX.GetMax(),
			  32,varY.GetMin(),varY.GetMax());
    m_HistToVars[hist] = 
      pair<const HistPlotVar*,const HistPlotVar*>(&varX,&varY);
    m_2DHists.push_back(hist);
  }
    
  void HistPlot::Fill(double weight){
    int N = m_1DHists.size();
    for(int i = 0; i < N; i++)
      m_1DHists[i]->Fill(m_HistToVar[m_1DHists[i]]->GetVal(), 
			 weight*m_Scale);
    N = m_2DHists.size();
    for(int i = 0; i < N; i++)
      m_2DHists[i]->Fill(m_HistToVars[m_2DHists[i]].first->GetVal(), 
			 m_HistToVars[m_2DHists[i]].second->GetVal(),
			 weight*m_Scale);
  }

  void HistPlot::Draw(){
    int N = m_1DHists.size();
    for(int i = 0; i < N; i++)
      DrawHist(m_1DHists[i]);
    N = m_2DHists.size();
    for(int i = 0; i < N; i++)
      DrawHist(m_2DHists[i]);
  }
  
  void HistPlot::DrawHist(TH1D* hist){
    const HistPlotVar& var = *m_HistToVar[hist];
    
    string name = "c_"+var.GetName()+"__"+GetName();
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),700,500);
    can->SetRightMargin(0.05);
    can->Draw();
    can->SetGridx();
    can->SetGridy();

    if(!m_SetScale)
      if(hist->Integral() > 0.) 
	hist->Scale(1./hist->Integral());

    hist->SetFillColor(kBlue);
    hist->SetFillStyle(3001);
    hist->Draw();
    hist->GetXaxis()->SetTitle(var.GetTitle().c_str());
    hist->GetXaxis()->SetTitleOffset(1.27);
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->SetTitle(m_ScaleLabel.c_str());
    hist->GetYaxis()->SetTitleOffset(1.13);
    hist->GetYaxis()->CenterTitle();
    hist->GetYaxis()->SetRangeUser(1e-6,1.1*hist->GetMaximum());

    TLatex l;
    l.SetTextFont(132);	
    l.SetNDC();	
    l.SetTextSize(0.04);
    l.SetTextFont(132);
    l.DrawLatex(0.6,0.943,m_PlotTitle.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.15,0.943,m_PlotLabel.c_str());

    AddCanvas(can);
  }
  
  void HistPlot::DrawHist(TH2D* hist){
    const HistPlotVar& varX = *m_HistToVars[hist].first;
    const HistPlotVar& varY = *m_HistToVars[hist].second;
    
    string name = "c_"+varX.GetName()+"_v_"+varY.GetName()+"__"+GetName();
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),600,500);
    can->Draw();
    can->SetGridx();
    can->SetGridy();
    can->SetLogz();

    if(!m_SetScale)
      if(hist->Integral() > 0.) 
	hist->Scale(1./hist->Integral());
  
    hist->Draw("COLZ");
    hist->GetXaxis()->SetTitle(varX.GetTitle().c_str());
    hist->GetXaxis()->SetTitleOffset(1.24);
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->SetTitle(varY.GetTitle().c_str());
    hist->GetYaxis()->SetTitleOffset(1.11);
    hist->GetYaxis()->CenterTitle();
    hist->GetZaxis()->SetTitle(m_ScaleLabel.c_str());
    hist->GetZaxis()->SetTitleOffset(1.5);
    hist->GetZaxis()->CenterTitle();
    hist->GetZaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.1*hist->GetMaximum());
    hist->Draw("COLZ");
  
    TLatex l;
    l.SetTextFont(132);	
    l.SetNDC();	
    l.SetTextSize(0.04);
    l.SetTextFont(132);
    l.DrawLatex(0.7,0.943,m_PlotTitle.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.15,0.943,m_PlotLabel.c_str());
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    l.DrawLatex(0.73,0.06,m_PlotCategory.c_str());
    
    AddCanvas(can);
  }

  void HistPlot::SetScale(double scale){
    if(scale <= 0){
      m_Scale = 1.;
      m_SetScale = false;
      m_ScaleLabel = "a. u.";
    } else{
      m_Scale = scale;
      m_SetScale = true;
    }
  }

  void HistPlot::SetScaleLabel(const string& label){
    m_ScaleLabel = label;
  }

  void HistPlot::SetPlotLabel(const string& label){
    m_PlotLabel = label;
  }

  void HistPlot::SetPlotTitle(const string& title){
    m_PlotTitle = title;
  }

  void HistPlot::SetPlotCategory(const string& cat){
    m_PlotCategory = cat;
  }

  void HistPlot::SetStyle(){
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
    
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    
    gStyle->cd();
  }
}
