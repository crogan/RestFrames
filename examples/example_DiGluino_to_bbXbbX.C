/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_DiGluino_to_bbXbbX.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 April
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

void example_DiGluino_to_bbXbbX(std::string output_name =
				"output_DiGluino_to_bbXbbX.root"){
  double mP = 1000.;
  double mC = 900.;
  double mX = 100.;
  double mB = 4.2;

  // Number of events to generate
  int Ngen = 10000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  // di-sbottom pair production
  ppLabGenFrame     LAB_Gen1("LAB_Gen1","LAB");
  DecayGenFrame     PP_Gen1("PP_Gen1","#tilde{b} #tilde{b}");
  DecayGenFrame     Pa_Gen1("Pa_Gen1","#tilde{b}_{a}");
  DecayGenFrame     Pb_Gen1("Pb_Gen1","#tilde{b}_{b}");
  VisibleGenFrame   B1a_Gen1("B1a_Gen1","b_{1a}");
  InvisibleGenFrame Xa_Gen1("Xa_Gen1","#tilde{#chi}_{a}");
  VisibleGenFrame   B1b_Gen1("B1b_Gen1","b_{1b}");
  InvisibleGenFrame Xb_Gen1("Xb_Gen1","#tilde{#chi}_{b}");

  // di-gluino pair production with decays through on-shell sbottoms
  ppLabGenFrame     LAB_Gen2("LAB_Gen2","LAB");
  DecayGenFrame     PP_Gen2("PP_Gen2","#tilde{g} #tilde{g}");
  DecayGenFrame     Pa_Gen2("Pa_Gen2","#tilde{g}_{a}");
  DecayGenFrame     Pb_Gen2("Pb_Gen2","#tilde{g}_{b}");
  DecayGenFrame     Ca_Gen2("Ca_Gen2","#tilde{b}_{a}");
  DecayGenFrame     Cb_Gen2("Cb_Gen2","#tilde{b}_{b}");
  VisibleGenFrame   B1a_Gen2("B1a_Gen2","b_{1a}");
  VisibleGenFrame   B2a_Gen2("B2a_Gen2","b_{2a}");
  InvisibleGenFrame Xa_Gen2("Xa_Gen2","#tilde{#chi}_{a}");
  VisibleGenFrame   B1b_Gen2("B1b_Gen2","b_{1b}");
  VisibleGenFrame   B2b_Gen2("B2b_Gen2","b_{2b}");
  InvisibleGenFrame Xb_Gen2("Xb_Gen2","#tilde{#chi}_{b}");

  // di-gluino pair production with decays three-body decays through virtual sbottoms
  ppLabGenFrame     LAB_Gen3("LAB_Gen3","LAB");
  DecayGenFrame     PP_Gen3("PP_Gen3","#tilde{g} #tilde{g}");
  DecayGenFrame     Pa_Gen3("Pa_Gen3","#tilde{g}_{a}");
  DecayGenFrame     Pb_Gen3("Pb_Gen3","#tilde{g}_{b}");
  VisibleGenFrame   B1a_Gen3("B1a_Gen3","b_{1a}");
  VisibleGenFrame   B2a_Gen3("B2a_Gen3","b_{2a}");
  InvisibleGenFrame Xa_Gen3("Xa_Gen3","#tilde{#chi}_{a}");
  VisibleGenFrame   B1b_Gen3("B1b_Gen3","b_{1b}");
  VisibleGenFrame   B2b_Gen3("B2b_Gen3","b_{2b}");
  InvisibleGenFrame Xb_Gen3("Xb_Gen3","#tilde{#chi}_{b}");

  // di-gluino pair production with mixed decays, one through sbottom, one not
  ppLabGenFrame     LAB_Gen4("LAB_Gen4","LAB");
  DecayGenFrame     PP_Gen4("PP_Gen4","#tilde{g} #tilde{g}");
  DecayGenFrame     Pa_Gen4("Pa_Gen4","#tilde{g}_{a}");
  DecayGenFrame     Pb_Gen4("Pb_Gen4","#tilde{g}_{b}");
  DecayGenFrame     Ca_Gen4("Ca_Gen4","#tilde{b}_{a}");
  VisibleGenFrame   B1a_Gen4("B1a_Gen4","b_{1a}");
  VisibleGenFrame   B2a_Gen4("B2a_Gen4","b_{2a}");
  InvisibleGenFrame Xa_Gen4("Xa_Gen4","#tilde{#chi}_{a}");
  VisibleGenFrame   B1b_Gen4("B1b_Gen4","b_{1b}");
  VisibleGenFrame   B2b_Gen4("B2b_Gen4","b_{2b}");
  InvisibleGenFrame Xb_Gen4("Xb_Gen4","#tilde{#chi}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen1.SetChildFrame(PP_Gen1);
  PP_Gen1.AddChildFrame(Pa_Gen1);
  PP_Gen1.AddChildFrame(Pb_Gen1);
  Pa_Gen1.AddChildFrame(B1a_Gen1);
  Pa_Gen1.AddChildFrame(Xa_Gen1);
  Pb_Gen1.AddChildFrame(B1b_Gen1);
  Pb_Gen1.AddChildFrame(Xb_Gen1);

  LAB_Gen2.SetChildFrame(PP_Gen2);
  PP_Gen2.AddChildFrame(Pa_Gen2);
  PP_Gen2.AddChildFrame(Pb_Gen2);
  Pa_Gen2.AddChildFrame(B1a_Gen2);
  Pa_Gen2.AddChildFrame(Ca_Gen2);
  Ca_Gen2.AddChildFrame(B2a_Gen2);
  Ca_Gen2.AddChildFrame(Xa_Gen2);
  Pb_Gen2.AddChildFrame(B1b_Gen2);
  Pb_Gen2.AddChildFrame(Cb_Gen2);
  Cb_Gen2.AddChildFrame(B2b_Gen2);
  Cb_Gen2.AddChildFrame(Xb_Gen2);

  LAB_Gen3.SetChildFrame(PP_Gen3);
  PP_Gen3.AddChildFrame(Pa_Gen3);
  PP_Gen3.AddChildFrame(Pb_Gen3);
  Pa_Gen3.AddChildFrame(B1a_Gen3);
  Pa_Gen3.AddChildFrame(B2a_Gen3);
  Pa_Gen3.AddChildFrame(Xa_Gen3);
  Pb_Gen3.AddChildFrame(B1b_Gen3);
  Pb_Gen3.AddChildFrame(B2b_Gen3);
  Pb_Gen3.AddChildFrame(Xb_Gen3);

  LAB_Gen4.SetChildFrame(PP_Gen4);
  PP_Gen4.AddChildFrame(Pa_Gen4);
  PP_Gen4.AddChildFrame(Pb_Gen4);
  Pa_Gen4.AddChildFrame(B1a_Gen4);
  Pa_Gen4.AddChildFrame(Ca_Gen4);
  Ca_Gen4.AddChildFrame(B2a_Gen4);
  Ca_Gen4.AddChildFrame(Xa_Gen4);
  Pb_Gen4.AddChildFrame(B1b_Gen4);
  Pb_Gen4.AddChildFrame(B2b_Gen4);
  Pb_Gen4.AddChildFrame(Xb_Gen4);

  if(LAB_Gen1.InitializeTree() && LAB_Gen2.InitializeTree() && LAB_Gen3.InitializeTree() && LAB_Gen4.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  PP_Gen1.SetVariableMass();
  // set sbottom masses
  Pa_Gen1.SetMass(mP);
  Pb_Gen1.SetMass(mP);
  // set X masses
  Xa_Gen1.SetMass(mX);
  Xb_Gen1.SetMass(mX);
  // set b-jet pT and eta cuts
  B1a_Gen1.SetMass(mB);                   B1b_Gen1.SetMass(mB);
  B1a_Gen1.SetPtCut(30.);                 B1a_Gen1.SetEtaCut(2.5);  
  B1b_Gen1.SetPtCut(30.);                 B1b_Gen1.SetEtaCut(2.5);  

  PP_Gen2.SetVariableMass();
  // set gluino masses
  Pa_Gen2.SetMass(mP);
  Pb_Gen2.SetMass(mP);
  // set intermediate sbottom masses
  Ca_Gen2.SetMass(mC);
  Cb_Gen2.SetMass(mC);
  // set X masses
  Xa_Gen2.SetMass(mX);
  Xb_Gen2.SetMass(mX);
  // set b-jet pT and eta cuts
  B1a_Gen2.SetMass(mB);                   B1b_Gen2.SetMass(mB);
  B2a_Gen2.SetMass(mB);                   B2b_Gen2.SetMass(mB);
  B1a_Gen2.SetPtCut(30.);                 B1a_Gen2.SetEtaCut(2.5);  
  B1b_Gen2.SetPtCut(30.);                 B1b_Gen2.SetEtaCut(2.5);  
  B2a_Gen2.SetPtCut(30.);                 B2a_Gen2.SetEtaCut(2.5);  
  B2b_Gen2.SetPtCut(30.);                 B2b_Gen2.SetEtaCut(2.5);  

  PP_Gen3.SetVariableMass();
  // set gluino masses
  Pa_Gen3.SetMass(mP);
  Pb_Gen3.SetMass(mP);
  // set X masses
  Xa_Gen3.SetMass(mX);
  Xb_Gen3.SetMass(mX);
  // set b-jet pT and eta cuts
  B1a_Gen3.SetMass(mB);                   B1b_Gen3.SetMass(mB);
  B2a_Gen3.SetMass(mB);                   B2b_Gen3.SetMass(mB);
  B1a_Gen3.SetPtCut(30.);                 B1a_Gen3.SetEtaCut(2.5);  
  B1b_Gen3.SetPtCut(30.);                 B1b_Gen3.SetEtaCut(2.5);  
  B2a_Gen3.SetPtCut(30.);                 B2a_Gen3.SetEtaCut(2.5);  
  B2b_Gen3.SetPtCut(30.);                 B2b_Gen3.SetEtaCut(2.5);  

  PP_Gen4.SetVariableMass();
  // set gluino masses
  Pa_Gen4.SetMass(mP);
  Pb_Gen4.SetMass(mP);
  // set intermediate sbottom mass
  Ca_Gen4.SetMass(mC);
  // set X masses
  Xa_Gen4.SetMass(mX);
  Xb_Gen4.SetMass(mX);
  // set b-jet pT and eta cuts
  B1a_Gen4.SetMass(mB);                   B1b_Gen4.SetMass(mB);
  B2a_Gen4.SetMass(mB);                   B2b_Gen4.SetMass(mB);
  B1a_Gen4.SetPtCut(30.);                 B1a_Gen4.SetEtaCut(2.5);  
  B1b_Gen4.SetPtCut(30.);                 B1b_Gen4.SetEtaCut(2.5);  
  B2a_Gen4.SetPtCut(30.);                 B2a_Gen4.SetEtaCut(2.5);  
  B2b_Gen4.SetPtCut(30.);                 B2b_Gen4.SetEtaCut(2.5);  

  if(LAB_Gen1.InitializeAnalysis() && LAB_Gen2.InitializeAnalysis() && LAB_Gen3.InitializeAnalysis() && LAB_Gen4.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analyses" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analyses" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
									       
  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB("LAB","LAB");
  DecayRecoFrame     PP("PP","#tilde{P} #tilde{P}");
  DecayRecoFrame     Pa("Pa","#tilde{P}_{a}");
  DecayRecoFrame     Pb("Pb","#tilde{P}_{b}");
  DecayRecoFrame     Ca("Ca","C_{a}");
  DecayRecoFrame     Cb("Cb","C_{b}");
  VisibleRecoFrame   B1a("B1a","b_{1a}");
  VisibleRecoFrame   B2a("B2a","b_{2a}");
  InvisibleRecoFrame Xa("Xa","#tilde{#chi}_{a}");
  VisibleRecoFrame   B1b("B1b","b_{1b}");
  VisibleRecoFrame   B2b("B2b","b_{2b}");
  InvisibleRecoFrame Xb("Xb","#tilde{#chi}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(PP);
  PP.AddChildFrame(Pa);
  PP.AddChildFrame(Pb);
  Pa.AddChildFrame(B1a);
  Pa.AddChildFrame(Ca);
  Ca.AddChildFrame(B2a);
  Ca.AddChildFrame(Xa);
  Pb.AddChildFrame(B1b);
  Pb.AddChildFrame(Cb);
  Cb.AddChildFrame(B2b);
  Cb.AddChildFrame(Xb);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  InvisibleGroup INV("INV","WIMP Jigsaws");
  INV.AddFrame(Xa);
  INV.AddFrame(Xb);
  
  CombinatoricGroup VIS("VIS","Visible Object Jigsaws");
  // visible frames in first decay step can have zero elements
  VIS.AddFrame(B1a);
  VIS.AddFrame(B1b);
  VIS.SetNElementsForFrame(B1a, 1);
  VIS.SetNElementsForFrame(B1b, 1);
  // visible frames in second decay step must have at least one element
  VIS.AddFrame(B2a);
  VIS.AddFrame(B2b);
  VIS.SetNElementsForFrame(B2a, 0);
  VIS.SetNElementsForFrame(B2b, 0);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  SetMassInvJigsaw XX_Mass("XX_Mass", "Set M_{#tilde{#chi} #tilde{#chi}}");
  INV.AddJigsaw(XX_Mass);

  SetRapidityInvJigsaw XX_Rapid("XX_Rapid", "Set #eta_{#tilde{#chi} #tilde{#chi}}");
  INV.AddJigsaw(XX_Rapid);
  XX_Rapid.AddVisibleFrames(LAB.GetListVisibleFrames());
  
  ContraBoostInvJigsaw XX_MinMP("CONTRA","Contraboost invariant Jigsaw");
  INV.AddJigsaw(XX_MinMP);
  XX_MinMP.AddVisibleFrames(Pa.GetListVisibleFrames(), 0);
  XX_MinMP.AddVisibleFrames(Pb.GetListVisibleFrames(), 1);
  XX_MinMP.AddInvisibleFrame(Xa, 0);
  XX_MinMP.AddInvisibleFrame(Xb, 1);

  MinMassesCombJigsaw B_split_ab("B_split_ab","Minimize M_{b_{a,b}}");
  VIS.AddJigsaw(B_split_ab);
  B_split_ab.AddFrame(B1a,0);
  B_split_ab.AddFrame(B2a,0);
  B_split_ab.AddFrame(B1b,1);
  B_split_ab.AddFrame(B2b,1);

  MinMassesSqCombJigsaw B_split_12a("B_split_12a","Minimize M _{C_{a}}",2,2);
  VIS.AddJigsaw(B_split_12a);
  B_split_12a.AddCombFrame(B1a,0);
  B_split_12a.AddCombFrame(B2a,1);
  B_split_12a.AddObjectFrame(B1a,0);
  B_split_12a.AddObjectFrame(B2a,1);
  B_split_12a.AddObjectFrame(Xa,0);

  MinMassesSqCombJigsaw B_split_12b("B_split_12b","Minimize M _{C_{b}}",2,2);
  VIS.AddJigsaw(B_split_12b);
  B_split_12b.AddCombFrame(B1b,0);
  B_split_12b.AddCombFrame(B2b,1);
  B_split_12b.AddObjectFrame(B1b,0);
  B_split_12b.AddObjectFrame(B2b,1);
  B_split_12b.AddObjectFrame(Xb,0);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
 
  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
  
  treePlot->SetTree(LAB_Gen1);
  treePlot->Draw("GenTree1", "Di-sbottom generator tree");

  treePlot->SetTree(LAB_Gen2);
  treePlot->Draw("GenTree2", "Di-gluino w/ sbottom generator tree");

  treePlot->SetTree(LAB_Gen3);
  treePlot->Draw("GenTree3", "Di-gluino generator tree");

  treePlot->SetTree(LAB_Gen4);
  treePlot->Draw("GenTree4", "Di-gluino mixed decay generator tree");

  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction tree");

  treePlot->SetTree(INV);
  treePlot->Draw("InvJigsaws", "Inivisible Jigsaws", true);
  
  treePlot->SetTree(VIS);
  treePlot->Draw("VisJigsaws", "Combinatoric Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow #tilde{g}#tilde{g} #rightarrow bbbb#chi#chi");
  histPlot->SetRebin(1);

  const HistPlotCategory& cat_bb   = histPlot->GetNewCategory("Reco_bb",   "2 x #tilde{b} #rightarrow b #tilde{#chi}");
  const HistPlotCategory& cat_ggbb = histPlot->GetNewCategory("Reco_ggbb", "2 x #tilde{g} #rightarrow b #tilde{b}(b#tilde{#chi})");
  const HistPlotCategory& cat_gg   = histPlot->GetNewCategory("Reco_gg",   "2 x #tilde{g} #rightarrow bb #tilde{#chi}");
  const HistPlotCategory& cat_ggb  = histPlot->GetNewCategory("Reco_ggb",  "2 x #tilde{g} mixed decays");

  RFList<const HistPlotCategory> cats_all;
  cats_all += cat_bb; cats_all += cat_ggbb; cats_all += cat_gg; cats_all += cat_ggb;
  RFList<const HistPlotCategory> cats_4b;
  cats_4b += cat_ggbb; cats_4b += cat_gg; cats_4b += cat_ggb;

  double Mdelta = (mP*mP-mX*mX)/mP; 
  
  const HistPlotVar& H11PP = histPlot->GetNewVar("H11PP",
	"H_{1,1}^{ #scale[0.8]{#tilde{P}#tilde{P}}} / 2(#gamma M_{#Delta})^{true}", 0., 1.);
  const HistPlotVar& H42PP = histPlot->GetNewVar("H42PP", "H_{4,2}^{ #scale[0.8]{#tilde{P}#tilde{P}}} / m_{#tilde{P}#tilde{P}}^{true}", 0., 1.8);
  const HistPlotVar& HT42PP = histPlot->GetNewVar("HT42PP", "H_{T,4,2}^{ #scale[0.8]{#tilde{P}#tilde{P}}} / m_{#tilde{P}#tilde{P}}^{true}", 0., 1.5);
  const HistPlotVar& H11Pa = histPlot->GetNewVar("H11Pa", "H_{1,1}^{ #scale[0.8]{#tilde{P}_{a}}}", 0., 2000., "[GeV]");
  const HistPlotVar& H11Pb = histPlot->GetNewVar("H11Pb", "H_{1,1}^{ #scale[0.8]{#tilde{P}_{b}}}", 0., 2000., "[GeV]");
  const HistPlotVar& H21Pa = histPlot->GetNewVar("H21Pa", "H_{2,1}^{ #scale[0.8]{#tilde{P}_{a}}}", 0., 2000., "[GeV]");
  const HistPlotVar& H21Pb = histPlot->GetNewVar("H21Pb", "H_{2,1}^{ #scale[0.8]{#tilde{P}_{b}}}", 0., 2000., "[GeV]");

  const HistPlotVar& H11o42PP   = histPlot->GetNewVar("H11o42PP",
	"H_{1,1}^{ #scale[0.8]{#tilde{P}#tilde{P}}} / H_{4,2}^{ #scale[0.8]{#tilde{P}#tilde{P}}}", 0., 1.);
  const HistPlotVar& H21Pao42PP = histPlot->GetNewVar("H21Pao42PP",
	"H_{2,1}^{ #scale[0.8]{#tilde{P}_{a}}} / H_{4,2}^{ #scale[0.8]{#tilde{P}#tilde{P}}}", 0., 1.);
  const HistPlotVar& H21Pbo42PP = histPlot->GetNewVar("H21Pbo42PP",
	"H_{2,1}^{ #scale[0.8]{#tilde{P}_{b}}} / H_{4,2}^{ #scale[0.8]{#tilde{P}#tilde{P}}}", 0., 1.);
  const HistPlotVar& H11o21Pa   = histPlot->GetNewVar("H11o21Pa",
	"H_{1,1}^{ #scale[0.8]{#tilde{P}_{a}}} / H_{2,1}^{ #scale[0.8]{#tilde{P}_{a}}}", 0., 1.);
  const HistPlotVar& H11o21Pb   = histPlot->GetNewVar("H11o21Pb",
	"H_{1,1}^{ #scale[0.8]{#tilde{P}_{b}}} / H_{2,1}^{ #scale[0.8]{#tilde{P}_{b}}}", 0., 1.);
  
  histPlot->AddPlot(H11PP, cats_all);
  histPlot->AddPlot(H42PP, cats_all);

  histPlot->AddPlot(H11o42PP,   cats_all);  
  histPlot->AddPlot(H21Pao42PP, cats_all); 
  histPlot->AddPlot(H21Pbo42PP, cats_all);
  histPlot->AddPlot(H11o21Pa,   cats_4b); 
  histPlot->AddPlot(H11o21Pb,   cats_4b);

  histPlot->AddPlot(H42PP, H11PP, cats_4b);
  histPlot->AddPlot(H11o21Pa, H11o21Pb, cats_4b);
 
  
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event type 1 event
    LAB_Gen1.ClearEvent();                            // clear the gen tree
    LAB_Gen1.AnalyzeEvent();                          // generate a new event

    // generate event type 2 event
    LAB_Gen2.ClearEvent();                            // clear the gen tree
    LAB_Gen2.AnalyzeEvent();                          // generate a new event

    // generate event type 3 event
    LAB_Gen3.ClearEvent();                            // clear the gen tree
    LAB_Gen3.AnalyzeEvent();                          // generate a new event

    // generate event type 4 event
    LAB_Gen4.ClearEvent();                            // clear the gen tree
    LAB_Gen4.AnalyzeEvent();                          // generate a new event

    // analyze four different events with same tree
    std::vector<TLorentzVector> JETS; 
    TVector3 MET;

    double mPP;
    double mP;
    
    for(int isig = 1; isig <= 4; isig++){
      if(isig == 1){
	MET = LAB_Gen1.GetInvisibleMomentum();    // Get the MET from gen tree
	MET.SetZ(0.);
	JETS.clear();                             // Get the Jets from gen tree
	JETS.push_back(B1a_Gen1.GetFourVector());
	JETS.push_back(B1b_Gen1.GetFourVector());
	mPP = PP_Gen1.GetMass();
	mP  = Pa_Gen1.GetMass();
      }
      if(isig == 2){
	MET = LAB_Gen2.GetInvisibleMomentum();    // Get the MET from gen tree
	MET.SetZ(0.);
	JETS.clear();                             // Get the Jets from gen tree
	TLorentzVector p1 = B1a_Gen2.GetFourVector();
	TLorentzVector p2 = B2a_Gen2.GetFourVector();
	if(p1.Pt() > p2.Pt()){
	  JETS.push_back(p1);
	  JETS.push_back(p2);
	} else {
	  JETS.push_back(p2);
	  JETS.push_back(p1);
	}
	JETS.push_back(B1b_Gen2.GetFourVector());
	JETS.push_back(B2b_Gen2.GetFourVector());
	mPP = PP_Gen2.GetMass();
	mP  = Pa_Gen2.GetMass();
      }
      if(isig == 3){
	MET = LAB_Gen3.GetInvisibleMomentum();    // Get the MET from gen tree
	MET.SetZ(0.);
	JETS.clear();                             // Get the Jets from gen tree
	TLorentzVector p1 = B1a_Gen3.GetFourVector();
	TLorentzVector p2 = B2a_Gen3.GetFourVector();
	if(p1.Pt() > p2.Pt()){
	  JETS.push_back(p1);
	  JETS.push_back(p2);
	} else {
	  JETS.push_back(p2);
	  JETS.push_back(p1);
	}
	JETS.push_back(B1b_Gen3.GetFourVector());
	JETS.push_back(B2b_Gen3.GetFourVector());
	mPP = PP_Gen3.GetMass();
	mP  = Pa_Gen3.GetMass();
      }
      if(isig == 4){
	MET = LAB_Gen3.GetInvisibleMomentum();    // Get the MET from gen tree
	MET.SetZ(0.);
	JETS.clear();                             // Get the Jets from gen tree
	TLorentzVector p1 = B1a_Gen4.GetFourVector();
	TLorentzVector p2 = B2a_Gen4.GetFourVector();
	if(p1.Pt() > p2.Pt()){
	  JETS.push_back(p1);
	  JETS.push_back(p2);
	} else {
	  JETS.push_back(p2);
	  JETS.push_back(p1);
	}
	JETS.push_back(B1b_Gen4.GetFourVector());
	JETS.push_back(B2b_Gen4.GetFourVector());
	mPP = PP_Gen4.GetMass();
	mP  = Pa_Gen4.GetMass();
      }

      LAB.ClearEvent();                              // clear reconstruction tree
      INV.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
      std::vector<RFKey> jetID;                      // ID for tracking jets in tree
      for(int i = 0; i < int(JETS.size()); i++) 
	jetID.push_back(VIS.AddLabFrameFourVector(JETS[i]));
      LAB.AnalyzeEvent();                            // analyze the event

      if(VIS.GetNElementsInFrame(B1a)+VIS.GetNElementsInFrame(B2a) !=
	 VIS.GetNElementsInFrame(B1b)+VIS.GetNElementsInFrame(B2b))
	continue;
      
      //////////////////////////////////////
      // Observable Calculations
      //////////////////////////////////////
      
      TLorentzVector vP_B1aPP = B1a.GetFourVector(PP);
      TLorentzVector vP_B2aPP = B2a.GetFourVector(PP);
      TLorentzVector vP_B1bPP = B1b.GetFourVector(PP);
      TLorentzVector vP_B2bPP = B2b.GetFourVector(PP);
      TLorentzVector vP_IaPP  = Xa.GetFourVector(PP);
      TLorentzVector vP_IbPP  = Xb.GetFourVector(PP);
      
      H11PP  = (vP_B1aPP     + vP_B2aPP     + vP_B1bPP     + vP_B2bPP).P() + (vP_IaPP     + vP_IbPP).P();
      H42PP  =  vP_B1aPP.P() + vP_B2aPP.P() + vP_B1bPP.P() + vP_B2bPP.P() +   vP_IaPP.P() + vP_IbPP.P();
      HT42PP = PP.GetTransverseMomentum(B1a.GetFourVector())+PP.GetTransverseMomentum(B1b.GetFourVector())+
	       PP.GetTransverseMomentum(B2a.GetFourVector())+PP.GetTransverseMomentum(B2b.GetFourVector())+
               PP.GetTransverseMomentum(Xa.GetFourVector())+PP.GetTransverseMomentum(Xb.GetFourVector());
      
      TLorentzVector vP_B1aPa = B1a.GetFourVector(Pa);
      TLorentzVector vP_B2aPa = B2a.GetFourVector(Pa);
      TLorentzVector vP_IaPa  = Xa.GetFourVector(Pa);
      
      H11Pa = (vP_B1aPa     + vP_B2aPa).P() + vP_IaPP.P();
      H21Pa =  vP_B1aPa.P() + vP_B2aPa.P()  + vP_IaPP.P();

      TLorentzVector vP_B1bPb = B1b.GetFourVector(Pb);
      TLorentzVector vP_B2bPb = B2b.GetFourVector(Pb);
      TLorentzVector vP_IbPb  = Xb.GetFourVector(Pb);
     
      H11Pb = (vP_B1bPb     + vP_B2bPb).P() + vP_IbPP.P();
      H21Pb =  vP_B1bPb.P() + vP_B2bPb.P()  + vP_IbPP.P();
      
      RestFrame const& frameA = VIS.GetFrame(jetID[0]);
      if(B1b.IsSame(frameA) || B2b.IsSame(frameA)){
      	double flip = H11Pa;
      	H11Pa = H11Pb;
      	H11Pb = flip;
      	flip = H21Pa;
      	H21Pa = H21Pb;
      	H21Pb = flip;
      }

      H11o42PP = H11PP/H42PP;
      H21Pao42PP  = H21Pa/H42PP;
      H11o21Pa    = H11Pa/H21Pa;
      H21Pbo42PP  = H21Pb/H42PP;
      H11o21Pb    = H11Pb/H21Pb;

      H11PP /= Mdelta*mPP/mP;
      H42PP /= mPP;
      
      histPlot->Fill(cats_all[isig-1]);
    }
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
  example_DiGluino_to_bbXbbX();
  return 0;
}
#endif
