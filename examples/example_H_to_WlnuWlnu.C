/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_H_to_WlnuWlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 June
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

void example_H_to_WlnuWlnu(const std::string& output_name =
			   "output_H_to_WlnuWlnu.root"){

  // set particle masses and widths
  double mW   = 80.385;  // GeV, PDG 2016
  double wW   = 2.085;
  double mL   = 0.106;   // muons
  double mN   = 0.;

  std::vector<double> mH; // vary neutral Higgs mass
  mH.push_back(125.);
  mH.push_back(400.);
  mH.push_back(750.);
  mH.push_back(1000.);
  mH.push_back(1500.);

  // Number of events to generate (per H mass)
  int Ngen = 10000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     H_Gen("H_Gen","H^{0}");
  ResonanceGenFrame Wa_Gen("Wa_Gen","W_{a}");
  ResonanceGenFrame Wb_Gen("Wb_Gen","W_{b}");
  VisibleGenFrame   La_Gen("La_Gen","#it{l}_{a}");
  InvisibleGenFrame Na_Gen("Na_Gen","#nu_{a}");
  VisibleGenFrame   Lb_Gen("Lb_Gen","#it{l}_{b}");
  InvisibleGenFrame Nb_Gen("Nb_Gen","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(H_Gen);
  H_Gen.AddChildFrame(Wa_Gen);
  H_Gen.AddChildFrame(Wb_Gen);
  Wa_Gen.AddChildFrame(La_Gen);
  Wa_Gen.AddChildFrame(Na_Gen);
  Wb_Gen.AddChildFrame(Lb_Gen);
  Wb_Gen.AddChildFrame(Nb_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;								    
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  if(mH.size() < 1) return;
  
  // set Higgs masses
  H_Gen.SetMass(mH[0]);
  // set W masses and widths
  Wa_Gen.SetMass(mW);                    Wa_Gen.SetWidth(wW);
  Wb_Gen.SetMass(mW);                    Wb_Gen.SetWidth(wW);
  // set lepton and neutrino masses
  La_Gen.SetMass(mL);                    Lb_Gen.SetMass(mL);

  // set lepton pT and eta cuts
  La_Gen.SetPtCut(10.);                  Lb_Gen.SetPtCut(10.);
  La_Gen.SetEtaCut(2.5);                 Lb_Gen.SetEtaCut(2.5);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame         LAB("LAB","LAB");
  DecayRecoFrame       H("H","H^{ 0}");
  DecayRecoFrame       Wa("Wa","W_{a}");
  DecayRecoFrame       Wb("Wb","W_{b}");
  VisibleRecoFrame     La("La","#it{l}_{a}");
  InvisibleRecoFrame   Na("Na","#nu_{a}");
  VisibleRecoFrame     Lb("Lb","#it{l}_{b}");
  InvisibleRecoFrame   Nb("Nb","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(H);
  H.AddChildFrame(Wa);
  H.AddChildFrame(Wb);
  Wa.AddChildFrame(La);
  Wa.AddChildFrame(Na);
  Wb.AddChildFrame(Lb);
  Wb.AddChildFrame(Nb);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV("INV","#nu #nu Jigsaws");
  INV.AddFrame(Na);
  INV.AddFrame(Nb);

  // Set nu nu mass equal to l l mass
  SetMassInvJigsaw NuNuM("NuNuM", "M_{#nu#nu} = m_{#it{l}#it{l}}");
  INV.AddJigsaw(NuNuM);

  SetRapidityInvJigsaw NuNuR("NuNuR", "#eta_{#nu#nu} = #eta_{#it{l}#it{l}}");
  INV.AddJigsaw(NuNuR);
  NuNuR.AddVisibleFrames(LAB.GetListVisibleFrames());

  //MinMassesSqInvJigsaw MinMW("MinMW","min M_{W}, M_{Wa}= M_{Wb}",2);
  ContraBoostInvJigsaw MinMW("MinMW","min M_{W}, M_{Wa}= M_{Wb}");
  INV.AddJigsaw(MinMW);
  MinMW.AddVisibleFrame(La, 0);
  MinMW.AddVisibleFrame(Lb, 1);
  MinMW.AddInvisibleFrame(Na, 0);
  MinMW.AddInvisibleFrame(Nb, 1);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);

  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(INV);
  treePlot->Draw("InvTree", "Invisible Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  std::string plot_title = "H^{ 0} #rightarrow W(#it{l} #nu) W(#it{l} #nu)";
  HistPlot* histPlot = new HistPlot("HistPlot", plot_title);

  RFList<const HistPlotCategory> cat_list;
  int Nmass = mH.size();
  for(int m = 0; m < Nmass; m++){
    char smass[50], scat[50];
    sprintf(scat, "MH%.0f", mH[m]);
    sprintf(smass, "m_{H^{ 0}} = %.0f", mH[m]);
    cat_list += histPlot->GetNewCategory(scat, smass);
  }
  
  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");

  const HistPlotVar& MH     = histPlot->GetNewVar("MH", "M_{H^{ 0}}", 0., 3000., "[GeV]");
  const HistPlotVar& MHN    = histPlot->GetNewVar("MHN", "M_{H^{ 0}} / m_{H^{ 0}}^{ true}", 0., 2.);
  const HistPlotVar& MWaN   = histPlot->GetNewVar("MWaN", "M_{Wa} / m_{Wa}^{ true}", 0., 3.);
  const HistPlotVar& cosH   = histPlot->GetNewVar("cosH","cos #theta_{H^{ 0}}", -1., 1.);
  const HistPlotVar& cosWa  = histPlot->GetNewVar("cosWa","cos #theta_{W_{a}}", -1., 1.);
  const HistPlotVar& dphiH  = histPlot->GetNewVar("dphiH", "#Delta #phi_{H^{ 0}}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosH  = histPlot->GetNewVar("DcosH","#theta_{H^{ 0}} - #theta_{H^{ 0}}^{true}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWa = histPlot->GetNewVar("DcosWa","#theta_{W_{a}} - #theta_{W_{a}}^{true}", 
						  -acos(-1.)/2., acos(-1.)/2.);
 
  histPlot->AddPlot(MH,     cat_list);
  histPlot->AddPlot(MHN,    cat_list);
  histPlot->AddPlot(MWaN,   cat_list);
  histPlot->AddPlot(DcosH,  cat_list);
  histPlot->AddPlot(DcosWa, cat_list);
  histPlot->AddPlot(MHN, MWaN,    cat_list[1]);
  histPlot->AddPlot(MHN, DcosH,   cat_list[0]);
  histPlot->AddPlot(MWaN, DcosWa, cat_list[1]);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int m = 0; m < Nmass; m++){
    g_Log << LogInfo << "Generating events for H^{0} mass = " << mH[m] << LogEnd;

    H_Gen.SetMass(mH[m]);
    LAB_Gen.InitializeAnalysis();
  
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

      // generate event
      LAB_Gen.ClearEvent();                            // clear the gen tree
      
      LAB_Gen.AnalyzeEvent();                          // generate a new event

      // analyze event
      LAB.ClearEvent();                                  // clear the reco tree

      TVector3 MET = LAB_Gen.GetInvisibleMomentum();     // Get the MET from gen tree
      MET.SetZ(0.);
      INV.SetLabFrameThreeVector(MET);                   // Set the MET in reco tree

      La.SetLabFrameFourVector(La_Gen.GetFourVector());  // set lepton 4-vectors
      Lb.SetLabFrameFourVector(Lb_Gen.GetFourVector());

      LAB.AnalyzeEvent();                                // analyze the event

      // Generator-level observables
      double cosHgen  = H_Gen.GetCosDecayAngle();
      cosH = cosHgen;
      double cosWagen  = Wa_Gen.GetCosDecayAngle();
      cosWa = cosWagen;

      if(m == 0)
	histPlot->Fill(cat_Gen);

      // Reconstruction-level observables
      MH   = H.GetMass();
      MHN  = H.GetMass()/H_Gen.GetMass();
      MWaN = Wa.GetMass()/Wa_Gen.GetMass();
      cosH  = H.GetCosDecayAngle();
      cosWa = Wa.GetCosDecayAngle();
      DcosH  = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);
      DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);

      histPlot->Fill(cat_list[m]);
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
  example_H_to_WlnuWlnu();
  return 0;
}
#endif
