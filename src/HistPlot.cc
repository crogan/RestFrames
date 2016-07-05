/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
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
#include <TLatex.h>
#include <TLegend.h>

#include "RestFrames/HistPlot.hh"
#include "RestFrames/HistPlotVar.hh"
#include "RestFrames/HistPlotCategory.hh"

namespace RestFrames {
				       
  HistPlot::HistPlot(const std::string& sname, const std::string& stitle)
    : RFPlot(sname, stitle)
  {
    SetPlotLabel("#bf{#it{RestFrames}} Event Generation");
    SetPlotTitle(GetTitle());
    SetScaleLabel("a. u.");
    m_Scale = 1.;
    m_SetScale = false;
    m_Rebin = 4;
    m_Log.SetSource("HistPlot "+GetName());
    RFPlot::SetStyle();
  }

  HistPlot::~HistPlot(){
    int Nv = m_Vars.GetN();
    for(int i = 0; i < Nv; i++)
      delete &m_Vars[i];
    int Nc = m_Cats.GetN();
    for(int i = 0; i < Nc; i++)
      delete &m_Cats[i];
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
    m_Plot1D_Color.clear();
    m_Plot2D_Vars.clear();
    m_Plot2D_Cat.clear();
    m_Plot2D_Color.clear();
    RFPlot::Clear();
  }

  HistPlotVar const& HistPlot::GetNewVar(const std::string& name,
					 const std::string& title, 
					 double minval, double maxval,
					 const std::string& unit){
    HistPlotVar* var = new HistPlotVar(name,title,minval,maxval,unit);
    m_Vars += *var;
    return *var;
  }

  HistPlotCategory const& HistPlot::GetNewCategory(const std::string& name, 
						   const std::string& title){
    HistPlotCategory* cat = new HistPlotCategory(name, title);
    m_Cats += *cat;
    return *cat;
  }

  void HistPlot::AddPlot(const HistPlotVar& var, 
			 RFList<const HistPlotCategory> cats,
			 bool invert_colors){
    if(!m_Vars.Contains(var))
      return;
    
    int Ncat = cats.GetN();
    for(int i = 0; i < Ncat; i++){
      if(m_Cats.Contains(cats[i]))
	continue;
      Ncat--;
      cats -= cats[i];
    }

    if(Ncat == 0){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      if(m_CatToHist1D.count(empty) <= 0)
	m_CatToHist1D[empty] = std::vector<TH1D*>();
      
      int Nhist = m_CatToHist1D[empty].size();
      bool exists = false;
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVar[m_CatToHist1D[empty][i]] == var){
	  exists = true;
	  break;
	}
      }
      if(!exists){
	std::string name = GetUniqueName(var.GetName()+"_"+GetName());
	TH1D* hist = new TH1D(name.c_str(),name.c_str(),
			      256,var.GetMin(),var.GetMax());
	m_HistToVar[hist] = &var;
	m_CatToHist1D[empty].push_back(hist);
	m_1DHists.push_back(hist);
      }
    } else {
      for(int c = 0; c < Ncat; c++){
	if(m_CatToHist1D.count(&cats[c]) <= 0)
	  m_CatToHist1D[&cats[c]] = std::vector<TH1D*>();
	
	int Nhist = m_CatToHist1D[&cats[c]].size();
	bool exists = false;
	for(int i = 0; i < Nhist; i++){
	  if(*m_HistToVar[m_CatToHist1D[&cats[c]][i]] == var){
	    exists = true;
	    break;
	  }
	}
	if(!exists){
	  std::string name = GetUniqueName(var.GetName()+"_"+cats[c].GetName()+"_"+GetName());
	  TH1D* hist = new TH1D(name.c_str(),name.c_str(),
				256,var.GetMin(),var.GetMax());
	  m_HistToVar[hist] = &var;
	  m_CatToHist1D[&cats[c]].push_back(hist);
	  m_1DHists.push_back(hist);
	}
      }
    }
    m_Plot1D_Var.push_back(&var);
    m_Plot1D_Cats.push_back(cats);
    m_Plot1D_Color.push_back(invert_colors);
  }

  void HistPlot::AddPlot(const HistPlotVar& varX, const HistPlotVar& varY, 
			 RFList<const HistPlotCategory> cats,
			 bool invert_colors){
    if(!m_Vars.Contains(varX) ||
       !m_Vars.Contains(varY))
      return;
    
    int Ncat = cats.GetN();
    for(int i = 0; i < Ncat; i++){
      if(m_Cats.Contains(cats[i]))
	continue;
      Ncat--;
      cats -= cats[i];
    }

    if(Ncat == 0){
      const HistPlotCategory* empty = &HistPlotCategory::Empty();
      if(m_CatToHist2D.count(empty) <= 0)
	m_CatToHist2D[empty] = std::vector<TH2D*>();
      
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
	std::string name = GetUniqueName(varX.GetName()+"_v_"+varY.GetName()+"_"+GetName());
	TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			      128,varX.GetMin(),varX.GetMax(),
			      128,varY.GetMin(),varY.GetMax());
	
	m_HistToVars[hist] = 
	  std::pair<const HistPlotVar*,const HistPlotVar*>(&varX,&varY);
	m_CatToHist2D[empty].push_back(hist);
	m_2DHists.push_back(hist);
	m_Plot2D_Vars.push_back(std::pair<const HistPlotVar*,
				const HistPlotVar*>(&varX,&varY));
	m_Plot2D_Cat.push_back(empty);
	m_Plot2D_Color.push_back(invert_colors);
      }
    } else {
      for(int c = 0; c < Ncat; c++){
	if(m_CatToHist2D.count(&cats[c]) <= 0)
	  m_CatToHist2D[&cats[c]] = std::vector<TH2D*>();
	
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
	  std::string name = GetUniqueName(varX.GetName()+"_v_"+varY.GetName()+"_"+
					   cats[c].GetName()+"_"+GetName());
	  TH2D* hist = new TH2D(name.c_str(),name.c_str(),
				128,varX.GetMin(),varX.GetMax(),
				128,varY.GetMin(),varY.GetMax());
	  m_HistToVars[hist] = 
	    std::pair<const HistPlotVar*,const HistPlotVar*>(&varX,&varY);
	  m_CatToHist2D[&cats[c]].push_back(hist);
	  m_2DHists.push_back(hist);
	  m_Plot2D_Vars.push_back(std::pair<const HistPlotVar*,
				  const HistPlotVar*>(&varX,&varY));
	  m_Plot2D_Cat.push_back(&cats[c]);
	  m_Plot2D_Color.push_back(invert_colors);
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

  void HistPlot::Draw(bool invert_colors){
    int N1D = m_Plot1D_Var.size();
    int N2D = m_Plot2D_Vars.size();

    for(int i = 0; i < N2D; i++)
      DrawPlot(m_Plot2D_Vars[i], *m_Plot2D_Cat[i],
	       (m_Plot2D_Color[i] || invert_colors));
    for(int i = 0; i < N1D; i++)
      DrawPlot(*m_Plot1D_Var[i], m_Plot1D_Cats[i],
	       (m_Plot1D_Color[i] || invert_colors));
  }
  
  void HistPlot::DrawPlot(const HistPlotVar& var,
			  const HistPlotCatList& cats,
			  bool invert_colors){
    std::vector<TH1D*> hists;
    int Ncat = cats.GetN();
    std::string catname = "";
    
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

    std::string name = GetUniqueName("c_"+var.GetName()+"_"+catname+GetName());
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),600,500);
    can->SetLeftMargin(0.2);
    can->SetRightMargin(0.05);
    can->Draw();
    if(invert_colors){
      can->SetFillColor(kBlack);
      can->Modified();
    }
    can->SetGridx();
    can->SetGridy();

    std::string XLabel = var.GetTitle();

    std::string ScaleLabel;
    if(!m_SetScale){
      ScaleLabel =  "#frac{1}{N} #frac{dN}{";
      ScaleLabel += "d( "+XLabel+" )}";
    } else {
      ScaleLabel = m_ScaleLabel;
    }

    if(var.GetUnit() != "")
      XLabel += " "+var.GetUnit();

    int N = hists.size();
    
    int imax  = -1;
    int imin  = -1;
    int imin0 = -1;
    double hmax  = -1.;
    double hmin  = -1.;
    double hmin0 = -1.;
    for(int i = 0; i < N; i++){
      hists[i]->Rebin(m_Rebin);
      if(!m_SetScale){
	if(hists[i]->Integral() > 0.) 
	  hists[i]->Scale(1./hists[i]->Integral());
      } else {
	hists[i]->Scale(m_Scale);
      }
      if(hists[i]->GetMaximum() > hmax || imax < 0){
	hmax = hists[i]->GetMaximum();
	imax = i;
      }
      if(hists[i]->GetMinimum(0.) < hmin || imin < 0){
	hmin = hists[i]->GetMinimum(0.);
	imin = i;
      }
      if(hists[i]->GetMinimum() < hmin0 || imin0 < 0){
	hmin0 = hists[i]->GetMinimum();
	imin0 = i;
      }
    }

    hists[imax]->Draw();
    hists[imax]->GetXaxis()->SetTitle(XLabel.c_str());
    hists[imax]->GetXaxis()->SetTitleOffset(1.27);
    hists[imax]->GetXaxis()->CenterTitle();
    hists[imax]->GetYaxis()->SetTitle(ScaleLabel.c_str());
    hists[imax]->GetYaxis()->SetTitleOffset(1.42);
    hists[imax]->GetYaxis()->CenterTitle();
    if(hmin0 > 0.)
      hists[imax]->GetYaxis()->SetRangeUser(0., 1.1*hmax);
    else
      hists[imax]->GetYaxis()->SetRangeUser(0.9*hmin, 1.1*hmax);
    
    if(invert_colors){
      hists[imax]->GetXaxis()->SetTitleColor(kWhite);
      hists[imax]->GetXaxis()->SetLabelColor(kWhite);
      hists[imax]->GetXaxis()->SetAxisColor(kWhite);
      hists[imax]->GetYaxis()->SetTitleColor(kWhite);
      hists[imax]->GetYaxis()->SetLabelColor(kWhite);
      hists[imax]->GetYaxis()->SetAxisColor(kWhite);
    }

    for(int i = N-1; i >= 0; i--){
      int icolor0 = 7003 + (i%8)*10;
      int icolor1 = 7000 + (i%8)*10;
      if(invert_colors){
	icolor0 = 7000 + (i%8)*10;
	icolor1 = 7003 + (i%8)*10;
      }
      hists[i]->SetFillColor(icolor1);
      hists[i]->SetFillStyle(3002);
      hists[i]->SetLineColor(icolor0);
      hists[i]->SetLineWidth(3);
      hists[i]->SetMarkerColor(icolor0);
      hists[i]->SetMarkerSize(0);
      hists[i]->Draw("same");
    }

    TLatex l(0.6,0.943,m_PlotTitle.c_str());
    l.SetNDC();
    if(invert_colors)
      l.SetTextColor(kWhite);
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    l.DrawLatex(0.48+std::max(0.,0.47-l.GetXsize()),0.947,m_PlotTitle.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.02,0.95,m_PlotLabel.c_str());

    if(N > 1){
      TLegend* leg = new TLegend(0.225,std::max(0.5,0.884-double(N)*0.073),0.488,0.884);
      AddTObject(leg);
      leg->SetShadowColor(kWhite);
      leg->SetLineColor(kWhite);
      leg->SetFillColor(kWhite);
      if(invert_colors){
	leg->SetShadowColor(kBlack);
	leg->SetLineColor(kBlack);
	leg->SetFillColor(kBlack);
	leg->SetTextColor(kWhite);
      }
      leg->SetTextFont(132);
      leg->SetTextSize(0.045);
      for(int i = 0; i < N; i++)
	leg->AddEntry(hists[i],cats[i].GetTitle().c_str());
      
      leg->Draw();
    }
    if(N == 1 && !cats[0].IsEmpty()){
      l.SetTextSize(0.05);
      l.SetTextFont(132);
      l.DrawLatex(0.23,0.84,cats[0].GetTitle().c_str());
    }

    AddCanvas(can);
  }
  
  void HistPlot::DrawPlot(const std::pair<const HistPlotVar*,
			  const HistPlotVar*>& vars,
			  const HistPlotCategory& cat,
			  bool invert_colors){
    const HistPlotVar& varX = *vars.first;
    const HistPlotVar& varY = *vars.second;
    TH2D* hist = nullptr;
    std::string catname = "";
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
      int Nhist = m_CatToHist2D[&cat].size();
      for(int i = 0; i < Nhist; i++){
	if(*m_HistToVars[m_CatToHist2D[&cat][i]].first  == varX &&
	   *m_HistToVars[m_CatToHist2D[&cat][i]].second == varY){
	  hist = m_CatToHist2D[&cat][i];
	  break;
	} 
      }
    }
    
    std::string name = GetUniqueName("c_"+varX.GetName()+"_v_"+varY.GetName()+"_"+catname+GetName());
    TCanvas* can = new TCanvas(name.c_str(),name.c_str(),600,500);
    can->Draw();
    if(invert_colors){
      can->SetFillColor(kBlack);
      can->Modified();
    }
    can->SetGridx();
    can->SetGridy();
    can->SetLogz();

    std::string XLabel = varX.GetTitle();
    std::string YLabel = varY.GetTitle();
    
    std::string ScaleLabel;
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
  
    hist->RebinX(m_Rebin);
    hist->RebinY(m_Rebin);

    if(varX.GetUnit() != "")
      XLabel += " "+varX.GetUnit();
    if(varY.GetUnit() != "")
      YLabel += " "+varY.GetUnit();
   
    RFPlot::SetZPalette(invert_colors);
    
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
    if(invert_colors){
      hist->GetXaxis()->SetTitleColor(kWhite);
      hist->GetXaxis()->SetLabelColor(kWhite);
      hist->GetXaxis()->SetAxisColor(kWhite);
      hist->GetYaxis()->SetTitleColor(kWhite);
      hist->GetYaxis()->SetLabelColor(kWhite);
      hist->GetYaxis()->SetAxisColor(kWhite);
      hist->GetZaxis()->SetTitleColor(kWhite);
      hist->GetZaxis()->SetLabelColor(kWhite);
      hist->GetZaxis()->SetAxisColor(kWhite);
    }
    hist->Draw("COLZ");
    
    std::string title = m_PlotTitle;
    if(!cat.IsEmpty())
      title = cat.GetTitle();

    TLatex l(0.6,0.943,title.c_str());
    l.SetNDC();
    if(invert_colors)
      l.SetTextColor(kWhite);
    l.SetTextSize(0.045);
    l.SetTextFont(132);
    l.DrawLatex(0.48+std::max(0.,0.32-l.GetXsize()),0.947,title.c_str());
    l.SetTextSize(0.04);
    l.SetTextFont(42);
    l.DrawLatex(0.02,0.95,m_PlotLabel.c_str());
    
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

  void HistPlot::SetScaleLabel(const std::string& label){
    m_ScaleLabel = label;
  }

  void HistPlot::SetPlotLabel(const std::string& label){
    m_PlotLabel = label;
  }

  void HistPlot::SetPlotTitle(const std::string& title){
    m_PlotTitle = title;
  }

  void HistPlot::SetRebin(int rebin){
    if(rebin > 0)
      m_Rebin = rebin;
  }

  void HistPlot::WriteHist(const std::string& name){
    TFile* file = new TFile(name.c_str(),"UPDATE");
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

}
