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

  double mH = 500.;
  double wH = 0.004;
  double mW = 80.;
  double wW = 2.;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 100000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame    LAB_G("LAB_G","LAB");
  ResonanceGenFrame H_G("H_G","H");
  //DecayGenFrame     H_G("H_G","H");
  ResonanceGenFrame Wa_G("Wa_G","W_{a}");
  ResonanceGenFrame Wb_G("Wb_G","W_{b}");
  VisibleGenFrame   La_G("La_G","#it{l}_{a}");
  InvisibleGenFrame Na_G("Na_G","#nu_{a}");
  VisibleGenFrame   Lb_G("Lb_G","#it{l}_{b}");
  InvisibleGenFrame Nb_G("Nb_G","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_G.SetChildFrame(H_G);
  H_G.AddChildFrame(Wa_G);
  H_G.AddChildFrame(Wb_G);
  Wa_G.AddChildFrame(La_G);
  Wa_G.AddChildFrame(Na_G);
  Wb_G.AddChildFrame(Lb_G);
  Wb_G.AddChildFrame(Nb_G);

  if(LAB_G.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;								    
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

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

  if(LAB_G.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame         LAB_R("LAB_R","LAB");
  DecayRecoFrame       H_R("H_R","H");
  DecayRecoFrame       Wa_R("Wa_R","W_{a}");
  DecayRecoFrame       Wb_R("Wb_R","W_{b}");
  VisibleRecoFrame     La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame   Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame     Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame   Nb_R("Nb_R","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_R.SetChildFrame(H_R);
  H_R.AddChildFrame(Wa_R);
  H_R.AddChildFrame(Wb_R);
  Wa_R.AddChildFrame(La_R);
  Wa_R.AddChildFrame(Na_R);
  Wb_R.AddChildFrame(Lb_R);
  Wb_R.AddChildFrame(Nb_R);

  if(LAB_R.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV_R("INV_R","#nu#nu Jigsaws");
  INV_R.AddFrame(Na_R);
  INV_R.AddFrame(Nb_R);

  // Set nu nu mass equal to l l mass
  SetMassInvJigsaw NuNuM_R("NuNuM_R", "M_{#nu#nu} = M_{#it{l}#it{l}}");
  INV_R.AddJigsaw(NuNuM_R);

  SetRapidityInvJigsaw NuNuR_R("NuNuR_R", "#eta_{#nu#nu} = #eta_{#it{l}#it{l}}");
  INV_R.AddJigsaw(NuNuR_R);
  NuNuR_R.AddVisibleFrames(LAB_R.GetListVisibleFrames());

  ContraBoostInvJigsaw MinMW_R("MinMW_R","min M_{W}, M_{Wa}=M_{Wb}");
  INV_R.AddJigsaw(MinMW_R);
  MinMW_R.AddVisibleFrames(Wa_R.GetListVisibleFrames(), 0);
  MinMW_R.AddVisibleFrames(Wb_R.GetListVisibleFrames(), 1);
  MinMW_R.AddInvisibleFrames(Wa_R.GetListInvisibleFrames(), 0);
  MinMW_R.AddInvisibleFrames(Wb_R.GetListInvisibleFrames(), 1);

  if(LAB_R.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_G);
  treePlot->Draw("GenTree", "Generator Tree", true);

  treePlot->SetTree(LAB_R);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(INV_R);
  treePlot->Draw("InvTree", "Invisible Jigsaws");

  treePlot->SetTree(NuNuR_R);
  treePlot->Draw("InvTree", "Invisible Jigsaws",true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot   = new HistPlot("HistPlot", "H #rightarrow W(#it{l} #nu) W(#it{l} #nu)");

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_Reco  = histPlot->GetNewCategory("Reco", "Reconstruction");

  const HistPlotVar& MH     = histPlot->GetNewVar("MH", "M_{H} / m_{H}", 0., 2.);
  const HistPlotVar& MWa    = histPlot->GetNewVar("MWa", "M_{Wa}", 0., mW*2., "[GeV]");
  const HistPlotVar& MWb    = histPlot->GetNewVar("MWb", "M_{Wb}", 0., mW*2., "[GeV]");
  const HistPlotVar& cosH   = histPlot->GetNewVar("cosH","cos #theta_{H}", -1., 1.);
  const HistPlotVar& dphiH  = histPlot->GetNewVar("dphiH", "#Delta #phi_{H}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosH  = histPlot->GetNewVar("DcosH","#theta_{H} - #theta_{H}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DdphiH = histPlot->GetNewVar("DdphiH","#Delta #phi_{H} - #Delta #phi_{H}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);

  histPlot->AddPlot(MWa, MWb, cat_Gen);
  histPlot->AddPlot(MH, cat_Reco);
  histPlot->AddPlot(MH, MWa, cat_Reco);
  histPlot->AddPlot(cosH,  cat_Gen+cat_Reco);
  histPlot->AddPlot(dphiH, cat_Gen+cat_Reco);
  histPlot->AddPlot(DcosH,  cat_Reco);
  histPlot->AddPlot(DdphiH, cat_Reco);
  histPlot->AddPlot(MH, DcosH,  cat_Reco);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTH = mH*gRandom->Rndm()*0.1;
    LAB_G.SetTransverseMomenta(PTH);               // give the Higgs some Pt
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    LAB_R.ClearEvent();                               // clear the reco tree
    La_R.SetLabFrameFourVector(La_G.GetFourVector());
    Lb_R.SetLabFrameFourVector(Lb_G.GetFourVector());
    TVector3 MET = LAB_G.GetInvisibleMomentum();      // Get the MET from gen tree
    MET.SetZ(0.);
    INV_R.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    La_R.SetLabFrameFourVector(La_G.GetFourVector());
    Lb_R.SetLabFrameFourVector(Lb_G.GetFourVector());
    LAB_R.AnalyzeEvent();                             // analyze the event

    // Generator-level observables
    double MHgen    = H_G.GetMass();
    double cosHgen  = H_G.GetCosDecayAngle();
    cosH = cosHgen;
    double dphiHgen = LAB_G.GetDeltaPhiDecayPlanes(H_G);
    dphiH = dphiHgen;
    MWa = Wa_G.GetMass();
    MWb = Wb_G.GetMass();
   
    histPlot->Fill(cat_Gen);

    // Reconstruction-level observables
    MH = H_R.GetMass()/MHgen;
    cosH  = H_R.GetCosDecayAngle();
    dphiH = LAB_R.GetDeltaPhiDecayPlanes(H_R);
    MWa = Wa_R.GetMass();
    MWb = Wa_R.GetMass();
    DcosH = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);
    DdphiH = asin(sin(dphiH-dphiHgen));

    histPlot->Fill(cat_Reco);
  }

  histPlot->Draw();

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_04();
  return 0;
}
#endif
