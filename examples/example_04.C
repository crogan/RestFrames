/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_04.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 June
///
///  \brief An example generating and analyzing Higgs to WW to 2l 2nu events
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

using namespace RestFrames;

void example_04(const std::string& output_name = "output_example_04.root"){
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
  g_Log << LogInfo << "Initializing generator frames and tree" << LogEnd;
  ppLabGenFrame LAB_G("LAB_G","LAB");
  ResonanceGenFrame H_G("H_G","H");
  //DecayGenFrame H_G("H_G","H");
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
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready to set masses etc." << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_G) << LogEnd;								    
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
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready for event generation" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize analysis from LabFrame: " << Log(LAB_G) << LogEnd;


  // Set up reco analysis tree
  g_Log << LogInfo << "Initializing reconstruction frames and tree" << LogEnd;
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
    g_Log << LAB_R.GetName() << std::endl;
    g_Log << "Ready for Group and Jigsaw initialization" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_R) << LogEnd;  

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

  g_Log << LogInfo << "Initializing tree for analysis" << LogEnd;
  // check reconstruction trees
  if(LAB_R.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "...Successfully initialized tree for analysis from LabFrame ";
    g_Log << LAB_R.GetName() << std::endl;
    g_Log << "Ready event analysis" << LogEnd;
  } else
    g_Log << LogError << "...Unable to initialize analysis from LabFrame: " << Log(LAB_R) << LogEnd;	

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
  TH1D* h_MW     = new TH1D("h_MW","h_MW",100,0.,200.);
  TH2D* h_MH_v_MW   = new TH2D("h_MH_v_MW","h_MH_v_MW",50,0.,2.,50,0.,2.);
  TH2D* h_mW_v_mW   = new TH2D("h_mW_v_mW","h_mW_v_mW",50,0.,mH,50,0.,mH);
  TH1D* h_pzH     = new TH1D("h_pzH","h_pzH",100,-10000., 10000.);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTH = mH*gRandom->Rndm()*0.0001;
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
    h_MW->Fill(MW);
    h_MH_v_MW->Fill(MH/H_G.GetMass(),MW/mW);
    h_mW_v_mW->Fill(Wa_G.GetMass(),Wb_G.GetMass());
    h_pzH->Fill(H_G.GetFourVector().Pz());
  }

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  // reco tree
  tree_plot->SetTree(LAB_R);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  // reco tree
  tree_plot->SetTree(INV_R);
  tree_plot->Draw("InvTree", "Invisible Jigsaws",true);

  // reco tree
  tree_plot->SetTree(RapidityJigsaw_R);
  tree_plot->Draw("InvTree", "Invisible Jigsaws",true);
 

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_04();
  return 0;
}
#endif
