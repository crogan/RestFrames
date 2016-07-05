/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_Wlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
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

void example_Wlnu(const std::string& output_name = "output_Wlnu.root"){

  double mW = 80.385; // GeV, PDG 2016
  double wW = 2.085;
  
  // Number of events to generate
  int Ngen = 100000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame       LAB_Gen("LAB_Gen","LAB");
  ResonanceGenFrame W_Gen("W_Gen","W");
  VisibleGenFrame   L_Gen("L_Gen","#it{l}");
  InvisibleGenFrame NU_Gen("NU_Gen","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB_Gen.SetChildFrame(W_Gen);
  W_Gen.AddChildFrame(L_Gen);
  W_Gen.AddChildFrame(NU_Gen);
 
  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
                
  // set W pole mass and width
  W_Gen.SetMass(mW);                   W_Gen.SetWidth(wW);

  // set lepton pT and eta cuts
  L_Gen.SetPtCut(20.);                 L_Gen.SetEtaCut(2.5);  

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
  DecayRecoFrame     W("W","W");
  VisibleRecoFrame   L("L","#it{l}");
  InvisibleRecoFrame NU("NU","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB.SetChildFrame(W);
  W.AddChildFrame(L);
  W.AddChildFrame(NU);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Now we add invisible jigsaws
  InvisibleGroup INV("INV","Neutrino Jigsaws");
  INV.AddFrame(NU);

  // Set the neutrino mass
  SetMassInvJigsaw MassJigsaw("MassJigsaw","m_{#nu} = 0");
  INV.AddJigsaw(MassJigsaw);

  // Set the neutrino rapidity
  SetRapidityInvJigsaw RapidityJigsaw("RapidityJigsaw","#eta_{#nu} = #eta_{#it{l}}");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame(L);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_Gen);
  tree_plot->Draw("GenTree", "Generator Tree", true);

  // reconstruction tree
  tree_plot->SetTree(LAB);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  // Invisible Jigsaws tree 
  tree_plot->SetTree(INV);
  tree_plot->Draw("InvTree", "InvisibleJigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  // Declare observables for histogram booking
  HistPlot* hist_plot = new HistPlot("HistPlot","W #rightarrow #it{l} #nu");

  const HistPlotVar& MW     = hist_plot->GetNewVar("MW", "M_{W}", 30., 120., "[GeV]");
  const HistPlotVar& cosW   = hist_plot->GetNewVar("cosW","cos #phi_{W}", -1., 1.);
  const HistPlotVar& dphiW  = hist_plot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosW  = hist_plot->GetNewVar("DcosW","#phi_{W} - #phi_{W}^{true}", -0.5, 0.5);
  const HistPlotVar& DdphiW = hist_plot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{true}", -0.5, 0.5);
  const HistPlotVar& pTWoMW = hist_plot->GetNewVar("pTW","p_{T}^{W} / m_{W}",0.,1.);

  const HistPlotCategory& cat_Gen  = hist_plot->GetNewCategory("Reco", "Generator");
  const HistPlotCategory& cat_Reco = hist_plot->GetNewCategory("Reco", "Reconstruction");
  
  hist_plot->AddPlot(DcosW,  cat_Reco);
  hist_plot->AddPlot(DdphiW, cat_Reco);
  hist_plot->AddPlot(MW,     cat_Gen+cat_Reco);
  hist_plot->AddPlot(DcosW,  MW, cat_Reco);
  hist_plot->AddPlot(DdphiW, MW, cat_Reco);
  hist_plot->AddPlot(MW,     pTWoMW, cat_Reco);
  hist_plot->AddPlot(DcosW,  pTWoMW, cat_Reco);
  hist_plot->AddPlot(DdphiW, pTWoMW, cat_Reco);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                                // clear the gen tree

    pTWoMW = gRandom->Rndm();
    LAB_Gen.SetPToverM(pTWoMW);                          // give the W some Pt
    double PzW = mW*(2.*gRandom->Rndm()-1.);
    LAB_Gen.SetLongitudinalMomentum(PzW);                // give the W some Pz
     
    LAB_Gen.AnalyzeEvent();                              // generate a new event

    // analyze event
    LAB.ClearEvent();                               // clear the reco tree
      
    L.SetLabFrameFourVector(L_Gen.GetFourVector()); // Set lepton 4-vec
      
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();       // Get the MET from gen tree
    MET.SetZ(0.);
    INV.SetLabFrameThreeVector(MET);                     // Set the MET in reco tree
      
    LAB.AnalyzeEvent();                             // analyze the event

    // generator-level observables
    MW    = W_Gen.GetMass();
    cosW  = cos(W_Gen.GetDeltaPhiDecayAngle());
    dphiW = LAB_Gen.GetDeltaPhiDecayPlanes(W_Gen);

    hist_plot->Fill(cat_Gen);

    // calculate observables
    MW    = W.GetMass();
    cosW  = cos(W.GetDeltaPhiDecayAngle());
    dphiW = LAB.GetDeltaPhiDecayPlanes(W);
      
    double cosWgen  = cos(W_Gen.GetDeltaPhiDecayAngle());
    double dphiWgen = LAB_Gen.GetDeltaPhiDecayPlanes(W_Gen);
    DcosW  = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    DdphiW = asin(sin(dphiW-dphiWgen));

    hist_plot->Fill(cat_Reco);
  }
 
  hist_plot->Draw();

  TFile fout(output_name.c_str(),"RECREATE");
  fout.Close();
  hist_plot->WriteOutput(output_name);
  hist_plot->WriteHist(output_name);
  tree_plot->WriteOutput(output_name);

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_Wlnu();
  return 0;
}
#endif
