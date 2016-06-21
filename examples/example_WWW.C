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

void example_WWW(const std::string& output_name = "output_example_WWW.root"){
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);
  SetLogMaxWidth(120);

  double mW = 80.;
  double wW = 2.5;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 10000;

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
  //CM_G.SetMass(600.);
  CM_G.SetVariableMass(true);

  if(LAB_G.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized analysis from LabFrame ";
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready for event generation" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize analysis from LabFrame: " << Log(LAB_G) << LogEnd;


  LabRecoFrame         LAB_R("LAB_R","LAB");
  DecayRecoFrame       CM_R("CM_R","CM");
  DecayRecoFrame       Wa_R("Wa_R","W_{a}");
  DecayRecoFrame       Wb_R("Wb_R","W_{b}");
  DecayRecoFrame       Wc_R("Wc_R","W_{c}");
  VisibleRecoFrame     La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame   Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame     Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame   Nb_R("Nb_R","#nu_{b}");
  VisibleRecoFrame     Lc_R("Lc_R","#it{l}_{c}");
  InvisibleRecoFrame   Nc_R("Nc_R","#nu_{c}");
  
  LAB_R.SetChildFrame(CM_R);
  CM_R.AddChildFrame(Wa_R);
  CM_R.AddChildFrame(Wb_R);
  CM_R.AddChildFrame(Wc_R);
  Wa_R.AddChildFrame(La_R);
  Wa_R.AddChildFrame(Na_R);
  Wb_R.AddChildFrame(Lb_R);
  Wb_R.AddChildFrame(Nb_R);
  Wc_R.AddChildFrame(Lc_R);
  Wc_R.AddChildFrame(Nc_R);

  if(LAB_R.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;

  // Invisible Group
  InvisibleGroup INV_R("INV_R","#nu #nu #nu Jigsaws");
  INV_R.AddFrame(Na_R);
  INV_R.AddFrame(Nb_R);
  INV_R.AddFrame(Nc_R);

  // Set nu nu mass equal to l l mass
  SetMassInvJigsaw NuNuM_R("NuNuM_R", "M_{#nu#nu#nu} ~ m_{#it{l}#it{l}#it{l}}");
  INV_R.AddJigsaw(NuNuM_R);

  SetRapidityInvJigsaw NuNuR_R("NuNuR_R", "#eta_{#nu#nu#nu} = #eta_{#it{l}#it{l}#it{l}}");
  INV_R.AddJigsaw(NuNuR_R);
  NuNuR_R.AddVisibleFrames(La_R+Lb_R+Lc_R);

  MinMassesSqInvJigsaw MinMW_R("MinMW_R","min #Sigma M_{W}^{ 2}", 3);
  INV_R.AddJigsaw(MinMW_R);
  MinMW_R.AddVisibleFrame(La_R, 0);
  MinMW_R.AddVisibleFrame(Lb_R, 1);
  MinMW_R.AddVisibleFrame(Lc_R, 2);
  MinMW_R.AddInvisibleFrame(Na_R, 0);
  MinMW_R.AddInvisibleFrame(Nb_R, 1);
  MinMW_R.AddInvisibleFrame(Nc_R, 2);

  if(LAB_R.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;
  
  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow W(#it{l} #nu) W(#it{l} #nu) W(#it{l} #nu)");
  //histPlot->SetRebin(1);

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen", "Generator");
  const HistPlotCategory& cat_Reco   = histPlot->GetNewCategory("Reco", "Reconstruction");

  const HistPlotVar& PzCM     = histPlot->GetNewVar("PzCM", "p_{z, CM}", -5000., 5000., "[GeV]");
  const HistPlotVar& MCM     = histPlot->GetNewVar("MCM", "M_{CM}", 0., 3000., "[GeV]");
  const HistPlotVar& MWa     = histPlot->GetNewVar("MWa", "M_{Wa}", 0., 150., "[GeV]");
  const HistPlotVar& MWb     = histPlot->GetNewVar("MWb", "M_{Wb}", 0., 150., "[GeV]");
  const HistPlotVar& MWc     = histPlot->GetNewVar("MWc", "M_{Wc}", 0., 150., "[GeV]");
  const HistPlotVar& Mab2     = histPlot->GetNewVar("Mab2", "M_{ab}^{2}", 0., 1.);
  const HistPlotVar& Mcb2     = histPlot->GetNewVar("Mcb2", "M_{cb}^{2}", 0., 1.);

  const HistPlotVar& cosWa     = histPlot->GetNewVar("cosWa", "cos #theta_{Wa}", -1., 1.);
  const HistPlotVar& cosWb     = histPlot->GetNewVar("cosWb", "cos #theta_{Wb}", -1., 1.);
  const HistPlotVar& cosWc     = histPlot->GetNewVar("cosWc", "cos #theta_{Wc}", -1., 1.);
 
  const HistPlotVar& MCMN     = histPlot->GetNewVar("MCMN", "M_{CM}", 0., 2.);
  const HistPlotVar& MWaN     = histPlot->GetNewVar("MWaN", "M_{Wa}", 0., 2.);
  const HistPlotVar& MWbN     = histPlot->GetNewVar("MWbN", "M_{Wb}", 0., 2.);
  const HistPlotVar& MWcN     = histPlot->GetNewVar("MWcN", "M_{Wc}", 0., 2.);
  const HistPlotVar& MWTOT    = histPlot->GetNewVar("MWTOT", "#sqrt{#Sigma M_{W}^{ 2} / #Sigma m_{W}^{ 2}}", 0., 4.);
  
  histPlot->AddPlot(MCM, cat_Gen);
  histPlot->AddPlot(PzCM, cat_Gen);
  histPlot->AddPlot(PzCM, MCM, cat_Gen);
  histPlot->AddPlot(MWa, MWb, cat_Gen);
  histPlot->AddPlot(MWb, MWc, cat_Gen);
  histPlot->AddPlot(MWa, MWc, cat_Gen);
  histPlot->AddPlot(cosWa, cosWb, cat_Gen);
  histPlot->AddPlot(cosWa, cosWc, cat_Gen);
  histPlot->AddPlot(cosWb, cosWc, cat_Gen);
  histPlot->AddPlot(MCM, MWa, cat_Gen);
  histPlot->AddPlot(Mab2, Mcb2, cat_Gen);
 
  histPlot->AddPlot(MCMN, cat_Reco);
  histPlot->AddPlot(MWTOT, cat_Reco);
  histPlot->AddPlot(MWTOT, MCMN, cat_Reco);
  histPlot->AddPlot(MWaN, cat_Reco);
  histPlot->AddPlot(MWbN, cat_Reco);
  histPlot->AddPlot(MWcN, cat_Reco);
  histPlot->AddPlot(MWaN, MCMN, cat_Reco);
  histPlot->AddPlot(MWaN, MWbN, cat_Reco);
  histPlot->AddPlot(MWaN, MWcN, cat_Reco);
  histPlot->AddPlot(MWcN, MWbN, cat_Reco);
   
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTCM = 3*mW*gRandom->Rndm()*0.;
    LAB_G.SetTransverseMomenta(PTCM);               // give the CM some Pt
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    // analyze event
    LAB_R.ClearEvent();                               // clear the reco tree
    
    INV_R.SetLabFrameThreeVector(MET);                // Set the MET in reco tree
    //INV_R.SetMass((La_G+Lb_G+Lc_G).GetMass());  

    La_R.SetLabFrameFourVector(La_G.GetFourVector());
    Lb_R.SetLabFrameFourVector(Lb_G.GetFourVector());
    Lc_R.SetLabFrameFourVector(Lc_G.GetFourVector());

    LAB_R.AnalyzeEvent();                             // analyze the event
    
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
    cosWa = Wa_G.GetCosDecayAngle();
    cosWb = Wb_G.GetCosDecayAngle();
    cosWc = Wc_G.GetCosDecayAngle();
    Mab2 = (Wa_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    Mcb2 = (Wc_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    
    histPlot->Fill(cat_Gen);

    MCMN = CM_R.GetMass()/MCM;
    // MWaN = (INVs[0]+LEPs[0]).M()/MWa;
    // MWbN = (INVs[1]+LEPs[1]).M()/MWb;
    // MWcN = (INVs[2]+LEPs[2]).M()/MWc;
    MWaN = Wa_R.GetMass()/MWa;
    MWbN = Wb_R.GetMass()/MWb;
    MWcN = Wc_R.GetMass()/MWc;
    // MWaN = (INVs[0]+LEPs[0]).M()/MWa;
    // MWbN = (INVs[1]+LEPs[1]).M()/MWb;
    // MWcN = (INVs[2]+LEPs[2]).M()/MWc;
    
    double MWgTOT = MWa*MWa + MWb*MWb + MWc*MWc;
    MWTOT =
      Wa_R.GetMass()*Wa_R.GetMass() +
      Wb_R.GetMass()*Wb_R.GetMass() +
      Wc_R.GetMass()*Wc_R.GetMass();
    MWTOT /= MWgTOT;
    MWTOT = sqrt(MWTOT);

    histPlot->Fill(cat_Reco);

    // g_Log << LogInfo;
    // g_Log << (INVs[0]+LEPs[0]).M() << " " << (INVs[1]+LEPs[1]).M()  << " " << (INVs[2]+LEPs[2]).M() << std::endl;
    // g_Log << Wa_R.GetMass() << " " << Wb_R.GetMass() << " " << Wc_R.GetMass() << std::endl;
    // g_Log << LogEnd;
    
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
