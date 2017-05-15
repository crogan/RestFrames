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
  double mC = 500.;
  double mX = 100.;

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
  InvisibleGenFrame Xb_Gen3("Xb_Gen4","#tilde{#chi}_{b}");

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
  Pb_Gen4.AddChildFrame(Cb_Gen4);
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
  B1a_Gen4.SetPtCut(30.);                 B1a_Gen2.SetEtaCut(2.5);  
  B1b_Gen4.SetPtCut(30.);                 B1b_Gen2.SetEtaCut(2.5);  
  B2a_Gen4.SetPtCut(30.);                 B2a_Gen2.SetEtaCut(2.5);  
  B2b_Gen4.SetPtCut(30.);                 B2b_Gen2.SetEtaCut(2.5);  

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
  // visible frames in first decay step must always have at least one element
  VIS.AddFrame(B1a);
  VIS.AddFrame(B1b);
  VIS.SetNElementsForFrame(B1a, 1);
  VIS.SetNElementsForFrame(B1b, 1);
  // visible frames in second decay step can have zero elements
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

  MinMassesCombJigsaw B_split_12a("B_split_12a","Minimize M _{C_{a}}");
  VIS.AddJigsaw(B_split_12a);
  B_split_12a.AddFrame(B1a,0);
  B_split_12a.AddFrame(B2a,1);
  B_split_12a.AddFrame(Xa,1);

  MinMassesCombJigsaw B_split_12b("B_split_12b","Minimize M _{C_{b}}");
  VIS.AddJigsaw(B_split_12b);
  B_split_12b.AddFrame(B1b,0);
  B_split_12b.AddFrame(B2b,1);
  B_split_12b.AddFrame(Xb,1);

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

  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow #tilde{g}#tilde{g} \rightarrow (bb#chi)(bb#chi)");

  const HistPlotCategory& cat_bb   = histPlot->GetNewCategory("Reco_bb",   "$2 x \tilde{b} \rightarrow b #tilde{#chi}$");
  const HistPlotCategory& cat_ggbb = histPlot->GetNewCategory("Reco_ggbb", "$2 x \tilde{b} \rightarrow b #tilde{#chi}$");
  const HistPlotCategory& cat_gg   = histPlot->GetNewCategory("Reco_gg",   "$2 x \tilde{b} \rightarrow b #tilde{#chi}$");
  const HistPlotCategory& cat_ggb  = histPlot->GetNewCategory("Reco_ggb",  "$2 x \tilde{b} \rightarrow b #tilde{#chi}$");

  RFList<const HistPlotCategory> cats_all;
  cats_all += cat_bb; cats_all += cat_ggbb; cats_all += cat_gg; cats_all += cat_ggb;
  RFList<const HistPlotCategory> cats_4b;
  cats_4b += cat_ggbb; cats_4b += cat_gg; cats_4b += cat_ggb;


  const HistPlotVar& MCM    = histPlot->GetNewVar("MCM", "M_{CM} / m_{CM}^{ true}", 0.2, 1.8);
  const HistPlotVar& sumMW2 = histPlot->GetNewVar("MWTOT",
		   "#sqrt{#Sigma M_{W}^{ 2} / #Sigma m_{W, true}^{ 2}}", 0., 1.5);
  double sumMW2gen;
  
  histPlot->AddPlot(MCM,    cat_2W+cat_3W+cat_4W);
  histPlot->AddPlot(sumMW2, cat_2W+cat_3W+cat_4W);
  histPlot->AddPlot(MCM, sumMW2, cat_2W+cat_3W+cat_4W);

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

    // type 1 event
    MET = LAB_Gen1.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    JETS.clear();                             // Get the Jets from gen tree
    JETS.push_back(B1a_Gen1.GetFourVector());
    JETS.push_back(B1b_Gen1.GetFourVector());

    LAB.ClearEvent();                              // clear reconstruction tree
    INV.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    std::vector<RFKey> jetID1;                     // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID1.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    LAB.AnalyzeEvent();                            // analyze the event

    // type 2 event
    MET = LAB_Gen2.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    JETS.clear();                             // Get the Jets from gen tree
    JETS.push_back(B1a_Gen2.GetFourVector());
    JETS.push_back(B1b_Gen2.GetFourVector());
    JETS.push_back(B2a_Gen2.GetFourVector());
    JETS.push_back(B2b_Gen2.GetFourVector());

    LAB.ClearEvent();                              // clear reconstruction tree
    INV.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    std::vector<RFKey> jetID2;                     // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID2.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    LAB.AnalyzeEvent();                            // analyze the event

    // type 3 event
    MET = LAB_Gen3.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    JETS.clear();                             // Get the Jets from gen tree
    JETS.push_back(B1a_Gen3.GetFourVector());
    JETS.push_back(B1b_Gen3.GetFourVector());
    JETS.push_back(B2a_Gen3.GetFourVector());
    JETS.push_back(B2b_Gen3.GetFourVector());

    LAB.ClearEvent();                              // clear reconstruction tree
    INV.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    std::vector<RFKey> jetID3;                     // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID3.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    LAB.AnalyzeEvent();                            // analyze the event

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////
    /*
    //
    // signal tree observables
    //

    //*** total CM mass
    double shat = GG.GetMass();
    //*** 'mass-less' gluino gamma in CM frame
    double gaminv = GG.GetVisibleShape();
    
    TVector3 vPGG = GG.GetFourVector(LAB).Vect();
    
    //*** ratio of CM pT to CM mass
    double RPT = vPGG.Pt() / (vPGG.Pt() + shat/4.);
    //*** ratio of CM pz to CM mass
    double RPZ = vPGG.Pz() / (vPGG.Pz() + shat/4.);
    //*** cos decay angle of GG system
    double cosGG = GG.GetCosDecayAngle();
    //*** delta phi between lab and GG decay planes
    double dphiLGG = LAB.GetDeltaPhiDecayPlanes(GG);
    
    TLorentzVector vV1 = G[0]->GetVisibleFourVector(*G[0]);
    TLorentzVector vV2 = G[1]->GetVisibleFourVector(*G[1]);

    //*** gluino mass
    double MG = (vV1.M2()-vV2.M2())/(2.*(vV1.E()-vV2.E()));
    
    double PG = G[0]->GetMomentum(GG);
    double MGG = 2.*sqrt(PG*PG + MG*MG);
    double gaminvGG = 2.*MG/MGG;
    double beta = sqrt(1.- gaminv*gaminv);
    double betaGG = sqrt(1.- gaminvGG*gaminvGG);
    
    //*** velocity difference between 'massive' and 'mass-less'
    double DeltaBetaGG = -(betaGG-beta)/(1.-betaGG*beta);
    //*** delta phi between GG visible decay products and GG decay axis
    double dphiVG = GG.GetDeltaPhiDecayVisible();
    //*** delta phi between GG visible decay products and GG momentum
    double dphiVGG = GG.GetDeltaPhiBoostVisible();
    
    // 'hemisphere' (one for each 'gluino') observables

    //*** number of visible objects (jets) in hemisphere
    double NV[2];
    //*** cosine gluino decay angle
    double cosG[2];
    //*** cosine intermediate child decay angle
    double cosC[2];
    //*** delta phi between gluino and child decay planes
    double dphiGC[2];
    //*** ratio of child and gluino masses (w/ WIMP masses subtracted)
    double RCG[2];
    //*** 1st leading jet pT _associated with this hemisphere_
    double jet1PT[2];
    //*** 2nd leading jet pT _associated with this hemisphere_
    double jet2PT[2];
    //*** Pinv / HG
    double Pinv[2];
      
    for(int i = 0; i < 2; i++){
      NV[i] =  VIS.GetNElementsInFrame(*VS[i]);
      NV[i] += VIS.GetNElementsInFrame(*VC[i]);

      TVector3 vP1 = VS[i]->GetFourVector(*G[i]).Vect();
      TVector3 vP2 = VC[i]->GetFourVector(*G[i]).Vect();
      Pinv[i] = 2.*(vP1+vP2).Mag()/(vP1.Mag()+vP2.Mag()+(vP1+vP2).Mag());

      cosG[i] = G[i]->GetCosDecayAngle();

      int N = jetID.size();
      double pTmax[2]; pTmax[0] = -1.; pTmax[1] = -1.;
      for(int j = 0; j < N; j++){
	const RestFrame& frame = VIS.GetFrame(jetID[j]);
	if(VS[i]->IsSame(frame) || VC[i]->IsSame(frame)){
	  double pT = VIS.GetLabFrameFourVector(jetID[j]).Pt();
	  if(pT > pTmax[0]){
	    pTmax[1] = pTmax[0];
	    pTmax[0] = pT;
	  } else {
	    if(pT > pTmax[1]) pTmax[1] = pT;
	  }
	}
      }
      
      jet1PT[i] = pTmax[0];
      jet2PT[i] = pTmax[1];

      if(NV[i] > 1){
	cosC[i] = C[i]->GetCosDecayAngle();
	dphiGC[i] = G[i]->GetDeltaPhiDecayPlanes(*C[i]);
	RCG[i] = (C[i]->GetMass()-X[i]->GetMass())/(G[i]->GetMass()-X[i]->GetMass());
      } else {
	cosC[i] = -2.;
	dphiGC[i] = -1.;
	RCG[i] = -1.;
	jet2PT[i] = -1.;
      }
    }

    //
    // background tree observables
    //
    TLorentzVector Psib = I_B.GetSiblingFrame().GetFourVector(LAB_B);
    TLorentzVector Pmet = I_B.GetFourVector(LAB_B);
      
    //*** 
    double Rpsib = std::max(0.,Psib.Vect().Dot(Pmet.Vect().Unit()));
    Rpsib = Rpsib / (Pmet.Pt() + Rpsib);
    
    TVector3 boostQCD = (Pmet+Psib).BoostVector();
    Psib.Boost(-boostQCD);
    double cosQCD = -1.*Psib.Vect().Unit().Dot(boostQCD.Unit());
    cosQCD = (1.-cosQCD)/2.;

    //*** 
    double DeltaQCD = (cosQCD-Rpsib)/(cosQCD+Rpsib);
    */

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
