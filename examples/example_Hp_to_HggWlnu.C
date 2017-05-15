/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_Hp_to_HggWlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Jul
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

#define COMPILER (!defined(__CINT__) && !defined(__CLING__))
#if defined(__MAKECINT__) || defined(__ROOTCLING__) || COMPILER
#include "RestFrames/RestFrames.hh"
#else
RestFrames::RFKey ensure_autoload(1);
#endif

using namespace RestFrames;

void example_Hp_to_HggWlnu(const std::string& output_name =
			   "output_Hp_to_HggWlnu.root"){
 
  // set particle masses and widths
  double mH   = 125.; 
  double mW   = 80.385; // GeV, PDG 2016
  double wW   = 2.085;
  
  std::vector<double> mHp; // vary charged Higgs mass
  mHp.push_back(300.);
  mHp.push_back(500.);
  mHp.push_back(750.);
  mHp.push_back(1000.);
  mHp.push_back(1500.);
  
  // Number of events to generate (per H+ mass)
  int Ngen = 10000;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame         LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame         Hp_Gen("Hp_Gen","H^{ +}");
  DecayGenFrame         H_Gen("H_Gen","h^{ 0}");
  ResonanceGenFrame     W_Gen("W_Gen","W^{ +}");
  VisibleGenFrame       G1_Gen("G1_Gen","#gamma_{1}");
  VisibleGenFrame       G2_Gen("G2_Gen","#gamma_{2}");
  VisibleGenFrame       L_Gen("L_Gen","#it{l}^{ +}");
  InvisibleGenFrame     NU_Gen("NU_Gen","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(Hp_Gen);
  Hp_Gen.AddChildFrame(H_Gen);
  Hp_Gen.AddChildFrame(W_Gen);
  H_Gen.AddChildFrame(G1_Gen);
  H_Gen.AddChildFrame(G2_Gen);
  W_Gen.AddChildFrame(L_Gen);
  W_Gen.AddChildFrame(NU_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  if(mHp.size() < 1) return;
  
  // set neutral and charged Higgs mass 
  Hp_Gen.SetMass(mHp[0]);              H_Gen.SetMass(mH);         
  // set W mass and width
  W_Gen.SetMass(mW);                   W_Gen.SetWidth(wW);

  // set photon/lepton pT and eta cuts
  L_Gen.SetPtCut(15.);                 L_Gen.SetEtaCut(2.5);
  G1_Gen.SetPtCut(20.);                G1_Gen.SetEtaCut(3.);
  G2_Gen.SetPtCut(20.);                G2_Gen.SetEtaCut(3.);  

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB("LAB","LAB");
  DecayRecoFrame     Hp("Hp","H^{ +}");
  DecayRecoFrame     H("H","h^{ 0}");
  DecayRecoFrame     W("W","W^{ +}");
  VisibleRecoFrame   G1("G1","#gamma_{1}");
  VisibleRecoFrame   G2("G2","#gamma_{2}");
  VisibleRecoFrame   L("L","#it{l}^{ +}");
  InvisibleRecoFrame NU("NU","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(Hp);
  Hp.AddChildFrame(H);
  Hp.AddChildFrame(W);
  H.AddChildFrame(G1);
  H.AddChildFrame(G2);
  W.AddChildFrame(L);
  W.AddChildFrame(NU);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV("INV","#nu Jigsaws");
  INV.AddFrame(NU);     

  // Set neutrino masses to zero
  SetMassInvJigsaw NuM("NuM","M_{#nu} = 0"); 
  INV.AddJigsaw(NuM);                        

  // Set neutrino rapidity to that of visible particles
  SetRapidityInvJigsaw NuR("NuR","#eta_{#nu} = #eta_{#gamma#gamma#it{l}}");
  INV.AddJigsaw(NuR);
  NuR.AddVisibleFrames(Hp.GetListVisibleFrames());

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);

  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  // Declare observables for histogram booking
  std::string plot_title = "pp #rightarrow H^{ +} #rightarrow h^{ 0}(#gamma #gamma ) W(#it{l} #nu)";
  HistPlot* histPlot   = new HistPlot("HistPlot", plot_title);

  RFList<const HistPlotCategory> cat_list;
  int Nmass = mHp.size();
  for(int m = 0; m < Nmass; m++){
    char smass[50], scat[50];
    sprintf(scat, "MHp%.0f", mHp[m]);
    sprintf(smass, "m_{H^{ +}} = %.0f", mHp[m]);
    cat_list += histPlot->GetNewCategory(scat, smass);
  }

  const HistPlotCategory& cat_Hp  = histPlot->GetNewCategory("HprodHp", "h^{ 0} prod. frame H^{ +}");
  const HistPlotCategory& cat_LAB = histPlot->GetNewCategory("HprodLAB", "h^{ 0} prod. frame LAB");
  
  const HistPlotVar& MHp    = histPlot->GetNewVar("MHp", "M_{H^{ +}}", 0., 2400., "[GeV]");
  const HistPlotVar& MHpN   = histPlot->GetNewVar("MHpN", "M_{H^{ +}} / m_{H^{ +}}^{true}", 0.7, 1.05);
  const HistPlotVar& MWN    = histPlot->GetNewVar("MWN", "M_{W} / m_{W}^{true}", 0., 2.);
  const HistPlotVar& cosHp  = histPlot->GetNewVar("cosHp","cos #theta_{H^{ +}}", -1., 1.);
  const HistPlotVar& cosW   = histPlot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  const HistPlotVar& cosH   = histPlot->GetNewVar("cosH","cos #theta_{h^{ 0}}", -1., 1.);
  const HistPlotVar& DcosHp = histPlot->GetNewVar("DcosHp","#theta_{H^{ +}} - #theta_{H^{ +}}^{true}", -1., 1.);
  const HistPlotVar& DcosW  = histPlot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{true}", -1., 1.);
  const HistPlotVar& DcosH  = histPlot->GetNewVar("DcosW","#theta_{h^{ 0}} - #theta_{h^{ 0}}^{true}", -1., 1.);

  histPlot->AddPlot(DcosH,  cat_list);
  histPlot->AddPlot(DcosW,  cat_list);
  histPlot->AddPlot(DcosHp, cat_list);
  histPlot->AddPlot(MWN,  cat_list);
  histPlot->AddPlot(MHpN, cat_list);
  histPlot->AddPlot(MHp,  cat_list);
  histPlot->AddPlot(MWN,    MHpN,   cat_list[Nmass/2]);
  histPlot->AddPlot(DcosW, MWN,  cat_list[Nmass/2]);
  histPlot->AddPlot(DcosHp, MHpN, cat_list[Nmass/2]);
  histPlot->AddPlot(DcosHp, DcosW,  cat_list[Nmass/2]);
  histPlot->AddPlot(DcosH,  MHpN,   cat_list[Nmass/2]);
  
  histPlot->AddPlot(DcosH,  cat_Hp+cat_LAB);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int m = 0; m < Nmass; m++){
    g_Log << LogInfo << "Generating events for H^{+} mass = " << mHp[m] << LogEnd;

    Hp_Gen.SetMass(mHp[m]);
    LAB_Gen.InitializeAnalysis();
  
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;
      
      // generate event
      LAB_Gen.ClearEvent();                            // clear the gen tree

      LAB_Gen.SetPToverM(gRandom->Rndm());             // give charged Higgs some Pt
      
      LAB_Gen.AnalyzeEvent();                          // generate a new event

      TVector3 MET = LAB_Gen.GetInvisibleMomentum();   // Get the MET from gen tree
      MET.SetZ(0.);
    
      // analyze event 
      LAB.ClearEvent();                                   // clear the reco tree
    
      L.SetLabFrameFourVector(L_Gen.GetFourVector());     // Set lepton 4-vec
      G1.SetLabFrameFourVector(G1_Gen.GetFourVector());   // Set photons' 4-vec
      G2.SetLabFrameFourVector(G2_Gen.GetFourVector());   
      INV.SetLabFrameThreeVector(MET);                    // Set the MET in reco tree
    
      LAB.AnalyzeEvent();                                 //analyze the event

      // Generator-level observables
      double MHpgen = Hp_Gen.GetMass();
      double MWgen  = W_Gen.GetMass();
      double cosHpgen = Hp_Gen.GetCosDecayAngle();
      double cosHgen  = H_Gen.GetCosDecayAngle();
      double cosWgen  = W_Gen.GetCosDecayAngle();

      // Reconstructed observables
      MHp    = Hp.GetMass();
      MHpN   = Hp.GetMass()/MHpgen;
      MWN    = W.GetMass()/MWgen;
      cosHp  = Hp.GetCosDecayAngle();
      cosH   = H.GetCosDecayAngle();
      cosW   = W.GetCosDecayAngle();
      DcosHp = asin(sqrt(1.-cosHp*cosHp)*cosHpgen-sqrt(1.-cosHpgen*cosHpgen)*cosHp);
      DcosH  = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);
      DcosW  = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);

      histPlot->Fill(cat_list[m]);

      if(m == 0){
	histPlot->Fill(cat_Hp);
	
	TVector3 Hboost = H.GetFourVector().BoostVector();
	TLorentzVector vP_G1 = G1.GetFourVector();
	vP_G1.Boost(-Hboost);
	cosH   = -vP_G1.Vect().Unit().Dot(Hboost.Unit());
	DcosH  = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);

	histPlot->Fill(cat_LAB);
      }
    }

    LAB_Gen.PrintGeneratorEfficiency();
  }

  histPlot->Draw();
  
  TFile fout(output_name.c_str(),"RECREATE");
  fout.Close();
  histPlot->WriteOutput(output_name);
  histPlot->WriteHist(output_name);
  treePlot->WriteOutput(output_name);

  g_Log << LogInfo << "Finished" << LogEnd;

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_Hp_to_HggWlnu();
  return 0;
}
#endif
