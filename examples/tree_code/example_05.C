#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TH1D.h>
#include <TStyle.h>

#include "RestFrames/RestFrames.hh"

using namespace std;
using namespace RestFrames;

void example_05(){
  HistPlot::SetStyle();

  /////////////////////////////////////////////////

  // Set up reco analysis tree
  g_Log << LogInfo << "Initializing reconstruction frames and tree" << LogEnd;
  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame TT_R("TT_R","t #bar{t}");
  DecayRecoFrame Ta_R("Ta_R","t_{a}");
  DecayRecoFrame Tb_R("Tb_R","t_{b}");
  DecayRecoFrame Wa_R("Wa_R","W_{a}");
  DecayRecoFrame Wb_R("Wb_R","W_{b}");
  VisibleRecoFrame Ba_R("Ba_R","b_{a}");
  VisibleRecoFrame La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame Bb_R("Bb_R","b_{b}");
  VisibleRecoFrame Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame Nb_R("Nb_R","#nu_{b}");
  LAB_R.SetChildFrame(TT_R);
  TT_R.AddChildFrame(Ta_R);
  TT_R.AddChildFrame(Tb_R);
  Ta_R.AddChildFrame(Ba_R);
  Ta_R.AddChildFrame(Wa_R);
  Tb_R.AddChildFrame(Bb_R);
  Tb_R.AddChildFrame(Wb_R);
  Wa_R.AddChildFrame(La_R);
  Wa_R.AddChildFrame(Na_R);
  Wb_R.AddChildFrame(Lb_R);
  Wb_R.AddChildFrame(Nb_R);

  if(LAB_R.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready for Group and Jigsaw initialization" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_R) << LogEnd;  

  // define groups for the reconstruction trees
  InvisibleGroup INV_R("INV_R","WIMP Jigsaws");
  INV_R.AddFrame(Na_R);
  INV_R.AddFrame(Nb_R);
  CombinatoricGroup B_R("VIS","b-jet Jigsaws");
  // visible frames in first decay step must always have at least one element
  B_R.AddFrame(Ba_R);
  B_R.AddFrame(Bb_R);
  B_R.SetNElementsForFrame(Ba_R,1,true);
  B_R.SetNElementsForFrame(Bb_R,1,true);

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);

  SetRapidityInvJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrames((LAB_R.GetListVisibleFrames()));

  ContraBoostInvJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrames((Ta_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrames((Tb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrame(Na_R, 0);
  ContraBoostJigsaw_R.AddInvisibleFrame(Nb_R, 1);

  MinMassesCombJigsaw HemiJigsaw_R("HEM_JIGSAW_R","Minimize m(b #it{l}) Jigsaw");
  B_R.AddJigsaw(HemiJigsaw_R);
  HemiJigsaw_R.AddFrames((Ta_R.GetListVisibleFrames()),0);
  HemiJigsaw_R.AddFrames((Tb_R.GetListVisibleFrames()),1);

  g_Log << LogInfo << "Initializing tree for analysis" << LogEnd;
  // check reconstruction trees
  if(LAB_R.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "...Successfully initialized tree for analysis from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready event analysis" << LogEnd;
  } else
    g_Log << LogError << "...Unable to initialize analysis from LabFrame: " << Log(LAB_R) << LogEnd;	

  //////////////////////////////////////////////////////////////
  // draw some pictures of our trees
  //////////////////////////////////////////////////////////////

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  
  // reco tree
  tree_plot->SetTree(LAB_R);
  tree_plot->Draw("RecoTree", "Reconstruction Tree");
  tree_plot->Draw("RecoTree_inv", "Reconstruction Tree", true);

  // Invisible Jigsaw tree
  tree_plot->SetTree(INV_R);
  tree_plot->Draw("InvTree", "Invisible Jigsaws");

  // Visible Jigsaw tree
  tree_plot->SetTree(B_R);
  tree_plot->Draw("VisTree", "Visible Jigsaws");

}
