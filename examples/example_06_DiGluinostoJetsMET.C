/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_06_DiGluinostoJetsMET.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 April
///
///  \brief An example generating and analyzing di-gluino events
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
#include <TF1.h>
#include <iostream>
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/RSelfAssemblingFrame.hh"
#include "RestFrames/GLabFrame.hh"
#include "RestFrames/GDecayFrame.hh"
#include "RestFrames/GVisibleFrame.hh"
#include "RestFrames/GInvisibleFrame.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/MinimizeMassesCombinatoricJigsaw.hh"
#include "RestFrames/FramePlot.hh"

void setstyle();
TCanvas* Plot_Me(string scan, TH2D* histo, string X, string Y, string title = "", string label = "");
TCanvas* Plot_Me(string scan, TH1D* histo, string X, string title = "", string label = "");

using namespace std;
using namespace RestFrames;

void example_06_DiGluinostoJetsMET(string output_name = "output_06.root"){
  setstyle();

  double mG = 1000.;
  double mX = 100.;
  int Ngen = 10000;

  //
  // Set up toy generation tree (not needed for reconstruction)
  GLabFrame LAB_G("LAB_G","LAB");
  GDecayFrame GG_G("GG_G","#tilde{g}#tilde{g}");
  GDecayFrame Ga_G("Ga_G","#tilde{g}_{a}");
  GDecayFrame Gb_G("Gb_G","#tilde{g}_{b}");
  GVisibleFrame V1a_G("V1a_G","j_{1a}");
  GVisibleFrame V2a_G("V2a_G","j_{2a}");
  GInvisibleFrame Xa_G("Xa_G","#tilde{#chi}_{a}");
  GVisibleFrame V1b_G("V1b_G","j_{1b}");
  GVisibleFrame V2b_G("V2b_G","j_{2b}");
  GInvisibleFrame Xb_G("Xb_G","#tilde{#chi}_{b}");
  LAB_G.SetChildFrame(GG_G);
  GG_G.AddChildFrame(Ga_G);
  GG_G.AddChildFrame(Gb_G);
  Ga_G.AddChildFrame(V1a_G);
  Ga_G.AddChildFrame(V2a_G);
  Ga_G.AddChildFrame(Xa_G);
  Gb_G.AddChildFrame(V1b_G);
  Gb_G.AddChildFrame(V2b_G);
  Gb_G.AddChildFrame(Xb_G);
  if(!LAB_G.InitializeTree()) cout << "Problem with generator tree" << endl;
  //
  //
									       

  // Set up 'signal-like' analysis tree
  RLabFrame LAB_R("LAB_R","LAB");
  RDecayFrame GG_R("GG_R","#tilde{g}#tilde{g}");
  RDecayFrame Ga_R("Ga_R","#tilde{g}_{a}");
  RDecayFrame Gb_R("Gb_R","#tilde{g}_{b}");
  RDecayFrame Ca_R("Ca_R","C_{a}");
  RDecayFrame Cb_R("Cb_R","C_{b}");
  RVisibleFrame V1a_R("V1a_R","j_{1a}");
  RVisibleFrame V2a_R("V2a_R","j_{2a}");
  RInvisibleFrame Xa_R("Xa_R","#tilde{#chi}_{a}");
  RVisibleFrame V1b_R("V1b_R","j_{1b}");
  RVisibleFrame V2b_R("V2b_R","j_{2b}");
  RInvisibleFrame Xb_R("Xb_R","#tilde{#chi}_{b}");
  LAB_R.SetChildFrame(GG_R);
  GG_R.AddChildFrame(Ga_R);
  GG_R.AddChildFrame(Gb_R);
  Ga_R.AddChildFrame(V1a_R);
  Ga_R.AddChildFrame(Ca_R);
  Ca_R.AddChildFrame(V2a_R);
  Ca_R.AddChildFrame(Xa_R);
  Gb_R.AddChildFrame(V1b_R);
  Gb_R.AddChildFrame(Cb_R);
  Cb_R.AddChildFrame(V2b_R);
  Cb_R.AddChildFrame(Xb_R);

  // Set up 'background-like' analysis tree
  RLabFrame LAB_B("LAB_B","LAB");
  RSelfAssemblingFrame CM_B("CM_B","CM");
  RVisibleFrame V_B("V_B","Vis");
  RInvisibleFrame I_B("I_B","Inv");
  LAB_B.SetChildFrame(CM_B);
  CM_B.AddChildFrame(V_B);
  CM_B.AddChildFrame(I_B);
 
  if(!LAB_R.InitializeTree()) cout << "Problem with signal-like reconstruction tree" << endl; 
  if(!LAB_B.InitializeTree()) cout << "Problem with background-like reconstruction tree" << endl; 

  // define groups for the reconstruction trees
  
  InvisibleGroup INV_R("INV_R","WIMP Jigsaws");
  INV_R.AddFrame(Xa_R);
  INV_R.AddFrame(Xb_R);
  CombinatoricGroup VIS_R("VIS","Visible Object Jigsaws");
  // visible frames in first decay step must always have at least one element
  VIS_R.AddFrame(V1a_R);
  VIS_R.AddFrame(V1b_R);
  VIS_R.SetNElementsForFrame(V1a_R,1,false);
  VIS_R.SetNElementsForFrame(V1b_R,1,false);
  // visible frames in second decay step can have zero elements
  VIS_R.AddFrame(V2a_R);
  VIS_R.AddFrame(V2b_R);
  VIS_R.SetNElementsForFrame(V2a_R,0,false);
  VIS_R.SetNElementsForFrame(V2b_R,0,false);

  InvisibleGroup INV_B("INV_B","Invisible State Jigsaws");
  INV_B.AddFrame(I_B);
  CombinatoricGroup VIS_B("VIS_B","Visible Object Jigsaws");
  VIS_B.AddFrame(V_B);
  VIS_B.SetNElementsForFrame(V_B,1,false);

  // define jigsaws for the reconstruction trees
  
  // signal-like jigsaws
  InvisibleMassJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);
  InvisibleRapidityJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrame((LAB_R.GetListVisibleFrames()));
  ContraBoostInvariantJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrame((Ga_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrame((Gb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrame((Ga_R.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw_R.AddInvisibleFrame((Gb_R.GetListInvisibleFrames()), 1);
  MinimizeMassesCombinatoricJigsaw HemiJigsaw_R("HEM_JIGSAW_R","Minimize m _{V_{a,b}} Jigsaw");
  VIS_R.AddJigsaw(HemiJigsaw_R);
  HemiJigsaw_R.AddFrame(V1a_R,0);
  HemiJigsaw_R.AddFrame(V1b_R,1);
  HemiJigsaw_R.AddFrame(V2a_R,0);
  HemiJigsaw_R.AddFrame(V2b_R,1);
  MinimizeMassesCombinatoricJigsaw CaHemiJigsaw_R("CbHEM_JIGSAW_R","Minimize m _{C_{a}} Jigsaw");
  VIS_R.AddJigsaw(CaHemiJigsaw_R);
  CaHemiJigsaw_R.AddFrame(V1a_R,0);
  CaHemiJigsaw_R.AddFrame(V2a_R,1);
  CaHemiJigsaw_R.AddFrame(Xa_R,1);
  MinimizeMassesCombinatoricJigsaw CbHemiJigsaw_R("CaHEM_JIGSAW_R","Minimize m _{C_{b}} Jigsaw");
  VIS_R.AddJigsaw(CbHemiJigsaw_R);
  CbHemiJigsaw_R.AddFrame(V1b_R,0);
  CbHemiJigsaw_R.AddFrame(V2b_R,1);
  CbHemiJigsaw_R.AddFrame(Xb_R,1);

  // background tree jigsaws
  InvisibleMassJigsaw MinMassJigsaw_B("MINMASS_B","Zero Mass for invisible system");
  INV_B.AddJigsaw(MinMassJigsaw_B);
  InvisibleRapidityJigsaw RapidityJigsaw_B("RAPIDITY_B","Invisible system rapidity Jigsaw");
  INV_B.AddJigsaw(RapidityJigsaw_B);
  RapidityJigsaw_B.AddVisibleFrame((LAB_B.GetListVisibleFrames()));

  // check reconstruction trees
  if(!LAB_R.InitializeAnalysis()) cout << "Problem with signal-tree jigsaws" << endl;
  if(!LAB_B.InitializeAnalysis()) cout << "Problem with background-tree jigsaws" << endl;

  //////////////////////////////////////////////////////////////
  // draw some pictures of our trees
  //////////////////////////////////////////////////////////////
  FramePlot* treePlot_G = new FramePlot("tree_G","Generator Tree");
  treePlot_G->AddFrameTree(LAB_G);
  treePlot_G->DrawFramePlot();
  TCanvas* c_gentree = treePlot_G->GetCanvas();

  FramePlot* treePlot_R = new FramePlot("tree_R","Signal-like Reconstruction Tree");
  treePlot_R->AddFrameTree(LAB_R);
  treePlot_R->DrawFramePlot();
  TCanvas* c_recotree = treePlot_R->GetCanvas();

  FramePlot* treePlot_B = new FramePlot("tree_B","Background-like Reconstruction Tree");
  treePlot_B->AddFrameTree(LAB_B);
  treePlot_B->DrawFramePlot();
  TCanvas* c_bkgtree = treePlot_B->GetCanvas();

  FramePlot* INVPlot_R = new FramePlot("INV_R","Invisible Objects Jigsaws");
  INVPlot_R->AddGroupTree(INV_R);
  INVPlot_R->DrawFramePlot();
  TCanvas* c_invRtree = INVPlot_R->GetCanvas();

  FramePlot* VISPlot_R = new FramePlot("VIS_R","Visible Objects Jigsaws");
  VISPlot_R->AddGroupTree(VIS_R);
  VISPlot_R->DrawFramePlot();
  TCanvas* c_visRtree = VISPlot_R->GetCanvas();

  // set gluino masses
  Ga_G.SetMass(mG);
  Gb_G.SetMass(mG);
  // set X masses
  Xa_G.SetMass(mX);
  Xb_G.SetMass(mX);
  // function for randomly determining di-gluino mass 
  // (relative to gluino mass via gamma)
  TF1 f_gamma("f_gamma","(x-1)*exp(-2.*x)",1.,10.);
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) cout << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_G.ClearEvent();                             // clear the gen tree
    double mGG = 2.*mG*f_gamma.GetRandom();         // get a random di-gluino mass
    GG_G.SetMass(mGG);
    double PTGG = mGG*gRandom->Rndm();
    LAB_G.SetTransverseMomenta(PTGG);               // give the di-gluinos some Pt
    double PzGG = mGG*(2.*gRandom->Rndm()-1.);
    LAB_G.SetLongitudinalMomenta(PzGG);             // give the di-gluinos some Pz
    LAB_G.AnalyzeEvent();                           // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    vector<TLorentzVector> JETS;                    // Get the Jets from gen tree
    JETS.push_back(V1a_G.GetFourVector());
    JETS.push_back(V2a_G.GetFourVector());
    JETS.push_back(V1b_G.GetFourVector());
    JETS.push_back(V2b_G.GetFourVector());
    
    // give the signal-like tree the event info and analyze
    LAB_R.ClearEvent();                              // clear the signal-like tree
    INV_R.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    vector<GroupElementID> jetID;                    // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID.push_back(VIS_R.AddLabFrameFourVector(JETS[i]));
    LAB_R.AnalyzeEvent();                            // analyze the event

    // give the background-like tree the event info and analyze
    LAB_B.ClearEvent();                                   // clear the bkg-like tree
    INV_B.SetLabFrameThreeVector(MET);                    // Set the MET in tree
    for(int i = 0; i < int(JETS.size()); i++){
      TLorentzVector jet = JETS[i];
      jet.SetPtEtaPhiM(jet.Pt(), 0., jet.Phi(), jet.M()); // only pass transverse info to bkg-like tree
      VIS_B.AddLabFrameFourVector(jet);
    }
    LAB_B.AnalyzeEvent();                                 // analyze the event

    RDecayFrame* G[2];
    RDecayFrame* C[2];
    RVisibleFrame* VS[2];
    RVisibleFrame* VC[2];
    RInvisibleFrame* X[2];
    // Randomize the two hemispheres
    int flip = (gRandom->Rndm() > 0.5);
    G[flip] = &Ga_R;
    G[(flip+1)%2] = &Gb_R;
    C[flip] = &Ca_R;
    C[(flip+1)%2] = &Cb_R;
    VS[flip] = &V1a_R;
    VS[(flip+1)%2] = &V1b_R;
    VC[flip] = &V2a_R;
    VC[(flip+1)%2] = &V2b_R;
    X[flip] = &Xa_R;
    X[(flip+1)%2] = &Xb_R;

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** total CM mass
    double shat = GG_R.GetMass();
    //*** 'mass-less' gluino gamma in CM frame
    double gaminv = GG_R.GetVisibleShape();
    
    TVector3 vPGG = GG_R.GetFourVector(LAB_R).Vect();
    
    //*** ratio of CM pT to CM mass
    double RPT = vPGG.Pt() / (vPGG.Pt() + shat/4.);
    //*** ratio of CM pz to CM mass
    double RPZ = vPGG.Pz() / (vPGG.Pz() + shat/4.);
    //*** cos decay angle of GG system
    double cosGG = GG_R.GetCosDecayAngle();
    //*** delta phi between lab and GG decay planes
    double dphiLGG = LAB_R.GetDeltaPhiDecayPlanes(GG_R);
    
    TLorentzVector vV1 = G[0]->GetVisibleFourVector(G[0]);
    TLorentzVector vV2 = G[1]->GetVisibleFourVector(G[1]);

    //*** gluino mass
    double MG = (vV1.M2()-vV2.M2())/(2.*(vV1.E()-vV2.E()));
    
    double PG = G[0]->GetMomentum(GG_R);
    double MGG = 2.*sqrt(PG*PG + MG*MG);
    double gaminvGG = 2.*MG/MGG;
    double beta = sqrt(1.- gaminv*gaminv);
    double betaGG = sqrt(1.- gaminvGG*gaminvGG);
    
    //*** velocity difference between 'massive' and 'mass-less'
    double DeltaBetaGG = -(betaGG-beta)/(1.-betaGG*beta);
    //*** delta phi between GG visible decay products and GG decay axis
    double dphiVG = GG_R.GetDeltaPhiDecayVisible();
    //*** delta phi between GG visible decay products and GG momentum
    double dphiVGG = GG_R.GetDeltaPhiBoostVisible();
    
    // 'hemisphere' (one for each 'gluino') observables

    //*** number of visible objects (jets) in hemisphere
    double NV[2];
    //*** cosine gluino decay angle
    double cosG[2];
    //*** cosine intermediate child decay angle
    double cosC[2];
    //*** delta phi between gluino and child decay planes
    double dphiGC[2];
    //*** ratio of child and gluino masses (w/ WIMP masses subtracted)
    double RCG[2];
    //*** 1st leading jet pT _associated with this hemisphere_
    double jet1PT[2];
    //*** 2nd leading jet pT _associated with this hemisphere_
    double jet2PT[2];
      
    for(int i = 0; i < 2; i++){
      NV[i] =  VIS_R.GetNElementsInFrame(VS[i]);
      NV[i] += VIS_R.GetNElementsInFrame(VC[i]);

      cosG[i] = G[i]->GetCosDecayAngle();

      int N = jetID.size();
      double pTmax[2]; pTmax[0] = -1.; pTmax[1] = -1.;
      for(int j = 0; j < N; j++){
	const RestFrame* frame = VIS_R.GetFrame(jetID[j]);
	if(VS[i]->IsSame(frame) || VC[i]->IsSame(frame)){
	  double pT = VIS_R.GetLabFrameFourVector(jetID[j]).Pt();
	  if(pT > pTmax[0]){
	    pTmax[1] = pTmax[0];
	    pTmax[0] = pT;
	  } else {
	    if(pT > pTmax[1]) pTmax[1] = pT;
	  }
	}
      }
      
      jet1PT[i] = pTmax[0];
      jet2PT[i] = pTmax[1];

      if(NV[i] > 1){
	cosC[i] = C[i]->GetCosDecayAngle();
	dphiGC[i] = G[i]->GetDeltaPhiDecayPlanes(C[i]);
	RCG[i] = (C[i]->GetMass()-X[i]->GetMass())/(G[i]->GetMass()-X[i]->GetMass());
      } else {
	cosC[i] = -2.;
	dphiGC[i] = -1.;
	RCG[i] = -1.;
	jet2PT[i] = -1.;
      }
    }

    //
    // background tree observables
    //
    TLorentzVector Psib = I_B.GetSiblingFrame()->GetFourVector(LAB_B);
    TLorentzVector Pmet = I_B.GetFourVector(LAB_B);
      
    //*** 
    double Rpsib = max(0.,Psib.Vect().Dot(Pmet.Vect().Unit()));
    Rpsib = Rpsib / (Pmet.Pt() + Rpsib);
    
    TVector3 boostQCD = (Pmet+Psib).BoostVector();
    Psib.Boost(-boostQCD);
    double cosQCD = -1.*Psib.Vect().Unit().Dot(boostQCD.Unit());
    cosQCD = (1.-cosQCD)/2.;

    //*** 
    double DeltaQCD = (cosQCD-Rpsib)/(cosQCD+Rpsib);

  }
  
  TFile *foutput = new TFile(output_name.c_str(),"RECREATE");
  foutput->cd();
  c_gentree->Write();
  c_recotree->Write();
  c_bkgtree->Write();
  c_invRtree->Write();
  c_visRtree->Write();
  delete c_gentree;
  delete c_recotree;
  delete c_bkgtree;
  delete c_invRtree;
  delete c_visRtree;
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
  example_06_DiGluinostoJetsMET();
  return 0;
}
#endif
