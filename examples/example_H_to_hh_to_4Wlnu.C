/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_H_to_hh_to_4Wlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 June
///
///  \brief An example generating and analyzing Higgs to 4W to 4l 4nu
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

void example_H_to_hh_to_4Wlnu(const std::string& output_name = "output_example_04.root"){
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);
  SetLogMaxWidth(120);

  double mH = 1000.;
  double mh = 125.;
  double mW = 80.;
  double wW = 2.;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 100000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_G("LAB_G","LAB");
  DecayGenFrame     H_G("H_G","H");
  DecayGenFrame     ha_G("ha_G","h_{a}");
  DecayGenFrame     hb_G("hb_G","h_{b}");
  ResonanceGenFrame Waa_G("Waa_G","W_{a,a}");
  ResonanceGenFrame Wab_G("Wab_G","W_{a,b}");
  ResonanceGenFrame Wba_G("Wba_G","W_{b,a}");
  ResonanceGenFrame Wbb_G("Wbb_G","W_{b,b}");
  VisibleGenFrame   Laa_G("Laa_G","#it{l}_{a,a}");
  InvisibleGenFrame Naa_G("Naa_G","#nu_{a,a}");
  VisibleGenFrame   Lab_G("Lab_G","#it{l}_{a,b}");
  InvisibleGenFrame Nab_G("Nab_G","#nu_{a,b}");
  VisibleGenFrame   Lba_G("Lba_G","#it{l}_{b,a}");
  InvisibleGenFrame Nba_G("Nba_G","#nu_{b,a}");
  VisibleGenFrame   Lbb_G("Lbb_G","#it{l}_{b,b}");
  InvisibleGenFrame Nbb_G("Nbb_G","#nu_{b,b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_G.SetChildFrame(H_G);
  H_G.AddChildFrame(ha_G);
  H_G.AddChildFrame(hb_G);
  ha_G.AddChildFrame(Waa_G);
  ha_G.AddChildFrame(Wab_G);
  hb_G.AddChildFrame(Wba_G);
  hb_G.AddChildFrame(Wbb_G);
  Waa_G.AddChildFrame(Laa_G);
  Waa_G.AddChildFrame(Naa_G);
  Wab_G.AddChildFrame(Lab_G);
  Wab_G.AddChildFrame(Nab_G);
  Wba_G.AddChildFrame(Lba_G);
  Wba_G.AddChildFrame(Nba_G);
  Wbb_G.AddChildFrame(Lbb_G);
  Wbb_G.AddChildFrame(Nbb_G);

  if(LAB_G.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;								    
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // set Higgs masses
  H_G.SetMass(mH);
  ha_G.SetMass(mh);
  hb_G.SetMass(mh);
  // set W masses and widths
  Waa_G.SetMass(mW);
  Wab_G.SetMass(mW);
  Wba_G.SetMass(mW);
  Wbb_G.SetMass(mW);
  Waa_G.SetWidth(wW);
  Wab_G.SetWidth(wW);
  Wba_G.SetWidth(wW);
  Wbb_G.SetWidth(wW);
  // set lepton masses
  Laa_G.SetMass(mL);
  Lab_G.SetMass(mL);
  Lba_G.SetMass(mL);
  Lbb_G.SetMass(mL);
  Laa_G.SetPtCut(8.);
  Lab_G.SetPtCut(8.);
  Lba_G.SetPtCut(8.);
  Lbb_G.SetPtCut(8.);
  Laa_G.SetEtaCut(2.5);
  Lab_G.SetEtaCut(2.5);
  Lba_G.SetEtaCut(2.5);
  Lbb_G.SetEtaCut(2.5);

  if(LAB_G.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////

  LabRecoFrame       LAB_R("LAB_R","LAB");
  DecayRecoFrame     H_R("H_R","H");
  DecayRecoFrame     ha_R("ha_R","h_{a}");
  DecayRecoFrame     hb_R("hb_R","h_{b}");
  DecayRecoFrame     Waa_R("Waa_R","W_{a,a}");
  DecayRecoFrame     Wab_R("Wab_R","W_{a,b}");
  DecayRecoFrame     Wba_R("Wba_R","W_{b,a}");
  DecayRecoFrame     Wbb_R("Wbb_R","W_{b,b}");
  VisibleRecoFrame   Laa_R("Laa_R","#it{l}_{a,a}");
  InvisibleRecoFrame Naa_R("Naa_R","#nu_{a,a}");
  VisibleRecoFrame   Lab_R("Lab_R","#it{l}_{a,b}");
  InvisibleRecoFrame Nab_R("Nab_R","#nu_{a,b}");
  VisibleRecoFrame   Lba_R("Lba_R","#it{l}_{b,a}");
  InvisibleRecoFrame Nba_R("Nba_R","#nu_{b,a}");
  VisibleRecoFrame   Lbb_R("Lbb_R","#it{l}_{b,b}");
  InvisibleRecoFrame Nbb_R("Nbb_R","#nu_{b,b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_R.SetChildFrame(H_R);
  H_R.AddChildFrame(ha_R);
  H_R.AddChildFrame(hb_R);
  ha_R.AddChildFrame(Waa_R);
  ha_R.AddChildFrame(Wab_R);
  hb_R.AddChildFrame(Wba_R);
  hb_R.AddChildFrame(Wbb_R);
  Waa_R.AddChildFrame(Laa_R);
  Waa_R.AddChildFrame(Naa_R);
  Wab_R.AddChildFrame(Lab_R);
  Wab_R.AddChildFrame(Nab_R);
  Wba_R.AddChildFrame(Lba_R);
  Wba_R.AddChildFrame(Nba_R);
  Wbb_R.AddChildFrame(Lbb_R);
  Wbb_R.AddChildFrame(Nbb_R);

  if(LAB_R.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV_R("INV_R","4 #nu Jigsaws");
  INV_R.AddFrame(Naa_R);
  INV_R.AddFrame(Nab_R);
  INV_R.AddFrame(Nba_R);
  INV_R.AddFrame(Nbb_R);

  // Set nu nu mass equal to l l mass
  SetMassInvJigsaw NuNuM_R("NuNuM_R", "M_{4 #nu} = f(4 #it{l})");
  INV_R.AddJigsaw(NuNuM_R);

  SetRapidityInvJigsaw NuNuR_R("NuNuR_R", "#eta_{4 #nu} = #eta_{4 #it{l}}");
  INV_R.AddJigsaw(NuNuR_R);
  NuNuR_R.AddVisibleFrames(LAB_R.GetListVisibleFrames());

  MinMassDiffInvJigsaw MinMh_R("MinMh_R","min M_{h}, M_{h}^{ a}= M_{h}^{ b}",2);
  //MinMassesSqInvJigsaw MinMh_R("MinMh_R","min M_{h}, M_{h}^{ a}= M_{h}^{ b}",2);
  //ContraBoostInvJigsaw MinMh_R("MinMh_R","min M_{h}, M_{h}^{ a}= M_{h}^{ b}");
  INV_R.AddJigsaw(MinMh_R);
  MinMh_R.AddVisibleFrames(ha_R.GetListVisibleFrames(), 0);
  MinMh_R.AddVisibleFrames(hb_R.GetListVisibleFrames(), 1);
  MinMh_R.AddInvisibleFrames(ha_R.GetListInvisibleFrames(), 0);
  MinMh_R.AddInvisibleFrames(hb_R.GetListInvisibleFrames(), 1);

  //ContraBoostInvJigsaw MinMWa_R("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{a,b}");
  MinMassesSqInvJigsaw MinMWa_R("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{a,b}", 2);
  //MaxProbBreitWignerInvJigsaw MinMWa_R("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{a,b}", 2);
  INV_R.AddJigsaw(MinMWa_R);
  MinMWa_R.AddVisibleFrames(Waa_R.GetListVisibleFrames(), 0);
  MinMWa_R.AddVisibleFrames(Wab_R.GetListVisibleFrames(), 1);
  MinMWa_R.AddInvisibleFrames(Waa_R.GetListInvisibleFrames(), 0);
  MinMWa_R.AddInvisibleFrames(Wab_R.GetListInvisibleFrames(), 1);

  // MinMWa_R.SetPoleMass(mW, 0);
  // MinMWa_R.SetPoleMass(mW, 1);
  // MinMWa_R.SetWidth(wW, 0);
  // MinMWa_R.SetWidth(wW, 1);

  //ContraBoostInvJigsaw MinMWb_R("MinMWa_R","min M_{W}, M_{W}^{b,a}= M_{W}^{b,b}");
  MinMassesSqInvJigsaw  MinMWb_R("MinMWa_R","min M_{W}, M_{W}^{b,a}= M_{W}^{b,b}", 2);
  //MaxProbBreitWignerInvJigsaw  MinMWb_R("MinMWa_R","min M_{W}, M_{W}^{b,a}= M_{W}^{b,b}", 2);
  INV_R.AddJigsaw(MinMWb_R);
  MinMWb_R.AddVisibleFrames(Wba_R.GetListVisibleFrames(), 0);
  MinMWb_R.AddVisibleFrames(Wbb_R.GetListVisibleFrames(), 1);
  MinMWb_R.AddInvisibleFrames(Wba_R.GetListInvisibleFrames(), 0);
  MinMWb_R.AddInvisibleFrames(Wbb_R.GetListInvisibleFrames(), 1);

  // MinMWb_R.SetPoleMass(mW, 0);
  // MinMWb_R.SetPoleMass(mW, 1);
  // MinMWb_R.SetWidth(wW, 0);
  // MinMWb_R.SetWidth(wW, 1);

  // Combinatoric Group for leptons
  CombinatoricGroup VIS_R("VIS_R","Lepton Combinatoric Jigsaws");
  VIS_R.AddFrame(Lab_R);
  VIS_R.AddFrame(Lbb_R);
  // lepton frames must have at least one element
  VIS_R.SetNElementsForFrame(Lab_R, 1);
  VIS_R.SetNElementsForFrame(Lbb_R, 1);

  MinMassesCombJigsaw MinMll_R("MinMll_R", "min Mll");
  VIS_R.AddJigsaw(MinMll_R);
  MinMll_R.AddCombFrame(Lab_R, 0);
  MinMll_R.AddCombFrame(Lbb_R, 1);
  MinMll_R.AddObjectFrames(Laa_R+Lab_R, 0);
  MinMll_R.AddObjectFrames(Lba_R+Lbb_R, 1);

  
  // ContraBoostInvJigsaw MinMWa_R("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{b,a}");
  // MinMWa_R.AddVisibleFrames(Waa_R.GetListVisibleFrames(), 0);
  // MinMWa_R.AddVisibleFrames(Wba_R.GetListVisibleFrames(), 1);
  // MinMWa_R.AddInvisibleFrames(Waa_R.GetListInvisibleFrames(), 0);
  // MinMWa_R.AddInvisibleFrames(Wba_R.GetListInvisibleFrames(), 1);

  // ContraBoostInvJigsaw MinMWb_R("MinMWa_R","min M_{W}, M_{W}^{a,b}= M_{W}^{b,b}");
  // MinMWb_R.AddVisibleFrames(Wab_R.GetListVisibleFrames(), 0);
  // MinMWb_R.AddVisibleFrames(Wbb_R.GetListVisibleFrames(), 1);
  // MinMWb_R.AddInvisibleFrames(Wab_R.GetListInvisibleFrames(), 0);
  // MinMWb_R.AddInvisibleFrames(Wbb_R.GetListInvisibleFrames(), 1);

  // CombinedCBInvJigsaw combCB_R("combCB_R", "min M_{W} , M_{W}^{a,i}= M_{W}^{b,i}", 2);
  // INV_R.AddJigsaw(combCB_R);
  // combCB_R.AddJigsaw(MinMWa_R, 0);
  // combCB_R.AddJigsaw(MinMWb_R, 1);

  // // Combinatoric Group for leptons
  // CombinatoricGroup VIS_R("VIS_R","Lepton Combinatoric Jigsaws");
  // VIS_R.AddFrame(Laa_R);
  // VIS_R.AddFrame(Lab_R);
  // VIS_R.AddFrame(Lba_R);
  // VIS_R.AddFrame(Lbb_R);
  // // lepton frames must have at least one element
  // VIS_R.SetNElementsForFrame(Laa_R, 1);
  // VIS_R.SetNElementsForFrame(Lab_R, 1);
  // VIS_R.SetNElementsForFrame(Lba_R, 1);
  // VIS_R.SetNElementsForFrame(Lbb_R, 1);

  // MinMassesCombJigsaw MinMll_R("MinMll_R", "min Mll");
  // VIS_R.AddJigsaw(MinMll_R);
  // MinMll_R.AddCombFrames(Laa_R+Lab_R, 0);
  // MinMll_R.AddCombFrames(Lba_R+Lbb_R, 1);
  // MinMll_R.AddObjectFrames(Laa_R+Lab_R, 0);
  // MinMll_R.AddObjectFrames(Lba_R+Lbb_R, 1);
  // MinMll_R.SetCombCharge(0, 0);
  // MinMll_R.SetCombCharge(0, 1);

  // MaxProbBreitWignerCombJigsaw MinMWchi2a_R("MinMWchi2a_R", "MW chi2 a", 2, 1);
  // VIS_R.AddJigsaw(MinMWchi2a_R);
  // MinMWchi2a_R.AddCombFrame(Laa_R, 0);
  // MinMWchi2a_R.AddCombFrame(Lab_R, 1);
  // MinMWchi2a_R.AddObjectFrame(Waa_R, 0);
  // MinMWchi2a_R.AddObjectFrame(Wab_R, 1);
  // MinMWchi2a_R.SetPoleMass(mW, 0);
  // MinMWchi2a_R.SetPoleMass(mW, 1);
  // MinMWchi2a_R.SetWidth(wW, 0);
  // MinMWchi2a_R.SetWidth(wW, 1);

  // MaxProbBreitWignerCombJigsaw MinMWchi2b_R("MinMWchi2b_R", "MW chi2 b", 2, 1);
  // VIS_R.AddJigsaw(MinMWchi2b_R);
  // MinMWchi2b_R.AddCombFrame(Lba_R, 0);
  // MinMWchi2b_R.AddCombFrame(Lbb_R, 1);
  // MinMWchi2b_R.AddObjectFrame(Wba_R, 0);
  // MinMWchi2b_R.AddObjectFrame(Wbb_R, 1);
  // MinMWchi2b_R.SetPoleMass(mW, 0);
  // MinMWchi2b_R.SetPoleMass(mW, 1);
  // MinMWchi2b_R.SetWidth(wW, 0);
  // MinMWchi2b_R.SetWidth(wW, 1);

  if(LAB_R.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_G);
  treePlot->Draw("GenTree", "Generator Tree", true);

  treePlot->SetTree(LAB_R);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(INV_R);
  treePlot->Draw("InvTree", "Invisible Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot = new HistPlot("HistPlot", "H #rightarrow W(#it{l} #nu) W(#it{l} #nu)");

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_Reco  = histPlot->GetNewCategory("Reco", "Reconstruction");

  const HistPlotVar& MH    = histPlot->GetNewVar("MH", "M_{H} / m_{H}", 0., 2.);
  const HistPlotVar& Mha   = histPlot->GetNewVar("Mha", "M_{h}^{ a} / m_{h}^{ a}", 0., 2.);
  const HistPlotVar& Mhb   = histPlot->GetNewVar("Mhb", "M_{h}^{ b} / m_{h}^{ b}", 0., 2.);
  const HistPlotVar& MWaa  = histPlot->GetNewVar("MWaa", "M_{W a,a}", 0., mW*2., "[GeV]");
  const HistPlotVar& MWab  = histPlot->GetNewVar("MWab", "M_{W a,b}", 0., mW*2., "[GeV]");
  const HistPlotVar& MWba  = histPlot->GetNewVar("MWba", "M_{W b,a}", 0., mW*2., "[GeV]");
  const HistPlotVar& MWbb  = histPlot->GetNewVar("MWbb", "M_{W b,b}", 0., mW*2., "[GeV]");
  const HistPlotVar& cosH  = histPlot->GetNewVar("cosH","cos #theta_{H}", -1., 1.);
  const HistPlotVar& cosha = histPlot->GetNewVar("cosha","cos #theta_{h a}", -1., 1.);
  const HistPlotVar& coshb = histPlot->GetNewVar("coshb","cos #theta_{h b}", -1., 1.);
  const HistPlotVar& cosWaa = histPlot->GetNewVar("cosWaa","cos #theta_{Waa}", -1., 1.);
  const HistPlotVar& cosWab = histPlot->GetNewVar("cosWab","cos #theta_{Wab}", -1., 1.);
  const HistPlotVar& DcosH = histPlot->GetNewVar("DcosH","#theta_{H} - #theta_{H}^{gen}", 
						 -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcosha = histPlot->GetNewVar("Dcosha","#theta_{h a} - #theta_{h a}^{gen}", 
  						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcoshb = histPlot->GetNewVar("Dcoshb","#theta_{h b} - #theta_{h b}^{gen}", 
  						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWaa = histPlot->GetNewVar("DcosWaa","#theta_{W a,a} - #theta_{W a,a}^{gen}", 
  						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWab = histPlot->GetNewVar("DcosWab","#theta_{W a,b} - #theta_{W a,b}^{gen}", 
  						  -acos(-1.)/2., acos(-1.)/2.);
  
  histPlot->AddPlot(MWaa, MWab, cat_Gen+cat_Reco);
  histPlot->AddPlot(MH, cat_Reco);
  histPlot->AddPlot(Mha, cat_Reco);
  histPlot->AddPlot(Mha, Mhb, cat_Reco);
  histPlot->AddPlot(MH, Mha, cat_Reco);
  histPlot->AddPlot(Mha, MWaa, cat_Reco);
  histPlot->AddPlot(cosH, cat_Gen+cat_Reco);
  histPlot->AddPlot(cosha, cat_Gen+cat_Reco);
  histPlot->AddPlot(DcosH, cat_Reco);
  histPlot->AddPlot(Dcosha, cat_Reco);
  histPlot->AddPlot(MH, DcosH, cat_Reco);
  histPlot->AddPlot(Mha, Dcosha, cat_Reco);
  histPlot->AddPlot(DcosH, Dcosha, cat_Reco);
  histPlot->AddPlot(Dcosha, Dcoshb, cat_Reco);

  histPlot->AddPlot(DcosWaa, cat_Reco);
  histPlot->AddPlot(cosWaa, cosWab, cat_Reco);
  histPlot->AddPlot(DcosWaa, DcosWab, cat_Reco);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTH = mH*gRandom->Rndm()*0.1;
    LAB_G.SetTransverseMomenta(PTH);               // give the Higgs some Pt
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    LAB_R.ClearEvent();                               // clear the reco tree

    TVector3 MET = LAB_G.GetInvisibleMomentum();      // Get the MET from gen tree
    MET.SetZ(0.);
    INV_R.SetLabFrameThreeVector(MET);                // Set the MET in reco tree

    // std::vector<RFKey> L_ID; // ID for tracking leptons in tree
    // L_ID.push_back(VIS_R.AddLabFrameFourVector(Laa_G.GetFourVector(),-1));
    // L_ID.push_back(VIS_R.AddLabFrameFourVector(Lab_G.GetFourVector(), 1));
    // L_ID.push_back(VIS_R.AddLabFrameFourVector(Lba_G.GetFourVector(),-1));
    // L_ID.push_back(VIS_R.AddLabFrameFourVector(Lbb_G.GetFourVector(), 1));

    Laa_R.SetLabFrameFourVector(Laa_G.GetFourVector(),-1);
    Lba_R.SetLabFrameFourVector(Lba_G.GetFourVector(),-1);
    std::vector<RFKey> L_ID; // ID for tracking anti-leptons in tree
    L_ID.push_back(VIS_R.AddLabFrameFourVector(Lab_G.GetFourVector(), 1));
    L_ID.push_back(VIS_R.AddLabFrameFourVector(Lbb_G.GetFourVector(), 1));

    // Laa_R.SetLabFrameFourVector(Laa_G.GetFourVector(),-1);
    // Lab_R.SetLabFrameFourVector(Lab_G.GetFourVector(), 1);
    // Lba_R.SetLabFrameFourVector(Lba_G.GetFourVector(),-1);
    // Lbb_R.SetLabFrameFourVector(Lbb_G.GetFourVector(), 1);

    LAB_R.AnalyzeEvent();                             // analyze the event

    // Generator-level observables
    double MHgen    = H_G.GetMass();
    double Mhagen   = ha_G.GetMass();
    double Mhbgen   = hb_G.GetMass();
    double cosHgen  = H_G.GetCosDecayAngle();
    double coshagen = ha_G.GetCosDecayAngle();
    double coshbgen = hb_G.GetCosDecayAngle();
    cosH  = cosHgen;
    cosha = coshagen;
    coshb = coshbgen;
    double cosWaagen = Waa_G.GetCosDecayAngle();
    double cosWabgen = Wab_G.GetCosDecayAngle();
    MWaa = Waa_G.GetMass();
    MWab = Wab_G.GetMass();
   
    histPlot->Fill(cat_Gen);

    // Reconstruction-level observables
    MH = H_R.GetMass()/MHgen;
    //MH = 2.*H_R.GetListVisibleFrames().GetEnergy(H_R)/MHgen;
    Mha = ha_R.GetMass()/Mhagen;
    Mhb = hb_R.GetMass()/Mhbgen;
    // Mha = 2.*ha_R.GetListVisibleFrames().GetEnergy(ha_R)/Mhagen;
    // Mhb = 2.*hb_R.GetListVisibleFrames().GetEnergy(hb_R)/Mhbgen;
    cosH = H_R.GetCosDecayAngle();
    cosha = ha_R.GetCosDecayAngle();
    coshb = hb_R.GetCosDecayAngle();
    cosWaa = Waa_R.GetCosDecayAngle();
    cosWab = Wab_R.GetCosDecayAngle();
    MWaa = Waa_R.GetMass();
    MWab = Wab_R.GetMass();
    MWba = Wba_R.GetMass();
    MWbb = Wbb_R.GetMass();
    DcosH = asin(sqrt(1.-cosH*cosH)*cosHgen-sqrt(1.-cosHgen*cosHgen)*cosH);
    Dcosha = asin(sqrt(1.-cosha*cosha)*coshagen-sqrt(1.-coshagen*coshagen)*cosha);
    Dcoshb = asin(sqrt(1.-coshb*coshb)*coshbgen-sqrt(1.-coshbgen*coshbgen)*coshb);
    DcosWaa = asin(sqrt(1.-cosWaa*cosWaa)*cosWaagen-sqrt(1.-cosWaagen*cosWaagen)*cosWaa);
    DcosWab = asin(sqrt(1.-cosWab*cosWab)*cosWabgen-sqrt(1.-cosWabgen*cosWabgen)*cosWab);
    
   
    histPlot->Fill(cat_Reco);

  //   g_Log << LogInfo;
//     g_Log << "Correct assignments " << std::endl;
//     g_Log << (VIS_R.GetFrame(L_ID[0]).GetParentFrame().GetParentFrame() == VIS_R.GetFrame(L_ID[1]).GetParentFrame().GetParentFrame()) << " ";
//     g_Log << (VIS_R.GetFrame(L_ID[2]).GetParentFrame().GetParentFrame() == VIS_R.GetFrame(L_ID[3]).GetParentFrame().GetParentFrame()) << " " << std::endl;
//   g_Log << (VIS_R.GetFrame(L_ID[0]).GetParentFrame().GetParentFrame() == VIS_R.GetFrame(L_ID[3]).GetParentFrame().GetParentFrame()) << " ";
// g_Log << (VIS_R.GetFrame(L_ID[2]).GetParentFrame().GetParentFrame() == VIS_R.GetFrame(L_ID[1]).GetParentFrame().GetParentFrame()) << " " << std::endl;
//     g_Log << (Laa_R+Lab_R).GetMass() << " " << (Lba_R+Lbb_R).GetMass() << std::endl;
//     g_Log << (Laa_R+Lbb_R).GetMass() << " " << (Lba_R+Lab_R).GetMass() << std::endl;
//     g_Log << LogEnd;
  }

  histPlot->Draw();
  
  LAB_G.PrintGeneratorEfficiency();
  
  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_H_to_hh_to_4Wlnu();
  return 0;
}
#endif
