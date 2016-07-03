/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_X2X2_to_ZllXHggX.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Nov
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

void example_X2X2_to_ZllXHggX(std::string output_name =
			      "output_X2X2_to_ZllXHggX.root"){

  // set particle masses and widths [GeV]
  double mX1 = 400.;
  double mX2 = 500.;
  double mZ   = 91.;
  double wZ   = 2.5;
  double mH   = 125.;
  double wH   = 0.04;

  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
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
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  X2X2_Gen.SetVariableMass();
  X2a_Gen.SetMass(mX2);
  X2b_Gen.SetMass(mX2);
  X1a_Gen.SetMass(mX1);  X1b_Gen.SetMass(mX1);
  Za_Gen.SetMass(mZ);
  Za_Gen.SetWidth(wZ);
  Hb_Gen.SetMass(mH);
  Hb_Gen.SetWidth(wH);
  L1_Gen.SetPtCut(10.);
  L2_Gen.SetPtCut(10.);
  G1_Gen.SetPtCut(25.);
  G2_Gen.SetPtCut(25.);
  L1_Gen.SetEtaCut(2.5);
  L2_Gen.SetEtaCut(2.5);
  G1_Gen.SetEtaCut(3.0);
  G2_Gen.SetEtaCut(3.0);
  
  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and tree..." << LogEnd;
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
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

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
  //MinMassDiffInvJigsaw X1X1_contra("MinMh_R","min M_{h}, M_{h}^{ a}= M_{h}^{ b}",2);
  //MinMassesSqInvJigsaw X1X1_contra("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{a,b}", 2);
  INV.AddJigsaw(X1X1_contra);
  X1X1_contra.AddVisibleFrames(X2a.GetListVisibleFrames(), 0);
  X1X1_contra.AddVisibleFrames(X2b.GetListVisibleFrames(), 1);
  X1X1_contra.AddInvisibleFrame(X1a, 0);
  X1X1_contra.AddInvisibleFrame(X1b, 1);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree");

  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  // Declare observables for histogram booking
  HistPlot* histPlot = new HistPlot("Plots", 
				    std::string("#tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}") +
				    "#rightarrow Z(#it{l}#it{l}) #tilde{#chi}_{1}^{ 0}"+
				    "H(#gamma #gamma) #tilde{#chi}_{1}^{ 0}"); 

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_Reco  = histPlot->GetNewCategory("Reco", "Reconstruction");
  
  const HistPlotVar& MCM  = histPlot->GetNewVar("MCM", 
						"M_{#tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}}", 
						0., 2.5);
  const HistPlotVar& MX2a   = histPlot->GetNewVar("MX2a", "M_{#tilde{#chi}_{2 a}^{ 0}}", 0., 2.5);
  const HistPlotVar& MX2b   = histPlot->GetNewVar("MX2b", "M_{#tilde{#chi}_{2 b}^{ 0}}", 0., 2.5);
  const HistPlotVar& EZX2a  = histPlot->GetNewVar("EZX2a", "E_{Z}^{ #tilde{#chi}_{2 a}^{ 0}}", 0., 2.5);
  const HistPlotVar& EHX2b  = histPlot->GetNewVar("EHX2b", "E_{H}^{ #tilde{#chi}_{2 b}^{ 0}}", 0., 2.5);
  const HistPlotVar& cosX2a = histPlot->GetNewVar("cosX2a","cos #theta_{#tilde{#chi}_{2 a}^{ 0}}", -1., 1.);
  const HistPlotVar& cosX2b = histPlot->GetNewVar("cosX2b","cos #theta_{#tilde{#chi}_{2 b}^{ 0}}", -1., 1.);
  const HistPlotVar& cosZ   = histPlot->GetNewVar("cosZ","cos #theta_{Z}", -1., 1.);
  const HistPlotVar& cosH   = histPlot->GetNewVar("cosH","cos #theta_{H}", -1., 1.);
  const HistPlotVar& DcosZ  = histPlot->GetNewVar("DcosZ","#theta_{Z} - #theta_{Z}^{gen}", -1., 1.);
  const HistPlotVar& DcosH  = histPlot->GetNewVar("DcosH","#theta_{H} - #theta_{H}^{gen}", -1., 1.);
  const HistPlotVar& DcosX2a  = histPlot->GetNewVar("DcosX2a","#theta_{X2a} - #theta_{X2a}^{gen}", -1., 1.);
  const HistPlotVar& DcosX2b  = histPlot->GetNewVar("DcosX2b","#theta_{X2b} - #theta_{X2b}^{gen}", -1., 1.);

  const HistPlotVar& RISR   = histPlot->GetNewVar("RISR","R_{ISR}", 0., 1.5);

  histPlot->AddPlot(MCM, cat_Reco);      
  histPlot->AddPlot(EZX2a, cat_Reco);
  histPlot->AddPlot(EHX2b, cat_Reco);   
  histPlot->AddPlot(cosZ, cat_Reco);  
  histPlot->AddPlot(cosH, cat_Reco);  
  histPlot->AddPlot(DcosZ, cat_Reco); 
  histPlot->AddPlot(DcosH, cat_Reco);
  histPlot->AddPlot(DcosX2a, cat_Reco);
  histPlot->AddPlot(DcosX2b, cat_Reco); 
  histPlot->AddPlot(MCM, EZX2a, cat_Reco);
  histPlot->AddPlot(MCM, EHX2b, cat_Reco);
  histPlot->AddPlot(EZX2a, EHX2b, cat_Reco);
  histPlot->AddPlot(EZX2a, DcosX2a, cat_Reco);
  histPlot->AddPlot(EHX2b, DcosX2b, cat_Reco);
  histPlot->AddPlot(cosZ, DcosZ, cat_Reco);
  histPlot->AddPlot(cosH, DcosH, cat_Reco);

  histPlot->AddPlot(RISR, cat_Reco);
  histPlot->AddPlot(RISR, EZX2a, cat_Reco);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                           // clear the gen tree
    
    double PTCM = 0.5*mX2*gRandom->Rndm();
    LAB_Gen.SetTransverseMomentum(800.);             // give X2X2 some Pt
    
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
    MCM = X2X2.GetMass() / X2X2_Gen.GetMass();
    MX2a  = X2a.GetMass() / X2a_Gen.GetMass();
    MX2b  = X2b.GetMass() / X2b_Gen.GetMass();

    EZX2a = Za.GetEnergy(X2a) / Za_Gen.GetEnergy(X2a_Gen);
    EHX2b = Hb.GetEnergy(X2b) / Hb_Gen.GetEnergy(X2b_Gen);

    cosX2a  = X2a.GetCosDecayAngle();
    double cosX2agen  = X2a_Gen.GetCosDecayAngle();
    cosX2b  = X2b.GetCosDecayAngle();
    double cosX2bgen  = X2b_Gen.GetCosDecayAngle();
    cosZ  = Za.GetCosDecayAngle();
    double cosZgen  = Za_Gen.GetCosDecayAngle();
    cosH  = Hb.GetCosDecayAngle();
    double cosHgen  = Hb_Gen.GetCosDecayAngle();

    DcosX2a = asin(sqrt(1.-cosX2a*cosX2a)*cosX2agen-sqrt(1.-cosX2agen*cosX2agen)*cosX2a);
    DcosX2b = asin(sqrt(1.-cosX2b*cosX2b)*cosX2bgen-sqrt(1.-cosX2bgen*cosX2bgen)*cosX2b);
    DcosZ   = asin(sqrt(1.-cosZ*cosZ)*cosZgen-sqrt(1.-cosZgen*cosZgen)*cosZ);
    DcosH   = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);

    TVector3 vP_ISR = X2X2.GetFourVector(LAB).Vect();
    TVector3 vP_I   = X2X2.GetListInvisibleFrames().GetFourVector(LAB).Vect();
    vP_ISR.SetZ(0.);
    vP_I.SetZ(0.);
    
    RISR = fabs(vP_I.Dot(vP_ISR.Unit())) / vP_ISR.Mag();
    
    histPlot->Fill(cat_Reco);
  }
  
  histPlot->Draw();

  LAB_Gen.PrintGeneratorEfficiency();
  
  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_X2X2_to_ZllXHggX();
  return 0;
}
#endif
