/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_02_InvisibleJigsaws_Wtolnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example generating and analyzing W(l nu) events
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

void example_02_InvisibleJigsaws_Wtolnu(string output_name = "output_02.root"){

  double mW = 81.;
  int Ngen = 100000;

  // Set up toy generation and event analysis trees:
  LabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame W_G("W_G","W");
  VisibleGenFrame L_G("L_G","#it{l}");
  InvisibleGenFrame NU_G("NU_G","#nu");

  LAB_G.SetChildFrame(W_G);
  W_G.AddChildFrame(L_G);
  W_G.AddChildFrame(NU_G);
 
  if(!LAB_G.InitializeTree()) cout << "Problem with generator tree" << endl; 

  W_G.SetMass(mW);

  if(!LAB_G.InitializeAnalysis()) cout << "Problem with generator tree" << endl; 

  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame W_R("W_R","W");
  VisibleRecoFrame L_R("L_R","#it{l}");
  InvisibleRecoFrame NU_R("NU_R","#nu");
  
  LAB_R.SetChildFrame(W_R);
  W_R.AddChildFrame(L_R);
  W_R.AddChildFrame(NU_R);

  if(!LAB_R.InitializeTree()) cout << "Problem with reconstruction tree" << endl; 
  
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetFrameTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  // reco tree
  tree_plot->SetFrameTree(LAB_R);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");


  // Now we add invisible jigsaws
  InvisibleGroup INV("INV","Neutrino Jigsaws");
  INV.AddFrame(NU_R);
 
  SetMassInvJigsaw MassJigsaw("MASS_JIGSAW","mass Jigsaw");
  INV.AddJigsaw(MassJigsaw);
  
  SetRapidityInvJigsaw RapidityJigsaw("RAPIDITY_JIGSAW","rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame(L_R);

  if(!LAB_R.InitializeAnalysis()) cout << "Problem with jigsaws" << endl;

  // Invisible Jigsaws
  tree_plot->SetGroupTree(INV);
  tree_plot->Draw("InvTree", "InvisibleJigsaws");
  
  // Declare observables for histogram booking
  HistPlot* hist_plot = new HistPlot("HistPlot","W #rightarrow #it{l} #nu"); 

  const HistPlotVar& MW     = hist_plot->GetNewVar("MW", "M_{W}", 0., 90., "[GeV]");
  const HistPlotVar& pTW    = hist_plot->GetNewVar("pTW","p_{T}^{W} / m_{W}",0.,1.);
  const HistPlotVar& cosW   = hist_plot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  const HistPlotVar& dphiW  = hist_plot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosW  = hist_plot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{true}", -1., 1.);
  const HistPlotVar& DdphiW = hist_plot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{true}", -1., 1.);
  
  hist_plot->AddPlot(MW);
  // hist_plot->AddPlot(cosW);
  // hist_plot->AddPlot(dphiW);
  hist_plot->AddPlot(DcosW);
  hist_plot->AddPlot(DdphiW);
  hist_plot->AddPlot(MW, cosW);
  hist_plot->AddPlot(MW, dphiW);
  hist_plot->AddPlot(MW, pTW);
  hist_plot->AddPlot(cosW, DcosW);
  hist_plot->AddPlot(dphiW, DdphiW);
  hist_plot->AddPlot(cosW, pTW);
  hist_plot->AddPlot(dphiW, pTW);
  hist_plot->AddPlot(DcosW, pTW);
  hist_plot->AddPlot(DdphiW, pTW);

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
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    INV.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    LAB_R.AnalyzeEvent();                           // analyze the event

    // calculate observables
    MW    = W_R.GetMass();
    pTW   = PTW/mW;
    cosW  = W_R.GetCosDecayAngle();
    dphiW = LAB_R.GetDeltaPhiDecayPlanes(W_R);
    double cosWgen  = W_G.GetCosDecayAngle();
    double dphiWgen = LAB_G.GetDeltaPhiDecayPlanes(W_G);
    DcosW = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    DdphiW = asin(sin(dphiW-dphiWgen));

    hist_plot->Fill();
  }
 
  hist_plot->Draw();
  hist_plot->WriteOutput(output_name);
  hist_plot->WriteHist(output_name);
  tree_plot->WriteOutput(output_name);

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_02_InvisibleJigsaws_Wtolnu();
  return 0;
}
#endif
