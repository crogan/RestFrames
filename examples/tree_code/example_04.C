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

void example_04(){

  /////////////////////////////////////////////////

   // Set up reco analysis tree
  g_Log << LogInfo << "Initializing reconstruction frames and tree" << LogEnd;
  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame H_R("H_R","H");
  DecayRecoFrame Wa_R("Wa_R","W_{a}");
  DecayRecoFrame Wb_R("Wb_R","W_{b}");
  VisibleRecoFrame La_R("La_R","#it{l}_{a}");
  InvisibleRecoFrame Na_R("Na_R","#nu_{a}");
  VisibleRecoFrame Lb_R("Lb_R","#it{l}_{b}");
  InvisibleRecoFrame Nb_R("Nb_R","#nu_{b}");
  LAB_R.SetChildFrame(H_R);
  H_R.AddChildFrame(Wa_R);
  H_R.AddChildFrame(Wb_R);
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

  // define jigsaws for the reconstruction tree
  SetMassInvJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);

  SetRapidityInvJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrames((LAB_R.GetListVisibleFrames()));

  ContraBoostInvJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrames((Wa_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrames((Wb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrames((Wa_R.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw_R.AddInvisibleFrames((Wb_R.GetListInvisibleFrames()), 1);

  g_Log << LogInfo << "Initializing tree for analysis" << LogEnd;
  // check reconstruction trees
  if(LAB_R.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "...Successfully initialized tree for analysis from LabFrame ";
    g_Log << LAB_R.GetName() << endl;
    g_Log << "Ready event analysis" << LogEnd;
  } else
    g_Log << LogError << "...Unable to initialize analysis from LabFrame: " << Log(LAB_R) << LogEnd;

  // Draw the RestFrames tree if you like!
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  tree_plot->SetTree(LAB_R);
  tree_plot->Draw("norm","norm");
  tree_plot->Draw("inv","inv",true);

}
