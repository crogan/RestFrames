/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_DiStop_to_hadtopXhadtopX.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 Aug
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

void example_DiStop_to_hadtopXhadtopX(const std::string& output_name =
			   "output_DiStop_to_hadtopXhadtopX.root"){

  // set particle masses and widths
  double mT   = 700;    // stop mass
  double mX   = 0;      // min neutralino mass
  double mtop = 173.21; // GeV, PDG 2016

  int NMX = 5;          // number of different neutralino masses to evaluate

  // Number of events to generate (per H mass)
  int Ngen = 10000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     TT_Gen("TT_Gen","#tilde{t} #tilde{t}");
  DecayGenFrame     Ta_Gen("Ta_Gen","#tilde{t}_{a}");
  DecayGenFrame     Tb_Gen("Tb_Gen","#tilde{t}_{b}");
  VisibleGenFrame   Thada_Gen("Thada_Gen","t_{had}^{ a}");
  InvisibleGenFrame Xa_Gen("Xa_Gen","#tilde{#chi}^{0}_{a}");
  VisibleGenFrame   Thadb_Gen("Thadb_Gen","t_{had}^{ b}");
  InvisibleGenFrame Xb_Gen("Xb_Gen","#tilde{#chi}^{0}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(TT_Gen);
  TT_Gen.AddChildFrame(Ta_Gen);
  TT_Gen.AddChildFrame(Tb_Gen);
  Ta_Gen.AddChildFrame(Thada_Gen);
  Ta_Gen.AddChildFrame(Xa_Gen);
  Tb_Gen.AddChildFrame(Thadb_Gen);
  Tb_Gen.AddChildFrame(Xb_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;								    
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  if(mX >= mT) return;
  
  // non-resonant stop production
  TT_Gen.SetVariableMass();
  // set stop masses
  Ta_Gen.SetMass(mT);            Tb_Gen.SetMass(mT);
  // set neutralino masses
  Xa_Gen.SetMass(mX);            Xb_Gen.SetMass(mX);
  // set hadronic top masses
  Thada_Gen.SetMass(mtop);       Thadb_Gen.SetMass(mtop);

  // set hadronic top pT and eta cuts
  Thada_Gen.SetPtCut(50.);       Thadb_Gen.SetPtCut(50.);
  Thada_Gen.SetEtaCut(2.5);      Thadb_Gen.SetEtaCut(2.5);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame         LAB("LAB","LAB");
  DecayRecoFrame       TT("TT","#tilde{t} #tilde{t}");
  DecayRecoFrame       Ta("Ta","#tilde{t}_{a}");
  DecayRecoFrame       Tb("Tb","#tilde{t}_{b}");
  VisibleRecoFrame     Thada("Thada","t_{had}^{ a}");
  InvisibleRecoFrame   Xa("Xa","#tilde{#chi}^{0}_{a}");
  VisibleRecoFrame     Thadb("Thadb","t_{had}^{ b}");
  InvisibleRecoFrame   Xb("Xb","#tilde{#chi}^{0}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(TT);
  TT.AddChildFrame(Ta);
  TT.AddChildFrame(Tb);
  Ta.AddChildFrame(Thada);
  Ta.AddChildFrame(Xa);
  Tb.AddChildFrame(Thadb);
  Tb.AddChildFrame(Xb);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;
 
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Group
  InvisibleGroup INV("INV","#nu #nu Jigsaws");
  INV.AddFrame(Xa);
  INV.AddFrame(Xb);

  // Set nu nu mass using visible hadronic tops
  SetMassInvJigsaw NuNuM("NuNuM", "M_{#nu#nu} ~ m_{t_{had} t_{had}}");
  INV.AddJigsaw(NuNuM);

  SetRapidityInvJigsaw NuNuR("NuNuR", "#eta_{#nu#nu} = #eta_{t_{had} t_{had}}");
  INV.AddJigsaw(NuNuR);
  NuNuR.AddVisibleFrames(LAB.GetListVisibleFrames());

  ContraBoostInvJigsaw MinMW("MinMW","min M_{top}, M_{top}^{ a}= M_{top}^{ b}");
  INV.AddJigsaw(MinMW);
  MinMW.AddVisibleFrame(Thada, 0);
  MinMW.AddVisibleFrame(Thadb, 1);
  MinMW.AddInvisibleFrame(Xa, 0);
  MinMW.AddInvisibleFrame(Xb, 1);

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

  treePlot->SetTree(INV);
  treePlot->Draw("InvTree", "Invisible Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  std::vector<double> vmX; // varying neutralino mass

  std::string plot_title = "#tilde{t} #tilde{t}#rightarrow t_{had} #tilde{#chi}^{0} t_{had} #tilde{#chi}^{0}";
  HistPlot* histPlot = new HistPlot("HistPlot", plot_title);

  RFList<const HistPlotCategory> cat_list;
  for(int m = 0; m < NMX; m++){
    vmX.push_back(mX + m*(mT-mX-200.)/double(NMX));
    char smass[50], scat[50];
    sprintf(scat, "MX%.0f", vmX[m]);
    sprintf(smass, "m_{#tilde{#chi}^{0}} = %.0f GeV", vmX[m]);
    cat_list += histPlot->GetNewCategory(scat, smass);
  }
  
  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen",  "Generator");

  const HistPlotVar& MTT = histPlot->GetNewVar("MTT", 
					       "M_{#tilde{t} #tilde{t}} / m_{#tilde{t} #tilde{t}}^{true}", 
					       0., 1.75);
  const HistPlotVar& EtTa   = histPlot->GetNewVar("EtTa", "E_{top}^{#tilde{t} a} / E_{top}^{#tilde{t} true}", 0., 1.1);
  const HistPlotVar& PtTa   = histPlot->GetNewVar("PtTa", "p_{top}^{#tilde{t} a} / p_{top}^{#tilde{t} true}", 0., 1.6);
  const HistPlotVar& cosTT  = histPlot->GetNewVar("cosTT","cos #theta_{#tilde{t} #tilde{t}}", -1., 1.);
  const HistPlotVar& cosTa  = histPlot->GetNewVar("cosTa","cos #theta_{#tilde{t}_{a}}", -1., 1.);
  const HistPlotVar& DcosTT = histPlot->GetNewVar("DcosTT",
						  "#theta_{#tilde{t} #tilde{t}} - #theta_{#tilde{t} #tilde{t}}", 
						  -acos(-1.)/2., acos(-1.)/2.);
  const HistPlotVar& DcosTa = histPlot->GetNewVar("DcosTa","#theta_{#tilde{t}_{a}} - #theta_{#tilde{t}_{a}}^{true}", 
						  -acos(-1.)/2., acos(-1.)/2.);
 
  histPlot->AddPlot(MTT,    cat_list);
  histPlot->AddPlot(EtTa,   cat_list);
  histPlot->AddPlot(PtTa,   cat_list);
  histPlot->AddPlot(DcosTT, cat_list);
  histPlot->AddPlot(DcosTa, cat_list);
  histPlot->AddPlot(MTT, EtTa,    cat_list[0]);
  histPlot->AddPlot(MTT, PtTa,    cat_list[0]);
  histPlot->AddPlot(EtTa, DcosTT, cat_list[0]);
  histPlot->AddPlot(EtTa, DcosTa, cat_list[1]);
  histPlot->AddPlot(DcosTT, DcosTa, cat_list[1]);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  for(int m = 0; m < NMX; m++){
    g_Log << LogInfo << "Generating events for X^{0} mass = " << vmX[m] << LogEnd;

    Xa_Gen.SetMass(vmX[m]);
    Xb_Gen.SetMass(vmX[m]);
    LAB_Gen.InitializeAnalysis();
  
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

      // generate event
      LAB_Gen.ClearEvent();                            // clear the gen tree
      
      LAB_Gen.AnalyzeEvent();                          // generate a new event

      // analyze event
      LAB.ClearEvent();                                       // clear the reco tree

      TVector3 MET = LAB_Gen.GetInvisibleMomentum();          // Get the MET from gen tree
      MET.SetZ(0.);
      INV.SetLabFrameThreeVector(MET);                        // Set the MET in reco tree

      Thada.SetLabFrameFourVector(Thada_Gen.GetFourVector()); // set had top 4-vectors
      Thadb.SetLabFrameFourVector(Thadb_Gen.GetFourVector());

      LAB.AnalyzeEvent();                                     // analyze the event

      // Generator-level observables
      double cosTTgen  = TT_Gen.GetCosDecayAngle();
      double cosTagen  = Ta_Gen.GetCosDecayAngle();

      // Reconstruction-level observables
      MTT    = TT.GetMass()/TT_Gen.GetMass();
      EtTa   = Thada.GetEnergy(Ta) / Thada_Gen.GetEnergy(Ta_Gen);
      PtTa   = Thada.GetMomentum(Ta) / Thada_Gen.GetMomentum(Ta_Gen);
      cosTT  = TT.GetCosDecayAngle();
      cosTa  = Ta.GetCosDecayAngle();
      DcosTT = asin(sqrt(1.-cosTT*cosTT)*cosTTgen-sqrt(1.-cosTTgen*cosTTgen)*cosTT);
      DcosTa = asin(sqrt(1.-cosTa*cosTa)*cosTagen-sqrt(1.-cosTagen*cosTagen)*cosTa);

      histPlot->Fill(cat_list[m]);
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
  example_DiStop_to_hadtopXhadtopX();
  return 0;
}
#endif
