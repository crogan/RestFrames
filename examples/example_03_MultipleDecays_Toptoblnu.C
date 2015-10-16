/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_03_MultipleDecays_Toptoblnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example generating and analyzing top to b W(l nu) events
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

#include "RestFrames/RestFrames.hh"

using namespace RestFrames;

void example_03_MultipleDecays_Toptoblnu(string output_name = "output_03.root"){
  setstyle();

  // set particle masses and widths
  double mtop = 173.;
  double wtop = 2.5;
  double mW   = 81.;
  double wW   = 2.5;
  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame           LAB_Gen("LAB_Gen","LAB");
  ResonanceGenFrame     T_Gen("T_Gen","t");
  ResonanceGenFrame     W_Gen("W_Gen","W");
  VisibleGenFrame       B_Gen("B_Gen","b");
  VisibleGenFrame       L_Gen("L_Gen","#it{l}");
  InvisibleGenFrame     NU_Gen("NU_Gen","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(T_Gen);
  T_Gen.AddChildFrame(B_Gen);
  T_Gen.AddChildFrame(W_Gen);
  W_Gen.AddChildFrame(L_Gen);
  W_Gen.AddChildFrame(NU_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << g_End;
  else
    g_Log << LogError << "...Failed initializing generator tree" << g_End;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  T_Gen.SetMass(mtop);
  T_Gen.SetWidth(wtop);
  W_Gen.SetMass(mW);
  W_Gen.SetWidth(wW);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB_Mt("LAB_Mt","LAB"); LabRecoFrame       LAB_MW("LAB_MW","LAB");
  DecayRecoFrame     T_Mt("T_Mt","t");       DecayRecoFrame     T_MW("T_MW","t");
  DecayRecoFrame     W_Mt("W_Mt","W");       DecayRecoFrame     W_MW("W_MW","W");
  VisibleRecoFrame   B_Mt("B_Mt","b");       VisibleRecoFrame   B_MW("B_MW","b");
  VisibleRecoFrame   L_Mt("L_Mt","#it{l}");  VisibleRecoFrame   L_MW("L_MW","#it{l}");
  InvisibleRecoFrame NU_Mt("NU_Mt","#nu");   InvisibleRecoFrame NU_MW("NU_MW","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Mt.SetChildFrame(T_Mt);                LAB_MW.SetChildFrame(T_MW);
  T_Mt.AddChildFrame(B_Mt);                  T_MW.AddChildFrame(B_MW);
  T_Mt.AddChildFrame(W_Mt);                  T_MW.AddChildFrame(W_MW);
  W_Mt.AddChildFrame(L_Mt);                  W_MW.AddChildFrame(L_MW);
  W_Mt.AddChildFrame(NU_Mt);                 W_MW.AddChildFrame(NU_MW);

  if(LAB_Mt.InitializeTree() && LAB_MW.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << g_End;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << g_End;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Now we add invisible jigsaws
  InvisibleGroup INV_Mt("INV_Mt","#nu Jigsaws");   InvisibleGroup INV_MW("INV_MW","nu Jigsaws");
  INV_Mt.AddFrame(NU_Mt);                          INV_MW.AddFrame(NU_MW);
 
  SetMassInvJigsaw NuM_Mt("NuM_Mt","M_{#nu} = 0"); SetMassInvJigsaw NuM_MW("NuM_MW","M_{#nu} = 0");
  INV.AddJigsaw(NuM_Mt);

  SetRapidityInvJigsaw RapidityJigsaw("RAPIDITY_JIGSAW","rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame(L_R);
  RapidityJigsaw.AddVisibleFrame(B_R);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing analyses" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  tree_plot.SetFrameTree(LAB_Gen);
  tree_plot.Draw("GenTree", "Generator Tree");

  tree_plot.SetFrameTree(LAB_Mt);
  tree_plot.Draw("RecoTree", "Reconstruction Tree");

  // Declare observables for histogram booking
  HistPlot* GenPlot   = new HistPlot("Gen","W #rightarrow #it{l} #nu", 
				     "Generator Level"); 
  HistPlot* minMtPlot = new HistPlot("minMt", "W #rightarrow #it{l} #nu",
				     "min M_{t} reconstruction"); 
  HistPlot* minWPlot  = new HistPlot("minMW", "W #rightarrow #it{l} #nu",
				     "min M_{W} reconstruction"); 

  const HistPlotVar& Mt     = GenPlot->GetNewVar("Mt", "M_{t}", 0., 190., "[GeV]");
  const HistPlotVar& MW     = GenPlot->GetNewVar("MW", "M_{W}", 0., 90., "[GeV]");
  const HistPlotVar& pTt    = GenPlot->GetNewVar("pTt","p_{T}^{t} / m_{t}", 0., 1.);
  const HistPlotVar& cosT   = GenPlot->GetNewVar("cosT","cos #theta_{t}", -1., 1.);
  const HistPlotVar& cosW   = GenPlot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  const HistPlotVar& dphiT  = GenPlot->GetNewVar("dphiT", "#Delta #phi_{t}", 0., 2.*acos(-1.));
  const HistPlotVar& dphiW  = GenPlot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosT  = GenPlot->GetNewVar("DcosW","#theta_{t} - #theta_{t}^{gen}", -1., 1.);
  const HistPlotVar& DcosW  = GenPlot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{gen}", -1., 1.);
  const HistPlotVar& DdphiT = GenPlot->GetNewVar("DdphiW","#Delta #phi_{t} - #Delta #phi_{t}^{gen}", -1., 1.);
  const HistPlotVar& DdphiW = GenPlot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{gen}", -1., 1.);

  GenPlot->AddHist(Mt);       GenPlot->AddHist(MW);
  minMtPlot->AddHist(Mt);     minMWPlot->AddHist(Mt);
  minMtPlot->AddHist(MW);     minMWPlot->AddHist(MW);
  minMtPlot->AddHist(pTt);    minMWPlot->AddHist(pTt);
  minMtPlot->AddHist(cosT);   minMWPlot->AddHist(cosT);
  minMtPlot->AddHist(cosW);   minMWPlot->AddHist(cosW);
  minMtPlot->AddHist(dphiT);  minMWPlot->AddHist(dphiT);
  minMtPlot->AddHist(dphiW);  minMWPlot->AddHist(dphiW);
  minMtPlot->AddHist(DcosT);  minMWPlot->AddHist(DcosT);
  minMtPlot->AddHist(DcosW);  minMWPlot->AddHist(DcosW);
  minMtPlot->AddHist(DdphiT); minMWPlot->AddHist(DdphiT);
  minMtPlot->AddHist(DdphiW); minMWPlot->AddHist(DdphiW);

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
    B_R.SetLabFrameFourVector(B_G.GetFourVector()); // Set b-jet 4-vec
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    INV.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    LAB_R.AnalyzeEvent();                           // analyze the event

    // calculate observables
    double MT    = T_R.GetMass();
    double EB    = B_R.GetEnergy(T_R);
    double MTgen = T_G.GetMass();
    double cosT     = T_R.GetCosDecayAngle();
    double cosTgen  = T_G.GetCosDecayAngle();
    double dphiT    = LAB_R.GetDeltaPhiDecayPlanes(T_R);
    double dphiTgen = LAB_G.GetDeltaPhiDecayPlanes(T_G);

    double MW    = W_R.GetMass();
    double MWgen = W_G.GetMass();
    double cosW     = W_R.GetCosDecayAngle();
    double cosWgen  = W_G.GetCosDecayAngle();
    double dphiW    = T_R.GetDeltaPhiDecayPlanes(W_R);
    double dphiWgen = T_G.GetDeltaPhiDecayPlanes(W_G);
    
    double DcosT = asin(sqrt(1.-cosT*cosT)*cosTgen-sqrt(1.-cosTgen*cosTgen)*cosT);
    double DdphiT = asin(sin(dphiT-dphiTgen));
    double DcosW = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    double DdphiW = asin(sin(dphiW-dphiWgen));

    h_MT->Fill(MT);
    h_EB->Fill(EB);
    h_cosT->Fill(cosT);
    h_dphiT->Fill(dphiT);
    h_DcosT->Fill(DcosT);
    h_DdphiT->Fill(DdphiT);
    h_MT_v_cosT->Fill(MT,cosT);
    h_MT_v_MW->Fill(MT,MW);
    h_EB_v_MW->Fill(EB,MW);
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
  string plot_title = "t #rightarrow b W(#it{l} #nu)";
  TCanvas *c_MT          = Plot_Me("c_MT", h_MT, "M_{top} [GeV]", plot_title);
  TCanvas *c_EB          = Plot_Me("c_EB", h_EB, "E^{top}_{b} [GeV]", plot_title);
  TCanvas *c_cosT        = Plot_Me("c_cosT", h_cosT, "cos #theta_{top}", plot_title);
  TCanvas *c_dphiT       = Plot_Me("c_dphiT", h_dphiT, "#Delta #phi (#hat{n}_{top}, #hat{n}_{W})", plot_title);
  TCanvas *c_DcosT       = Plot_Me("c_DcosT", h_DcosT, "#theta_{top} - #theta_{top}^{true}", plot_title);
  TCanvas *c_DdphiT      = Plot_Me("c_DdphiT", h_DdphiT, "#Delta #phi_{top} - #Delta #phi_{top}^{true}", plot_title);
  TCanvas *c_MT_v_cosT   = Plot_Me("c_MT_v_cosT", h_MT_v_cosT, "M_{top} [GeV]", 
				   "cos #theta_{top}", plot_title);
  TCanvas *c_MT_v_MW     = Plot_Me("c_MT_v_MW", h_MT_v_MW, "M_{top} [GeV]", 
				   "M_{W} [GeV]", plot_title);
  TCanvas *c_EB_v_MW     = Plot_Me("c_EB_v_MW", h_EB_v_MW, "E^{top}_{b} [GeV]", 
				   "M_{W} [GeV]", plot_title);
  // TCanvas *c_MT_v_PT     = Plot_Me("c_MW_v_PT", h_MW_v_PT, "M_{W} [GeV]", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_cosT_v_PT   = Plot_Me("c_cosW_v_PT", h_cosW_v_PT, "cos #theta_{W}", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_dphiT_v_PT  = Plot_Me("c_dphiW_v_PT", h_dphiW_v_PT, 
  // 				   "#Delta #phi (#hat{n}_{W}, (#hat{z} #times #hat{p}_{W}))", 
  // 				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_DcosT_v_PT  = Plot_Me("c_DcosW_v_PT", h_DcosW_v_PT, 
  // 				   "#theta_{W} - #theta_{W}^{true}", 
  // 				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_DdphiT_v_PT = Plot_Me("c_DdphiW_v_PT", h_DdphiW_v_PT, 
  // 				   "#Delta #phi_{W} - #Delta #phi_{W}^{true}", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);


  TCanvas *c_MW          = Plot_Me("c_MW", h_MW, "M_{W} [GeV]", plot_title);
  TCanvas *c_cosW        = Plot_Me("c_cosW", h_cosW, "cos #theta_{W}", plot_title);
  TCanvas *c_dphiW       = Plot_Me("c_dphiW", h_dphiW, "#Delta #phi (#hat{n}_{W}, (#hat{z} #times #hat{p}_{W}))", plot_title);
  TCanvas *c_DcosW       = Plot_Me("c_DcosW", h_DcosW, "#theta_{W} - #theta_{W}^{true}", plot_title);
  TCanvas *c_DdphiW      = Plot_Me("c_DdphiW", h_DdphiW, "#Delta #phi_{W} - #Delta #phi_{W}^{true}", plot_title);
  TCanvas *c_MW_v_cosW   = Plot_Me("c_MW_v_cosW", h_MW_v_cosW, "M_{W} [GeV]", 
  				   "cos #theta_{W}", plot_title);
  // TCanvas *c_MW_v_PT     = Plot_Me("c_MW_v_PT", h_MW_v_PT, "M_{W} [GeV]", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_cosW_v_PT   = Plot_Me("c_cosW_v_PT", h_cosW_v_PT, "cos #theta_{W}", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_dphiW_v_PT  = Plot_Me("c_dphiW_v_PT", h_dphiW_v_PT, 
  // 				   "#Delta #phi (#hat{n}_{W}, (#hat{z} #times #hat{p}_{W}))", 
  // 				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_DcosW_v_PT  = Plot_Me("c_DcosW_v_PT", h_DcosW_v_PT, 
  // 				   "#theta_{W} - #theta_{W}^{true}", 
  // 				  "p_{T}^{W} / m_{W}^{true}", plot_title);
  // TCanvas *c_DdphiW_v_PT = Plot_Me("c_DdphiW_v_PT", h_DdphiW_v_PT, 
  // 				   "#Delta #phi_{W} - #Delta #phi_{W}^{true}", 
  // 				   "p_{T}^{W} / m_{W}^{true}", plot_title);

  TFile *foutput = new TFile(output_name.c_str(),"RECREATE");
  foutput->cd();
 
  // c_MW->Write();
  // c_cosW->Write(); 
  // c_dphiW->Write(); 
  // c_DcosW->Write(); 
  // c_DdphiW->Write();
  // c_MW_v_cosW->Write(); 
  // c_MW_v_PT->Write();
  // c_cosW_v_PT->Write();
  // c_dphiW_v_PT->Write();
  // c_DcosW_v_PT->Write(); 
  // c_DdphiW_v_PT->Write();
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
  example_03_MultipleDecays_Toptoblnu();
  return 0;
}
#endif
