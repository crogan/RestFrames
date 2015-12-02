/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_07.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Nov
///
///  \brief An example generating and analyzing di-neutralinos decaying 
///         through Z and H to LSP's
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

#include <TF1.h>
#include "RestFrames/RestFrames.hh"

using namespace RestFrames;

void example_07(string output_name = "output_example_07.root"){

  // set particle masses and widths [GeV]
  double mX1 = 100.;
  double mX2 = 500.;
  double wtop = 2.5;
  double mZ   = 91.;
  double wZ   = 2.5;
  double mH   = 125.;
  double wH   = 0.04;

  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame       LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     X2X2_Gen("X2X2_Gen","#tilde{#chi}^{ 0}_{2} #tilde{#chi}^{ 0}_{2}");
  DecayGenFrame     X2a_Gen("X2a_Gen","#tilde{#chi}^{ 0}_{2 a}");
  DecayGenFrame     X2b_Gen("X2b_Gen","#tilde{#chi}^{ 0}_{2 b}");
  ResonanceGenFrame Za_Gen("Za_Gen","Z_{a}");
  ResonanceGenFrame Hb_Gen("Hb_Gen","H_{b}");
  VisibleGenFrame   L1_Gen("L1_Gen","#it{l}_{1}");
  VisibleGenFrame   L2_Gen("L2_Gen","#it{l}_{2}");
  VisibleGenFrame   G1_Gen("G1_Gen","#gamma_{1}");
  VisibleGenFrame   G2_Gen("G2_Gen","#gamma_{2}");
  InvisibleGenFrame X1a_Gen("X1a_Gen","#tilde{#chi}^{ 0}_{1 a}");
  InvisibleGenFrame X1b_Gen("X1b_Gen","#tilde{#chi}^{ 0}_{1 b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(X2X2_Gen);
  X2X2_Gen.AddChildFrame(X2a_Gen);
  X2X2_Gen.AddChildFrame(X2b_Gen);
  X2a_Gen.AddChildFrame(Za_Gen);
  X2a_Gen.AddChildFrame(X1a_Gen);
  X2b_Gen.AddChildFrame(Hb_Gen);
  X2b_Gen.AddChildFrame(X1b_Gen);
  Za_Gen.AddChildFrame(L1_Gen);
  Za_Gen.AddChildFrame(L2_Gen);
  Hb_Gen.AddChildFrame(G1_Gen);
  Hb_Gen.AddChildFrame(G2_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << g_End;
  else
    g_Log << LogError << "...Failed initializing generator tree" << g_End;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  X2a_Gen.SetMass(mX2);
  X2b_Gen.SetMass(mX2);
  X1a_Gen.SetMass(mX1);  X1b_Gen.SetMass(mX1);
  Za_Gen.SetMass(mZ);
  Za_Gen.SetWidth(wZ);
  Hb_Gen.SetMass(mH);
  Hb_Gen.SetWidth(wH);
  
  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and tree..." << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB("LAB","LAB");
  DecayRecoFrame     X2X2("X2X2","#tilde{#chi}^{ 0}_{2} #tilde{#chi}^{ 0}_{2}");
  DecayRecoFrame     X2a("X2a","#tilde{#chi}^{ 0}_{2 a}");
  DecayRecoFrame     X2b("X2b","#tilde{#chi}^{ 0}_{2 b}");
  DecayRecoFrame     Za("Za","Z_{a}");
  DecayRecoFrame     Hb("Hb","H_{b}");
  VisibleRecoFrame   L1("L1","#it{l}_{1}");
  VisibleRecoFrame   L2("L2","#it{l}_{2}");
  VisibleRecoFrame   G1("G1","#gamma_{1}");
  VisibleRecoFrame   G2("G2","#gamma_{2}");
  InvisibleRecoFrame X1a("X1a","#tilde{#chi}^{ 0}_{1 a}");
  InvisibleRecoFrame X1b("X1b","#tilde{#chi}^{ 0}_{1 b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(X2X2);
  X2X2.AddChildFrame(X2a);
  X2X2.AddChildFrame(X2b);
  X2a.AddChildFrame(Za);
  X2a.AddChildFrame(X1a);
  X2b.AddChildFrame(Hb);
  X2b.AddChildFrame(X1b);
  Za.AddChildFrame(L1);
  Za.AddChildFrame(L2);
  Hb.AddChildFrame(G1);
  Hb.AddChildFrame(G2);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << g_End;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << g_End;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Groups
  InvisibleGroup INV("INV","#tilde{#chi}_{1}^{ 0} Jigsaws");
  INV.AddFrames(X1a+X1b);     
 
  // Set di-LSP mass to minimum Lorentz-invariant expression
  SetMassInvJigsaw X1_mass("X1_mass","Set M_{#tilde{#chi}_{1}^{ 0} #tilde{#chi}_{1}^{ 0}} to minimum"); 
  INV.AddJigsaw(X1_mass);         

  // Set di-LSP rapidity to that of visible particles
  SetRapidityInvJigsaw X1_eta("X1_eta","#eta_{#tilde{#chi}_{1}^{ 0} #tilde{#chi}_{1}^{ 0}} = #eta_{2#gamma+2#it{l}}");
  INV.AddJigsaw(X1_eta);
  X1_eta.AddVisibleFrames(X2X2.GetListVisibleFrames());

  ContraBoostInvJigsaw X1X1_contra("X1X1_contra","Contraboost invariant Jigsaw");
  INV.AddJigsaw(X1X1_contra);
  X1X1_contra.AddVisibleFrames(X2a.GetListVisibleFrames(), 0);
  X1X1_contra.AddVisibleFrames(X2b.GetListVisibleFrames(), 1);
  X1X1_contra.AddInvisibleFrame(X1a, 0);
  X1X1_contra.AddInvisibleFrame(X1b, 1);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing analysis" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetFrameTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree");

  treePlot->SetFrameTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  // Declare observables for histogram booking
  HistPlot* histPlot = new HistPlot("Plots", 
				    string("#tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}") +
				    "#rightarrow Z(#it{l}#it{l}) #tilde{#chi}_{1}^{ 0}"+
				    "H(#gamma #gamma) #tilde{#chi}_{1}^{ 0}"); 

  const HistPlotVar& HX2X2  = histPlot->GetNewVar("HX2X2", 
						  "H_{4, 2}^{ #tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}}", 
						  0., 2000., "[GeV]");
  const HistPlotVar& HX2a   = histPlot->GetNewVar("HX2a", "H_{2, 1}^{ #tilde{#chi}_{2 a}^{ 0}}", 0., 800., "[GeV]");
  const HistPlotVar& HX2b   = histPlot->GetNewVar("HX2b", "H_{2, 1}^{ #tilde{#chi}_{2 b}^{ 0}}", 0., 800., "[GeV]");
  const HistPlotVar& pTX2X2 = histPlot->GetNewVar("pTX2X2",
						  string("p_{T #tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}}^{ lab}") +
						  " / m_{#tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}}", 0., 1.);
  const HistPlotVar& cosZ   = histPlot->GetNewVar("cosZ","cos #theta_{Z}", -1., 1.);
  const HistPlotVar& cosH   = histPlot->GetNewVar("cosH","cos #theta_{H}", -1., 1.);
  const HistPlotVar& dphiZ  = histPlot->GetNewVar("dphiZ", "#Delta #phi_{Z}", 0., 2.*acos(-1.));
  const HistPlotVar& dphiH  = histPlot->GetNewVar("dphiH", "#Delta #phi_{H}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosZ  = histPlot->GetNewVar("DcosZ","#theta_{Z} - #theta_{t}^{gen}", -1., 1.);
  const HistPlotVar& DcosH  = histPlot->GetNewVar("DcosH","#theta_{H} - #theta_{W}^{gen}", -1., 1.);
  const HistPlotVar& DdphiZ = histPlot->GetNewVar("DdphiZ","#Delta #phi_{Z} - #Delta #phi_{Z}^{gen}", -1., 1.);
  const HistPlotVar& DdphiH = histPlot->GetNewVar("DdphiH","#Delta #phi_{H} - #Delta #phi_{H}^{gen}", -1., 1.);

  histPlot->AddHist(HX2X2);      
  histPlot->AddHist(HX2a);   
  histPlot->AddHist(HX2b);   
  histPlot->AddHist(pTX2X2); 
  histPlot->AddHist(cosZ);  
  histPlot->AddHist(cosH);  
  histPlot->AddHist(dphiZ); 
  histPlot->AddHist(dphiH); 
  histPlot->AddHist(DcosZ); 
  histPlot->AddHist(DcosH); 
  histPlot->AddHist(DdphiZ); 
  histPlot->AddHist(DdphiH); 
  histPlot->AddHist(HX2X2, HX2a);
  histPlot->AddHist(HX2X2, HX2b);
  histPlot->AddHist(HX2a, HX2b);
  histPlot->AddHist(HX2a, cosZ);
  histPlot->AddHist(HX2b, cosH);
  histPlot->AddHist(cosZ, DcosZ);
  histPlot->AddHist(cosH, DcosH);

  // function for randomly determining di-X2 mass 
  // (relative to X2 mass via gamma) - to be replaced
  TF1 f_gamma("f_gamma","(x-1)*exp(-2.*x)",1.,10.);
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((max(Ngen,10))/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_Gen.ClearEvent();                           // clear the gen tree
    double mX2X2 = 2.*mX2*f_gamma.GetRandom();      // get a random di-X2 mass
    X2X2_Gen.SetMass(mX2X2);
    double PTCM = mX2X2*gRandom->Rndm();
    LAB_Gen.SetTransverseMomenta(PTCM);             // give X2X2 some Pt
    double PZCM = mX2X2*(2.*gRandom->Rndm()-1.);
    LAB_Gen.SetLongitudinalMomenta(PZCM);           // give X2X2 some Pz
    LAB_Gen.AnalyzeEvent();                         // generate a new event

    // analyze event
    LAB.ClearEvent();                                 // clear the reco tree
    L1.SetLabFrameFourVector(L1_Gen.GetFourVector()); // Set lepton 4-vectors
    L2.SetLabFrameFourVector(L2_Gen.GetFourVector()); 
    G1.SetLabFrameFourVector(G1_Gen.GetFourVector()); // Set photon 4-vectors
    G2.SetLabFrameFourVector(G2_Gen.GetFourVector()); 
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    INV.SetLabFrameThreeVector(MET);                  // Set the MET in reco tree
    LAB.AnalyzeEvent();                               //analyze the event

    // calculate observables
    HX2X2 = X1a.GetMomentum(X2X2) + X1b.GetMomentum(X2X2) +
            L1.GetMomentum(X2X2)  + L2.GetMomentum(X2X2)  +
            G1.GetMomentum(X2X2)  + G2.GetMomentum(X2X2);
    HX2a  = X1a.GetMomentum(X2a) + L1.GetMomentum(X2a) + L2.GetMomentum(X2a);
    HX2b  = X1b.GetMomentum(X2b) + G1.GetMomentum(X2b) + G2.GetMomentum(X2b);
   
    cosZ  = Za.GetCosDecayAngle();
    double cosZgen  = Za_Gen.GetCosDecayAngle();
    dphiZ = X2a.GetDeltaPhiDecayPlanes(Za);
    double dphiZgen = X2a_Gen.GetDeltaPhiDecayPlanes(Za_Gen);

    cosH  = Hb.GetCosDecayAngle();
    double cosHgen  = Hb_Gen.GetCosDecayAngle();
    dphiH = X2b.GetDeltaPhiDecayPlanes(Hb);
    double dphiHgen = X2b_Gen.GetDeltaPhiDecayPlanes(Hb_Gen);
    
    DcosZ  = asin(sqrt(1.-cosZ*cosZ)*cosZgen-sqrt(1.-cosZgen*cosZgen)*cosZ);
    DdphiZ = asin(sin(dphiZ-dphiZgen));
    DcosH  = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);
    DdphiH = asin(sin(dphiH-dphiHgen));

    pTX2X2 = PTCM / mX2X2;

    histPlot->Fill();
  }
  
  histPlot->Draw();
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_07();
  return 0;
}
#endif
