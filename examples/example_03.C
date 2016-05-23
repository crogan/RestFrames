/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_03.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 March
///
///  \brief A simple example generating and analyzing top to b W(l nu) events
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

void example_03(const string& output_name = "output_example_03.root"){
  
  // set particle masses and widths
  double mtop = 173.;
  double wtop = 2.5;
  double mW   = 81.;
  double wW   = 2.5;
  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame           LAB_Gen("LAB_Gen","LAB");
  ResonanceGenFrame     T_Gen("T_Gen","t");
  ResonanceGenFrame     W_Gen("W_Gen","W");
  VisibleGenFrame       B_Gen("B_Gen","b");
  VisibleGenFrame       L_Gen("L_Gen","#it{l}");
  InvisibleGenFrame     NU_Gen("NU_Gen","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(T_Gen);
  T_Gen.AddChildFrame(B_Gen);
  T_Gen.AddChildFrame(W_Gen);
  W_Gen.AddChildFrame(L_Gen);
  W_Gen.AddChildFrame(NU_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  T_Gen.SetMass(mtop);
  T_Gen.SetWidth(wtop);
  W_Gen.SetMass(mW);
  W_Gen.SetWidth(wW);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB_Mt("LAB_Mt","LAB"); LabRecoFrame       LAB_MW("LAB_MW","LAB");
  DecayRecoFrame     T_Mt("T_Mt","t");       DecayRecoFrame     T_MW("T_MW","t");
  DecayRecoFrame     W_Mt("W_Mt","W");       DecayRecoFrame     W_MW("W_MW","W");
  VisibleRecoFrame   B_Mt("B_Mt","b");       VisibleRecoFrame   B_MW("B_MW","b");
  VisibleRecoFrame   L_Mt("L_Mt","#it{l}");  VisibleRecoFrame   L_MW("L_MW","#it{l}");
  InvisibleRecoFrame NU_Mt("NU_Mt","#nu");   InvisibleRecoFrame NU_MW("NU_MW","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Mt.SetChildFrame(T_Mt);                LAB_MW.SetChildFrame(T_MW);
  T_Mt.AddChildFrame(B_Mt);                  T_MW.AddChildFrame(B_MW);
  T_Mt.AddChildFrame(W_Mt);                  T_MW.AddChildFrame(W_MW);
  W_Mt.AddChildFrame(L_Mt);                  W_MW.AddChildFrame(L_MW);
  W_Mt.AddChildFrame(NU_Mt);                 W_MW.AddChildFrame(NU_MW);

  if(LAB_Mt.InitializeTree() && LAB_MW.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Groups
  InvisibleGroup INV_Mt("INV_Mt","#nu Jigsaws");
  INV_Mt.AddFrame(NU_Mt);     
  InvisibleGroup INV_MW("INV_MW","nu Jigsaws");
  INV_MW.AddFrame(NU_MW);
 
  // Set neutrino masses to zero
  SetMassInvJigsaw NuM_Mt("NuM_Mt","M_{#nu} = 0"); 
  INV_Mt.AddJigsaw(NuM_Mt);                        
  SetMassInvJigsaw NuM_MW("NuM_MW","M_{#nu} = 0");
  INV_MW.AddJigsaw(NuM_MW);

  // Set neutrino rapidity to that of visible particles
  SetRapidityInvJigsaw NuR_Mt("NuR_Mt","#eta_{#nu} = #eta_{b+#it{l}}");
  INV_Mt.AddJigsaw(NuR_Mt);
  NuR_Mt.AddVisibleFrame(L_Mt);
  NuR_Mt.AddVisibleFrame(B_Mt);
  SetRapidityInvJigsaw NuR_MW("NuR_Mt","#eta_{#nu} = #eta_{#it{l}}");
  INV_MW.AddJigsaw(NuR_MW);
  NuR_MW.AddVisibleFrame(L_MW);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree");

  treePlot->SetTree(LAB_Mt);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  
  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow t #rightarrow W(#it{l} #nu) b");

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen", "Generator");
  const HistPlotCategory& cat_minMt = histPlot->GetNewCategory("minMt", "min M_{t} Reco");
  const HistPlotCategory& cat_minMW = histPlot->GetNewCategory("minMW", "min M_{W} Reco");

  const HistPlotVar& Mt     = histPlot->GetNewVar("Mt", "M_{t}", 0., 250., "[GeV]");
  const HistPlotVar& MW     = histPlot->GetNewVar("MW", "M_{W}", 0., 150., "[GeV]");
  const HistPlotVar& pTt    = histPlot->GetNewVar("pTt","p_{T}^{t} / m_{t}", 0., 1.);
  const HistPlotVar& cosT   = histPlot->GetNewVar("cosT","cos #theta_{t}", -1., 1.);
  const HistPlotVar& cosW   = histPlot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  const HistPlotVar& dphiT  = histPlot->GetNewVar("dphiT", "#Delta #phi_{t}", 0., 2.*acos(-1.));
  const HistPlotVar& dphiW  = histPlot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosT  = histPlot->GetNewVar("DcosT","#theta_{t} - #theta_{t}^{gen}", -1., 1.);
  const HistPlotVar& DcosW  = histPlot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{gen}", -1., 1.);
  const HistPlotVar& DdphiT = histPlot->GetNewVar("DdphiT","#Delta #phi_{t} - #Delta #phi_{t}^{gen}", -1., 1.);
  const HistPlotVar& DdphiW = histPlot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{gen}", -1., 1.);

  histPlot->AddPlot(cosW,   cat_minMt+cat_minMW);
  histPlot->AddPlot(cosT,   cat_minMt+cat_minMW, true);
  histPlot->AddPlot(dphiT,  cat_minMt+cat_minMW, true);
  histPlot->AddPlot(dphiW,  cat_minMt+cat_minMW);
  histPlot->AddPlot(DcosT,  cat_minMt+cat_minMW);
  histPlot->AddPlot(DcosW,  cat_minMt+cat_minMW);
  histPlot->AddPlot(DdphiT, cat_minMt+cat_minMW);
  histPlot->AddPlot(DdphiW, cat_minMt+cat_minMW);
  histPlot->AddPlot(Mt,   MW, cat_minMt+cat_minMW+cat_Gen);
  histPlot->AddPlot(MW, cosW, cat_minMt+cat_minMW);
  histPlot->AddPlot(Mt, cosT, cat_minMt+cat_minMW);
  histPlot->AddPlot(MW, DcosW, cat_minMt+cat_minMW);
  histPlot->AddPlot(Mt, DcosT,    cat_minMt+cat_minMW);
  histPlot->AddPlot(DcosT, DcosW, cat_minMt+cat_minMW);
  histPlot->AddPlot(MW,     cat_minMt+cat_minMW+cat_Gen);
  histPlot->AddPlot(Mt,     cat_minMt+cat_minMW+cat_Gen);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((max(Ngen,10))/10) == 0) cout << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_Gen.ClearEvent();                           // clear the gen tree
    double PTt = mtop*gRandom->Rndm();
    LAB_Gen.SetTransverseMomenta(PTt);              // give the Top some Pt
    double PZt = 100.*mtop*(2.*gRandom->Rndm()-1.);
    LAB_Gen.SetLongitudinalMomenta(PZt);            // give the Top some Pz
    LAB_Gen.AnalyzeEvent();                         // generate a new event

    // analyze event
    LAB_Mt.ClearEvent();                             // clear the reco tree
    L_Mt.SetLabFrameFourVector(L_Gen.GetFourVector()); // Set lepton 4-vec
    B_Mt.SetLabFrameFourVector(B_Gen.GetFourVector()); // Set b-jet 4-vec
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    INV_Mt.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    LAB_Mt.AnalyzeEvent();                          //analyze the event

    // analyze event
    LAB_MW.ClearEvent();                             // clear the reco tree
    L_MW.SetLabFrameFourVector(L_Gen.GetFourVector()); // Set lepton 4-vec
    B_MW.SetLabFrameFourVector(B_Gen.GetFourVector()); // Set b-jet 4-vec
    INV_MW.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    LAB_MW.AnalyzeEvent();                          //analyze the event

    // Generator-level
    double MTgen = T_Gen.GetMass();
    double cosTgen  = T_Gen.GetCosDecayAngle();
    double dphiTgen = LAB_Gen.GetDeltaPhiDecayPlanes(T_Gen);
    double MWgen = W_Gen.GetMass();
    double cosWgen  = W_Gen.GetCosDecayAngle();
    double dphiWgen = T_Gen.GetDeltaPhiDecayPlanes(W_Gen);

    pTt = PTt / MTgen;

    Mt = MTgen;
    MW = MWgen;
    histPlot->Fill(cat_Gen);

    // minMt observables
    Mt     = T_Mt.GetMass();
    cosT   = T_Mt.GetCosDecayAngle();
    dphiT  = LAB_Mt.GetDeltaPhiDecayPlanes(T_Mt);
    MW     = W_Mt.GetMass();
    cosW   = W_Mt.GetCosDecayAngle();
    dphiW  = T_Mt.GetDeltaPhiDecayPlanes(W_Mt);
    DcosT  = asin(sqrt(1.-cosT*cosT)*cosTgen-sqrt(1.-cosTgen*cosTgen)*cosT);
    DdphiT = asin(sin(dphiT-dphiTgen));
    DcosW  = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    DdphiW = asin(sin(dphiW-dphiWgen));
    histPlot->Fill(cat_minMt);

    // minMW observables
    Mt     = T_MW.GetMass();
    cosT   = T_MW.GetCosDecayAngle();
    dphiT  = LAB_MW.GetDeltaPhiDecayPlanes(T_MW);
    MW     = W_MW.GetMass();
    cosW   = W_MW.GetCosDecayAngle();
    dphiW  = T_MW.GetDeltaPhiDecayPlanes(W_MW);
    DcosT  = asin(sqrt(1.-cosT*cosT)*cosTgen-sqrt(1.-cosTgen*cosTgen)*cosT);
    DdphiT = asin(sin(dphiT-dphiTgen));
    DcosW  = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    DdphiW = asin(sin(dphiW-dphiWgen));
    histPlot->Fill(cat_minMW);
  }

  histPlot->Draw();
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_03();
  return 0;
}
#endif
