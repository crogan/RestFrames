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
  int Ngen = 10000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame TT_Gen("TT_Gen","t #bar{t}");
  ResonanceGenFrame Ta_Gen("Ta_Gen","t_{a}");
  ResonanceGenFrame Tb_Gen("Tb_Gen","t_{b}");
  DecayGenFrame Wa_Gen("Wa_Gen","W_{a}");
  DecayGenFrame Wb_Gen("Wb_Gen","W_{b}");
  VisibleGenFrame Ba_Gen("Ba_Gen","b_{a}");
  VisibleGenFrame La_Gen("La_Gen","#it{l}_{a}");
  InvisibleGenFrame Na_Gen("Na_Gen","#nu_{a}");
  VisibleGenFrame Bb_Gen("Bb_Gen","b_{b}");
  VisibleGenFrame Lb_Gen("Lb_Gen","#it{l}_{b}");
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
  Ta_Gen.SetWidth(1.);
  Tb_Gen.SetWidth(1.);
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

  La_Gen.SetPtCut(20.);
  Lb_Gen.SetPtCut(20.);
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
  LabRecoFrame       LAB_Mt("LAB_Mt","LAB");      LabRecoFrame       LAB_MW("LAB_MW","LAB");
  DecayRecoFrame     TT_Mt("TT_Mt","t #bar{t}");  DecayRecoFrame     TT_MW("TT_MW","t #bar{t}");
  DecayRecoFrame     Ta_Mt("Ta_Mt","t_{a}");      DecayRecoFrame     Ta_MW("Ta_MW","t_{a}");
  DecayRecoFrame     Tb_Mt("Tb_Mt","t_{b}");      DecayRecoFrame     Tb_MW("Tb_MW","t_{b}");
  DecayRecoFrame     Wa_Mt("Wa_Mt","W_{a}");      DecayRecoFrame     Wa_MW("Wa_MW","W_{a}");
  DecayRecoFrame     Wb_Mt("Wb_Mt","W_{b}");      DecayRecoFrame     Wb_MW("Wb_MW","W_{b}");
  VisibleRecoFrame   Ba_Mt("Ba_Mt","b_{a}");      VisibleRecoFrame   Ba_MW("Ba_MW","b_{a}");
  VisibleRecoFrame   La_Mt("La_Mt","#it{l}_{a}"); VisibleRecoFrame   La_MW("La_MW","#it{l}_{a}");
  InvisibleRecoFrame Na_Mt("Na_Mt","#nu_{a}");    InvisibleRecoFrame Na_MW("Na_MW","#nu_{a}");
  VisibleRecoFrame   Bb_Mt("Bb_Mt","b_{b}");      VisibleRecoFrame   Bb_MW("Bb_MW","b_{b}");
  VisibleRecoFrame   Lb_Mt("Lb_Mt","#it{l}_{b}"); VisibleRecoFrame   Lb_MW("Lb_MW","#it{l}_{b}");
  InvisibleRecoFrame Nb_Mt("Nb_Mt","#nu_{b}");    InvisibleRecoFrame Nb_MW("Nb_MW","#nu_{b}");

  LabRecoFrame       LAB_minMt2("LAB_minMt2","LAB");
  DecayRecoFrame     TT_minMt2("TT_minMt2","t #bar{t}");
  DecayRecoFrame     Ta_minMt2("Ta_minMt2","t_{a}");
  DecayRecoFrame     Tb_minMt2("Tb_minMt2","t_{b}");
  DecayRecoFrame     Wa_minMt2("Wa_minMt2","W_{a}");
  DecayRecoFrame     Wb_minMt2("Wb_minMt2","W_{b}");
  VisibleRecoFrame   Ba_minMt2("Ba_minMt2","b_{a}");
  VisibleRecoFrame   La_minMt2("La_minMt2","#it{l}_{a}");
  InvisibleRecoFrame Na_minMt2("Na_minMt2","#nu_{a}");
  VisibleRecoFrame   Bb_minMt2("Bb_minMt2","b_{b}");
  VisibleRecoFrame   Lb_minMt2("Lb_minMt2","#it{l}_{b}");
  InvisibleRecoFrame Nb_minMt2("Nb_minMt2","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Mt.SetChildFrame(TT_Mt);                    LAB_MW.SetChildFrame(TT_MW);
  TT_Mt.AddChildFrame(Ta_Mt);                     TT_MW.AddChildFrame(Ta_MW);
  TT_Mt.AddChildFrame(Tb_Mt);                     TT_MW.AddChildFrame(Tb_MW);
  Ta_Mt.AddChildFrame(Ba_Mt);                     Ta_MW.AddChildFrame(Ba_MW);
  Ta_Mt.AddChildFrame(Wa_Mt);                     Ta_MW.AddChildFrame(Wa_MW);
  Tb_Mt.AddChildFrame(Bb_Mt);                     Tb_MW.AddChildFrame(Bb_MW);
  Tb_Mt.AddChildFrame(Wb_Mt);                     Tb_MW.AddChildFrame(Wb_MW);
  Wa_Mt.AddChildFrame(La_Mt);                     Wa_MW.AddChildFrame(La_MW);
  Wa_Mt.AddChildFrame(Na_Mt);                     Wa_MW.AddChildFrame(Na_MW);
  Wb_Mt.AddChildFrame(Lb_Mt);                     Wb_MW.AddChildFrame(Lb_MW);
  Wb_Mt.AddChildFrame(Nb_Mt);                     Wb_MW.AddChildFrame(Nb_MW);

  LAB_minMt2.SetChildFrame(TT_minMt2);
  TT_minMt2.AddChildFrame(Ta_minMt2);
  TT_minMt2.AddChildFrame(Tb_minMt2);
  Ta_minMt2.AddChildFrame(Ba_minMt2);
  Ta_minMt2.AddChildFrame(Wa_minMt2);
  Tb_minMt2.AddChildFrame(Bb_minMt2);
  Tb_minMt2.AddChildFrame(Wb_minMt2);
  Wa_minMt2.AddChildFrame(La_minMt2);
  Wa_minMt2.AddChildFrame(Na_minMt2);
  Wb_minMt2.AddChildFrame(Lb_minMt2);
  Wb_minMt2.AddChildFrame(Nb_minMt2);

  if(LAB_Mt.InitializeTree() && LAB_MW.InitializeTree() && LAB_minMt2.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  //////////////////////// define Groups for reconstruction trees ////////////////////////
  std::string group_name;

  // Invisible Group
  group_name = "#splitline{#nu #nu Jigsaws for}{min M_{top} , M_{top}^{ a} = M_{top}^{ b}}";
  InvisibleGroup INV_Mt("INV_Mt", group_name);
  INV_Mt.AddFrame(Na_Mt);
  INV_Mt.AddFrame(Nb_Mt);
  // Combinatoric Group for b's
  CombinatoricGroup B_Mt("VIS_Mt","b-jet Jigsaws");
  B_Mt.AddFrame(Ba_Mt);
  B_Mt.AddFrame(Bb_Mt);
  // b-jet frames must have at least one element
  B_Mt.SetNElementsForFrame(Ba_Mt,1,true);
  B_Mt.SetNElementsForFrame(Bb_Mt,1,true);

  group_name = "#splitline{#nu #nu Jigsaws for}{min M_{W}, M_{W}^{ a} = M_{W}^{ b}}";
  InvisibleGroup INV_MW("INV_MW", group_name);
  INV_MW.AddFrame(Na_MW);
  INV_MW.AddFrame(Nb_MW);
  CombinatoricGroup B_MW("VIS_MW","b-jet Jigsaws");
  B_MW.AddFrame(Ba_MW);
  B_MW.AddFrame(Bb_MW);
  B_MW.SetNElementsForFrame(Ba_MW,1,true);
  B_MW.SetNElementsForFrame(Bb_MW,1,true);

  group_name = "#splitline{#nu #nu Jigsaws for}{min #Sigma M_{top}^{2}}";
  InvisibleGroup INV_minMt2("INV_minMt2", group_name);
  INV_minMt2.AddFrame(Na_minMt2);
  INV_minMt2.AddFrame(Nb_minMt2);
  CombinatoricGroup B_minMt2("VIS_minMt2","b-jet Jigsaws");
  B_minMt2.AddFrame(Ba_minMt2);
  B_minMt2.AddFrame(Bb_minMt2);
  B_minMt2.SetNElementsForFrame(Ba_minMt2,1,true);
  B_minMt2.SetNElementsForFrame(Bb_minMt2,1,true);

  //////////////////////// define Jigsaws for reconstruction trees ////////////////////////
  std::string jigsaw_name;

  // Minimize equal top masses neutrino jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{b#it{l}b#it{l}} , m_{b#it{l}}^{ a} , m_{b#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_Mt("NuNuM_Mt", jigsaw_name);
  INV_Mt.AddJigsaw(NuNuM_Mt);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_Mt("NuNuR_Mt", jigsaw_name);
  INV_Mt.AddJigsaw(NuNuR_Mt);
  NuNuR_Mt.AddVisibleFrames(La_Mt+Ba_Mt+Lb_Mt+Bb_Mt);

  jigsaw_name = "min M_{top}, M_{top}^{ a} = M_{top}^{ b}";
  ContraBoostInvJigsaw MinMt_Mt("MinMt_Mt", jigsaw_name);
  INV_Mt.AddJigsaw(MinMt_Mt);
  MinMt_Mt.AddVisibleFrames(La_Mt+Ba_Mt, 0);
  MinMt_Mt.AddVisibleFrames(Lb_Mt+Bb_Mt, 1);
  MinMt_Mt.AddInvisibleFrame(Na_Mt, 0);
  MinMt_Mt.AddInvisibleFrame(Nb_Mt, 1);

  // Minimize equal W masses neutrino jigsaws
  jigsaw_name = "M_{#nu#nu} = f(m_{#it{l}#it{l}} , m_{#it{l}}^{ a} , m_{#it{l}}^{ b})";
  SetMassInvJigsaw NuNuM_MW("NuNuM_MW", jigsaw_name);
  INV_MW.AddJigsaw(NuNuM_MW);

  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_MW("NuNuR_MW", jigsaw_name);
  INV_MW.AddJigsaw(NuNuR_MW);
  NuNuR_MW.AddVisibleFrames(La_MW+Ba_MW+Lb_MW+Bb_MW);

  jigsaw_name = "min M_{W}, M_{W}^{ a} = M_{W}^{ b}";
  ContraBoostInvJigsaw MinMW_MW("MinMW_MW", jigsaw_name);
  INV_MW.AddJigsaw(MinMW_MW);
  MinMW_MW.AddVisibleFrame(La_MW, 0);
  MinMW_MW.AddVisibleFrame(Lb_MW, 1);
  MinMW_MW.AddInvisibleFrame(Na_MW, 0);
  MinMW_MW.AddInvisibleFrame(Nb_MW, 1);

  // Minimize sum Mt^2 jigsaws
  jigsaw_name = "#eta_{#nu#nu} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR_minMt2("NuNuR_minMt2", jigsaw_name);
  INV_minMt2.AddJigsaw(NuNuR_minMt2);
  NuNuR_minMt2.AddVisibleFrames(LAB_minMt2.GetListVisibleFrames());

  jigsaw_name = "min #Sigma M_{top}^{2}";
  MinMassesSqInvJigsaw MinMt_minMt2("MinM_minMt2", jigsaw_name, 2);
  INV_minMt2.AddJigsaw(MinMt_minMt2);
  MinMt_minMt2.AddVisibleFrames(La_minMt2+Ba_minMt2, 0);
  MinMt_minMt2.AddVisibleFrames(Lb_minMt2+Bb_minMt2, 1);
  MinMt_minMt2.AddInvisibleFrame(Na_minMt2, 0);
  MinMt_minMt2.AddInvisibleFrame(Nb_minMt2, 1);

  // b-jet combinatoric jigsaws for all trees
  jigsaw_name = "Minimize M(b #it{l} )_{a} , M(b #it{l} )_{b}";

  MinMassesCombJigsaw MinBL_Mt("MinBL_Mt", jigsaw_name);
  B_Mt.AddJigsaw(MinBL_Mt);
  MinBL_Mt.AddFrames(La_Mt+Ba_Mt,0);
  MinBL_Mt.AddFrames(Lb_Mt+Bb_Mt,1);

  MinMassesCombJigsaw MinBL_MW("MinBL_MW", jigsaw_name);
  B_MW.AddJigsaw(MinBL_MW);
  MinBL_MW.AddFrames(La_MW+Ba_MW,0);
  MinBL_MW.AddFrames(Lb_MW+Bb_MW,1);

  MinMassesCombJigsaw MinBL_minMt2("MinBL_minMt2", jigsaw_name);
  B_minMt2.AddJigsaw(MinBL_minMt2);
  MinBL_minMt2.AddFrames(La_minMt2+Ba_minMt2,0);
  MinBL_minMt2.AddFrames(Lb_minMt2+Bb_minMt2,1);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis() && 
     LAB_minMt2.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);
  
  treePlot->SetTree(LAB_Mt);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(B_Mt);
  treePlot->Draw("VisTree", "b-jet Jigsaws", true);

  treePlot->SetTree(INV_Mt);
  treePlot->Draw("InvTree_Mt", "Inivisibl Jigsaws", true);

  treePlot->SetTree(INV_MW);
  treePlot->Draw("InvTree_MW", "Inivisibl Jigsaws", true);

  treePlot->SetTree(INV_minMt2);
  treePlot->Draw("InvTree_minMt2", "Inivisibl Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot = new HistPlot("HistPlot", "t #bar{t} #rightarrow b W(#it{l} #nu) b W(#it{l} #nu)");

  const HistPlotCategory& cat_Gen    = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_Mt     = histPlot->GetNewCategory("Reco", "M_{top}^{ a} = M_{top}^{ b} Reco");
  const HistPlotCategory& cat_MW     = histPlot->GetNewCategory("Reco", "M_{W}^{ a} = M_{W}^{ b} Reco");
  const HistPlotCategory& cat_minMt2 = histPlot->GetNewCategory("Reco", "min #Sigma M_{top}^{ 2} Reco");

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

  histPlot->AddPlot(Mtt, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Pt_tt, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Mta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(MWa, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Eb_ta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(El_Wa, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(costt, cat_Mt+cat_MW+cat_minMt2+cat_Gen);
  histPlot->AddPlot(costa, cat_Mt+cat_MW+cat_minMt2+cat_Gen);
  histPlot->AddPlot(cosWa, cat_Mt+cat_MW+cat_minMt2+cat_Gen);
  histPlot->AddPlot(Dcostt, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcosta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(DcosWa, cat_Mt+cat_MW+cat_minMt2);

  histPlot->AddPlot(Mtt, Eb_ta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Mtt, El_Wa, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Eb_ta, Eb_tb, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(El_Wa, El_Wb, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Eb_ta, El_Wa, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Eb_ta, El_Wb, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcostt, Mtt, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcosta, Eb_ta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(DcosWa, El_Wa, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcostt, Dcosta, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcosta, Dcostb, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(DcosWa, DcosWb, cat_Mt+cat_MW+cat_minMt2);
  histPlot->AddPlot(Dcosta, DcosWa, cat_Mt+cat_MW+cat_minMt2);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                             // clear the gen tree
    double PTtt = 1.*mT*gRandom->Rndm();
    LAB_Gen.SetTransverseMomenta(PTtt);               // give the di-tops some Pt
    LAB_Gen.AnalyzeEvent();                           // generate a new event

    // analyze event three different ways
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    LAB_Mt.ClearEvent();   
    LAB_MW.ClearEvent(); 
    LAB_minMt2.ClearEvent(); 
             
    INV_Mt.SetLabFrameThreeVector(MET); 
    INV_MW.SetLabFrameThreeVector(MET); 
    INV_minMt2.SetLabFrameThreeVector(MET);

    INV_minMt2.SetMass(2.*GetP((La_Gen+Lb_Gen).GetMass(), // set invisible system mass by hand
			       La_Gen.GetMass(),Lb_Gen.GetMass()));                  

    La_Mt.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_Mt.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    La_MW.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_MW.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    La_minMt2.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_minMt2.SetLabFrameFourVector(Lb_Gen.GetFourVector());

    std::vector<RFKey> B_Mt_ID; // ID for tracking jets in tree
    B_Mt_ID.push_back(B_Mt.AddLabFrameFourVector(Ba_Gen.GetFourVector()));
    B_Mt_ID.push_back(B_Mt.AddLabFrameFourVector(Bb_Gen.GetFourVector()));
    B_MW.AddLabFrameFourVector(Ba_Gen.GetFourVector());
    B_MW.AddLabFrameFourVector(Bb_Gen.GetFourVector());
    B_minMt2.AddLabFrameFourVector(Ba_Gen.GetFourVector());
    B_minMt2.AddLabFrameFourVector(Bb_Gen.GetFourVector());
    
    LAB_Mt.AnalyzeEvent(); // analyze the event
    LAB_MW.AnalyzeEvent();     
    LAB_minMt2.AnalyzeEvent(); 

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

    Mtt = TT_Mt.GetMass()/Mttgen;
    Pt_tt = Ta_Mt.GetFourVector(TT_Mt).P()/Pt_ttgen;
    Mta = Ta_Mt.GetMass();
    Mtb = Tb_Mt.GetMass();
    MWa = Wa_Mt.GetMass();
    MWb = Wb_Mt.GetMass();
    Eb_ta = Ba_Mt.GetFourVector(Ta_Mt).E()/Eb_tagen;
    Eb_tb = Bb_Mt.GetFourVector(Tb_Mt).E()/Eb_tbgen;
    El_Wa = La_Mt.GetFourVector(Wa_Mt).E()/El_Wagen;
    El_Wb = Lb_Mt.GetFourVector(Wb_Mt).E()/El_Wbgen;
    costt = TT_Mt.GetCosDecayAngle();
    costa = Ta_Mt.GetCosDecayAngle();
    costb = Tb_Mt.GetCosDecayAngle();
    cosWa = Wa_Mt.GetCosDecayAngle();
    cosWb = Wb_Mt.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_Mt);

    Mtt = TT_MW.GetMass()/Mttgen;
    Pt_tt = Ta_MW.GetFourVector(TT_MW).P()/Pt_ttgen;
    Mta = Ta_MW.GetMass();
    Mtb = Tb_MW.GetMass();
    MWa = Wa_MW.GetMass();
    MWb = Wb_MW.GetMass();
    Eb_ta = Ba_MW.GetFourVector(Ta_MW).E()/Eb_tagen;
    Eb_tb = Bb_MW.GetFourVector(Tb_MW).E()/Eb_tbgen;
    El_Wa = La_MW.GetFourVector(Wa_MW).E()/El_Wagen;
    El_Wb = Lb_MW.GetFourVector(Wb_MW).E()/El_Wbgen;
    costt = TT_MW.GetCosDecayAngle();
    costa = Ta_MW.GetCosDecayAngle();
    costb = Tb_MW.GetCosDecayAngle();
    cosWa = Wa_MW.GetCosDecayAngle();
    cosWb = Wb_MW.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_MW);

    Mtt = TT_minMt2.GetMass()/Mttgen;
    Pt_tt = Ta_minMt2.GetFourVector(TT_minMt2).P()/Pt_ttgen;
    Mta = Ta_minMt2.GetMass();
    Mtb = Tb_minMt2.GetMass();
    MWa = Wa_minMt2.GetMass();
    MWb = Wb_minMt2.GetMass();
    Eb_ta = Ba_minMt2.GetFourVector(Ta_minMt2).E()/Eb_tagen;
    Eb_tb = Bb_minMt2.GetFourVector(Tb_minMt2).E()/Eb_tbgen;
    El_Wa = La_minMt2.GetFourVector(Wa_minMt2).E()/El_Wagen;
    El_Wb = Lb_minMt2.GetFourVector(Wb_minMt2).E()/El_Wbgen;
    costt = TT_minMt2.GetCosDecayAngle();
    costa = Ta_minMt2.GetCosDecayAngle();
    costb = Tb_minMt2.GetCosDecayAngle();
    cosWa = Wa_minMt2.GetCosDecayAngle();
    cosWb = Wb_minMt2.GetCosDecayAngle();
    Dcostt = asin(sqrt(1.-costt*costt)*costtgen-sqrt(1.-costtgen*costtgen)*costt);
    Dcosta = asin(sqrt(1.-costa*costa)*costagen-sqrt(1.-costagen*costagen)*costa);
    Dcostb = asin(sqrt(1.-costb*costb)*costbgen-sqrt(1.-costbgen*costbgen)*costb);
    DcosWa = asin(sqrt(1.-cosWa*cosWa)*cosWagen-sqrt(1.-cosWagen*cosWagen)*cosWa);
    DcosWb = asin(sqrt(1.-cosWb*cosWb)*cosWbgen-sqrt(1.-cosWbgen*cosWbgen)*cosWb);

    histPlot->Fill(cat_minMt2);

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
