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

#include <TFile.h>

#include "RestFrames/HistPlot.hh"
#include "RestFrames/HistPlotVar.hh"
#include "RestFrames/HistPlotCategory.hh"

namespace RestFrames {

  HistPlot::HistPlot(const string& sname, const string& stitle)
    : RFPlot(sname, stitle)
  {
    SetPlotLabel("#bf{#it{RestFrames}} Toy Event Generation");
    SetPlotTitle(GetTitle());
    SetScaleLabel("a. u.");
    m_Scale = 1.;
    m_SetScale = false;
    m_Log.SetSource("HistPlot "+GetName());
    HistPlot::SetStyle();
  }

  HistPlot::~HistPlot(){
    int Nv = m_Vars.size();
    for(int i = 0; i < Nv; i++)
      delete m_Vars[i];
    int Nc = m_Cats.size();
    for(int i = 0; i < Nc; i++)
      delete m_Cats[i];
    Clear();
  }

  void HistPlot::Clear(){
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
    m_CatToHist1D.clear();
    m_CatToHist2D.clear();
    m_Plot1D_Var.clear();
    m_Plot1D_Cats.clear();
    m_Plot2D_Vars.clear();
    m_Plot2D_Cat.clear();
    RFPlot::Clear();
  }

  HistPlotVar const& HistPlot::GetNewVar(const string& name,
					 const string& title, 
					 double minval, double maxval,
					 const string& unit){
    HistPlotVar* var = new HistPlotVar(name,title,minval,maxval,unit);
    m_Vars.push_back(var);
    return *var;
  }

  HistPlotCategory const& HistPlot::GetNewCategory(const string& name, 
						   const string& title){
    HistPlotCategory* cat = new HistPlotCategory(name, title);
    m_Cats.push_back(cat);
    return *cat;
  }

  void HistPlot::AddPlot(const HistPlotVar& var, RFList<HistPlotCategory> cats){
    int Ncat = cats.GetN();
    if(Ncat == 0){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      if(m_CatToHist1D.count(empty) <= 0)
	m_CatToHist1D[empty] = vector<TH1D*>();
      
      int Nhist = m_CatToHist1D[empty].size();
      bool exists = false;
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVar[m_CatToHist1D[empty][i]] == var){
	  exists = true;
	  break;
	}
      }
      if(!exists){
	string name = var.GetName()+"_"+GetName();
	TH1D* hist = new TH1D(name.c_str(),name.c_str(),
			      128,var.GetMin(),var.GetMax());
	m_HistToVar[hist] = &var;
	m_CatToHist1D[empty].push_back(hist);
	m_1DHists.push_back(hist);
      }
    } else {
      for(int c = 0; c < Ncat; c++){
	if(m_CatToHist1D.count(&cats[c]) <= 0)
	  m_CatToHist1D[&cats[c]] = vector<TH1D*>();
	
	int Nhist = m_CatToHist1D[&cats[c]].size();
	bool exists = false;
	for(int i = 0; i < Nhist; i++){
	  if(*m_HistToVar[m_CatToHist1D[&cats[c]][i]] == var){
	    exists = true;
	    break;
	  }
	}
	if(!exists){
	  string name = var.GetName()+"_"+cats[c].GetName()+"_"+GetName();
	  TH1D* hist = new TH1D(name.c_str(),name.c_str(),
				128,var.GetMin(),var.GetMax());
	  m_HistToVar[hist] = &var;
	  m_CatToHist1D[&cats[c]].push_back(hist);
	  m_1DHists.push_back(hist);
	}
      }
    }
    m_Plot1D_Var.push_back(&var);
    m_Plot1D_Cats.push_back(cats);
  }

  void HistPlot::AddPlot(const HistPlotVar& varX, const HistPlotVar& varY, 
			 RFList<HistPlotCategory> cats){
    int Ncat = cats.GetN();
    if(Ncat == 0){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      if(m_CatToHist2D.count(empty) <= 0)
	m_CatToHist2D[empty] = vector<TH2D*>();
      
      int Nhist = m_CatToHist2D[empty].size();
      bool exists = false;
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVars[m_CatToHist2D[empty][i]].first == varX &&
	   *m_HistToVars[m_CatToHist2D[empty][i]].second == varY){
	  exists = true;
	  break;
	}
      }
      if(!exists){
	string name = varX.GetName()+"_v_"+varY.GetName()+"_"+GetName();
	TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			      32,varX.GetMin(),varX.GetMax(),
			      32,varY.GetMin(),varY.GetMax());
	
	m_HistToVars[hist] = 
	  pair<const HistPlotVar*,const HistPlotVar*>(&varX,&varY);
	m_CatToHist2D[empty].push_back(hist);
	m_2DHists.push_back(hist);
	m_Plot2D_Vars.push_back(pair<const HistPlotVar*,
				const HistPlotVar*>(&varX,&varY));
	m_Plot2D_Cat.push_back(empty);
      }
    } else {
      for(int c = 0; c < Ncat; c++){
	if(m_CatToHist2D.count(&cats[c]) <= 0)
	  m_CatToHist2D[&cats[c]] = vector<TH2D*>();
	
	int Nhist = m_CatToHist2D[&cats[c]].size();
	bool exists = false;
	for(int i = 0; i < Nhist; i++){
	  if(*m_HistToVars[m_CatToHist2D[&cats[c]][i]].first == varX &&
	     *m_HistToVars[m_CatToHist2D[&cats[c]][i]].second == varY){
	    exists = true;
	    break;
	  }
	}
	if(!exists){
	  string name = varX.GetName()+"_v_"+varY.GetName()+"_"+
	    cats[c].GetName()+"_"+GetName();
	  TH2D* hist = new TH2D(name.c_str(),name.c_str(),
				32,varX.GetMin(),varX.GetMax(),
				32,varY.GetMin(),varY.GetMax());
	  m_HistToVars[hist] = 
	    pair<const HistPlotVar*,const HistPlotVar*>(&varX,&varY);
	  m_CatToHist2D[&cats[c]].push_back(hist);
	  m_2DHists.push_back(hist);
	  m_Plot2D_Vars.push_back(pair<const HistPlotVar*,
				  const HistPlotVar*>(&varX,&varY));
	  m_Plot2D_Cat.push_back(&cats[c]);
	}
      }
    }
  }
    
  void HistPlot::Fill(double weight){
    const HistPlotCategory* empty = &HistPlotCategory::Empty();
    int N = m_CatToHist1D[empty].size();
    for(int i = 0; i < N; i++)
      m_CatToHist1D[empty][i]->Fill(m_HistToVar[m_CatToHist1D[empty][i]]->GetVal(), 
				    weight);
    N = m_CatToHist2D[empty].size();
    for(int i = 0; i < N; i++)
      m_CatToHist2D[empty][i]->Fill(m_HistToVars[m_CatToHist2D[empty][i]].first->GetVal(), 
				    m_HistToVars[m_CatToHist2D[empty][i]].second->GetVal(),
				    weight);
  }

  void HistPlot::Fill(const HistPlotCategory& cat, double weight){
    if(!cat){
      Fill(weight);
      return;
    }

    int N = m_CatToHist1D[&cat].size();
    for(int i = 0; i < N; i++)
      m_CatToHist1D[&cat][i]->Fill(m_HistToVar[m_CatToHist1D[&cat][i]]->GetVal(), 
				    weight);
    N = m_CatToHist2D[&cat].size();
    for(int i = 0; i < N; i++)
      m_CatToHist2D[&cat][i]->Fill(m_HistToVars[m_CatToHist2D[&cat][i]].first->GetVal(), 
				    m_HistToVars[m_CatToHist2D[&cat][i]].second->GetVal(),
				    weight);    
  }

  void HistPlot::Draw(){
    int N = m_Plot1D_Var.size();
    for(int i = 0; i < N; i++)
      DrawPlot(*m_Plot1D_Var[i], m_Plot1D_Cats[i]);
    N = m_Plot2D_Vars.size();
    for(int i = 0; i < N; i++)
      DrawPlot(m_Plot2D_Vars[i], *m_Plot2D_Cat[i]);
  }
  
  void HistPlot::DrawPlot(const HistPlotVar& var, const RFList<HistPlotCategory>& cats){
    vector<TH1D*> hists;
    int Ncat = cats.GetN();
    string catname = "";
    if(Ncat == 0){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      int Nhist = m_CatToHist1D[empty].size();
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVar[m_CatToHist1D[empty][i]] == var){
	  hists.push_back(m_CatToHist1D[empty][i]);
	  break;
	} 
      }
    } else {
      for(int c = 0; c < Ncat; c++){
	catname += cats[c].GetName() + "_";
	int Nhist = m_CatToHist1D[&cats[c]].size();
	for(int i = 0; i < Nhist; i++){
	  if(*m_HistToVar[m_CatToHist1D[&cats[c]][i]] == var){
	    hists.push_back(m_CatToHist1D[&cats[c]][i]);
	    break;
	  } 
	}
      }
    }
    
    string name = "c_"+var.GetName()+"_"+catname+GetName();
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),600,500);
    can->SetLeftMargin(0.2);
    can->SetRightMargin(0.05);
    can->Draw();
    can->SetGridx();
    can->SetGridy();

    string XLabel = var.GetTitle();

    string ScaleLabel;
    if(!m_SetScale){
      ScaleLabel =  "#frac{1}{N} #frac{dN}{";
      ScaleLabel += "d( "+XLabel+" )}";
    } else {
      ScaleLabel = m_ScaleLabel;
    }

    if(var.GetUnit() != "")
      XLabel += " "+var.GetUnit();

    int N = hists.size();
    int imax = 0;
    double hmax = -1.;
    for(int i = 0; i < N; i++){
      if(!m_SetScale){
	if(hists[i]->Integral() > 0.) 
	  hists[i]->Scale(1./hists[i]->Integral());
      } else {
	hists[i]->Scale(m_Scale);
      }
      if(hists[i]->Integral() > hmax){
	hmax = hists[i]->Integral();
	imax = i;
      }
    }

    hists[imax]->SetFillColor(kBlue);
    hists[imax]->SetFillStyle(3001);
    hists[imax]->Draw();
    hists[imax]->GetXaxis()->SetTitle(XLabel.c_str());
    hists[imax]->GetXaxis()->SetTitleOffset(1.27);
    hists[imax]->GetXaxis()->CenterTitle();
    hists[imax]->GetYaxis()->SetTitle(ScaleLabel.c_str());
    hists[imax]->GetYaxis()->SetTitleOffset(1.42);
    hists[imax]->GetYaxis()->CenterTitle();
    hists[imax]->GetYaxis()->SetRangeUser(1e-6,1.1*hists[imax]->GetMaximum());

    TLatex l;
    l.SetTextFont(132);	
    l.SetNDC();	
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    l.DrawLatex(0.6,0.943,m_PlotTitle.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.15,0.943,m_PlotLabel.c_str());
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    //l.DrawLatex(0.73,0.06,m_PlotCategory.c_str());

    AddCanvas(can);
  }
  
  void HistPlot::DrawPlot(const pair<const HistPlotVar*,const HistPlotVar*>& vars,
			  const HistPlotCategory& cat){
    const HistPlotVar& varX = *vars.first;
    const HistPlotVar& varY = *vars.second;
    TH2D* hist = nullptr;
    string catname = "";
    if(!cat){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      int Nhist = m_CatToHist2D[empty].size();
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVars[m_CatToHist2D[empty][i]].first  == varX &&
	   *m_HistToVars[m_CatToHist2D[empty][i]].second == varY){
	  hist = m_CatToHist2D[empty][i];
	  break;
	} 
      }
    } else {
      catname += cat.GetName() + "_";
      int Nhist = m_CatToHist1D[&cat].size();
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVars[m_CatToHist2D[&cat][i]].first  == varX &&
	   *m_HistToVars[m_CatToHist2D[&cat][i]].second == varY){
	  hist = m_CatToHist2D[&cat][i];
	  break;
	} 
      }
    }
    
    string name = "c_"+varX.GetName()+"_v_"+varY.GetName()+"_"+catname+GetName();
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),600,500);
    can->Draw();
    can->SetGridx();
    can->SetGridy();
    can->SetLogz();

    string XLabel = varX.GetTitle();
    string YLabel = varY.GetTitle();

    string ScaleLabel;
    if(!m_SetScale){
      if(hist->Integral() > 0.) 
	hist->Scale(1./hist->Integral());
      ScaleLabel =  "#frac{1}{N} #frac{dN}{";
      ScaleLabel += "d( "+XLabel+" ) ";
      ScaleLabel += "d( "+YLabel+" )";
      ScaleLabel += "}";
    } else {
      hist->Scale(m_Scale);
      ScaleLabel = m_ScaleLabel;
    }
  
    if(varX.GetUnit() != "")
      XLabel += " "+varX.GetUnit();
    if(varY.GetUnit() != "")
      YLabel += " "+varY.GetUnit();

    hist->Draw("COLZ");
    hist->GetXaxis()->SetTitle(XLabel.c_str());
    hist->GetXaxis()->SetTitleOffset(1.24);
    hist->GetXaxis()->CenterTitle();
    hist->GetYaxis()->SetTitle(YLabel.c_str());
    hist->GetYaxis()->SetTitleOffset(1.11);
    hist->GetYaxis()->CenterTitle();
    hist->GetZaxis()->SetTitle(ScaleLabel.c_str());
    hist->GetZaxis()->SetTitleOffset(1.5);
    hist->GetZaxis()->CenterTitle();
    hist->GetZaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.1*hist->GetMaximum());
    hist->Draw("COLZ");
  
    TLatex l;
    l.SetTextFont(132);	
    l.SetNDC();	
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    l.DrawLatex(0.7,0.943,m_PlotTitle.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.15,0.943,m_PlotLabel.c_str());
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    //l.DrawLatex(0.73,0.06,m_PlotCategory.c_str());
    
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

  void HistPlot::WriteHist(const string& name){
    TFile *file = new TFile(name.c_str(),"UPDATE");
    file->mkdir(GetName().c_str());
    file->mkdir((GetName()+"/hist").c_str());
    file->cd((GetName()+"/hist").c_str());
    int N = m_1DHists.size();
    for(int i = 0; i < N; i++)
      m_1DHists[i]->Write("",TObject::kOverwrite);
    N = m_2DHists.size();
    for(int i = 0; i < N; i++)
      m_2DHists[i]->Write("",TObject::kOverwrite);
    file->Close();
    delete file;
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
    const Int_t NCont = 28;

    Double_t stops[NRGBs] = { 0.00, 0.5, 0.70, 0.82, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.74, 1.00, 1. };
    Double_t green[NRGBs] = { 0.00, 0.61, 0.82, 0.70, 1.00 };
    Double_t blue[NRGBs]  = { 0.31, 0.73, 0.08, 0.00, 1.00 };
    
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    
    gStyle->cd();
  }
}
