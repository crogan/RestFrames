/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_05.C
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

void example_05(string output_name = "output_example_05.root"){
  
  SetLogPrint(LogVerbose,true);
  //SetLogPrint(LogDebug,true);

  double mT = 175.;
  double mW = 80.;
  double mB = 2.;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 10000;

  //
  // Set up toy generation tree (not needed for reconstruction)
  g_Log << LogInfo << "Initializing generator frames and tree" << LogEnd;
  ppLabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame TT_G("TT_G","t #bar{t}");
  ResonanceGenFrame Ta_G("Ta_G","t_{a}");
  ResonanceGenFrame Tb_G("Tb_G","t_{b}");
  DecayGenFrame Wa_G("Wa_G","W_{a}");
  DecayGenFrame Wb_G("Wb_G","W_{b}");
  VisibleGenFrame Ba_G("Ba_G","b_{a}");
  VisibleGenFrame La_G("La_G","#it{l}_{a}");
  InvisibleGenFrame Na_G("Na_G","#nu_{a}");
  VisibleGenFrame Bb_G("Bb_G","b_{b}");
  VisibleGenFrame Lb_G("Lb_G","#it{l}_{b}");
  InvisibleGenFrame Nb_G("Nb_G","#nu_{b}");
  LAB_G.SetChildFrame(TT_G);
  TT_G.AddChildFrame(Ta_G);
  TT_G.AddChildFrame(Tb_G);
  Ta_G.AddChildFrame(Ba_G);
  Ta_G.AddChildFrame(Wa_G);
  Tb_G.AddChildFrame(Bb_G);
  Tb_G.AddChildFrame(Wb_G);
  Wa_G.AddChildFrame(La_G);
  Wa_G.AddChildFrame(Na_G);
  Wb_G.AddChildFrame(Lb_G);
  Wb_G.AddChildFrame(Nb_G);

  LAB_G.InitializeTree();

   // set top masses
  
  Ta_G.SetMass(mT);
  Tb_G.SetMass(mT);
  Ta_G.SetWidth(2.);
  Tb_G.SetWidth(2.);
  // set W masses
  Wa_G.SetMass(mW);
  Wb_G.SetMass(mW);
  // set B masses
  Ba_G.SetMass(mB);
  Bb_G.SetMass(mB);
  // set : masses
  La_G.SetMass(mL);
  Lb_G.SetMass(mL);
  // set neutrino masses
  Na_G.SetMass(mN);
  Nb_G.SetMass(mN);
  TT_G.SetVariableMass();

  if(LAB_G.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_G.GetName() << endl;
    g_Log << "Ready for event generation" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_G) << LogEnd;								    
  // Set up reco analysis tree
  g_Log << LogInfo << "Initializing reconstruction frames and tree" << LogEnd;
  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame TT_R("TT_R","t #bar{t}");
  DecayRecoFrame Ta_R("Ta_R","t_{a}");
  DecayRecoFrame Tb_R("Tb_R","t_{b}");
  DecayRecoFrame Wa_R("Wa_R","W_{a}");
  DecayRecoFrame Wb_R("Wb_R","W_{b}");
  VisibleRecoFrame Ba_R("Ba_R","b_{a}");
  VisibleRecoFrame La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame Bb_R("Bb_R","b_{b}");
  VisibleRecoFrame Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame Nb_R("Nb_R","#nu_{b}");
  LAB_R.SetChildFrame(TT_R);
  TT_R.AddChildFrame(Ta_R);
  TT_R.AddChildFrame(Tb_R);
  Ta_R.AddChildFrame(Ba_R);
  Ta_R.AddChildFrame(Wa_R);
  Tb_R.AddChildFrame(Bb_R);
  Tb_R.AddChildFrame(Wb_R);
  Wa_R.AddChildFrame(La_R);
  Wa_R.AddChildFrame(Na_R);
  Wb_R.AddChildFrame(Lb_R);
  Wb_R.AddChildFrame(Nb_R);

  if(LAB_R.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready for Group and Jigsaw initialization" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_R) << LogEnd;  

  // define groups for the reconstruction trees
  InvisibleGroup INV_R("INV_R","WIMP Jigsaws");
  INV_R.AddFrame(Na_R);
  INV_R.AddFrame(Nb_R);
  CombinatoricGroup B_R("VIS","b-jet Jigsaws");
  // visible frames in first decay step must always have at least one element
  B_R.AddFrame(Ba_R);
  B_R.AddFrame(Bb_R);
  B_R.SetNElementsForFrame(Ba_R,1,true);
  B_R.SetNElementsForFrame(Bb_R,1,true);

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);

  SetRapidityInvJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrames((LAB_R.GetListVisibleFrames()));

  ContraBoostInvJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrames((Ta_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrames((Tb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrame(Na_R, 0);
  ContraBoostJigsaw_R.AddInvisibleFrame(Nb_R, 1);

  MinMassesCombJigsaw HemiJigsaw_R("HEM_JIGSAW_R","Minimize m(b #it{l}) Jigsaw");
  B_R.AddJigsaw(HemiJigsaw_R);
  HemiJigsaw_R.AddFrames((Ta_R.GetListVisibleFrames()),0);
  HemiJigsaw_R.AddFrames((Tb_R.GetListVisibleFrames()),1);

  g_Log << LogInfo << "Initializing tree for analysis" << LogEnd;
  // check reconstruction trees
  if(LAB_R.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "...Successfully initialized tree for analysis from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready event analysis" << LogEnd;
  } else
    g_Log << LogError << "...Unable to initialize analysis from LabFrame: " << Log(LAB_R) << LogEnd;	

  //////////////////////////////////////////////////////////////
  // draw some pictures of our trees
  //////////////////////////////////////////////////////////////

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");
  
  // reco tree
  tree_plot->SetTree(LAB_R);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  // Invisible Jigsaw tree
  tree_plot->SetTree(INV_R);
  tree_plot->Draw("InvTree", "Invisible Jigsaws");

  // Visible Jigsaw tree
  tree_plot->SetTree(B_R);
  tree_plot->Draw("VisTree", "Visible Jigsaws");

  DecayRecoFrame *T[2], *W[2];
  VisibleRecoFrame *B[2], *L[2];
  InvisibleRecoFrame *N[2];
  T[0] = &Ta_R;
  T[1] = &Tb_R;
  W[0] = &Wa_R;
  W[1] = &Wb_R;
  B[0] = &Ba_R;
  B[1] = &Bb_R;
  L[0] = &La_R;
  L[1] = &Lb_R;
  N[0] = &Na_R;
  N[1] = &Nb_R;

  double EBtrue = (mT*mT - mW*mW)/mT/2.;
  double ELtrue = (mW*mW - mN*mN)/mW/2.;

  // Now we book some histograms of kinematic variables
  TH1D* h_MT     = new TH1D("h_MT","h_MT",100,0.,2.);
  TH1D* h_MW     = new TH1D("h_MW","h_MW",100,0.,2.);
  TH1D* h_EB     = new TH1D("h_EB","h_EB",100,0.,2.);
  TH1D* h_EL     = new TH1D("h_EL","h_EL",100,0.,2.);
  TH2D* h_MT_v_MW   = new TH2D("h_MT_v_MW","h_MT_v_MW",50,0.,2.,50,0.,2.);
  TH2D* h_EB_v_MW   = new TH2D("h_EB_v_MW","h_EB_v_MW",50,0.,2.,50,0.,2.);

  // // function for randomly determining di-top mass 
  // // (relative to top mass via gamma)
  // TF1 f_gamma("f_gamma","(x-1)*exp(-2.*x)",1.,10.);
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                             // clear the gen tree
    // get a random di-gluino mass
 
    double PTTT = 2.*mT*gRandom->Rndm();
    LAB_G.SetTransverseMomenta(PTTT);               // give the di-gluinos some Pt
    LAB_G.AnalyzeEvent();                           // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    
    // give the signal-like tree the event info and analyze
    LAB_R.ClearEvent();                              // clear the signal-like tree
    INV_R.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    vector<RFKey> B_ID;                      // ID for tracking jets in tree
    B_ID.push_back(B_R.AddLabFrameFourVector(Ba_G.GetFourVector()));
    B_ID.push_back(B_R.AddLabFrameFourVector(Bb_G.GetFourVector()));
    La_R.SetLabFrameFourVector(La_G.GetFourVector());
    Lb_R.SetLabFrameFourVector(Lb_G.GetFourVector());
    LAB_R.AnalyzeEvent();                            // analyze the event

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** total CM mass
    double shat = TT_R.GetMass();
    
    TVector3 vPTT = TT_R.GetFourVector(LAB_R).Vect();

    //*** ratio of CM pT to CM mass
    double RPT = vPTT.Pt() / (vPTT.Pt() + shat/4.);
    //*** ratio of CM pz to CM mass
    double RPZ = vPTT.Pz() / (vPTT.Pz() + shat/4.);
    //*** cos decay angle of TT system
    double cosTT = TT_R.GetCosDecayAngle();
    //*** delta phi between lab and TT decay planes
    double dphiLTT = LAB_R.GetDeltaPhiDecayPlanes(TT_R);
    
    double PT = T[0]->GetMomentum(TT_R);
    
    //*** delta phi between TT visible decay products and TT decay axis
    double dphiVT = TT_R.GetDeltaPhiDecayVisible();
    //*** delta phi between TT visible decay products and TT momentum
    double dphiVTT = TT_R.GetDeltaPhiBoostVisible();

    // 'hemisphere' (one for each 'top') observables

    //*** cosine top decay angle
    double cosT[2];
    //*** cosine W decay angle
    double cosW[2];
    //*** delta phi between top and W decay planes
    double dphiTW[2];
    //*** Eb in top frame
    double EB[2];
    //*** El in W frame
    double EL[2];
    //*** top mass
    double MT[2];
    //*** W mass
    double MW[2];
    

    for(int h = 0; h < 2; h++){
      cosT[h] = T[h]->GetCosDecayAngle();
      cosW[h] = W[h]->GetCosDecayAngle();
      EB[h]   = B[h]->GetEnergy(*T[h]);
      EL[h]   = L[h]->GetEnergy(*W[h]);

      double PB = B[h]->GetMomentum(*T[h]);
      MW[h] = 2.*EL[h];
      MT[h] = EB[h] + sqrt( PB*PB + MW[h]*MW[h] );

      h_MT->Fill(MT[h]/mT);
      h_MW->Fill(MW[h]/mW);
      h_EB->Fill(EB[h]/EBtrue);
      h_EL->Fill(EL[h]/ELtrue);
      h_MT_v_MW->Fill(MT[h]/mT,MW[h]/mW);
      h_EB_v_MW->Fill(EB[h]/EBtrue,MW[h]/mW);
    }

    double MTT = sqrt(MT[0]*MT[0] + PT*PT) + sqrt(MT[1]*MT[1] + PT*PT);
  }

  tree_plot->WriteOutput(output_name);

  g_Log << LogInfo << "Finished" << LogEnd;
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
  example_05();
  return 0;
}
#endif
