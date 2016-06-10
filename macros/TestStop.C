/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   TestStop.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 June
///
///  \brief A simple example generating and analyzing top to b W(l nu) events
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

#include "RestFrames/RestFrames.hh"

using namespace RestFrames;

void TestStop(){
  // set particle masses and widths
  double mstop = 150.;
  double mtop = 173.5;
  double wtop = 1.33313;
  double mW   = 80.399;
  double wW   = 2.085;
  double mLSP = 1.;
  // Number of events to generate
  int Ngen = 100000;

  ////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  LabGenFrame           LAB_0("LAB_0","LAB");
  DecayGenFrame         ST_0("ST_0", "#tilde{t}");
  InvisibleGenFrame     N_0("N_0", "#tilde{#chi}");
  ResonanceGenFrame     W_0("W_0","W");
  VisibleGenFrame       B_0("B_0","b");
  VisibleGenFrame       L_0("L_0","#it{l}");
  InvisibleGenFrame     NU_0("NU_0","#nu");

  LabGenFrame           LAB_1("LAB_0","LAB");
  DecayGenFrame         ST_1("ST_0", "#tilde{t}");
  InvisibleGenFrame     N_1("N_1", "#tilde{#chi}");
  ResonanceGenFrame     T_1("T_1","t");
  ResonanceGenFrame     W_1("W_1","W");
  VisibleGenFrame       B_1("B_1","b");
  VisibleGenFrame       L_1("L_1","#it{l}");
  InvisibleGenFrame     NU_1("NU_1","#nu");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_0.SetChildFrame(ST_0);
  ST_0.AddChildFrame(N_0);
  ST_0.AddChildFrame(W_0);
  ST_0.AddChildFrame(B_0);
  W_0.AddChildFrame(L_0);
  W_0.AddChildFrame(NU_0);

  LAB_1.SetChildFrame(ST_1);
  ST_1.AddChildFrame(N_1);
  ST_1.AddChildFrame(T_1);
  T_1.AddChildFrame(W_1);
  T_1.AddChildFrame(B_1);
  W_1.AddChildFrame(L_1);
  W_1.AddChildFrame(NU_1);

  if(LAB_0.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  if(LAB_1.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  ST_0.SetMass(mstop);
  ST_1.SetMass(mstop);
  N_0.SetMass(mLSP);
  N_1.SetMass(mLSP);
  T_1.SetMass(mtop);
  T_1.SetWidth(wtop);
  W_0.SetMass(mW);
  W_0.SetWidth(wW);
  W_1.SetMass(mW);
  W_1.SetWidth(wW);

  if(LAB_0.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;

  if(LAB_1.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_0);
  treePlot->Draw("GenTree", "Generator Tree 3-body");

  treePlot->SetTree(LAB_1);
  treePlot->Draw("RecoTree", "Generator Tree 2-body");

  HistPlot* histPlot   = new HistPlot("HistPlot","#tilde{t} #rightarrow (b W #tilde{#chi}}");

  const HistPlotCategory& cat_0 = histPlot->GetNewCategory("three", "3-body");
  const HistPlotCategory& cat_1 = histPlot->GetNewCategory("two", "2-body");
 

  const HistPlotVar& Mt = histPlot->GetNewVar("Mt", "m_{b+W}", mW-10., mstop-mLSP+10., "[GeV]");

  histPlot->AddPlot(Mt,   cat_0+cat_1);

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0) std::cout << "Generating event " << igen << " of " << Ngen << std::endl;

    // generate event
    LAB_0.ClearEvent();    
    //LAB_0.SetTransverseMomenta(1.);
    LAB_0.AnalyzeEvent();     

    LAB_1.ClearEvent();      
    //LAB_1.SetTransverseMomenta(1.);
    LAB_1.AnalyzeEvent(); 

    Mt = (W_0.GetFourVector()+B_0.GetFourVector()).M();
    histPlot->Fill(cat_0);
    Mt = (W_1.GetFourVector()+B_1.GetFourVector()).M();
    histPlot->Fill(cat_1);
  }

  histPlot->Draw();
}
