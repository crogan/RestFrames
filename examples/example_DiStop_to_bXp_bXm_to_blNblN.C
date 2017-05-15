/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_DiStop_to_bXp_bXm_to_blNblN.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Jul
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

#define COMPILER (!defined(__CINT__) && !defined(__CLING__))
#if defined(__MAKECINT__) || defined(__ROOTCLING__) || COMPILER
#include "RestFrames/RestFrames.hh"
#else
RestFrames::RFKey ensure_autoload(1);
#endif

using namespace RestFrames;

void example_DiStop_to_bXp_bXm_to_blNblN(std::string output_name =
					 "output_DiStop_to_bXp_bXm_to_blNblN.root"){

  double mT = 800.;      // stop mass
  double mN = 100.;      // sneutrino mass
  double mB = 4.18;
  double mL = 0.106;

  int NMX = 64;           // number of different chargino masses to evaluate

  bool fix_MXb = true;  // fix the "b" chargino mass while varying "a"?
  double mXb   = 200;
  
  // number of events to generate (per chargino mass)
  int Ngen = 1000000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     CM_Gen("CM_Gen","CM");
  DecayGenFrame     Ta_Gen("Ta_Gen","#tilde{t}_{a}");
  DecayGenFrame     Tb_Gen("Tb_Gen","#tilde{t}_{b}");
  DecayGenFrame     Xa_Gen("Xa_Gen","#tilde{#chi}_{a}^{ #pm}");
  DecayGenFrame     Xb_Gen("Xb_Gen","#tilde{#chi}_{b}^{ #mp}");
  VisibleGenFrame   Ba_Gen("Ba_Gen","b_{a}");
  VisibleGenFrame   La_Gen("La_Gen","#it{l}_{a}");
  InvisibleGenFrame Na_Gen("Na_Gen","#tilde{#nu}_{a}");
  VisibleGenFrame   Bb_Gen("Bb_Gen","b_{b}");
  VisibleGenFrame   Lb_Gen("Lb_Gen","#it{l}_{b}");
  InvisibleGenFrame Nb_Gen("Nb_Gen","#tilde{#nu}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(CM_Gen);
  CM_Gen.AddChildFrame(Ta_Gen);
  CM_Gen.AddChildFrame(Tb_Gen);
  Ta_Gen.AddChildFrame(Ba_Gen);
  Ta_Gen.AddChildFrame(Xa_Gen);
  Tb_Gen.AddChildFrame(Bb_Gen);
  Tb_Gen.AddChildFrame(Xb_Gen);
  Xa_Gen.AddChildFrame(La_Gen);
  Xa_Gen.AddChildFrame(Na_Gen);
  Xb_Gen.AddChildFrame(Lb_Gen);
  Xb_Gen.AddChildFrame(Nb_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;							        
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  if(NMX < 1) return;
  
  // non-resonant stop production
  CM_Gen.SetVariableMass();
  // set stop masses
  Ta_Gen.SetMass(mT);            Tb_Gen.SetMass(mT);
  // set chargino masses
  Xa_Gen.SetMass(0.5*(mT+mN));   Xb_Gen.SetMass(0.5*(mT+mN));
  // set B masses
  Ba_Gen.SetMass(mB);            Bb_Gen.SetMass(mB);
  // set lepton masses
  La_Gen.SetMass(mL);            Lb_Gen.SetMass(mL);
  // set sneutrino masses
  Na_Gen.SetMass(mN);            Nb_Gen.SetMass(mN);

  // set b-jet/lepton pT/eta cuts
  Ba_Gen.SetPtCut(20.);          Bb_Gen.SetPtCut(20.);
  Ba_Gen.SetEtaCut(2.5);         Bb_Gen.SetEtaCut(2.5);
  La_Gen.SetPtCut(15.);          Lb_Gen.SetPtCut(15.);
  La_Gen.SetEtaCut(2.5);         Lb_Gen.SetEtaCut(2.5);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////								    
  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////

  LabRecoFrame       LAB("LAB","LAB");     
  DecayRecoFrame     CM("CM","CM"); 
  DecayRecoFrame     Ta("Ta","#tilde{t}_{a}");   
  DecayRecoFrame     Tb("Tb","#tilde{t}_{b}");    
  DecayRecoFrame     Xa("Xa","#tilde{#chi}_{a}^{ #pm}");     
  DecayRecoFrame     Xb("Xb","#tilde{#chi}_{b}^{ #mp}");   
  VisibleRecoFrame   Ba("Ba","b_{a}");     
  VisibleRecoFrame   La("La","#it{l}_{a}");
  InvisibleRecoFrame Na("Na","#tilde{#nu}_{a}");    
  VisibleRecoFrame   Bb("Bb","b_{b}");     
  VisibleRecoFrame   Lb("Lb","#it{l}_{b}");
  InvisibleRecoFrame Nb("Nb","#tilde{#nu}_{b}");   

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(CM);                  
  CM.AddChildFrame(Ta);                 
  CM.AddChildFrame(Tb);                   
  Ta.AddChildFrame(Ba);                  
  Ta.AddChildFrame(Xa);                   
  Tb.AddChildFrame(Bb);                  
  Tb.AddChildFrame(Xb);                
  Xa.AddChildFrame(La);                  
  Xa.AddChildFrame(Na);                  
  Xb.AddChildFrame(Lb);                  
  Xb.AddChildFrame(Nb);                  

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  //////////////////////// define Groups for reconstruction tree ////////////////////////
  std::string group_name;

  // Invisible Group
  group_name = "#tilde{#nu} #tilde{#nu} Jigsaws";
  InvisibleGroup INV("INV", group_name);
  INV.AddFrame(Na);
  INV.AddFrame(Nb);
  CombinatoricGroup B("VIS","b-jet Jigsaws");
  B.AddFrame(Ba);
  B.AddFrame(Bb);
  B.SetNElementsForFrame(Ba, 1);
  B.SetNElementsForFrame(Bb, 1);

  //////////////////////// define Jigsaws for reconstruction tree ////////////////////////
  std::string jigsaw_name;

  // Minimize difference Mt jigsaws
  jigsaw_name = "M_{#tilde{#nu} #tilde{#nu}} ~ m_{#it{l} #it{l}}";
  SetMassInvJigsaw NuNuM("NuNuM", jigsaw_name);
  INV.AddJigsaw(NuNuM);

  jigsaw_name = "#eta_{#tilde{#nu} #tilde{#nu}} = #eta_{b #it{l} b #it{l}}";
  SetRapidityInvJigsaw NuNuR("NuNuR", jigsaw_name);
  INV.AddJigsaw(NuNuR);
  NuNuR.AddVisibleFrames(CM.GetListVisibleFrames());

  jigsaw_name = "min ( M_{#tilde{t} a}- M_{#tilde{t} b} )^{2}";
  //MinMassDiffInvJigsaw MinDeltaMt("MinDeltaMt", jigsaw_name, 2);
  ContraBoostInvJigsaw MinDeltaMt("MinDeltaMt", jigsaw_name);
  INV.AddJigsaw(MinDeltaMt);
  MinDeltaMt.AddInvisibleFrame(Na, 0);
  MinDeltaMt.AddInvisibleFrame(Nb, 1);
  MinDeltaMt.AddVisibleFrames(La+Ba, 0);
  MinDeltaMt.AddVisibleFrames(Lb+Bb, 1);
  MinDeltaMt.AddMassFrame(La, 0);
  MinDeltaMt.AddMassFrame(Lb, 1);

  // b-jet combinatoric jigsaws for all trees
  jigsaw_name = "Minimize M(b #it{l} )_{a} , M(b #it{l} )_{b}";

  MinMassesCombJigsaw MinBL("MinBL", jigsaw_name);
  B.AddJigsaw(MinBL);
  MinBL.AddFrames(La+Ba,0);
  MinBL.AddFrames(Lb+Bb,1);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);
  
  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree");

  treePlot->SetTree(B);
  treePlot->Draw("VisTree", "b-jet Jigsaws", true);

  treePlot->SetTree(INV);
  treePlot->Draw("InvTree", "Inivisible Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  std::string plot_title =
    "#tilde{t} #tilde{t} #rightarrow 2x [ b #tilde{#chi}^{ #pm}(#it{l} #tilde{#nu}) ]";
  char smasses[200];
  sprintf(smasses, "m_{#tilde{t}}= %.0f, m_{#tilde{#nu}}= %.0f", mT, mN);
  HistPlot* histPlot = new HistPlot("HistPlot", plot_title+" , "+std::string(smasses));

  histPlot->SetRebin(1);

  std::string R_MXN = "#frac{m_{#tilde{#chi}^{ #pm}} - m_{#tilde{#nu}}}";
  std::string R_MXD = "{m_{#tilde{t}} - m_{#tilde{#nu}}}";
  RFList<const HistPlotCategory> cat_list;
  for(int m = 0; m < NMX; m++){
    char smass[200], scat[50];
    sprintf(scat, "MX%d_%d", m+1, NMX+1);
    sprintf(smass, " = #frac{%d}{%d}", m+1, NMX+1);
    
    cat_list += histPlot->GetNewCategory(scat, "#scale[0.7]{"+R_MXN+R_MXD+std::string(smass)+"}");
  }

  //const HistPlotCategory& cat_Reco = histPlot->GetNewCategory("Reco",  smasses);
  const HistPlotCategory& cat_Reco = histPlot->GetNewCategory("Reco",
							      plot_title+" , "+std::string(smasses));
  
  const HistPlotVar& MCM    = histPlot->GetNewVar("MCM", "M_{CM} / m_{CM}^{ true}", 0., 2.);
  const HistPlotVar& Eb_Ta  = histPlot->GetNewVar("Eb_Ta",
      "E_{b a}^{#tilde{t} a} / E_{b a}^{#tilde{t} a true}", 0., 2.);
  const HistPlotVar& Eb_Tb  = histPlot->GetNewVar("Eb_Tb",
      "E_{b b}^{#tilde{t} b} / E_{b b}^{#tilde{t} b true}", 0., 2.);
  const HistPlotVar& El_Xa  = histPlot->GetNewVar("El_Xa",
      "E_{#it{l} a}^{#tilde{#chi}^{ #pm} a} / E_{#it{l} a}^{#tilde{#chi}^{ #pm} a true}", 0., 2.);
  const HistPlotVar& El_Xb  = histPlot->GetNewVar("El_Xb",
      "E_{#it{l} b}^{#tilde{#chi}^{ #pm} b} / E_{#it{l} b}^{#tilde{#chi}^{ #pm} b true}", 0., 2.);
  const HistPlotVar& cosCM  = histPlot->GetNewVar("cosCM","cos #theta_{CM}", -1., 1.);
  const HistPlotVar& cosTa  = histPlot->GetNewVar("cosTa","cos #theta_{#tilde{t} a}", -1., 1.);
  const HistPlotVar& cosTb  = histPlot->GetNewVar("cosTb","cos #theta_{#tilde{t} b}", -1., 1.);
  const HistPlotVar& cosXa  = histPlot->GetNewVar("cosXa","cos #theta_{#tilde{#chi}^{ #pm} a}", -1., 1.);
  const HistPlotVar& cosXb  = histPlot->GetNewVar("cosXb","cos #theta_{#tilde{#chi}^{ #pm} b}", -1., 1.);
  const HistPlotVar& DcosCM = histPlot->GetNewVar("DcosCM","#theta_{CM} - #theta_{CM}^{true}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosTa = histPlot->GetNewVar("Dcosta",
      "#theta_{#tilde{t} a} - #theta_{#tilde{t} a}^{true}", -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosTb = histPlot->GetNewVar("Dcostb",
      "#theta_{#tilde{t} b} - #theta_{#tilde{t} b}^{true}", -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosXa = histPlot->GetNewVar("DcosXa",
      "#theta_{#tilde{#chi}^{ #pm} a} - #theta_{#tilde{#chi}^{ #pm} a}^{true}", -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosXb = histPlot->GetNewVar("DcosXb",
      "#theta_{#tilde{#chi}^{ #pm} b} - #theta_{#tilde{#chi}^{ #pm} b}^{true}", -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& RMX = histPlot->GetNewVar("RMX", "#scale[0.6]{"+R_MXN+R_MXD+"}", 0., 1.);
  
  histPlot->AddPlot(MCM,    cat_list);
  histPlot->AddPlot(Eb_Ta, cat_list);
  histPlot->AddPlot(El_Xa, cat_list);
  histPlot->AddPlot(DcosCM,  cat_list);
  histPlot->AddPlot(DcosTa, cat_list);
  histPlot->AddPlot(DcosXa, cat_list);

  histPlot->AddPlot(MCM, Eb_Ta, cat_list[NMX/2]);
  histPlot->AddPlot(MCM, El_Xa, cat_list[NMX/2]);
  histPlot->AddPlot(Eb_Ta, Eb_Tb, cat_list[NMX/2]);
  histPlot->AddPlot(El_Xa, El_Xb, cat_list[NMX/2]);
  histPlot->AddPlot(Eb_Ta, El_Xa, cat_list[NMX/2]);
  histPlot->AddPlot(DcosCM, MCM,  cat_list[NMX/2]);
  histPlot->AddPlot(DcosTa, Eb_Ta, cat_list[NMX/2]);
  histPlot->AddPlot(DcosXa, El_Xa, cat_list[NMX/2]);
  histPlot->AddPlot(DcosCM, DcosTa, cat_list[NMX/2]);
  histPlot->AddPlot(DcosTa, DcosTb, cat_list[NMX/2]);
  histPlot->AddPlot(DcosXa, DcosXb, cat_list[NMX/2]);

  if(NMX > 1){
    histPlot->AddPlot(MCM,    RMX, cat_Reco);
    histPlot->AddPlot(Eb_Ta,  RMX, cat_Reco);
    histPlot->AddPlot(El_Xa,  RMX, cat_Reco);
    histPlot->AddPlot(DcosTa, RMX, cat_Reco);
    histPlot->AddPlot(DcosXa, RMX, cat_Reco);
    histPlot->AddPlot(Eb_Tb,  RMX, cat_Reco);
    histPlot->AddPlot(El_Xb,  RMX, cat_Reco);
    histPlot->AddPlot(DcosTb, RMX, cat_Reco);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int m = 0; m < NMX; m++){
    g_Log << LogInfo << "Generating events for ";
    g_Log << "(mXp - mN) / (mT - mN)";
    g_Log << " = " << m+1 << " / " << NMX+1 << LogEnd;

    RMX = double(m+1)/double(NMX+1);
    Xa_Gen.SetMass(RMX*mT + (1.-RMX)*mN);

    if(fix_MXb){
      Xb_Gen.SetMass(mXb);
    } else {
      Xb_Gen.SetMass(RMX*mT + (1.-RMX)*mN);
    }
    
    LAB_Gen.InitializeAnalysis();
     
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

      // generate event
      LAB_Gen.ClearEvent();                             // clear the gen tree
  
      LAB_Gen.AnalyzeEvent();                           // generate a new event

      TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
      MET.SetZ(0.);

      // analyze event
      LAB.ClearEvent();                                  // clear the event
             
      INV.SetLabFrameThreeVector(MET);                   // Set MET in reco tree

      La.SetLabFrameFourVector(La_Gen.GetFourVector());  // Set lepton 4-vectors
      Lb.SetLabFrameFourVector(Lb_Gen.GetFourVector());

      B.AddLabFrameFourVector(Ba_Gen.GetFourVector());   // Set b-jet 4-vectors
      B.AddLabFrameFourVector(Bb_Gen.GetFourVector());
   
      LAB.AnalyzeEvent();                                // analyze the event

      //////////////////////////////////////
      // Observable Calculations
      //////////////////////////////////////

      double MCMgen   = CM_Gen.GetMass();
      double Eb_Tagen = Ba_Gen.GetFourVector(Ta_Gen).E();
      double Eb_Tbgen = Bb_Gen.GetFourVector(Tb_Gen).E();
      double El_Xagen = La_Gen.GetFourVector(Xa_Gen).E();
      double El_Xbgen = Lb_Gen.GetFourVector(Xb_Gen).E();
      double cosCMgen = CM_Gen.GetCosDecayAngle();
      double cosTagen = Ta_Gen.GetCosDecayAngle();
      double cosTbgen = Tb_Gen.GetCosDecayAngle();
      double cosXagen = Xa_Gen.GetCosDecayAngle();
      double cosXbgen = Xb_Gen.GetCosDecayAngle();

      MCM   = CM.GetMass()/MCMgen;
      Eb_Ta = Ba.GetFourVector(Ta).E()/Eb_Tagen;
      Eb_Tb = Bb.GetFourVector(Tb).E()/Eb_Tbgen;
      El_Xa = La.GetFourVector(Xa).E()/El_Xagen;
      El_Xb = Lb.GetFourVector(Xb).E()/El_Xbgen;
      cosCM = CM.GetCosDecayAngle();
      cosTa = Ta.GetCosDecayAngle();
      cosTb = Tb.GetCosDecayAngle();
      cosXa = Xa.GetCosDecayAngle();
      cosXb = Xb.GetCosDecayAngle();
      DcosCM = asin(sqrt(1.-cosCM*cosCM)*cosCMgen-sqrt(1.-cosCMgen*cosCMgen)*cosCM);
      DcosTa = asin(sqrt(1.-cosTa*cosTa)*cosTagen-sqrt(1.-cosTagen*cosTagen)*cosTa);
      DcosTb = asin(sqrt(1.-cosTb*cosTb)*cosTbgen-sqrt(1.-cosTbgen*cosTbgen)*cosTb);
      DcosXa = asin(sqrt(1.-cosXa*cosXa)*cosXagen-sqrt(1.-cosXagen*cosXagen)*cosXa);
      DcosXb = asin(sqrt(1.-cosXb*cosXb)*cosXbgen-sqrt(1.-cosXbgen*cosXbgen)*cosXb);

      histPlot->Fill(cat_list[m]);
      histPlot->Fill(cat_Reco);
    }

    LAB_Gen.PrintGeneratorEfficiency();
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
  example_DiStop_to_bXp_bXm_to_blNblN();
  return 0;
}
#endif
