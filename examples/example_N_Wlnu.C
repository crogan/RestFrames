/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_N_Wlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 May
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

void example_N_Wlnu(const std::string& output_name =
		    "output_example_N_Wlnu.root"){

  double mW   = 80.385;  // GeV, PDG 2016
  double wW   = 2.085;
  double mL   = 0.106;   // muons
  double mN   = 0.;

  // number of events to generate (for each W multiplicity)
  int Ngen = 100000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  
  ppLabGenFrame     LAB_G2W("LAB_G2W","LAB");      ppLabGenFrame     LAB_G3W("LAB_G3W","LAB");
  DecayGenFrame     CM_G2W("CM_G2W","CM");         DecayGenFrame     CM_G3W("CM_G3W","CM");
  ResonanceGenFrame Wa_G2W("Wa_G2W","W_{a}");      ResonanceGenFrame Wa_G3W("Wa_G3W","W_{a}");
  ResonanceGenFrame Wb_G2W("Wb_G2W","W_{b}");      ResonanceGenFrame Wb_G3W("Wb_G3W","W_{b}");
                                                   ResonanceGenFrame Wc_G3W("Wc_G3W","W_{c}");
  VisibleGenFrame   La_G2W("La_G2W","#it{l}_{a}"); VisibleGenFrame   La_G3W("La_G3W","#it{l}_{a}");
  InvisibleGenFrame Na_G2W("Na_G2W","#nu_{a}");    InvisibleGenFrame Na_G3W("Na_G3W","#nu_{a}");
  VisibleGenFrame   Lb_G2W("Lb_G2W","#it{l}_{b}"); VisibleGenFrame   Lb_G3W("Lb_G3W","#it{l}_{b}");
  InvisibleGenFrame Nb_G2W("Nb_G2W","#nu_{b}");    InvisibleGenFrame Nb_G3W("Nb_G3W","#nu_{b}");
                                                   VisibleGenFrame   Lc_G3W("Lc_G3W","#it{l}_{c}");
                                                   InvisibleGenFrame Nc_G3W("Nc_G3W","#nu_{c}");

  ppLabGenFrame     LAB_G4W("LAB_G4W","LAB");     
  DecayGenFrame     CM_G4W("CM_G4W","CM");        
  ResonanceGenFrame Wa_G4W("Wa_G4W","W_{a}");     
  ResonanceGenFrame Wb_G4W("Wb_G4W","W_{b}");    
  ResonanceGenFrame Wc_G4W("Wc_G4W","W_{c}");
  ResonanceGenFrame Wd_G4W("Wd_G4W","W_{d}");
  VisibleGenFrame   La_G4W("La_G4W","#it{l}_{a}"); 
  InvisibleGenFrame Na_G4W("Na_G4W","#nu_{a}");   
  VisibleGenFrame   Lb_G4W("Lb_G4W","#it{l}_{b}");
  InvisibleGenFrame Nb_G4W("Nb_G4W","#nu_{b}");
  VisibleGenFrame   Lc_G4W("Lc_G4W","#it{l}_{c}");
  InvisibleGenFrame Nc_G4W("Nc_G4W","#nu_{c}");
  VisibleGenFrame   Ld_G4W("Ld_G4W","#it{l}_{d}");
  InvisibleGenFrame Nd_G4W("Nd_G4W","#nu_{d}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB_G2W.SetChildFrame(CM_G2W);                   LAB_G3W.SetChildFrame(CM_G3W);
  CM_G2W.AddChildFrame(Wa_G2W);                    CM_G3W.AddChildFrame(Wa_G3W);
  CM_G2W.AddChildFrame(Wb_G2W);                    CM_G3W.AddChildFrame(Wb_G3W);
  Wa_G2W.AddChildFrame(La_G2W);                    CM_G3W.AddChildFrame(Wc_G3W);
  Wa_G2W.AddChildFrame(Na_G2W);                    Wa_G3W.AddChildFrame(La_G3W);
  Wb_G2W.AddChildFrame(Lb_G2W);                    Wa_G3W.AddChildFrame(Na_G3W);
  Wb_G2W.AddChildFrame(Nb_G2W);                    Wb_G3W.AddChildFrame(Lb_G3W);
                                                   Wb_G3W.AddChildFrame(Nb_G3W);
  LAB_G4W.SetChildFrame(CM_G4W);                   Wc_G3W.AddChildFrame(Lc_G3W);
  CM_G4W.AddChildFrame(Wa_G4W);                    Wc_G3W.AddChildFrame(Nc_G3W);                               
  CM_G4W.AddChildFrame(Wb_G4W);
  CM_G4W.AddChildFrame(Wc_G4W);
  CM_G4W.AddChildFrame(Wd_G4W);
  Wa_G4W.AddChildFrame(La_G4W);
  Wa_G4W.AddChildFrame(Na_G4W);
  Wb_G4W.AddChildFrame(Lb_G4W);
  Wb_G4W.AddChildFrame(Nb_G4W);
  Wc_G4W.AddChildFrame(Lc_G4W);
  Wc_G4W.AddChildFrame(Nc_G4W);
  Wd_G4W.AddChildFrame(Ld_G4W);
  Wd_G4W.AddChildFrame(Nd_G4W);

  if(LAB_G2W.InitializeTree() && LAB_G3W.InitializeTree() &&
     LAB_G4W.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  // non-resonant production
  CM_G2W.SetVariableMass(true); CM_G3W.SetVariableMass(true); CM_G4W.SetVariableMass(true);
  // set W masses and widths
  Wa_G2W.SetMass(mW);  Wb_G2W.SetMass(mW);
  Wa_G3W.SetMass(mW);  Wb_G3W.SetMass(mW);  Wc_G3W.SetMass(mW);
  Wa_G4W.SetMass(mW);  Wb_G4W.SetMass(mW);  Wc_G4W.SetMass(mW);   Wd_G4W.SetMass(mW);
  Wa_G2W.SetWidth(wW); Wb_G2W.SetWidth(wW);
  Wa_G3W.SetWidth(wW); Wb_G3W.SetWidth(wW); Wc_G3W.SetWidth(wW);
  Wa_G4W.SetWidth(wW); Wb_G4W.SetWidth(wW); Wc_G4W.SetWidth(wW); Wd_G4W.SetWidth(wW);
  // set lepton masses
  La_G2W.SetMass(mL);  Lb_G2W.SetMass(mL);
  La_G3W.SetMass(mL);  Lb_G3W.SetMass(mL);  Lc_G3W.SetMass(mL);
  La_G4W.SetMass(mL);  Lb_G4W.SetMass(mL);  Lc_G4W.SetMass(mL);  Ld_G4W.SetMass(mL);
  // set neutrino masses
  Na_G2W.SetMass(mN);  Nb_G2W.SetMass(mN);
  Na_G3W.SetMass(mN);  Nb_G3W.SetMass(mN);  Nc_G3W.SetMass(mN);
  Na_G4W.SetMass(mN);  Nb_G4W.SetMass(mN);  Nc_G4W.SetMass(mN);  Nd_G4W.SetMass(mN);

  // lepton pT/eta cuts
  La_G2W.SetPtCut(15.);  Lb_G2W.SetPtCut(15.);
  La_G2W.SetEtaCut(2.5); Lb_G2W.SetEtaCut(2.5);
  La_G3W.SetPtCut(15.);  Lb_G3W.SetPtCut(15.);  Lc_G3W.SetPtCut(15.);
  La_G3W.SetEtaCut(2.5); Lb_G3W.SetEtaCut(2.5); Lc_G3W.SetEtaCut(2.5);
  La_G4W.SetPtCut(15.);  Lb_G4W.SetPtCut(15.);  Lc_G4W.SetPtCut(15.);  Ld_G4W.SetPtCut(15.);
  La_G4W.SetEtaCut(2.5); Lb_G4W.SetEtaCut(2.5); Lc_G4W.SetEtaCut(2.5); Ld_G4W.SetEtaCut(2.5);
  
  if(LAB_G2W.InitializeAnalysis() && LAB_G3W.InitializeAnalysis() &&
     LAB_G4W.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analyses" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analyses" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////

  LabRecoFrame       LAB_2W("LAB_2W","LAB");       LabRecoFrame       LAB_3W("LAB_3W","LAB");
  DecayRecoFrame     CM_2W("CM_2W","CM");          DecayRecoFrame     CM_3W("CM_3W","CM");
  DecayRecoFrame     Wa_2W("Wa_2W","W_{a}");       DecayRecoFrame     Wa_3W("Wa_3W","W_{a}");
  DecayRecoFrame     Wb_2W("Wb_2W","W_{b}");       DecayRecoFrame     Wb_3W("Wb_3W","W_{b}");
                                                   DecayRecoFrame     Wc_3W("Wc_3W","W_{c}");
  VisibleRecoFrame   La_2W("La_2W","#it{l}_{a}");  VisibleRecoFrame   La_3W("La_3W","#it{l}_{a}");
  InvisibleRecoFrame Na_2W("Na_2W","#nu_{a}");     InvisibleRecoFrame Na_3W("Na_3W","#nu_{a}");
  VisibleRecoFrame   Lb_2W("Lb_2W","#it{l}_{b}");  VisibleRecoFrame   Lb_3W("Lb_3W","#it{l}_{b}");
  InvisibleRecoFrame Nb_2W("Nb_2W","#nu_{b}");     InvisibleRecoFrame Nb_3W("Nb_3W","#nu_{b}");
                                                   VisibleRecoFrame   Lc_3W("Lc_3W","#it{l}_{c}");
                                                   InvisibleRecoFrame Nc_3W("Nc_3W","#nu_{c}");

  LabRecoFrame       LAB_4W("LAB_4W","LAB");     
  DecayRecoFrame     CM_4W("CM_4W","CM");        
  DecayRecoFrame     Wa_4W("Wa_4W","W_{a}");     
  DecayRecoFrame     Wb_4W("Wb_4W","W_{b}");    
  DecayRecoFrame     Wc_4W("Wc_4W","W_{c}");
  DecayRecoFrame     Wd_4W("Wd_4W","W_{d}");
  VisibleRecoFrame   La_4W("La_4W","#it{l}_{a}"); 
  InvisibleRecoFrame Na_4W("Na_4W","#nu_{a}");   
  VisibleRecoFrame   Lb_4W("Lb_4W","#it{l}_{b}");
  InvisibleRecoFrame Nb_4W("Nb_4W","#nu_{b}");
  VisibleRecoFrame   Lc_4W("Lc_4W","#it{l}_{c}");
  InvisibleRecoFrame Nc_4W("Nc_4W","#nu_{c}");
  VisibleRecoFrame   Ld_4W("Ld_4W","#it{l}_{d}");
  InvisibleRecoFrame Nd_4W("Nd_4W","#nu_{d}");
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB_2W.SetChildFrame(CM_2W);                   LAB_3W.SetChildFrame(CM_3W);
  CM_2W.AddChildFrame(Wa_2W);                    CM_3W.AddChildFrame(Wa_3W);
  CM_2W.AddChildFrame(Wb_2W);                    CM_3W.AddChildFrame(Wb_3W);
  Wa_2W.AddChildFrame(La_2W);                    CM_3W.AddChildFrame(Wc_3W);
  Wa_2W.AddChildFrame(Na_2W);                    Wa_3W.AddChildFrame(La_3W);
  Wb_2W.AddChildFrame(Lb_2W);                    Wa_3W.AddChildFrame(Na_3W);
  Wb_2W.AddChildFrame(Nb_2W);                    Wb_3W.AddChildFrame(Lb_3W);
                                                 Wb_3W.AddChildFrame(Nb_3W);
  LAB_4W.SetChildFrame(CM_4W);                   Wc_3W.AddChildFrame(Lc_3W);
  CM_4W.AddChildFrame(Wa_4W);                    Wc_3W.AddChildFrame(Nc_3W);                               
  CM_4W.AddChildFrame(Wb_4W);
  CM_4W.AddChildFrame(Wc_4W);
  CM_4W.AddChildFrame(Wd_4W);
  Wa_4W.AddChildFrame(La_4W);
  Wa_4W.AddChildFrame(Na_4W);
  Wb_4W.AddChildFrame(Lb_4W);
  Wb_4W.AddChildFrame(Nb_4W);
  Wc_4W.AddChildFrame(Lc_4W);
  Wc_4W.AddChildFrame(Nc_4W);
  Wd_4W.AddChildFrame(Ld_4W);
  Wd_4W.AddChildFrame(Nd_4W);

  if(LAB_2W.InitializeTree() && LAB_3W.InitializeTree() &&
     LAB_4W.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Groups
  InvisibleGroup INV_2W("INV_2W","#nu #nu Jigsaws");
  INV_2W.AddFrame(Na_2W);
  INV_2W.AddFrame(Nb_2W);
  InvisibleGroup INV_3W("INV_3W","#nu #nu #nu Jigsaws");
  INV_3W.AddFrame(Na_3W);
  INV_3W.AddFrame(Nb_3W);
  INV_3W.AddFrame(Nc_3W);
  InvisibleGroup INV_4W("INV_4W","#nu #nu #nu #nu Jigsaws");
  INV_4W.AddFrame(Na_4W);
  INV_4W.AddFrame(Nb_4W);
  INV_4W.AddFrame(Nc_4W);
  INV_4W.AddFrame(Nd_4W);

  // Set N nu mass equal to ~ N lep mass
  SetMassInvJigsaw NuNuM_2W("NuNuM_2W", "M_{#nu#nu} ~ m_{#it{l}#it{l}}");
  INV_2W.AddJigsaw(NuNuM_2W);
  SetMassInvJigsaw NuNuM_3W("NuNuM_3W", "M_{#nu#nu#nu} ~ m_{#it{l}#it{l}#it{l}}");
  INV_3W.AddJigsaw(NuNuM_3W);
  SetMassInvJigsaw NuNuM_4W("NuNuM_4W", "M_{#nu#nu#nu#nu} ~ m_{#it{l}#it{l}#it{l}#it{l}}");
  INV_4W.AddJigsaw(NuNuM_4W);

  // Set N nu rapidity equal to N lep rapidity
  SetRapidityInvJigsaw NuNuR_2W("NuNuR_2W", "#eta_{#nu#nu} = #eta_{#it{l}#it{l}}");
  INV_2W.AddJigsaw(NuNuR_2W);
  NuNuR_2W.AddVisibleFrames(La_2W+Lb_2W);
  SetRapidityInvJigsaw NuNuR_3W("NuNuR_3W", "#eta_{#nu#nu#nu} = #eta_{#it{l}#it{l}#it{l}}");
  INV_3W.AddJigsaw(NuNuR_3W);
  NuNuR_3W.AddVisibleFrames(La_3W+Lb_3W+Lc_3W);
  SetRapidityInvJigsaw NuNuR_4W("NuNuR_4W", "#eta_{#nu#nu#nu#nu} = #eta_{#it{l}#it{l}#it{l}#it{l}}");
  INV_4W.AddJigsaw(NuNuR_4W);
  NuNuR_4W.AddVisibleFrames(La_4W+Lb_4W+Lc_4W+Ld_4W);

  // Set neutrino 4-vectors by minimizing N W masses ^2
  MinMassesSqInvJigsaw MinMW_2W("MinMW_2W","min #Sigma M_{W}^{ 2}", 2);
  INV_2W.AddJigsaw(MinMW_2W);
  MinMW_2W.AddVisibleFrame(La_2W, 0);   MinMW_2W.AddInvisibleFrame(Na_2W, 0);
  MinMW_2W.AddVisibleFrame(Lb_2W, 1);   MinMW_2W.AddInvisibleFrame(Nb_2W, 1);
  MinMassesSqInvJigsaw MinMW_3W("MinMW_3W","min #Sigma M_{W}^{ 2}", 3);
  INV_3W.AddJigsaw(MinMW_3W);
  MinMW_3W.AddVisibleFrame(La_3W, 0);   MinMW_3W.AddInvisibleFrame(Na_3W, 0);
  MinMW_3W.AddVisibleFrame(Lb_3W, 1);   MinMW_3W.AddInvisibleFrame(Nb_3W, 1);
  MinMW_3W.AddVisibleFrame(Lc_3W, 2);   MinMW_3W.AddInvisibleFrame(Nc_3W, 2);
  MinMassesSqInvJigsaw MinMW_4W("MinMW_4W","min #Sigma M_{W}^{ 2}", 4);
  INV_4W.AddJigsaw(MinMW_4W);
  MinMW_4W.AddVisibleFrame(La_4W, 0);   MinMW_4W.AddInvisibleFrame(Na_4W, 0);
  MinMW_4W.AddVisibleFrame(Lb_4W, 1);   MinMW_4W.AddInvisibleFrame(Nb_4W, 1);
  MinMW_4W.AddVisibleFrame(Lc_4W, 2);   MinMW_4W.AddInvisibleFrame(Nc_4W, 2);
  MinMW_4W.AddVisibleFrame(Ld_4W, 3);   MinMW_4W.AddInvisibleFrame(Nd_4W, 3);

  if(LAB_2W.InitializeAnalysis() && LAB_3W.InitializeAnalysis() &&
     LAB_4W.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
  
  treePlot->SetTree(CM_2W);
  treePlot->Draw("Reco_2W_Tree", "2W Reconstruction Tree");

  treePlot->SetTree(CM_3W);
  treePlot->Draw("Reco_3W_Tree", "3W Reconstruction Tree");

  treePlot->SetTree(CM_4W);
  treePlot->Draw("Reco_4W_Tree", "4W Reconstruction Tree");

  treePlot->SetTree(INV_2W);
  treePlot->Draw("Inv_2W_Tree", "2W Inivisible Jigsaws", true);

  treePlot->SetTree(INV_3W);
  treePlot->Draw("Inv_3W_Tree", "3W Inivisible Jigsaws", true);

  treePlot->SetTree(INV_4W);
  treePlot->Draw("Inv_4W_Tree", "4W Inivisible Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow N W(#it{l} #nu)");

  const HistPlotCategory& cat_2W = histPlot->GetNewCategory("Reco_2W", "2 W(#it{l} #nu)");
  const HistPlotCategory& cat_3W = histPlot->GetNewCategory("Reco_3W", "3 W(#it{l} #nu)");
  const HistPlotCategory& cat_4W = histPlot->GetNewCategory("Reco_4W", "4 W(#it{l} #nu)");

  const HistPlotVar& MCM    = histPlot->GetNewVar("MCM", "M_{CM} / m_{CM}^{ true}", 0., 2.);
  const HistPlotVar& sumMW2 = histPlot->GetNewVar("MWTOT",
		   "#sqrt{#Sigma M_{W}^{ 2} / #Sigma m_{W, true}^{ 2}}", 0., 1.5);
  double sumMW2gen;
  
  histPlot->AddPlot(MCM,    cat_2W+cat_3W+cat_4W);
  histPlot->AddPlot(sumMW2, cat_2W+cat_3W+cat_4W);
  histPlot->AddPlot(MCM, sumMW2, cat_2W+cat_3W+cat_4W);

  
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G2W.ClearEvent();                            // clear the gen tree;
    LAB_G2W.AnalyzeEvent();                          // generate a new event
    LAB_G3W.ClearEvent();                                   
    LAB_G3W.AnalyzeEvent();                         
    LAB_G4W.ClearEvent();                                
    LAB_G4W.AnalyzeEvent();                        

    TVector3 MET_2W = LAB_G2W.GetInvisibleMomentum();  // Get the MET from gen tree
    MET_2W.SetZ(0.);
    TVector3 MET_3W = LAB_G3W.GetInvisibleMomentum();  
    MET_3W.SetZ(0.);
    TVector3 MET_4W = LAB_G4W.GetInvisibleMomentum(); 
    MET_4W.SetZ(0.);

    // analyze event(s)
    LAB_2W.ClearEvent();                                  // clear the reco tree
    INV_2W.SetLabFrameThreeVector(MET_2W);                // Set the MET in reco tree
    LAB_3W.ClearEvent();                              
    INV_3W.SetLabFrameThreeVector(MET_3W);            
    LAB_4W.ClearEvent();                           
    INV_4W.SetLabFrameThreeVector(MET_4W);              

    La_2W.SetLabFrameFourVector(La_G2W.GetFourVector());  // Set "measured" lep 4-vectors
    Lb_2W.SetLabFrameFourVector(Lb_G2W.GetFourVector());
    La_3W.SetLabFrameFourVector(La_G3W.GetFourVector()); 
    Lb_3W.SetLabFrameFourVector(Lb_G3W.GetFourVector());
    Lc_3W.SetLabFrameFourVector(Lc_G3W.GetFourVector());
    La_4W.SetLabFrameFourVector(La_G4W.GetFourVector()); 
    Lb_4W.SetLabFrameFourVector(Lb_G4W.GetFourVector());
    Lc_4W.SetLabFrameFourVector(Lc_G4W.GetFourVector());
    Ld_4W.SetLabFrameFourVector(Ld_G4W.GetFourVector());

    LAB_2W.AnalyzeEvent();                                // analyze the event
    LAB_3W.AnalyzeEvent();
    LAB_4W.AnalyzeEvent();
    
    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////
    
    MCM = CM_2W.GetMass()/CM_G2W.GetMass();
    sumMW2  = Wa_2W.GetMass()*Wa_2W.GetMass();
    sumMW2 += Wb_2W.GetMass()*Wb_2W.GetMass();
    sumMW2gen  = Wa_G2W.GetMass()*Wa_G2W.GetMass();
    sumMW2gen += Wb_G2W.GetMass()*Wb_G2W.GetMass();
    sumMW2 /= sumMW2gen;
    sumMW2 = sqrt(sumMW2);

    histPlot->Fill(cat_2W);

    MCM = CM_3W.GetMass()/CM_G3W.GetMass();
    sumMW2  = Wa_3W.GetMass()*Wa_3W.GetMass();
    sumMW2 += Wb_3W.GetMass()*Wb_3W.GetMass();
    sumMW2 += Wc_3W.GetMass()*Wc_3W.GetMass();
    sumMW2gen  = Wa_G3W.GetMass()*Wa_G3W.GetMass();
    sumMW2gen += Wb_G3W.GetMass()*Wb_G3W.GetMass();
    sumMW2gen += Wc_G3W.GetMass()*Wc_G3W.GetMass();
    sumMW2 /= sumMW2gen;
    sumMW2 = sqrt(sumMW2);

    histPlot->Fill(cat_3W);

    MCM = CM_4W.GetMass()/CM_G4W.GetMass();
    sumMW2  = Wa_4W.GetMass()*Wa_4W.GetMass();
    sumMW2 += Wb_4W.GetMass()*Wb_4W.GetMass();
    sumMW2 += Wc_4W.GetMass()*Wc_4W.GetMass();
    sumMW2 += Wd_4W.GetMass()*Wd_4W.GetMass();
    sumMW2gen  = Wa_G4W.GetMass()*Wa_G4W.GetMass();
    sumMW2gen += Wb_G4W.GetMass()*Wb_G4W.GetMass();
    sumMW2gen += Wc_G4W.GetMass()*Wc_G4W.GetMass();
    sumMW2gen += Wd_G4W.GetMass()*Wd_G4W.GetMass();
    sumMW2 /= sumMW2gen;
    sumMW2 = sqrt(sumMW2);

    histPlot->Fill(cat_4W);
  }

  histPlot->Draw();

  TFile fout(output_name.c_str(),"RECREATE");
  fout.Close();
  histPlot->WriteOutput(output_name);
  histPlot->WriteHist(output_name);
  treePlot->WriteOutput(output_name);

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_N_Wlnu();
  return 0;
}
#endif
