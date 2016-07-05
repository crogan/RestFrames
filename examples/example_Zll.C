/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_01_Zll.C
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

using namespace std;
using namespace RestFrames;

void example_Zll(const string& output_name = "output_Zll.root"){

  double mZ = 91.188; // GeV, PDG 2016
  double wZ = 2.495;

  // Number of events to generate
  int Ngen = 1000000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame       LAB_Gen("LAB_Gen","LAB");
  ResonanceGenFrame Z_Gen("Z_Gen","Z");
  VisibleGenFrame   Lp_Gen("Lp_Gen","#it{l}^{+}");
  VisibleGenFrame   Lm_Gen("Lm_Gen","#it{l}^{-}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB_Gen.SetChildFrame(Z_Gen);
  Z_Gen.AddChildFrame(Lp_Gen);
  Z_Gen.AddChildFrame(Lm_Gen);
 
  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Set Z pole mass and width
  Z_Gen.SetMass(mZ);                   Z_Gen.SetWidth(wZ);

  // set lepton pT and eta cuts
  Lp_Gen.SetPtCut(15.);                 Lp_Gen.SetEtaCut(2.5);
  Lm_Gen.SetPtCut(15.);                 Lm_Gen.SetEtaCut(2.5); 

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame     LAB("LAB","LAB");
  DecayRecoFrame   Z("Z","Z");
  VisibleRecoFrame Lp("Lp","#it{l}^{+}");
  VisibleRecoFrame Lm("Lm","#it{l}^{-}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB.SetChildFrame(Z);
  Z.AddChildFrame(Lp);
  Z.AddChildFrame(Lm);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;
  
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

  // reco tree
  tree_plot->SetTree(LAB);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  // Declare observables for histogram booking
  HistPlot* hist_plot = new HistPlot("HistPlot","Z #rightarrow #it{l}^{+} #it{l}^{-}"); 

  const HistPlotVar& MZ     = hist_plot->GetNewVar("MZ", "M_{Z}", 70., 110., "[GeV]");
  const HistPlotVar& cosZ   = hist_plot->GetNewVar("cosZ","cos #theta_{Z}", -1., 1.);
  const HistPlotVar& dphiZ  = hist_plot->GetNewVar("dphiZ", "#Delta #phi_{Z}", 0., 2.*acos(-1.));
  
  hist_plot->AddPlot(MZ);
  hist_plot->AddPlot(cosZ);
  hist_plot->AddPlot(dphiZ);
  hist_plot->AddPlot(cosZ, MZ);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                             // clear the gen tree
    
    double PTZ = mZ*gRandom->Rndm();
    LAB_Gen.SetTransverseMomentum(PTZ);               // give the Z some Pt
    double PzZ = mZ*(2.*gRandom->Rndm()-1.);
    LAB_Gen.SetLongitudinalMomentum(PzZ);             // give the Z some Pz
    
    LAB_Gen.AnalyzeEvent();                           // generate a new event

    // analyze event
    LAB.ClearEvent();                                 // clear the reco tree
    
    Lp.SetLabFrameFourVector(Lp_Gen.GetFourVector(), 1); // Set lepton 4-vec and charge
    Lm.SetLabFrameFourVector(Lm_Gen.GetFourVector(),-1); // Set lepton 4-vec and charge
    
    LAB.AnalyzeEvent();                               // analyze the event

    // calculate observables
    MZ    = Z.GetMass();
    cosZ  = Z.GetCosDecayAngle();
    dphiZ = LAB.GetDeltaPhiDecayPlanes(Z);

    hist_plot->Fill();
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
  example_Zll();
  return 0;
}
#endif
