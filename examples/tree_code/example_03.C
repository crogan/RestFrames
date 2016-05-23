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

void example_03(){

  /////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame       LAB_Mt("LAB_Mt","LAB"); LabRecoFrame       LAB_MW("LAB_MW","LAB");
  DecayRecoFrame     T_Mt("T_Mt","t");       DecayRecoFrame     T_MW("T_MW","t");
  DecayRecoFrame     W_Mt("W_Mt","W");       DecayRecoFrame     W_MW("W_MW","W");
  VisibleRecoFrame   B_Mt("B_Mt","b");       VisibleRecoFrame   B_MW("B_MW","b");
  VisibleRecoFrame   L_Mt("L_Mt","#it{l}");  VisibleRecoFrame   L_MW("L_MW","#it{l}");
  InvisibleRecoFrame NU_Mt("NU_Mt","#nu");   InvisibleRecoFrame NU_MW("NU_MW","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Mt.SetChildFrame(T_Mt);                LAB_MW.SetChildFrame(T_MW);
  T_Mt.AddChildFrame(B_Mt);                  T_MW.AddChildFrame(B_MW);
  T_Mt.AddChildFrame(W_Mt);                  T_MW.AddChildFrame(W_MW);
  W_Mt.AddChildFrame(L_Mt);                  W_MW.AddChildFrame(L_MW);
  W_Mt.AddChildFrame(NU_Mt);                 W_MW.AddChildFrame(NU_MW);

  if(LAB_Mt.InitializeTree() && LAB_MW.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Groups
  InvisibleGroup INV_Mt("INV_Mt","#nu Jigsaws");
  INV_Mt.AddFrame(NU_Mt);     
  InvisibleGroup INV_MW("INV_MW","nu Jigsaws");
  INV_MW.AddFrame(NU_MW);
 
  // Set neutrino masses to zero
  SetMassInvJigsaw NuM_Mt("NuM_Mt","M_{#nu} = 0"); 
  INV_Mt.AddJigsaw(NuM_Mt);                        
  SetMassInvJigsaw NuM_MW("NuM_MW","M_{#nu} = 0");
  INV_MW.AddJigsaw(NuM_MW);

  // Set neutrino rapidity to that of visible particles
  SetRapidityInvJigsaw NuR_Mt("NuR_Mt","#eta_{#nu} = #eta_{b+#it{l}}");
  INV_Mt.AddJigsaw(NuR_Mt);
  NuR_Mt.AddVisibleFrame(L_Mt);
  NuR_Mt.AddVisibleFrame(B_Mt);
  SetRapidityInvJigsaw NuR_MW("NuR_Mt","#eta_{#nu} = #eta_{#it{l}}");
  INV_MW.AddJigsaw(NuR_MW);
  NuR_MW.AddVisibleFrame(L_MW);

  if(LAB_Mt.InitializeAnalysis() && LAB_MW.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");

  // Draw the RestFrames tree if you like!
  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
  tree_plot->SetTree(LAB_Mt);
  tree_plot->Draw("norm","norm");
  tree_plot->Draw("inv","inv",true);

}
