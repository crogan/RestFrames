/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_WWW.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 May
///
///  \brief An example generating and analyzing pp -> WWW to 3l 3nu events
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

#include "Math/Minimizer.h"
#include "Math/GSLMinimizer.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "RestFrames/RestFrames.hh"

using namespace RestFrames;
using namespace std;

TVector3 g_X;
TVector3 g_Y;
TVector3 g_Z;

int g_N;

double g_MAX;

vector<TVector3> g_P;
vector<TVector3> g_U;

double dot_func(const double* param){
  double phi1 = param[0];
  double phi2 = param[1];
  double phi3 = param[2];

  double sum = 0.;

  TVector3 Z = g_Z;
  Z.Rotate(phi1, g_Y);
  TVector3 Y = g_Y;
  Y.Rotate(phi1, g_Z);

  for(int i = 0; i < g_N; i++){
    TVector3 V = g_U[i];
    V.Rotate(phi1, g_Z);
    V.Rotate(phi2, g_X);
    V.Rotate(phi3, g_Y);

    // V.Rotate(phi1, g_Z);
    // V.Rotate(phi2, Y);

    //V.Rotate(phi3, g_X);
    sum -= g_P[i].Dot(V);
  }

  return sum;
}

void flip_axes(){
  TVector3 temp = g_Z;
  g_Z = g_Y;
  g_Y = g_X;
  g_X = temp;
}

double dot_funcXYZ(const double* param){
  double phi1 = param[0];
  double phi2 = param[1];
  double phi3 = param[2];

  double c1 = cos(phi1);
  double c2 = cos(phi2);
  double c3 = cos(phi3);

  double s1 = sin(phi1);
  double s2 = sin(phi2);
  double s3 = sin(phi3);

  double sum = 0.;

  for(int i = 0; i < g_N; i++){
    TVector3 V(0.,0.,0.);
    V.SetX((c2*c3)*g_U[i].X()+
	   (-c2*s3)*g_U[i].Y()+
	   (s2)*g_U[i].Z());
    V.SetY((c1*s3+c3*s1*s2)*g_U[i].X()+
	   (c1*c3-s1*s2*s3)*g_U[i].Y()+
	   (-c2*s1)*g_U[i].Z());
    V.SetZ((s1*s3-c1*c3*s2)*g_U[i].X()+
	   (c3*s1+c1*s2*s3)*g_U[i].Y()+
	   (c1*c2)*g_U[i].Z());
    
    sum += g_P[i].Dot(V);
  }

  //return 1./(sum+g_MAX);
  return -sum;
}

double grad_funcXYZ(const double* param, int icoord){
  double phi1 = param[0];
  double phi2 = param[1];
  double phi3 = param[2];

  double c1 = cos(phi1);
  double c2 = cos(phi2);
  double c3 = cos(phi3);

  double s1 = sin(phi1);
  double s2 = sin(phi2);
  double s3 = sin(phi3);

  if(icoord < 0 || icoord > 2)
    cout << "ADADSASD" << endl;

  if(icoord == 0){
    c1 = -sin(phi1);
    s1 =  cos(phi1);
  }
  if(icoord == 1){
    c2 = -sin(phi2);
    s2 =  cos(phi2);
  }
  if(icoord == 2){
    c3 = -sin(phi3);
    s3 =  cos(phi3);
  }

  double sum = 0.;

  for(int i = 0; i < g_N; i++){
    TVector3 V(0.,0.,0.);
    V.SetX((c2*c3)*g_U[i].X()+
	   (-c2*s3)*g_U[i].Y()+
	   (s2)*g_U[i].Z());
    V.SetY((c1*s3+c3*s1*s2)*g_U[i].X()+
	   (c1*c3-s1*s2*s3)*g_U[i].Y()+
	   (-c2*s1)*g_U[i].Z());
    V.SetZ((s1*s3-c1*c3*s2)*g_U[i].X()+
	   (c3*s1+c1*s2*s3)*g_U[i].Y()+
	   (c1*c2)*g_U[i].Z());
    
    sum += g_P[i].Dot(V);
  }

  //double func = dot_funcXYZ(param);

  return -sum;
}

void set_axes(){
  TVector3 V(0.,0.,0.);
  TVector3 z(0.,0.,1.);
  g_MAX = 0.;
  for(int i = 0; i < g_N; i++){
    double delta = (g_P[i]-g_U[i]).Dot(z);
    V += delta*g_U[i];
    g_MAX += g_P[i].Mag()*g_U[i].Mag();
  }
  
  if(V.Mag() < 1e-8)
    V.SetXYZ(1.,0.,0.);

  // g_Z = V.Unit();
  // g_Y = g_Z.Cross(z).Unit();
  // g_X = g_Y.Cross(g_Z).Unit();

  g_Z.SetXYZ(0.,0.,1.);
  g_Y.SetXYZ(0.,1.,0.);
  g_X.SetXYZ(1.,0.,0.);
  
}

void example_WWW(const std::string& output_name = "output_example_04.root"){
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);
  SetLogMaxWidth(120);

  double mW = 80.;
  double wW = 2.5;
  double mL = 0.501;
  double mN = 0.;
  int Ngen = 1000000;

  // Set up toy generation tree (not needed for reconstruction)
  g_Log << LogInfo << "Initializing generator frames and tree" << LogEnd;
  ppLabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame CM_G("CM_G","CM");
  ResonanceGenFrame Wa_G("Wa_G","W_{a}");
  ResonanceGenFrame Wb_G("Wb_G","W_{b}");
  ResonanceGenFrame Wc_G("Wc_G","W_{c}");
  VisibleGenFrame La_G("La_G","#it{l}_{a}");
  InvisibleGenFrame Na_G("Na_G","#nu_{a}");
  VisibleGenFrame Lb_G("Lb_G","#it{l}_{b}");
  InvisibleGenFrame Nb_G("Nb_G","#nu_{b}");
  VisibleGenFrame Lc_G("Lc_G","#it{l}_{c}");
  InvisibleGenFrame Nc_G("Nc_G","#nu_{c}");
  LAB_G.SetChildFrame(CM_G);
  CM_G.AddChildFrame(Wa_G);
  CM_G.AddChildFrame(Wb_G);
  CM_G.AddChildFrame(Wc_G);
  Wa_G.AddChildFrame(La_G);
  Wa_G.AddChildFrame(Na_G);
  Wb_G.AddChildFrame(Lb_G);
  Wb_G.AddChildFrame(Nb_G);
  Wc_G.AddChildFrame(Lc_G);
  Wc_G.AddChildFrame(Nc_G);

  if(LAB_G.InitializeTree()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized tree from LabFrame ";
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready to set masses etc." << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize tree from LabFrame: " << Log(LAB_G) << LogEnd;								    

  // set W masses
  Wa_G.SetMass(mW);
  Wb_G.SetMass(mW);
  Wc_G.SetMass(mW);
  Wa_G.SetWidth(wW);
  Wb_G.SetWidth(wW);
  Wc_G.SetWidth(wW);
  // set lepton masses
  La_G.SetMass(mL);
  Lb_G.SetMass(mL);
  Lc_G.SetMass(mL);
  // set neutrino masses
  Na_G.SetMass(mN);
  Nb_G.SetMass(mN);
  Nc_G.SetMass(mN);
  //CM_G.SetMass(600.);
  CM_G.SetVariableMass(true);

  if(LAB_G.InitializeAnalysis()){
    g_Log << LogInfo;
    g_Log << "Successfully initialized analysis from LabFrame ";
    g_Log << LAB_G.GetName() << std::endl;
    g_Log << "Ready for event generation" << LogEnd;
  } else
    g_Log << LogError << "Unable to initialize analysis from LabFrame: " << Log(LAB_G) << LogEnd;

  HistPlot* histPlot   = new HistPlot("HistPlot","pp #rightarrow W(#it{l} #nu) W(#it{l} #nu) W(#it{l} #nu)");
  //histPlot->SetRebin(1);

  const HistPlotCategory& cat_Gen   = histPlot->GetNewCategory("Gen", "Generator");
  const HistPlotCategory& cat_Reco   = histPlot->GetNewCategory("Reco", "Reconstruction");

  const HistPlotVar& PzCM     = histPlot->GetNewVar("PzCM", "p_{z, CM}", -5000., 5000., "[GeV]");
  const HistPlotVar& MCM     = histPlot->GetNewVar("MCM", "M_{CM}", 0., 3000., "[GeV]");
  const HistPlotVar& MWa     = histPlot->GetNewVar("MWa", "M_{Wa}", 0., 150., "[GeV]");
  const HistPlotVar& MWb     = histPlot->GetNewVar("MWb", "M_{Wb}", 0., 150., "[GeV]");
  const HistPlotVar& MWc     = histPlot->GetNewVar("MWc", "M_{Wc}", 0., 150., "[GeV]");
  const HistPlotVar& Mab2     = histPlot->GetNewVar("Mab2", "M_{ab}^{2}", 0., 1.);
  const HistPlotVar& Mcb2     = histPlot->GetNewVar("Mcb2", "M_{cb}^{2}", 0., 1.);

  const HistPlotVar& cosWa     = histPlot->GetNewVar("cosWa", "cos #theta_{Wa}", -1., 1.);
  const HistPlotVar& cosWb     = histPlot->GetNewVar("cosWb", "cos #theta_{Wb}", -1., 1.);
  const HistPlotVar& cosWc     = histPlot->GetNewVar("cosWc", "cos #theta_{Wc}", -1., 1.);
 
  const HistPlotVar& MCMN     = histPlot->GetNewVar("MCMN", "M_{CM}", 0., 2.);
  const HistPlotVar& MWaN     = histPlot->GetNewVar("MWaN", "M_{Wa}", 0., 2.);
  const HistPlotVar& MWbN     = histPlot->GetNewVar("MWbN", "M_{Wb}", 0., 2.);
  const HistPlotVar& MWcN     = histPlot->GetNewVar("MWcN", "M_{Wc}", 0., 2.);
  const HistPlotVar& MWTOT    = histPlot->GetNewVar("MWTOT", "#sqrt{#Sigma M_{W}^{ 2} / #Sigma m_{W}^{ 2}}", 0., 4.);
  
  histPlot->AddPlot(MCM, cat_Gen);
  histPlot->AddPlot(PzCM, cat_Gen);
  histPlot->AddPlot(PzCM, MCM, cat_Gen);
  histPlot->AddPlot(MWa, MWb, cat_Gen);
  histPlot->AddPlot(MWb, MWc, cat_Gen);
  histPlot->AddPlot(MWa, MWc, cat_Gen);
  histPlot->AddPlot(cosWa, cosWb, cat_Gen);
  histPlot->AddPlot(cosWa, cosWc, cat_Gen);
  histPlot->AddPlot(cosWb, cosWc, cat_Gen);
  histPlot->AddPlot(MCM, MWa, cat_Gen);
  histPlot->AddPlot(Mab2, Mcb2, cat_Gen);
 
   histPlot->AddPlot(MCMN, cat_Reco);
   histPlot->AddPlot(MWTOT, cat_Reco);
   histPlot->AddPlot(MWTOT, MCMN, cat_Reco);
   histPlot->AddPlot(MWaN, cat_Reco);
   histPlot->AddPlot(MWbN, cat_Reco);
   histPlot->AddPlot(MWcN, cat_Reco);
   histPlot->AddPlot(MWaN, MCMN, cat_Reco);
   histPlot->AddPlot(MWaN, MWbN, cat_Reco);
   histPlot->AddPlot(MWaN, MWcN, cat_Reco);
   histPlot->AddPlot(MWcN, MWbN, cat_Reco);

  // Minimizer
  // ROOT::Math::Minimizer* MIN = 
  //   ROOT::Math::Factory::CreateMinimizer("Minuit2", "");
  ROOT::Minuit2::Minuit2Minimizer* MIN = 
    new ROOT::Minuit2::Minuit2Minimizer(ROOT::Minuit2::kCombined);
  // ROOT::Math::Minimizer* MIN2 = 
  //   ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  // ROOT::Math::GSLMinimizer* MIN2 = 
  //   new ROOT::Math::GSLMinimizer(ROOT::Math::kVectorBFGS);
  ROOT::Minuit2::Minuit2Minimizer* MIN2 = 
    new ROOT::Minuit2::Minuit2Minimizer(ROOT::Minuit2::kCombined);

  MIN->SetMaxFunctionCalls(10000000);
  //MIN->SetTolerance(0.001);
  MIN->SetPrintLevel(0);
  MIN2->SetMaxFunctionCalls(10000000);
  MIN2->SetMaxIterations(100000);
  MIN2->SetTolerance(0.001);
  MIN2->SetPrintLevel(0);

  ROOT::Math::Functor f(&dot_func, 3);
  ROOT::Math::Functor f2(&dot_funcXYZ, 3);
  //ROOT::Math::GradFunctor f2(&dot_funcXYZ, &grad_funcXYZ, 3);

  double step[3] = {0.001, 0.001, 0.001};
  double var[3] = {0.,0.,0.};

  MIN->SetFunction(f);
  MIN2->SetFunction(f2);

  MIN->SetVariable(0, "phi1", var[0], step[0]);
  MIN->SetVariable(1, "phi2", var[1], step[1]);
  MIN->SetVariable(2, "phi3", var[2], step[2]);
  MIN2->SetVariable(0, "phi1", var[0], step[0]);
  MIN2->SetVariable(1, "phi2", var[1], step[1]);
  MIN2->SetVariable(2, "phi3", var[2], step[2]);

  // MIN2->SetVariableLimits(0, -acos(-1.), acos(-1.));
  // MIN2->SetVariableLimits(1, -acos(-1.), acos(-1.));
  // MIN2->SetVariableLimits(2, -acos(-1.), acos(-1.));

  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(Ngen/10) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                            // clear the gen tree
    double PTCM = 3*mW*gRandom->Rndm()*1.;
    LAB_G.SetTransverseMomenta(PTCM);               // give the CM some Pt
    LAB_G.AnalyzeEvent();                          // generate a new event

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** Higgs mass
    PzCM = CM_G.GetFourVector().Z();
    MCM = CM_G.GetMass();
    MWa = Wa_G.GetMass();
    MWb = Wb_G.GetMass();
    MWc = Wc_G.GetMass();
    cosWa = Wa_G.GetCosDecayAngle();
    cosWb = Wb_G.GetCosDecayAngle();
    cosWc = Wc_G.GetCosDecayAngle();
    Mab2 = (Wa_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    Mcb2 = (Wc_G.GetFourVector()+Wb_G.GetFourVector()).M2()/MCM/MCM;
    
    histPlot->Fill(cat_Gen);

    // minimization 
    // MIN->SetVariable(0, "phi1", var[0], step[0]);
    // MIN->SetVariable(1, "phi2", var[1], step[1]);
    // MIN->SetVariable(2, "phi3", var[2], step[2]);
    //MIN->SetVariableValues(&var[0]);
    MIN->SetVariableValue(0, 0.);
    MIN->SetVariableValue(1, 0.);
    MIN->SetVariableValue(2, 0.);
    MIN2->SetVariableValues(&var[0]);

    g_N = 3;

    vector<TLorentzVector> LEPs;
    LEPs.push_back(La_G.GetFourVector());
    LEPs.push_back(Lb_G.GetFourVector());
    LEPs.push_back(Lc_G.GetFourVector());

    TLorentzVector LEP(0.,0.,0.,0.);
    for(int i = 0; i < g_N; i++){
      //LEPs[i].SetPtEtaPhiM(LEPs[i].Pt(),0.,LEPs[i].Phi(),LEPs[i].M());
      LEP += LEPs[i];
    }

    TVector3 boostZ = LEP.BoostVector();
    boostZ.SetX(0.);
    boostZ.SetY(0.);
   
    TLorentzVector INV(0.,0.,0.,0.);
    INV.SetPtEtaPhiM(MET.Mag(),0.,MET.Phi(),LEP.M());
    INV.Boost(boostZ);

    // Go to CM frame of LEP+INV system
    TVector3 boostCM = (LEP+INV).BoostVector();
    LEP.Boost(-boostCM);
    INV.Boost(-boostCM);
    for(int i = 0; i < g_N; i++){
      LEPs[i].Boost(-boostCM);
    }
    
    g_P.clear();
    g_U.clear();

    // Calculate the boost to the LEP RF
    TVector3 boostLEP = LEP.BoostVector();
    for(int i = 0; i < g_N; i++){
      LEPs[i].Boost(-boostLEP);
      g_U.push_back(LEPs[i].Vect());
      LEPs[i].Boost(boostLEP);
    }
    // Calculate the boost to the INV RF
    TVector3 boostINV = INV.BoostVector();
    for(int i = 0; i < g_N; i++){
      LEPs[i].Boost(-boostINV);
      g_P.push_back(LEPs[i].Vect());
      LEPs[i].Boost(boostINV);
    }

    set_axes();
    MIN->Minimize();
    //MIN2->Minimize();
    
    // const double *PHIs = MIN->X();
    // const double *PHIs2 = MIN2->X();
    // cout << endl << "func at min " << dot_func(PHIs) << " " << dot_func(&var[0]) << endl;
    // cout << "func2 at min " << dot_funcXYZ(PHIs2) << " " << dot_funcXYZ(&var[0]) << endl << endl;

    if(MIN->Status() > 0){
      flip_axes();
      MIN->SetVariableValues(&var[0]);
      MIN->Minimize();
      // cout << "new STATUS = " << MIN->Status() << endl;
      // cout << "STATUS = " << MIN2->Status() << endl;
      // cout << "func2 at min " << dot_funcXYZ(PHIs2) << " " << dot_funcXYZ(&var[0]) << endl;
    }

    const double *PHIs = MIN->X();
    
    vector<TLorentzVector> INVs;
    TLorentzVector INVcheck(0.,0.,0.,0.);
    for(int i = 0; i < g_N; i++){
      TLorentzVector inv;
      TVector3 vinv = g_U[i];
      vinv.Rotate(PHIs[0], g_Z);
      vinv.Rotate(PHIs[1], g_X);
      vinv.Rotate(PHIs[2], g_Y);
      inv.SetVectM(vinv,LEPs[i].M());
      INVs.push_back(inv); 
      INVs[i].Boost(boostINV);
      INVcheck += INVs[i];
    }

    // cout << INV.M() << " " << INV.Px() << " " << INV.Py() << " " << INV.Pz() << endl;
    // cout << INVcheck.M() << " " << INVcheck.Px() << " " << INVcheck.Py() << " " << INVcheck.Pz() << endl << endl;

    MCMN = (INV+LEP).M()/MCM;
    MWaN = (INVs[0]+LEPs[0]).M()/MWa;
    MWbN = (INVs[1]+LEPs[1]).M()/MWb;
    MWcN = (INVs[2]+LEPs[2]).M()/MWc;
    MWTOT = 0.;
    double MWgTOT = MWa*MWa + MWb*MWb + MWc*MWc;
    for(int i = 0; i < 3; i++){
      MWTOT += (INVs[i]+LEPs[i]).M2()/MWgTOT;
    }
    MWTOT = sqrt(MWTOT);

    histPlot->Fill(cat_Reco);
  }

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  histPlot->Draw();

 

  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_WWW();
  return 0;
}
#endif
