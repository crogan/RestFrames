/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_01_basics_Ztoll.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example analyzing a Z to ll events
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

void example_01(const string& output_name = "output_example_01.root"){

  double mZ = 90.;
  int Ngen = 100000;

  // Set up toy generation and event analysis trees:
  LabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame Z_G("Z_G","Z");
  VisibleGenFrame Lp_G("Lp_G","#it{l}^{+}");
  VisibleGenFrame Lm_G("Lm_G","#it{l}^{-}");

  LAB_G.SetChildFrame(Z_G);
  Z_G.AddChildFrame(Lp_G);
  Z_G.AddChildFrame(Lm_G);
 
  if(!LAB_G.InitializeTree()) cout << "Problem with generator tree" << endl; 

  Z_G.SetMass(mZ);

  if(!LAB_G.InitializeAnalysis()) cout << "Problem with generator tree" << endl; 

  LabRecoFrame LAB("LAB","LAB");
  DecayRecoFrame Z("Z","Z");
  VisibleRecoFrame Lp("Lp","#it{l}^{+}");
  VisibleRecoFrame Lm("Lm","#it{l}^{-}");
  
  LAB.SetChildFrame(Z);
  Z.AddChildFrame(Lp);
  Z.AddChildFrame(Lm);

  if(!LAB.InitializeTree()) cout << "Problem with reconstruction tree" << endl; 
  
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  // reco tree
  tree_plot->SetTree(LAB);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  if(!LAB.InitializeAnalysis()) cout << "Problem with jigsaws" << endl;
  
  // Declare observables for histogram booking
  HistPlot* hist_plot = new HistPlot("HistPlot","Z #rightarrow #it{l}^{+} #it{l}^{-}"); 

  const HistPlotVar& MZ     = hist_plot->GetNewVar("MZ", "M_{Z}", 85., 90., "[GeV]");
  const HistPlotVar& cosZ   = hist_plot->GetNewVar("cosZ","cos #theta_{Z}", -1., 1.);
  const HistPlotVar& dphiZ  = hist_plot->GetNewVar("dphiZ", "#Delta #phi_{Z}", 0., 2.*acos(-1.));
  
  hist_plot->AddPlot(MZ);
  hist_plot->AddPlot(cosZ);
  hist_plot->AddPlot(dphiZ);
  hist_plot->AddPlot(cosZ, MZ);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) cout << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_G.ClearEvent();                             // clear the gen tree
    double PTZ = mZ*gRandom->Rndm();
    LAB_G.SetTransverseMomenta(PTZ);                // give the Z some Pt
    double PzZ = mZ*(2.*gRandom->Rndm()-1.);
    LAB_G.SetLongitudinalMomenta(PzZ);              // give the Z some Pz
    LAB_G.AnalyzeEvent();                           // generate a new event

    // analyze event
    LAB.ClearEvent();                             // clear the reco tree
    Lp.SetLabFrameFourVector(Lp_G.GetFourVector()); // Set lepton 4-vec
    Lm.SetLabFrameFourVector(Lm_G.GetFourVector()); // Set lepton 4-vec
    LAB.AnalyzeEvent();                           // analyze the event

    // calculate observables
    MZ    = Z.GetMass();
    cosZ  = Z.GetCosDecayAngle();
    dphiZ = LAB.GetDeltaPhiDecayPlanes(Z);

    hist_plot->Fill();
  }
 
  hist_plot->Draw();
  hist_plot->WriteOutput(output_name);
  hist_plot->WriteHist(output_name);
  tree_plot->WriteOutput(output_name);

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_01();
  return 0;
}
#endif
