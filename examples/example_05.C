/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_05.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 June
///
///  \brief An example generating and analyzing di-leptonic ttbar events
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

void example_05(std::string output_name = "output_example_05.root"){
  
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);

  double mT = 175.;
  double mW = 80.;
  double mB = 2.;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 100000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     TT_Gen("TT_Gen","t #bar{t}");
  DecayGenFrame     Ta_Gen("Ta_Gen","t_{a}");
  DecayGenFrame     Tb_Gen("Tb_Gen","t_{b}");
  DecayGenFrame     Wa_Gen("Wa_Gen","W_{a}");
  DecayGenFrame     Wb_Gen("Wb_Gen","W_{b}");
  VisibleGenFrame   Ba_Gen("Ba_Gen","b_{a}");
  VisibleGenFrame   La_Gen("La_Gen","#it{l}_{a}");
  InvisibleGenFrame Na_Gen("Na_Gen","#nu_{a}");
  VisibleGenFrame   Bb_Gen("Bb_Gen","b_{b}");
  VisibleGenFrame   Lb_Gen("Lb_Gen","#it{l}_{b}");
  InvisibleGenFrame Nb_Gen("Nb_Gen","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(TT_Gen);
  TT_Gen.AddChildFrame(Ta_Gen);
  TT_Gen.AddChildFrame(Tb_Gen);
  Ta_Gen.AddChildFrame(Ba_Gen);
  Ta_Gen.AddChildFrame(Wa_Gen);
  Tb_Gen.AddChildFrame(Bb_Gen);
  Tb_Gen.AddChildFrame(Wb_Gen);
  Wa_Gen.AddChildFrame(La_Gen);
  Wa_Gen.AddChildFrame(Na_Gen);
  Wb_Gen.AddChildFrame(Lb_Gen);
  Wb_Gen.AddChildFrame(Nb_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;							        
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  TT_Gen.SetMass(1000.);
  //TT_Gen.SetVariableMass();
  // set top masses
  Ta_Gen.SetMass(mT);
  Tb_Gen.SetMass(mT);
  // set W masses
  Wa_Gen.SetMass(mW);
  Wb_Gen.SetMass(mW);
  // set B masses
  Ba_Gen.SetMass(mB);
  Bb_Gen.SetMass(mB);
  // set : masses
  La_Gen.SetMass(mL);
  Lb_Gen.SetMass(mL);
  // set neutrino masses
  Na_Gen.SetMass(mN);
  Nb_Gen.SetMass(mN);

  Ba_Gen.SetPtCut(20.);
  Bb_Gen.SetPtCut(20.);
  Ba_Gen.SetEtaCut(2.5);
  Bb_Gen.SetEtaCut(2.5);

  La_Gen.SetPtCut(15.);
  Lb_Gen.SetPtCut(15.);
  La_Gen.SetEtaCut(2.5);
  Lb_Gen.SetEtaCut(2.5);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////								    
  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB_R1("LAB_R1","LAB");      LabRecoFrame       LAB_R2("LAB_R2","LAB");
  DecayRecoFrame     TT_R1("TT_R1","t #bar{t}");  DecayRecoFrame     TT_R2("TT_R2","t #bar{t}");
  DecayRecoFrame     Ta_R1("Ta_R1","t_{a}");      DecayRecoFrame     Ta_R2("Ta_R2","t_{a}");
  DecayRecoFrame     Tb_R1("Tb_R1","t_{b}");      DecayRecoFrame     Tb_R2("Tb_R2","t_{b}");
  DecayRecoFrame     Wa_R1("Wa_R1","W_{a}");      DecayRecoFrame     Wa_R2("Wa_R2","W_{a}");
  DecayRecoFrame     Wb_R1("Wb_R1","W_{b}");      DecayRecoFrame     Wb_R2("Wb_R2","W_{b}");
  VisibleRecoFrame   Ba_R1("Ba_R1","b_{a}");      VisibleRecoFrame   Ba_R2("Ba_R2","b_{a}");
  VisibleRecoFrame   La_R1("La_R1","#it{l}_{a}"); VisibleRecoFrame   La_R2("La_R2","#it{l}_{a}");
  InvisibleRecoFrame Na_R1("Na_R1","#nu_{a}");    InvisibleRecoFrame Na_R2("Na_R2","#nu_{a}");
  VisibleRecoFrame   Bb_R1("Bb_R1","b_{b}");      VisibleRecoFrame   Bb_R2("Bb_R2","b_{b}");
  VisibleRecoFrame   Lb_R1("Lb_R1","#it{l}_{b}"); VisibleRecoFrame   Lb_R2("Lb_R2","#it{l}_{b}");
  InvisibleRecoFrame Nb_R1("Nb_R1","#nu_{b}");    InvisibleRecoFrame Nb_R2("Nb_R2","#nu_{b}");

  LabRecoFrame       LAB_R3("LAB_R3","LAB");      LabRecoFrame       LAB_R4("LAB_R4","LAB");
  DecayRecoFrame     TT_R3("TT_R3","t #bar{t}");  DecayRecoFrame     TT_R4("TT_R4","t #bar{t}");
  DecayRecoFrame     Ta_R3("Ta_R3","t_{a}");      DecayRecoFrame     Ta_R4("Ta_R4","t_{a}");
  DecayRecoFrame     Tb_R3("Tb_R3","t_{b}");      DecayRecoFrame     Tb_R4("Tb_R4","t_{b}");
  DecayRecoFrame     Wa_R3("Wa_R3","W_{a}");      DecayRecoFrame     Wa_R4("Wa_R4","W_{a}");
  DecayRecoFrame     Wb_R3("Wb_R3","W_{b}");      DecayRecoFrame     Wb_R4("Wb_R4","W_{b}");
  VisibleRecoFrame   Ba_R3("Ba_R3","b_{a}");      VisibleRecoFrame   Ba_R4("Ba_R4","b_{a}");
  VisibleRecoFrame   La_R3("La_R3","#it{l}_{a}"); VisibleRecoFrame   La_R4("La_R4","#it{l}_{a}");
  InvisibleRecoFrame Na_R3("Na_R3","#nu_{a}");    InvisibleRecoFrame Na_R4("Na_R4","#nu_{a}");
  VisibleRecoFrame   Bb_R3("Bb_R3","b_{b}");      VisibleRecoFrame   Bb_R4("Bb_R4","b_{b}");
  VisibleRecoFrame   Lb_R3("Lb_R3","#it{l}_{b}"); VisibleRecoFrame   Lb_R4("Lb_R4","#it{l}_{b}");
  InvisibleRecoFrame Nb_R3("Nb_R3","#nu_{b}");    InvisibleRecoFrame Nb_R4("Nb_R4","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_R1.SetChildFrame(TT_R1);                    LAB_R2.SetChildFrame(TT_R2);
  TT_R1.AddChildFrame(Ta_R1);                     TT_R2.AddChildFrame(Ta_R2);
  TT_R1.AddChildFrame(Tb_R1);                     TT_R2.AddChildFrame(Tb_R2);
  Ta_R1.AddChildFrame(Ba_R1);                     Ta_R2.AddChildFrame(Ba_R2);
  Ta_R1.AddChildFrame(Wa_R1);                     Ta_R2.AddChildFrame(Wa_R2);
  Tb_R1.AddChildFrame(Bb_R1);                     Tb_R2.AddChildFrame(Bb_R2);
  Tb_R1.AddChildFrame(Wb_R1);                     Tb_R2.AddChildFrame(Wb_R2);
  Wa_R1.AddChildFrame(La_R1);                     Wa_R2.AddChildFrame(La_R2);
  Wa_R1.AddChildFrame(Na_R1);                     Wa_R2.AddChildFrame(Na_R2);
  Wb_R1.AddChildFrame(Lb_R1);                     Wb_R2.AddChildFrame(Lb_R2);
  Wb_R1.AddChildFrame(Nb_R1);                     Wb_R2.AddChildFrame(Nb_R2);

  LAB_R3.SetChildFrame(TT_R3);                    LAB_R4.SetChildFrame(TT_R4);
  TT_R3.AddChildFrame(Ta_R3);                     TT_R4.AddChildFrame(Ta_R4);
  TT_R3.AddChildFrame(Tb_R3);                     TT_R4.AddChildFrame(Tb_R4);
  Ta_R3.AddChildFrame(Ba_R3);                     Ta_R4.AddChildFrame(Ba_R4);
  Ta_R3.AddChildFrame(Wa_R3);                     Ta_R4.AddChildFrame(Wa_R4);
  Tb_R3.AddChildFrame(Bb_R3);                     Tb_R4.AddChildFrame(Bb_R4);
  Tb_R3.AddChildFrame(Wb_R3);                     Tb_R4.AddChildFrame(Wb_R4);
  Wa_R3.AddChildFrame(La_R3);                     Wa_R4.AddChildFrame(La_R4);
  Wa_R3.AddChildFrame(Na_R3);                     Wa_R4.AddChildFrame(Na_R4);
  Wb_R3.AddChildFrame(Lb_R3);                     Wb_R4.AddChildFrame(Lb_R4);
  Wb_R3.AddChildFrame(Nb_R3);                     Wb_R4.AddChildFrame(Nb_R4);

  if(LAB_R1.InitializeTree() && LAB_R2.InitializeTree() && 
     LAB_R3.InitializeTree() && LAB_R4.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  //////////////////////// define Groups for reconstruction trees ////////////////////////
  std::string group_name;

  // Invisible Group
  group_name = "#splitline{#nu #nu Jigsaws for}{min M_{top} , M_{top}^{ a} = M_{top}^{ b}}";
  InvisibleGroup INV_R1("INV_R1", group_name);
  INV_R1.AddFrame(Na_R1);
  INV_R1.AddFrame(Nb_R1);
  // Combinatoric Group for b's
  CombinatoricGroup B_R1("VIS_R1","b-jet Jigsaws");
  B_R1.AddFrame(Ba_R1);
  B_R1.AddFrame(Bb_R1);
  // b-jet frames must have at least one element
  B_R1.SetNElementsForFrame(Ba_R1, 1);
  B_R1.SetNElementsForFrame(Bb_R1, 1);

  group_name = "#splitline{#nu #nu Jigsaws for}{min M_{W}, M_{W}^{ a} = M_{W}^{ b}}";
  InvisibleGroup INV_R2("INV_R2", group_name);
  INV_R2.AddFrame(Na_R2);
  INV_R2.AddFrame(Nb_R2);
  CombinatoricGroup B_R2("VIS_R2","b-jet Jigsaws");
  B_R2.AddFrame(Ba_R2);
  B_R2.AddFrame(Bb_R2);
  B_R2.SetNElementsForFrame(Ba_R2, 1);
  B_R2.SetNElementsForFrame(Bb_R2, 1);

  group_name = "#splitline{#nu #nu Jigsaws for}{min M_{top a}^{2}+ M_{top b}^{2}}";
  InvisibleGroup INV_R3("INV_R3", group_name);
  INV_R3.AddFrame(Na_R3);
  INV_R3.AddFrame(Nb_R3);
  CombinatoricGroup B_R3("VIS_R3","b-jet Jigsaws");
  B_R3.AddFrame(Ba_R3);
  B_R3.AddFrame(Bb_R3);
  B_R3.SetNElementsForFrame(Ba_R3, 1);
  B_R3.SetNElementsForFrame(Bb_R3, 1);

  group_name = "#splitline{#nu #nu Jigsaws for}{min (M_{top a}- M_{top b})^{2}}";
  InvisibleGroup INV_R4("INV_R4", group_name);
  INV_R4.AddFrame(Na_R4);
  INV_R4.AddFrame(Nb_R4);
  CombinatoricGroup B_R4("VIS_R4","b-jet Jigsaws");
  B_R4.AddFrame(Ba_R4);
  B_R4.AddFrame(Bb_R4);
  B_R4.SetNElementsForFrame(Ba_R4, 1);
  B_R4.SetNElementsForFrame(Bb_R4, 1);

  //////////////////////// define Jigsaws for reconstruction trees ////////////////////////
  std::string jigsaw_name;

  // Minimize equal top masses neutrino jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{b#it{l}b#it{l}} , m_{b#it{l}}^{ a} , m_{b#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_R1("NuNuM_R1", jigsaw_name);
  INV_R1.AddJigsaw(NuNuM_R1);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_R1("NuNuR_R1", jigsaw_name);
  INV_R1.AddJigsaw(NuNuR_R1);
  NuNuR_R1.AddVisibleFrames(La_R1+Ba_R1+Lb_R1+Bb_R1);

  jigsaw_name = "min M_{top}, M_{top}^{ a} = M_{top}^{ b}";
  ContraBoostInvJigsaw MinMt_R1("MinMt_R1", jigsaw_name);
  INV_R1.AddJigsaw(MinMt_R1);
  MinMt_R1.AddVisibleFrames(La_R1+Ba_R1, 0);
  MinMt_R1.AddVisibleFrames(Lb_R1+Bb_R1, 1);
  MinMt_R1.AddInvisibleFrame(Na_R1, 0);
  MinMt_R1.AddInvisibleFrame(Nb_R1, 1);

  // Minimize equal W masses neutrino jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{#it{l}#it{l}} , m_{#it{l}}^{ a} , m_{#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_R2("NuNuM_R2", jigsaw_name);
  INV_R2.AddJigsaw(NuNuM_R2);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_R2("NuNuR_R2", jigsaw_name);
  INV_R2.AddJigsaw(NuNuR_R2);
  NuNuR_R2.AddVisibleFrames(La_R2+Ba_R2+Lb_R2+Bb_R2);

  jigsaw_name = "min M_{W}, M_{W}^{ a} = M_{W}^{ b}";
  ContraBoostInvJigsaw MinMW_R2("MinMW_R2", jigsaw_name);
  INV_R2.AddJigsaw(MinMW_R2);
  MinMW_R2.AddVisibleFrame(La_R2, 0);
  MinMW_R2.AddVisibleFrame(Lb_R2, 1);
  MinMW_R2.AddInvisibleFrame(Na_R2, 0);
  MinMW_R2.AddInvisibleFrame(Nb_R2, 1);

  // Minimize sum Mt^2 jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{#it{l}#it{l}} , m_{#it{l}}^{ a} , m_{#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_R3("NuNuM_R3", jigsaw_name);
  INV_R3.AddJigsaw(NuNuM_R3);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_R3("NuNuR_R3", jigsaw_name);
  INV_R3.AddJigsaw(NuNuR_R3);
  NuNuR_R3.AddVisibleFrames(LAB_R3.GetListVisibleFrames());

  jigsaw_name = "min #Sigma M_{top}^{2}";
  MinMassesSqInvJigsaw MinMt_R3("MinMt_R3", jigsaw_name, 2);
  INV_R3.AddJigsaw(MinMt_R3);
  MinMt_R3.AddInvisibleFrame(Na_R3, 0);
  MinMt_R3.AddInvisibleFrame(Nb_R3, 1);
  MinMt_R3.AddVisibleFrames(La_R3+Ba_R3, 0);
  MinMt_R3.AddVisibleFrames(Lb_R3+Bb_R3, 1);
  MinMt_R3.AddMassFrame(La_R3, 0);
  MinMt_R3.AddMassFrame(Lb_R3, 1);

  // Minimize difference Mt jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{#it{l}#it{l}} , m_{#it{l}}^{ a} , m_{#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_R4("NuNuM_R4", jigsaw_name);
  INV_R4.AddJigsaw(NuNuM_R4);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_R4("NuNuR_R4", jigsaw_name);
  INV_R4.AddJigsaw(NuNuR_R4);
  NuNuR_R4.AddVisibleFrames(LAB_R4.GetListVisibleFrames());

  jigsaw_name = "min ( M_{top a}- M_{top b} )^{2}";
  MinMassDiffInvJigsaw MinDeltaMt_R4("MinDeltaMt_R4", jigsaw_name, 2);
  INV_R4.AddJigsaw(MinDeltaMt_R4);
  MinDeltaMt_R4.AddInvisibleFrame(Na_R4, 0);
  MinDeltaMt_R4.AddInvisibleFrame(Nb_R4, 1);
  MinDeltaMt_R4.AddVisibleFrames(La_R4+Ba_R4, 0);
  MinDeltaMt_R4.AddVisibleFrames(Lb_R4+Bb_R4, 1);
  MinDeltaMt_R4.AddMassFrame(La_R4, 0);
  MinDeltaMt_R4.AddMassFrame(Lb_R4, 1);

  // b-jet combinatoric jigsaws for all trees
  jigsaw_name = "Minimize M(b #it{l} )_{a} , M(b #it{l} )_{b}";

  MinMassesCombJigsaw MinBL_R1("MinBL_R1", jigsaw_name);
  B_R1.AddJigsaw(MinBL_R1);
  MinBL_R1.AddFrames(La_R1+Ba_R1,0);
  MinBL_R1.AddFrames(Lb_R1+Bb_R1,1);

  MinMassesCombJigsaw MinBL_R2("MinBL_R2", jigsaw_name);
  B_R2.AddJigsaw(MinBL_R2);
  MinBL_R2.AddFrames(La_R2+Ba_R2,0);
  MinBL_R2.AddFrames(Lb_R2+Bb_R2,1);

  MinMassesCombJigsaw MinBL_R3("MinBL_R3", jigsaw_name);
  B_R3.AddJigsaw(MinBL_R3);
  MinBL_R3.AddFrames(La_R3+Ba_R3,0);
  MinBL_R3.AddFrames(Lb_R3+Bb_R3,1);

  MinMassesCombJigsaw MinBL_R4("MinBL_R4", jigsaw_name);
  B_R4.AddJigsaw(MinBL_R4);
  MinBL_R4.AddFrames(La_R4+Ba_R4,0);
  MinBL_R4.AddFrames(Lb_R4+Bb_R4,1);

  if(LAB_R1.InitializeAnalysis() && LAB_R2.InitializeAnalysis() && 
     LAB_R3.InitializeAnalysis() && LAB_R4.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);
  
  treePlot->SetTree(LAB_R1);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(B_R1);
  treePlot->Draw("VisTree", "b-jet Jigsaws", true);

  treePlot->SetTree(INV_R1);
  treePlot->Draw("InvTree_R1", "Inivisibl Jigsaws", true);

  treePlot->SetTree(INV_R2);
  treePlot->Draw("InvTree_R2", "Inivisibl Jigsaws", true);

  treePlot->SetTree(INV_R3);
  treePlot->Draw("InvTree_R3", "Inivisibl Jigsaws", true);

  treePlot->SetTree(INV_R4);
  treePlot->Draw("InvTree_R4", "Inivisibl Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot = new HistPlot("HistPlot", "t #bar{t} #rightarrow b W(#it{l} #nu) b W(#it{l} #nu)");

  const HistPlotCategory& cat_Gen = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_R1  = histPlot->GetNewCategory("Reco1", "M_{top}^{ a} = M_{top}^{ b} Reco");
  const HistPlotCategory& cat_R2  = histPlot->GetNewCategory("Reco2", "M_{W}^{ a} = M_{W}^{ b} Reco");
  const HistPlotCategory& cat_R3  = histPlot->GetNewCategory("Reco3", "min #Sigma M_{top}^{ 2} Reco");
  const HistPlotCategory& cat_R4  = histPlot->GetNewCategory("Reco4", "min #Delta M_{top}");

  const HistPlotVar& Mtt    = histPlot->GetNewVar("Mtt", "M_{t #bar{t}} / m_{t #bar{t}}", 0., 2.);
  const HistPlotVar& Pt_tt  = histPlot->GetNewVar("Pt_tt", "p_{t}^{t #bar{t}} / p_{t}^{t #bar{t} gen}", 0., 2.);
  const HistPlotVar& Mta    = histPlot->GetNewVar("Mta", "M_{top}^{ a}", 0., mT*2., "[GeV]");
  const HistPlotVar& Mtb    = histPlot->GetNewVar("Mtb", "M_{top}^{ b}", 0., mT*2., "[GeV]");
  const HistPlotVar& MWa    = histPlot->GetNewVar("MWa", "M_{W}^{ a}", 0., mW*2.5, "[GeV]");
  const HistPlotVar& MWb    = histPlot->GetNewVar("MWb", "M_{W}^{ b}", 0., mW*2.5, "[GeV]");
  const HistPlotVar& Eb_ta  = histPlot->GetNewVar("Eb_ta", "E_{b a}^{top a} / E_{b a}^{top a gen}", 0., 2.);
  const HistPlotVar& Eb_tb  = histPlot->GetNewVar("Eb_tb", "E_{b b}^{top b} / E_{b b}^{top b gen}", 0., 2.);
  const HistPlotVar& El_Wa  = histPlot->GetNewVar("El_Wa", "E_{#it{l} a}^{W a} / E_{#it{l} a}^{W a gen}", 0., 2.);
  const HistPlotVar& El_Wb  = histPlot->GetNewVar("El_Wb", "E_{#it{l} b}^{W b} / E_{#it{l} b}^{W b gen}", 0., 2.);
  const HistPlotVar& costt  = histPlot->GetNewVar("costt","cos #theta_{t #bar{t}}", -1., 1.);
  const HistPlotVar& costa  = histPlot->GetNewVar("costa","cos #theta_{top a}", -1., 1.);
  const HistPlotVar& costb  = histPlot->GetNewVar("costb","cos #theta_{top b}", -1., 1.);
  const HistPlotVar& cosWa  = histPlot->GetNewVar("cosWa","cos #theta_{W a}", -1., 1.);
  const HistPlotVar& cosWb  = histPlot->GetNewVar("cosWb","cos #theta_{W b}", -1., 1.);
  const HistPlotVar& Dcostt = histPlot->GetNewVar("Dcostt","#theta_{t #bar{t}} - #theta_{t #bar{t}}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcosta = histPlot->GetNewVar("Dcosta","#theta_{top a} - #theta_{top a}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcostb = histPlot->GetNewVar("Dcostb","#theta_{top b} - #theta_{top b}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWa = histPlot->GetNewVar("DcosWa","#theta_{W a} - #theta_{W a}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWb = histPlot->GetNewVar("DcosWb","#theta_{W b} - #theta_{W b}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);

  histPlot->AddPlot(Mtt, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Pt_tt, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Mta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(MWa, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Eb_ta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(El_Wa, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(costt, cat_R1+cat_R2+cat_R3+cat_Gen+cat_R4);
  histPlot->AddPlot(costa, cat_R1+cat_R2+cat_R3+cat_Gen+cat_R4);
  histPlot->AddPlot(cosWa, cat_R1+cat_R2+cat_R3+cat_Gen+cat_R4);
  histPlot->AddPlot(Dcostt, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcosta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(DcosWa, cat_R1+cat_R2+cat_R3+cat_R4);

  histPlot->AddPlot(Mtt, Eb_ta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Mtt, El_Wa, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Eb_ta, Eb_tb, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(El_Wa, El_Wb, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Eb_ta, El_Wa, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Eb_ta, El_Wb, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcostt, Mtt, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcosta, Eb_ta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(DcosWa, El_Wa, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcostt, Dcosta, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcosta, Dcostb, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(DcosWa, DcosWb, cat_R1+cat_R2+cat_R3+cat_R4);
  histPlot->AddPlot(Dcosta, DcosWa, cat_R1+cat_R2+cat_R3+cat_R4);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                             // clear the gen tree
    double PTtt = 1.*mT*gRandom->Rndm();
    LAB_Gen.SetTransverseMomentum(PTtt);               // give the di-tops some Pt
    LAB_Gen.AnalyzeEvent();                           // generate a new event

    // analyze event three different ways
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    LAB_R1.ClearEvent();   
    LAB_R2.ClearEvent(); 
    LAB_R3.ClearEvent(); 
    LAB_R4.ClearEvent(); 
             
    INV_R1.SetLabFrameThreeVector(MET); 
    INV_R2.SetLabFrameThreeVector(MET); 
    INV_R3.SetLabFrameThreeVector(MET);
    INV_R4.SetLabFrameThreeVector(MET);

    // INV_R1.SetMass(2.*GetP((La_Gen+Lb_Gen).GetMass(),    
    // 			   La_Gen.GetMass(),Lb_Gen.GetMass())); 

    La_R1.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_R1.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    La_R2.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_R2.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    La_R3.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_R3.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    La_R4.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_R4.SetLabFrameFourVector(Lb_Gen.GetFourVector());

    std::vector<RFKey> B_R1_ID; // ID for tracking jets in tree
    B_R1_ID.push_back(B_R1.AddLabFrameFourVector(Ba_Gen.GetFourVector()));
    B_R1_ID.push_back(B_R1.AddLabFrameFourVector(Bb_Gen.GetFourVector()));
    B_R2.AddLabFrameFourVector(Ba_Gen.GetFourVector());
    B_R2.AddLabFrameFourVector(Bb_Gen.GetFourVector());
    B_R3.AddLabFrameFourVector(Ba_Gen.GetFourVector());
    B_R3.AddLabFrameFourVector(Bb_Gen.GetFourVector());
    B_R4.AddLabFrameFourVector(Ba_Gen.GetFourVector());
    B_R4.AddLabFrameFourVector(Bb_Gen.GetFourVector());
   
    LAB_R1.AnalyzeEvent(); // analyze the event
    LAB_R2.AnalyzeEvent();     
    LAB_R3.AnalyzeEvent(); 
    LAB_R4.AnalyzeEvent(); 

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    double Mttgen   = TT_Gen.GetMass();
    double Pt_ttgen = Ta_Gen.GetFourVector(TT_Gen).P();
    double Eb_tagen = Ba_Gen.GetFourVector(Ta_Gen).E();
    double Eb_tbgen = Bb_Gen.GetFourVector(Tb_Gen).E();
    double El_Wagen = La_Gen.GetFourVector(Wa_Gen).E();
    double El_Wbgen = Lb_Gen.GetFourVector(Wb_Gen).E();
    double costtgen = TT_Gen.GetCosDecayAngle();
    double costagen = Ta_Gen.GetCosDecayAngle();
    double costbgen = Tb_Gen.GetCosDecayAngle();
    double cosWagen = Wa_Gen.GetCosDecayAngle();
    double cosWbgen = Wb_Gen.GetCosDecayAngle();

    Mtt = TT_R1.GetMass()/Mttgen;
    Pt_tt = Ta_R1.GetFourVector(TT_R1).P()/Pt_ttgen;
    Mta = Ta_R1.GetMass();
    Mtb = Tb_R1.GetMass();
    MWa = Wa_R1.GetMass();
    MWb = Wb_R1.GetMass();
    Eb_ta = Ba_R1.GetFourVector(Ta_R1).E()/Eb_tagen;
    Eb_tb = Bb_R1.GetFourVector(Tb_R1).E()/Eb_tbgen;
    El_Wa = La_R1.GetFourVector(Wa_R1).E()/El_Wagen;
    El_Wb = Lb_R1.GetFourVector(Wb_R1).E()/El_Wbgen;
    costt = TT_R1.GetCosDecayAngle();
    costa = Ta_R1.GetCosDecayAngle();
    costb = Tb_R1.GetCosDecayAngle();
    cosWa = Wa_R1.GetCosDecayAngle();
    cosWb = Wb_R1.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_R1);

    Mtt = TT_R2.GetMass()/Mttgen;
    Pt_tt = Ta_R2.GetFourVector(TT_R2).P()/Pt_ttgen;
    Mta = Ta_R2.GetMass();
    Mtb = Tb_R2.GetMass();
    MWa = Wa_R2.GetMass();
    MWb = Wb_R2.GetMass();
    Eb_ta = Ba_R2.GetFourVector(Ta_R2).E()/Eb_tagen;
    Eb_tb = Bb_R2.GetFourVector(Tb_R2).E()/Eb_tbgen;
    El_Wa = La_R2.GetFourVector(Wa_R2).E()/El_Wagen;
    El_Wb = Lb_R2.GetFourVector(Wb_R2).E()/El_Wbgen;
    costt = TT_R2.GetCosDecayAngle();
    costa = Ta_R2.GetCosDecayAngle();
    costb = Tb_R2.GetCosDecayAngle();
    cosWa = Wa_R2.GetCosDecayAngle();
    cosWb = Wb_R2.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_R2);

    Mtt = TT_R3.GetMass()/Mttgen;
    Pt_tt = Ta_R3.GetFourVector(TT_R3).P()/Pt_ttgen;
    Mta = Ta_R3.GetMass();
    Mtb = Tb_R3.GetMass();
    MWa = Wa_R3.GetMass();
    MWb = Wb_R3.GetMass();
    Eb_ta = Ba_R3.GetFourVector(Ta_R3).E()/Eb_tagen;
    Eb_tb = Bb_R3.GetFourVector(Tb_R3).E()/Eb_tbgen;
    El_Wa = La_R3.GetFourVector(Wa_R3).E()/El_Wagen;
    El_Wb = Lb_R3.GetFourVector(Wb_R3).E()/El_Wbgen;
    costt = TT_R3.GetCosDecayAngle();
    costa = Ta_R3.GetCosDecayAngle();
    costb = Tb_R3.GetCosDecayAngle();
    cosWa = Wa_R3.GetCosDecayAngle();
    cosWb = Wb_R3.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_R3);

    Mtt = TT_R4.GetMass()/Mttgen;
    Pt_tt = Ta_R4.GetFourVector(TT_R4).P()/Pt_ttgen;
    Mta = Ta_R4.GetMass();
    Mtb = Tb_R4.GetMass();
    MWa = Wa_R4.GetMass();
    MWb = Wb_R4.GetMass();
    Eb_ta = Ba_R4.GetFourVector(Ta_R4).E()/Eb_tagen;
    Eb_tb = Bb_R4.GetFourVector(Tb_R4).E()/Eb_tbgen;
    El_Wa = La_R4.GetFourVector(Wa_R4).E()/El_Wagen;
    El_Wb = Lb_R4.GetFourVector(Wb_R4).E()/El_Wbgen;
    costt = TT_R4.GetCosDecayAngle();
    costa = Ta_R4.GetCosDecayAngle();
    costb = Tb_R4.GetCosDecayAngle();
    cosWa = Wa_R4.GetCosDecayAngle();
    cosWb = Wb_R4.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_R4);

  }

  //treePlot->WriteOutput(output_name);

  histPlot->Draw();

  LAB_Gen.PrintGeneratorEfficiency();

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_05();
  return 0;
}
#endif
