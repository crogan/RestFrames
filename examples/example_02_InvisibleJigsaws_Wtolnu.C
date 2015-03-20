/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_01_InvisibleJigsaws_Wtolnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example generating and analyzing W(l nu) events
//
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

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TColor.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TRandom.h>
#include <iostream>
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/GLabFrame.hh"
#include "RestFrames/GDecayFrame.hh"
#include "RestFrames/GVisibleFrame.hh"
#include "RestFrames/GInvisibleFrame.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/FramePlot.hh"

void setstyle();
TCanvas* Plot_Me(string scan, TH2D* histo, string X, string Y, string title = "", string label = "");
TCanvas* Plot_Me(string scan, TH1D* histo, string X, string title = "", string label = "");

using namespace std;
using namespace RestFrames;

void example_02_InvisibleJigsaws_Wtolnu(string output_name = "output_02.root"){
  setstyle();

  double mW = 81.;
  int Ngen = 1000000;

  // Set up toy generation and event analysis trees:
  GLabFrame LAB_G("LAB_G","LAB");
  GDecayFrame W_G("W_G","W");
  GVisibleFrame L_G("L_G","#it{l}");
  GInvisibleFrame NU_G("NU_G","#nu");

  RLabFrame LAB_R("LAB_R","LAB");
  RDecayFrame W_R("W_R","W");
  RVisibleFrame L_R("L_R","#it{l}");
  RInvisibleFrame NU_R("NU_R","#nu");

  LAB_G.SetChildFrame(W_G);
  W_G.AddChildFrame(L_G);
  W_G.AddChildFrame(NU_G);

  LAB_R.SetChildFrame(W_R);
  W_R.AddChildFrame(L_R);
  W_R.AddChildFrame(NU_R);
 
  if(!LAB_G.InitializeTree()) cout << "Problem with generator tree" << endl; 
  if(!LAB_R.InitializeTree()) cout << "Problem with reconstruction tree" << endl; 
  
  FramePlot* treePlot_G = new FramePlot("tree_G","Generator Tree");
  treePlot_G->AddFrameTree(LAB_G);
  treePlot_G->DrawFramePlot();
  TCanvas* c_gentree = treePlot_G->GetCanvas();

  FramePlot* treePlot_R = new FramePlot("tree_R","Reconstruction Tree");
  treePlot_R->AddFrameTree(LAB_R);
  treePlot_R->DrawFramePlot();
  TCanvas* c_recotree = treePlot_R->GetCanvas();

  // Now we add invisible jigsaws
  InvisibleGroup INV("INV","Neutrino Jigsaws");
  INV.AddFrame(NU_R);
 
  InvisibleMassJigsaw MassJigsaw("MASS_JIGSAW","mass Jigsaw");
  INV.AddJigsaw(MassJigsaw);

  InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW","rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame(L_R);

  if(!LAB_R.InitializeAnalysis()) cout << "Problem with jigsaws" << endl;

  // Now we book some histograms of kinematic variables
  TH1D* h_MW     = new TH1D("h_MW","h_MW",100,0.,90.);
  TH1D* h_cosW   = new TH1D("h_cosW","h_cosW",100,-1.,1.);
  TH1D* h_dphiW  = new TH1D("h_dphiW","h_dphiW",100,0.,2.*TMath::Pi());
  TH1D* h_DcosW  = new TH1D("h_DcosW","h_DcosW",100,-1.,1.);
  TH1D* h_DdphiW = new TH1D("h_DdphiW","h_DdphiW",100,-1.,1.);
  
  TH2D* h_MW_v_cosW   = new TH2D("h_MW_v_cosW","h_MW_v_cosW",50,0.,90.,50,-1.,1.);
  TH2D* h_MW_v_PT     = new TH2D("h_MW_v_PT","h_MW_v_PT",50,0.,90.,50,0.,1.);
  TH2D* h_cosW_v_PT   = new TH2D("h_cosW_v_PT","h_cosW_v_PT",50,-1.,1.,50,0.,1.);
  TH2D* h_dphiW_v_PT  = new TH2D("h_dphiW_v_PT","h_dphiW_v_PT",50,0.,2.*TMath::Pi(),50,0.,1.);
  TH2D* h_DcosW_v_PT  = new TH2D("h_DcosW_v_PT","h_DcosW_v_PT",50,-1.,1.,50,0.,1.);
  TH2D* h_DdphiW_v_PT = new TH2D("h_DdphiW_v_PT","h_DdphiW_v_PT",50,-1.,1.,50,0.,1.);

  W_G.SetMass(mW);
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) cout << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_G.ClearEvent();                             // clear the gen tree
    double PTW = mW*gRandom->Rndm();
    LAB_G.SetTransverseMomenta(PTW);                // give the W some Pt
    double PzW = mW*(2.*gRandom->Rndm()-1.);
    LAB_G.SetLongitudinalMomenta(PzW);              // give the W some Pz
    LAB_G.AnalyzeEvent();                           // generate a new event

    // analyze event
    LAB_R.ClearEvent();                             // clear the reco tree
    L_R.SetLabFrameFourVector(L_G.GetFourVector()); // Set lepton 4-vec
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    INV.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    LAB_R.AnalyzeEvent();                           // analyze the event

    // calculate observables
    double MW    = W_R.GetMass();
    double MWgen = W_G.GetMass();
    double cosW     = W_R.GetCosDecayAngle();
    double cosWgen  = W_G.GetCosDecayAngle();
    double dphiW    = LAB_R.GetDeltaPhiDecayPlanes(W_R);
    double dphiWgen = LAB_G.GetDeltaPhiDecayPlanes(W_G);
    
    double DcosW = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    double DdphiW = asin(sin(dphiW-dphiWgen));

    h_MW->Fill(MW);
    h_cosW->Fill(cosW);
    h_dphiW->Fill(dphiW);
    h_DcosW->Fill(DcosW);
    h_DdphiW->Fill(DdphiW);
    h_MW_v_cosW->Fill(MW,cosW);
  
    PTW /= MWgen;
    h_MW_v_PT->Fill(MW,PTW);
    h_cosW_v_PT->Fill(cosW,PTW);
    h_dphiW_v_PT->Fill(dphiW,PTW);
    h_DcosW_v_PT->Fill(DcosW,PTW);
    h_DdphiW_v_PT->Fill(DdphiW,PTW);
  }
  setstyle();
  string plot_title = "W #rightarrow #it{l} #nu";
  TCanvas *c_MW          = Plot_Me("c_MW", h_MW, "M_{W} [GeV]", plot_title);
  TCanvas *c_cosW        = Plot_Me("c_cosW", h_cosW, "cos #theta_{W}", plot_title);
  TCanvas *c_dphiW       = Plot_Me("c_dphiW", h_dphiW, "#Delta #phi (#hat{n}_{W}, (#hat{z} #times #hat{p}_{W}))", plot_title);
  TCanvas *c_DcosW       = Plot_Me("c_DcosW", h_DcosW, "#theta_{W} - #theta_{W}^{true}", plot_title);
  TCanvas *c_DdphiW      = Plot_Me("c_DdphiW", h_DdphiW, "#Delta #phi_{W} - #Delta #phi_{W}^{true}", plot_title);
  TCanvas *c_MW_v_cosW   = Plot_Me("c_MW_v_cosW", h_MW_v_cosW, "M_{W} [GeV]", 
				   "cos #theta_{W}", plot_title);
  TCanvas *c_MW_v_PT     = Plot_Me("c_MW_v_PT", h_MW_v_PT, "M_{W} [GeV]", 
				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  TCanvas *c_cosW_v_PT   = Plot_Me("c_cosW_v_PT", h_cosW_v_PT, "cos #theta_{W}", 
				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  TCanvas *c_dphiW_v_PT  = Plot_Me("c_dphiW_v_PT", h_dphiW_v_PT, 
				   "#Delta #phi (#hat{n}_{W}, (#hat{z} #times #hat{p}_{W}))", 
				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  TCanvas *c_DcosW_v_PT  = Plot_Me("c_DcosW_v_PT", h_DcosW_v_PT, 
				   "#theta_{W} - #theta_{W}^{true}", 
				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  TCanvas *c_DdphiW_v_PT = Plot_Me("c_DdphiW_v_PT", h_DdphiW_v_PT, 
				   "#Delta #phi_{W} - #Delta #phi_{W}^{true}", 
				   "p_{T}^{W} / m_{W}^{true}", plot_title);

  TFile *foutput = new TFile(output_name.c_str(),"RECREATE");
  foutput->cd();
  c_gentree->Write();
  c_recotree->Write();
  c_MW->Write();
  c_cosW->Write(); 
  c_dphiW->Write(); 
  c_DcosW->Write(); 
  c_DdphiW->Write();
  c_MW_v_cosW->Write(); 
  c_MW_v_PT->Write();
  c_cosW_v_PT->Write();
  c_dphiW_v_PT->Write();
  c_DcosW_v_PT->Write(); 
  c_DdphiW_v_PT->Write();
  foutput->Close();
}

TCanvas* Plot_Me(string scan, TH2D* histo, string X, string Y, string title, string label){
  TCanvas *c1 = new TCanvas(scan.c_str(),scan.c_str(),600,500);
  c1->Draw();
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLogz();

  if(histo->Integral() > 0.) histo->Scale(1./histo->Integral());
  
  histo->Draw("COLZ");
  histo->GetXaxis()->SetTitle(X.c_str());
  histo->GetXaxis()->SetTitleOffset(1.24);
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->SetTitle(Y.c_str());
  histo->GetYaxis()->SetTitleOffset(1.11);
  histo->GetYaxis()->CenterTitle();
  histo->GetZaxis()->SetTitle("N_{bin} / N_{total}");
  histo->GetZaxis()->SetTitleOffset(1.5);
  histo->GetZaxis()->CenterTitle();
  histo->GetZaxis()->SetRangeUser(0.9*histo->GetMinimum(0.0),1.1*histo->GetMaximum());
  histo->Draw("COLZ");
  
  TLatex l;
  l.SetTextFont(132);	
  l.SetNDC();	
  l.SetTextSize(0.04);
  l.SetTextFont(132);
  l.DrawLatex(0.7,0.943,title.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{#it{RestFrames}} Toy Event Generation");
  l.SetTextSize(0.045);
  l.SetTextFont(132);
  l.DrawLatex(0.75,0.06,label.c_str());
	
  return c1;
}
TCanvas* Plot_Me(string scan, TH1D* histo, string X, string title, string label){
  TCanvas *c1 = new TCanvas(scan.c_str(),scan.c_str(),700,500);
  c1->SetRightMargin(0.05);
  c1->Draw();
  c1->SetGridx();
  c1->SetGridy();
  
  if(histo->Integral() > 0.) histo->Scale(1./histo->Integral());

  histo->SetFillColor(kBlue);
  histo->SetFillStyle(3001);
  histo->Draw();
  histo->GetXaxis()->SetTitle(X.c_str());
  histo->GetXaxis()->SetTitleOffset(1.27);
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->SetTitle("a. u.");
  histo->GetYaxis()->SetTitleOffset(1.13);
  histo->GetYaxis()->CenterTitle();
  histo->GetYaxis()->SetRangeUser(0.,1.1*histo->GetMaximum());

  TLatex l;
  l.SetTextFont(132);	
  l.SetNDC();	
  l.SetTextSize(0.04);
  l.SetTextFont(132);
  l.DrawLatex(0.6,0.943,title.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{#it{RestFrames}} Toy Event Generation");

  l.SetTextSize(0.045);
  l.SetTextFont(132);
  l.DrawLatex(0.75,0.06,label.c_str());

  return c1;
}
void setstyle() {
	
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefX(0);   //Position on screen
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

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_02_InvisibleJigsaws_Wtolnu();
  return 0;
}
#endif
