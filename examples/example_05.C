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
  LabRecoFrame       LAB_Mt("LAB_Mt","LAB");
  DecayRecoFrame     TT_Mt("TT_Mt","t #bar{t}");
  DecayRecoFrame     Ta_Mt("Ta_Mt","t_{a}");
  DecayRecoFrame     Tb_Mt("Tb_Mt","t_{b}");
  DecayRecoFrame     Wa_Mt("Wa_Mt","W_{a}");
  DecayRecoFrame     Wb_Mt("Wb_Mt","W_{b}");
  VisibleRecoFrame   Ba_Mt("Ba_Mt","b_{a}");
  VisibleRecoFrame   La_Mt("La_Mt","#it{l}_{a}");
  InvisibleRecoFrame Na_Mt("Na_Mt","#nu_{a}");
  VisibleRecoFrame   Bb_Mt("Bb_Mt","b_{b}");
  VisibleRecoFrame   Lb_Mt("Lb_Mt","#it{l}_{b}");
  InvisibleRecoFrame Nb_Mt("Nb_Mt","#nu_{b}");

  LabRecoFrame       LAB_MW("LAB_MW","LAB");
  DecayRecoFrame     TT_MW("TT_MW","t #bar{t}");
  DecayRecoFrame     Ta_MW("Ta_MW","t_{a}");
  DecayRecoFrame     Tb_MW("Tb_MW","t_{b}");
  DecayRecoFrame     Wa_MW("Wa_MW","W_{a}");
  DecayRecoFrame     Wb_MW("Wb_MW","W_{b}");
  VisibleRecoFrame   Ba_MW("Ba_MW","b_{a}");
  VisibleRecoFrame   La_MW("La_MW","#it{l}_{a}");
  InvisibleRecoFrame Na_MW("Na_MW","#nu_{a}");
  VisibleRecoFrame   Bb_MW("Bb_MW","b_{b}");
  VisibleRecoFrame   Lb_MW("Lb_MW","#it{l}_{b}");
  InvisibleRecoFrame Nb_MW("Nb_MW","#nu_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Mt.SetChildFrame(TT_Mt);
  TT_Mt.AddChildFrame(Ta_Mt);
  TT_Mt.AddChildFrame(Tb_Mt);
  Ta_Mt.AddChildFrame(Ba_Mt);
  Ta_Mt.AddChildFrame(Wa_Mt);
  Tb_Mt.AddChildFrame(Bb_Mt);
  Tb_Mt.AddChildFrame(Wb_Mt);
  Wa_Mt.AddChildFrame(La_Mt);
  Wa_Mt.AddChildFrame(Na_Mt);
  Wb_Mt.AddChildFrame(Lb_Mt);
  Wb_Mt.AddChildFrame(Nb_Mt);

  LAB_MW.SetChildFrame(TT_MW);
  TT_MW.AddChildFrame(Ta_MW);
  TT_MW.AddChildFrame(Tb_MW);
  Ta_MW.AddChildFrame(Ba_MW);
  Ta_MW.AddChildFrame(Wa_MW);
  Tb_MW.AddChildFrame(Bb_MW);
  Tb_MW.AddChildFrame(Wb_MW);
  Wa_MW.AddChildFrame(La_MW);
  Wa_MW.AddChildFrame(Na_MW);
  Wb_MW.AddChildFrame(Lb_MW);
  Wb_MW.AddChildFrame(Nb_MW);

  if(LAB_Mt.InitializeTree() && LAB_MW.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV_Mt("INV_Mt","#nu #nu Jigsaws");
  INV_Mt.AddFrame(Na_Mt);
  INV_Mt.AddFrame(Nb_Mt);
  // Combinatoric Group for b's
  CombinatoricGroup B_Mt("VIS","b-jet Jigsaws");
  B_Mt.AddFrame(Ba_Mt);
  B_Mt.AddFrame(Bb_Mt);
  // visible frames in first decay step must always have at least one element
  B_Mt.SetNElementsForFrame(Ba_Mt,1,true);
  B_Mt.SetNElementsForFrame(Bb_Mt,1,true);

  // Invisible Group
  InvisibleGroup INV_MW("INV_MW","#nu #nu Jigsaws");
  INV_MW.AddFrame(Na_MW);
  INV_MW.AddFrame(Nb_MW);
  // Combinatoric Group for b's
  CombinatoricGroup B_MW("VIS","b-jet Jigsaws");
  B_MW.AddFrame(Ba_MW);
  B_MW.AddFrame(Bb_MW);
  // visible frames in first decay step must always have at least one element
  B_MW.SetNElementsForFrame(Ba_MW,1,true);
  B_MW.SetNElementsForFrame(Bb_MW,1,true);

  //////////////////

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw NuNuM_Mt("NuNuM_Mt", 
			   "M_{#nu#nu} = f(m_{b#it{l}b#it{l}}, m_{b#it{l} a}, m_{b#it{l} b}");
  INV_Mt.AddJigsaw(NuNuM_Mt);

  SetRapidityInvJigsaw NuNuR_Mt("NuNuR_Mt", "#eta_{#nu#nu} = #eta_{b#it{l}b#it{l}}");
  INV_Mt.AddJigsaw(NuNuR_Mt);
  NuNuR_Mt.AddVisibleFrames(LAB_Mt.GetListVisibleFrames());

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw NuNuM_MW("NuNuM_MW", 
			   "M_{#nu#nu} = f(m_{b#it{l}b#it{l}}, m_{b#it{l} a}, m_{b#it{l} b}");
  INV_MW.AddJigsaw(NuNuM_MW);

  SetRapidityInvJigsaw NuNuR_MW("NuNuR_MW", "#eta_{#nu#nu} = #eta_{b#it{l}b#it{l}}");
  INV_MW.AddJigsaw(NuNuR_MW);
  NuNuR_MW.AddVisibleFrames(LAB_MW.GetListVisibleFrames());
 
  ContraBoostInvJigsaw MinMt_Mt("MinMt_Mt","min M_{t}, M_{ta}= M_{tb}");
  INV_Mt.AddJigsaw(MinMt_Mt);
  MinMt_Mt.AddVisibleFrames(La_Mt+Ba_Mt, 0);
  MinMt_Mt.AddVisibleFrames(Lb_Mt+Bb_Mt, 1);
  MinMt_Mt.AddInvisibleFrame(Na_Mt, 0);
  MinMt_Mt.AddInvisibleFrame(Nb_Mt, 1);

  ContraBoostInvJigsaw MinMW_MW("MinMW_MW","min M_{t}, M_{Wa}= M_{Wb}");
  INV_MW.AddJigsaw(MinMW_MW);
  MinMW_MW.AddVisibleFrame(La_MW, 0);
  MinMW_MW.AddVisibleFrame(Lb_MW, 1);
  MinMW_MW.AddInvisibleFrame(Na_MW, 0);
  MinMW_MW.AddInvisibleFrame(Nb_MW, 1);

  MinMassesCombJigsaw MinBL_Mt("MinBL_Mt","Minimize M(b#it{l} a), M(b#it{l} b)");
  B_Mt.AddJigsaw(MinBL_Mt);
  MinBL_Mt.AddFrames(La_Mt+Ba_Mt,0);
  MinBL_Mt.AddFrames(Lb_Mt+Bb_Mt,1);

  MinMassesCombJigsaw MinBL_MW("MinBL_MW","Minimize M(b#it{l} a), M(b#it{l} b)");
  B_MW.AddJigsaw(MinBL_MW);
  MinBL_MW.AddFrames(La_MW+Ba_MW,0);
  MinBL_MW.AddFrames(Lb_MW+Bb_MW,1);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  tree_plot->SetTree(LAB_Gen);
  tree_plot->Draw("GenTree", "Generator Tree", true);
  
  tree_plot->SetTree(LAB_Mt);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");

  tree_plot->SetTree(INV_Mt);
  tree_plot->Draw("InvTree", "Invisible Jigsaws");

  tree_plot->SetTree(B_Mt);
  tree_plot->Draw("VisTree", "b-jet Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot = new HistPlot("HistPlot", "t #bar{t} #rightarrow b W(#it{l} #nu) b W(#it{l} #nu)");

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");
  const HistPlotCategory& cat_Mt  = histPlot->GetNewCategory("Reco", "M_{t a} = M_{t b} Reco");
  const HistPlotCategory& cat_MW  = histPlot->GetNewCategory("Reco", "M_{W a} = M_{W b} Reco");

  const HistPlotVar& Mtt    = histPlot->GetNewVar("Mtt", "M_{t#bar{t}} / m_{t#bar{t}}", 0., 2.);
  const HistPlotVar& Pt_tt  = histPlot->GetNewVar("Pt_tt", "p_{t}^{t#bar{t}} / p_{t}^{t#bar{t} gen}", 0., 2.);
  const HistPlotVar& Mta    = histPlot->GetNewVar("Mta", "M_{ta}", 0., mT*2., "[GeV]");
  const HistPlotVar& Mtb    = histPlot->GetNewVar("Mtb", "M_{tb}", 0., mT*2., "[GeV]");
  const HistPlotVar& MWa    = histPlot->GetNewVar("MWa", "M_{Wa}", 0., mW*2., "[GeV]");
  const HistPlotVar& MWb    = histPlot->GetNewVar("MWb", "M_{Wb}", 0., mW*2., "[GeV]");
  const HistPlotVar& Eb_ta  = histPlot->GetNewVar("Eb_ta", "E_{b a}^{ta} / E_{b a}^{ta gen}", 0., 2.);
  const HistPlotVar& Eb_tb  = histPlot->GetNewVar("Eb_tb", "E_{b b}^{tb} / E_{b b}^{tb gen}", 0., 2.);
  const HistPlotVar& El_Wa  = histPlot->GetNewVar("El_Wa", "E_{#it{l} a}^{Wa} / E_{#it{l} a}^{ta gen}", 0., 2.);
  const HistPlotVar& El_Wb  = histPlot->GetNewVar("El_Wb", "E_{#it{l} b}^{Wb} / E_{#it{l} b}^{tb gen}", 0., 2.);
  const HistPlotVar& costt  = histPlot->GetNewVar("costt","cos #theta_{t#bar{t}}", -1., 1.);
  const HistPlotVar& costa  = histPlot->GetNewVar("costa","cos #theta_{t a}", -1., 1.);
  const HistPlotVar& costb  = histPlot->GetNewVar("costb","cos #theta_{t b}", -1., 1.);
  const HistPlotVar& cosWa  = histPlot->GetNewVar("cosWa","cos #theta_{W a}", -1., 1.);
  const HistPlotVar& cosWb  = histPlot->GetNewVar("cosWb","cos #theta_{W b}", -1., 1.);
  const HistPlotVar& Dcostt = histPlot->GetNewVar("Dcostt","#theta_{t#bar{t}} - #theta_{t#bar{t}}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcosta = histPlot->GetNewVar("Dcosta","#theta_{t a} - #theta_{t a}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& Dcostb = histPlot->GetNewVar("Dcostb","#theta_{t b} - #theta_{t b}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWa = histPlot->GetNewVar("DcosWa","#theta_{W a} - #theta_{W a}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosWb = histPlot->GetNewVar("DcosWb","#theta_{W b} - #theta_{W b}^{gen}", 
						  -acos(-1.)/2., acos(-1.)/2.);

  histPlot->AddPlot(Mtt, cat_Mt+cat_MW);
  histPlot->AddPlot(Pt_tt, cat_Mt+cat_MW);
  histPlot->AddPlot(Mta, cat_Mt+cat_MW);
  histPlot->AddPlot(MWa, cat_Mt+cat_MW);
  histPlot->AddPlot(Eb_ta, cat_Mt+cat_MW);
  histPlot->AddPlot(El_Wa, cat_Mt+cat_MW);
  histPlot->AddPlot(costt, cat_Mt+cat_MW);
  histPlot->AddPlot(costa, cat_Mt+cat_MW);
  histPlot->AddPlot(cosWa, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcostt, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcosta, cat_Mt+cat_MW);
  histPlot->AddPlot(DcosWa, cat_Mt+cat_MW);

  histPlot->AddPlot(Mtt, Eb_ta, cat_Mt+cat_MW);
  histPlot->AddPlot(Mtt, El_Wa, cat_Mt+cat_MW);
  histPlot->AddPlot(Eb_ta, Eb_tb, cat_Mt+cat_MW);
  histPlot->AddPlot(El_Wa, El_Wb, cat_Mt+cat_MW);
  histPlot->AddPlot(Eb_ta, El_Wa, cat_Mt+cat_MW);
  histPlot->AddPlot(Eb_ta, El_Wb, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcostt, Mtt, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcosta, Eb_ta, cat_Mt+cat_MW);
  histPlot->AddPlot(DcosWa, El_Wa, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcostt, Dcosta, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcosta, Dcostb, cat_Mt+cat_MW);
  histPlot->AddPlot(DcosWa, DcosWb, cat_Mt+cat_MW);
  histPlot->AddPlot(Dcosta, DcosWa, cat_Mt+cat_MW);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                             // clear the gen tree
    double PTtt = 0.*mT*gRandom->Rndm();
    LAB_Gen.SetTransverseMomenta(PTtt);               // give the di-tops some Pt
    LAB_Gen.AnalyzeEvent();                           // generate a new event

    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    // analyze event
    LAB_Mt.ClearEvent();                              // clear the signal-like tree
    INV_Mt.SetLabFrameThreeVector(MET);               // Set the MET in reco tree

    std::vector<RFKey> B_Mt_ID;                      // ID for tracking jets in tree
    B_Mt_ID.push_back(B_Mt.AddLabFrameFourVector(Ba_Gen.GetFourVector()));
    B_Mt_ID.push_back(B_Mt.AddLabFrameFourVector(Bb_Gen.GetFourVector()));
    
    La_Mt.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_Mt.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    LAB_Mt.AnalyzeEvent();                            // analyze the event

    // analyze event
    LAB_MW.ClearEvent();                              // clear the signal-like tree
    INV_MW.SetLabFrameThreeVector(MET);               // Set the MET in reco tree

    std::vector<RFKey> B_MW_ID;                      // ID for tracking jets in tree
    B_MW_ID.push_back(B_MW.AddLabFrameFourVector(Ba_Gen.GetFourVector()));
    B_MW_ID.push_back(B_MW.AddLabFrameFourVector(Bb_Gen.GetFourVector()));
    
    La_MW.SetLabFrameFourVector(La_Gen.GetFourVector());
    Lb_MW.SetLabFrameFourVector(Lb_Gen.GetFourVector());
    LAB_MW.AnalyzeEvent();                            // analyze the event

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

    Mtt = TT_Mt.GetMass()/Mttgen;
    // Mtt = (TT_Mt.GetListVisibleFrames().GetFourVector(TT_Mt).E()+
    // 	   TT_Mt.GetListInvisibleFrames().GetFourVector(TT_Mt).P())/Mttgen;
    
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
  }

  //tree_plot->WriteOutput(output_name);

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
