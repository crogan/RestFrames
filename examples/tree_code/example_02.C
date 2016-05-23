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

void example_02(){
  HistPlot::SetStyle();

  /////////////////////////////////////////////////

  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame W_R("W_R","W");
  VisibleRecoFrame L_R("L_R","#it{l}");
  InvisibleRecoFrame NU_R("NU_R","#nu");
  
  LAB_R.SetChildFrame(W_R);
  W_R.AddChildFrame(L_R);
  W_R.AddChildFrame(NU_R);

  if(!LAB_R.InitializeTree()) cout << "Problem with reconstruction tree" << endl; 

  // Draw the RestFrames tree if you like!
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  tree_plot->SetTree(LAB_R);
  tree_plot->Draw("norm","norm");
  tree_plot->Draw("inv","inv",true);

  // Now we add invisible jigsaws
  InvisibleGroup INV("INV","Neutrino Jigsaws");
  INV.AddFrame(NU_R);
 
  SetMassInvJigsaw MassJigsaw("MASS_JIGSAW","mass Jigsaw");
  INV.AddJigsaw(MassJigsaw);
  
  SetRapidityInvJigsaw RapidityJigsaw("RAPIDITY_JIGSAW","rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame(L_R);

  if(!LAB_R.InitializeAnalysis()) cout << "Problem with jigsaws" << endl;

  // Invisible Jigsaws
  tree_plot->SetTree(INV);
  tree_plot->Draw("InvTree", "InvisibleJigsaws");

}
