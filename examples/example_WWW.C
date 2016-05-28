/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_WWW.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 May
///
///  \brief An example generating and analyzing pp -> WWW to 3l 3nu events
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

void example_WWW(const std::string& output_name = "output_example_04.root"){
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);
  SetLogMaxWidth(120);

  double mW = 80.;
  double wW = 2.5;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 100000;

  // Set up toy generation tree (not needed for reconstruction)
  g_Log << LogInfo << "Initializing generator frames and tree" << LogEnd;
  ppLabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame CM_G("CM_G","CM");
  ResonanceGenFrame Wa_G("Wa_G","W_{a}");
  ResonanceGenFrame Wb_G("Wb_G","W_{b}");
  ResonanceGenFrame Wc_G("Wc_G","W_{c}");
  VisibleGenFrame La_G("La_G","#it{l}_{a}");
  InvisibleGenFrame Na_G("Na_G","#nu_{a}");
  VisibleGenFrame Lb_G("Lb_G","#it{l}_{b}");
  InvisibleGenFrame Nb_G("Nb_G","#nu_{b}");
  VisibleGenFrame Lc_G("Lc_G","#it{l}_{c}");
  InvisibleGenFrame Nc_G("Nc_G","#nu_{c}");
  LAB_G.SetChildFrame(CM_G);
  CM_G.AddChildFrame(Wa_G);
  CM_G.AddChildFrame(Wb_G);
  CM_G.AddChildFrame(Wc_G);
  Wa_G.AddChildFrame(La_G);
  Wa_G.AddChildFrame(Na_G);
  Wb_G.AddChildFrame(Lb_G);
  Wb_G.AddChildFrame(Nb_G);
  Wc_G.AddChildFrame(Lc_G);
  Wc_G.AddChildFrame(Nc_G);

  if(LAB_G.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready to set masses etc." << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_G) << LogEnd;								    

  // set W masses
  Wa_G.SetMass(mW);
  Wb_G.SetMass(mW);
  Wc_G.SetMass(mW);
  Wa_G.SetWidth(wW);
  Wb_G.SetWidth(wW);
  Wc_G.SetWidth(wW);
  // set lepton masses
  La_G.SetMass(mL);
  Lb_G.SetMass(mL);
  Lc_G.SetMass(mL);
  // set neutrino masses
  Na_G.SetMass(mN);
  Nb_G.SetMass(mN);
  Nc_G.SetMass(mN);

  CM_G.SetVariableMass(true);

  if(LAB_G.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized analysis from LabFrame ";
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready for event generation" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize analysis from LabFrame: " << Log(LAB_G) << LogEnd;

  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow W(#it{l} #nu) W(#it{l} #nu) W(#it{l} #nu)");
  //histPlot->SetRebin(1);

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen", "Generator");

  const HistPlotVar& PzCM     = histPlot->GetNewVar("PzCM", "p_{z, CM}", -5000., 5000., "[GeV]");
  const HistPlotVar& MCM     = histPlot->GetNewVar("MCM", "M_{CM}", 0., 3000., "[GeV]");
  const HistPlotVar& MWa     = histPlot->GetNewVar("MWa", "M_{Wa}", 0., 150., "[GeV]");
  const HistPlotVar& MWb     = histPlot->GetNewVar("MWb", "M_{Wb}", 0., 150., "[GeV]");
  const HistPlotVar& MWc     = histPlot->GetNewVar("MWc", "M_{Wc}", 0., 150., "[GeV]");
  const HistPlotVar& Mab2     = histPlot->GetNewVar("Mab2", "M_{ab}^{2}", 0., 1.);
  const HistPlotVar& Mcb2     = histPlot->GetNewVar("Mcb2", "M_{cb}^{2}", 0., 1.);
  // const HistPlotVar& pTt    = histPlot->GetNewVar("pTt","p_{T}^{t} / m_{t}", 0., 1.);
  // const HistPlotVar& cosT   = histPlot->GetNewVar("cosT","cos #theta_{t}", -1., 1.);
  // const HistPlotVar& cosW   = histPlot->GetNewVar("cosW","cos #theta_{W}", -1., 1.);
  // const HistPlotVar& dphiT  = histPlot->GetNewVar("dphiT", "#Delta #phi_{t}", 0., 2.*acos(-1.));
  // const HistPlotVar& dphiW  = histPlot->GetNewVar("dphiW", "#Delta #phi_{W}", 0., 2.*acos(-1.));
  // const HistPlotVar& DcosT  = histPlot->GetNewVar("DcosT","#theta_{t} - #theta_{t}^{gen}", -1., 1.);
  // const HistPlotVar& DcosW  = histPlot->GetNewVar("DcosW","#theta_{W} - #theta_{W}^{gen}", -1., 1.);
  // const HistPlotVar& DdphiT = histPlot->GetNewVar("DdphiT","#Delta #phi_{t} - #Delta #phi_{t}^{gen}", -1., 1.);
  // const HistPlotVar& DdphiW = histPlot->GetNewVar("DdphiW","#Delta #phi_{W} - #Delta #phi_{W}^{gen}", -1., 1.);
  
  histPlot->AddPlot(MCM, cat_Gen);
  histPlot->AddPlot(PzCM, cat_Gen);
  histPlot->AddPlot(PzCM, MCM, cat_Gen);
  histPlot->AddPlot(MWa, MWb, cat_Gen);
  histPlot->AddPlot(MWb, MWc, cat_Gen);
  histPlot->AddPlot(MWa, MWc, cat_Gen);
  histPlot->AddPlot(MCM, MWa, cat_Gen);
  histPlot->AddPlot(Mab2, Mcb2, cat_Gen);
 

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTCM = 3*mW*gRandom->Rndm()*0.0001;
    LAB_G.SetTransverseMomenta(PTCM);               // give the CM some Pt
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** Higgs mass
    PzCM = CM_G.GetFourVector().Z();
    MCM = CM_G.GetMass();
    MWa = Wa_G.GetMass();
    MWb = Wb_G.GetMass();
    MWc = Wc_G.GetMass();
    Mab2 = (Wa_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    Mcb2 = (Wc_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    histPlot->Fill(cat_Gen);
  }

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  histPlot->Draw();

 

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_WWW();
  return 0;
}
#endif
