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

void RestFramesTree(){
  HistPlot::SetStyle();

  /////////////////////////////////////////////////

  // Define the frames in our reconstruction tree
  LabRecoFrame        LAB("LAB","lab");
  DecayRecoFrame      CM("CM","");
  DecayRecoFrame      D1("D1","");
  DecayRecoFrame      D2("D2","");
  VisibleRecoFrame      D3("D3","F");
  DecayRecoFrame      D4("D4","");
  
  InvisibleRecoFrame   V1("V1","T");
  VisibleRecoFrame   V2("V2","R");
  VisibleRecoFrame   V3("V3","A");
  VisibleRecoFrame   V4("V4","M");
  VisibleRecoFrame   V5("V5","E");
  VisibleRecoFrame   V6("V6","");
  InvisibleRecoFrame I1("I1","E");
  InvisibleRecoFrame I2("I2","S");
  InvisibleRecoFrame I3("I3","");
  VisibleRecoFrame I4("I4","S");
  InvisibleRecoFrame I5("I5","R");
  InvisibleRecoFrame I6("I6","");

  // connect the tree according to our interpretation
  LAB.SetChildFrame(CM);
  CM.AddChildFrame(D1);
  CM.AddChildFrame(I5);
  CM.AddChildFrame(D2);

  D1.AddChildFrame(D3);
  D1.AddChildFrame(D4);
  D1.AddChildFrame(I1);
  
  D2.AddChildFrame(I2);
  D2.AddChildFrame(V1);
  
  //D3.AddChildFrame(I3);
  D4.AddChildFrame(V2);
  D4.AddChildFrame(V3);
  D4.AddChildFrame(V4);
  D4.AddChildFrame(V5);
  D4.AddChildFrame(I4);

  // U1.AddChildFrame(I1);
  // U1.AddChildFrame(U2);
  // U2.AddChildFrame(V2);
  // U2.AddChildFrame(V3);
  // U2.AddChildFrame(U3);
  // U3.AddChildFrame(I4);
  // U3.AddChildFrame(U4);
  // U4.AddChildFrame(V5);
  // U4.AddChildFrame(V6);
  

  // Initialize the tree skeleton (check for problems)
  if(!LAB.InitializeTree())
    cout << "PROBLEM with reco tree structure" << endl;

  // InvisibleGroup INV("INV","Invisible System");
  // INV.AddFrame(I);

  // CombinatoricGroup VIS("VIS","Visible Objects");
  // VIS.AddFrame(ISR);
  // VIS.SetNElementsForFrame(ISR,1,false);
  // VIS.AddFrame(V);
  // VIS.SetNElementsForFrame(V,0,false);

  // // set the invisible system mass to zero
  // SetMassInvJigsaw InvMass("InvMass", "Invisible system mass Jigsaw");
  // INV.AddJigsaw(InvMass);

  // // define the rule for partitioning objects between "ISR" and "V"
  // MinMassesCombJigsaw SplitVis("SplitVis","Minimize M_{ISR} and M_{S} Jigsaw");
  // VIS.AddJigsaw(SplitVis);
  // // "0" group (ISR)
  // SplitVis.AddFrame(ISR, 0);
  // // "1" group (V + I)
  // SplitVis.AddFrame(V,1);
  // SplitVis.AddFrame(I,1);

  // if(!LAB.InitializeAnalysis())
  //   cout << "PROBLEM with analysis set-up of tree" << endl;

  /////////////////////////////////////////////////////////

  // Draw the RestFrames tree if you like!
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  tree_plot->SetTree(CM);
  tree_plot->Draw("norm","norm");
  tree_plot->Draw("inv","inv",true);

}
