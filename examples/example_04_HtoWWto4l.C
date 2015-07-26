/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_04_HtoWWto4l.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 June
///
///  \brief An example generating and analyzing di-leptonic ttbar events
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

#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TColor.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include "RestFrames/RestFrames.hh"

void setstyle();
TCanvas* Plot_Me(string scan, TH2D* histo, string X, string Y, string title = "", string label = "");
TCanvas* Plot_Me(string scan, TH1D* histo, string X, string title = "", string label = "");

using namespace std;
using namespace RestFrames;

void example_04_HtoWWto4l(const string& output_name = "output_04.root"){
  setstyle();
  
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);
  SetLogMaxWidth(120);

  double mH = 125.;
  double wH = 0.04;
  double mW = 80.;
  double wW = 2.5;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 100000;

  //
  // Set up toy generation tree (not needed for reconstruction)
  g_Log << LogInfo << "Initializing generator frames and tree" << g_End;
  LabGenFrame LAB_G("LAB_G","LAB");
  ResonanceGenFrame H_G("H_G","H");
  ResonanceGenFrame Wa_G("Wa_G","W_{a}");
  ResonanceGenFrame Wb_G("Wb_G","W_{b}");
  VisibleGenFrame La_G("La_G","#it{l}_{a}");
  InvisibleGenFrame Na_G("Na_G","#nu_{a}");
  VisibleGenFrame Lb_G("Lb_G","#it{l}_{b}");
  InvisibleGenFrame Nb_G("Nb_G","#nu_{b}");
  LAB_G.SetChildFrame(H_G);
  H_G.AddChildFrame(Wa_G);
  H_G.AddChildFrame(Wb_G);
  Wa_G.AddChildFrame(La_G);
  Wa_G.AddChildFrame(Na_G);
  Wb_G.AddChildFrame(Lb_G);
  Wb_G.AddChildFrame(Nb_G);

  if(LAB_G.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_G.GetName() << endl;
    g_Log << "Ready to set masses etc." << g_End;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_G) << g_End;								    
  // set Higgs masses
  H_G.SetMass(mH);
  H_G.SetWidth(wH);
  // set W masses
  Wa_G.SetMass(mW);
  Wb_G.SetMass(mW);
  Wa_G.SetWidth(wW);
  Wb_G.SetWidth(wW);
  // set lepton masses
  La_G.SetMass(mL);
  Lb_G.SetMass(mL);
  // set neutrino masses
  Na_G.SetMass(mN);
  Nb_G.SetMass(mN);

  if(LAB_G.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized analysis from LabFrame ";
    g_Log << LAB_G.GetName() << endl;
    g_Log << "Ready for event generation" << g_End;
  } else
    g_Log << LogError << "Unable to initialize analysis from LabFrame: " << Log(LAB_G) << g_End;


  // Set up reco analysis tree
  g_Log << LogInfo << "Initializing reconstruction frames and tree" << g_End;
  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame H_R("H_R","H");
  DecayRecoFrame Wa_R("Wa_R","W_{a}");
  DecayRecoFrame Wb_R("Wb_R","W_{b}");
  VisibleRecoFrame La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame Nb_R("Nb_R","#nu_{b}");
  LAB_R.SetChildFrame(H_R);
  H_R.AddChildFrame(Wa_R);
  H_R.AddChildFrame(Wb_R);
  Wa_R.AddChildFrame(La_R);
  Wa_R.AddChildFrame(Na_R);
  Wb_R.AddChildFrame(Lb_R);
  Wb_R.AddChildFrame(Nb_R);
 
  if(LAB_R.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready for Group and Jigsaw initialization" << g_End;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_R) << g_End;  

  // define groups for the reconstruction trees
  InvisibleGroup INV_R("INV_R","WIMP Jigsaws");
  INV_R.AddFrame(Na_R);
  INV_R.AddFrame(Nb_R);

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);

  SetRapidityInvJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrames((LAB_R.GetListVisibleFrames()));

  ContraBoostInvJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrames((Wa_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrames((Wb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrames((Wa_R.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw_R.AddInvisibleFrames((Wb_R.GetListInvisibleFrames()), 1);

  g_Log << LogInfo << "Initializing tree for analysis" << m_End;
  // check reconstruction trees
  if(LAB_R.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "...Successfully initialized tree for analysis from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready event analysis" << g_End;
  } else
    g_Log << LogError << "...Unable to initialize analysis from LabFrame: " << Log(LAB_R) << g_End;	

  DecayRecoFrame *W[2];
  VisibleRecoFrame *L[2];
  InvisibleRecoFrame *N[2];
  W[0] = &Wa_R;
  W[1] = &Wb_R;
  L[0] = &La_R;
  L[1] = &Lb_R;
  N[0] = &Na_R;
  N[1] = &Nb_R;

  // Now we book some histograms of kinematic variables
  TH1D* h_MH     = new TH1D("h_MH","h_MH",100,0.,2.);
  TH1D* h_mH     = new TH1D("h_mH","h_mH",100,0.,mH*2.);
  TH1D* h_MW     = new TH1D("h_MW","h_MW",100,0.,2.);
  TH2D* h_MH_v_MW   = new TH2D("h_MH_v_MW","h_MH_v_MW",50,0.,2.,50,0.,2.);
  TH2D* h_mW_v_mW   = new TH2D("h_mW_v_mW","h_mW_v_mW",50,0.,mH,50,0.,mH);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << g_End;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTH = mH*gRandom->Rndm();
    LAB_G.SetTransverseMomenta(PTH);               // give the Higgs some Pt
    double PzH = mH*(2.*gRandom->Rndm()-1.);
    LAB_G.SetLongitudinalMomenta(PzH);             // give the Higgs some Pz
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    
    // give the signal-like tree the event info and analyze
    LAB_R.ClearEvent();                              // clear the signal-like tree
    INV_R.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    La_R.SetLabFrameFourVector(La_G.GetFourVector());
    Lb_R.SetLabFrameFourVector(Lb_G.GetFourVector());
    LAB_R.AnalyzeEvent();                            // analyze the event

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** Higgs mass
    double MH = H_R.GetMass();
    double MW = Wa_R.GetMass();

    h_MH->Fill(MH/H_G.GetMass());		
    h_mH->Fill(H_G.GetMass());
    h_MW->Fill(MW/mW);
    h_MH_v_MW->Fill(MH/H_G.GetMass(),MW/mW);
    h_mW_v_mW->Fill(Wa_G.GetMass(),Wb_G.GetMass());
  }

  setstyle();
  string plot_title = "H #rightarrow W(#it{l} #nu)W(#it{l} #nu)";
  TCanvas *c_MH          = Plot_Me("c_MH", h_MH, "M_{H} / m_{H}^{true}", plot_title);
  TCanvas *c_mH          = Plot_Me("c_mH", h_mH, "m_{H}^{true} [GeV]", plot_title);
  TCanvas *c_MW          = Plot_Me("c_MW", h_MW, "M_{W} / m_{W}^{true}", plot_title);
  TCanvas *c_MH_v_MW     = Plot_Me("c_MH_v_MW", h_MH_v_MW, "M_{H} / m_{H}^{true}", "M_{W} / m_{W}^{true}", plot_title);
  TCanvas *c_mW_v_mW     = Plot_Me("c_mW_v_mW", h_mW_v_mW, "m_{Wa}^{true} [GeV]", "m_{Wb}^{true} [GeV]", plot_title);

  TreePlot tree_plot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot.SetFrameTree(LAB_G);
  tree_plot.Draw("GenTree", "Generator Tree");

  // reco tree
  tree_plot.SetFrameTree(LAB_R);
  tree_plot.Draw("RecoTree", "Reconstruction Tree");

  g_Log << LogInfo << "Finished" << g_End;
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
  example_04_HtoWWto4l();
  return 0;
}
#endif
