/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_03_MultipleDecays_Toptoblnu.C
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

void example_03_MultipleDecays_Toptoblnu(string output_name = "output_03.root"){

  // set particle masses and widths
  double mtop = 173.;
  double wtop = 2.5;
  double mW   = 81.;
  double wW   = 2.5;
  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << g_End;
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
    g_Log << LogInfo << "...Successfully initialized generator tree" << g_End;
  else
    g_Log << LogError << "...Failed initializing generator tree" << g_End;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  T_Gen.SetMass(mtop);
  T_Gen.SetWidth(wtop);
  W_Gen.SetMass(mW);
  W_Gen.SetWidth(wW);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << g_End;
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
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << g_End;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << g_End;

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
  NuR_MW.AddVisibleFrame(B_MW);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << endl << g_End;
  else
    g_Log << LogError << "...Failed initializing analyses" << g_End;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetFrameTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree");

  treePlot->SetFrameTree(LAB_Mt);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  // Declare observables for histogram booking
  HistPlot* GenPlot   = new HistPlot("Gen","W #rightarrow #it{l} #nu", 
				     "Generator Level"); 
  HistPlot* minMtPlot = new HistPlot("minMt", "W #rightarrow #it{l} #nu",
				     "min M_{t} reconstruction"); 
  HistPlot* minMWPlot = new HistPlot("minMW", "W #rightarrow #it{l} #nu",
				     "min M_{W} reconstruction"); 

  const HistPlotVar& Mt     = GenPlot->GetNewVar("Mt", "M_{t}", 0., 190., "[GeV]");
  const HistPlotVar& MW     = GenPlot->GetNewVar("MW", "M_{W}", 0., 90., "[GeV]");
  const HistPlotVar& pTt    = GenPlot->GetNewVar("pTt","p_{T}^{t} / m_{t}", 0., 1.);
  const HistPlotVar& cosT   = GenPlot->GetNewVar("cosT","cos #theta_{t}", -1., 1.);
  const HistPlotVar& cosW   = GenPlot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  const HistPlotVar& dphiT  = GenPlot->GetNewVar("dphiT", "#Delta #phi_{t}", 0., 2.*acos(-1.));
  const HistPlotVar& dphiW  = GenPlot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  const HistPlotVar& DcosT  = GenPlot->GetNewVar("DcosW","#theta_{t} - #theta_{t}^{gen}", -1., 1.);
  const HistPlotVar& DcosW  = GenPlot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{gen}", -1., 1.);
  const HistPlotVar& DdphiT = GenPlot->GetNewVar("DdphiW","#Delta #phi_{t} - #Delta #phi_{t}^{gen}", -1., 1.);
  const HistPlotVar& DdphiW = GenPlot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{gen}", -1., 1.);

  GenPlot->AddHist(Mt);       GenPlot->AddHist(MW);
  minMtPlot->AddHist(Mt);     minMWPlot->AddHist(Mt);
  minMtPlot->AddHist(MW);     minMWPlot->AddHist(MW);
  minMtPlot->AddHist(pTt);    minMWPlot->AddHist(pTt);
  minMtPlot->AddHist(cosT);   minMWPlot->AddHist(cosT);
  minMtPlot->AddHist(cosW);   minMWPlot->AddHist(cosW);
  minMtPlot->AddHist(dphiT);  minMWPlot->AddHist(dphiT);
  minMtPlot->AddHist(dphiW);  minMWPlot->AddHist(dphiW);
  minMtPlot->AddHist(DcosT);  minMWPlot->AddHist(DcosT);
  minMtPlot->AddHist(DcosW);  minMWPlot->AddHist(DcosW);
  minMtPlot->AddHist(DdphiT); minMWPlot->AddHist(DdphiT);
  minMtPlot->AddHist(DdphiW); minMWPlot->AddHist(DdphiW);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((max(Ngen,10))/10) == 0) cout << "Generating event " << igen << " of " << Ngen << endl;

    // generate event
    LAB_Gen.ClearEvent();                           // clear the gen tree
    double PTt = mtop*gRandom->Rndm();
    LAB_Gen.SetTransverseMomenta(PTt);              // give the Top some Pt
    double PZt = mtop*(2.*gRandom->Rndm()-1.);
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

    // calculate observables
    Mt    = T_Mt.GetMass();
    double MTgen = T_Gen.GetMass();
    cosT     = T_Mt.GetCosDecayAngle();
    double cosTgen  = T_Gen.GetCosDecayAngle();
    dphiT    = LAB_Mt.GetDeltaPhiDecayPlanes(T_Mt);
    double dphiTgen = LAB_Gen.GetDeltaPhiDecayPlanes(T_Gen);

    MW    = W_Mt.GetMass();
    double MWgen = W_Gen.GetMass();
    cosW     = W_Mt.GetCosDecayAngle();
    double cosWgen  = W_Gen.GetCosDecayAngle();
    dphiW    = T_Mt.GetDeltaPhiDecayPlanes(W_Mt);
    double dphiWgen = T_Gen.GetDeltaPhiDecayPlanes(W_Gen);
    
    DcosT = asin(sqrt(1.-cosT*cosT)*cosTgen-sqrt(1.-cosTgen*cosTgen)*cosT);
    DdphiT = asin(sin(dphiT-dphiTgen));
    DcosW = asin(sqrt(1.-cosW*cosW)*cosWgen-sqrt(1.-cosWgen*cosWgen)*cosW);
    DdphiW = asin(sin(dphiW-dphiWgen));

    pTt = PTt / MTgen;

    minMtPlot->Fill();
  }
  minMtPlot->Draw();
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_03_MultipleDecays_Toptoblnu();
  return 0;
}
#endif
