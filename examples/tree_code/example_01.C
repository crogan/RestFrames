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

void example_01(){
  /////////////////////////////////////////////////

  LabRecoFrame LAB("LAB","LAB");
  DecayRecoFrame Z("Z","Z");
  VisibleRecoFrame Lp("Lp","#it{l}^{ +}");
  VisibleRecoFrame Lm("Lm","#it{l}^{ -}");
  
  LAB.SetChildFrame(Z);
  Z.AddChildFrame(Lp);
  Z.AddChildFrame(Lm);

  if(!LAB.InitializeTree()) cout << "Problem with reconstruction tree" << endl; 

  // Draw the RestFrames tree if you like!
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  tree_plot->SetTree(LAB);
  tree_plot->Draw("norm","norm");
  tree_plot->Draw("inv","inv",true);

}
