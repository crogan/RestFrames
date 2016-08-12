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
  double mG = 1000.;
  double mX = 100.;

  // Number of events to generate
  int Ngen = 10000;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame GG_Gen("GG_Gen","#tilde{g}#tilde{g}");
  DecayGenFrame Ga_Gen("Ga_Gen","#tilde{g}_{a}");
  DecayGenFrame Gb_Gen("Gb_Gen","#tilde{g}_{b}");
  VisibleGenFrame V1a_Gen("V1a_Gen","j_{1a}");
  VisibleGenFrame V2a_Gen("V2a_Gen","j_{2a}");
  InvisibleGenFrame Xa_Gen("Xa_Gen","#tilde{#chi}_{a}");
  VisibleGenFrame V1b_Gen("V1b_Gen","j_{1b}");
  VisibleGenFrame V2b_Gen("V2b_Gen","j_{2b}");
  InvisibleGenFrame Xb_Gen("Xb_Gen","#tilde{#chi}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB_Gen.SetChildFrame(GG_Gen);
  GG_Gen.AddChildFrame(Ga_Gen);
  GG_Gen.AddChildFrame(Gb_Gen);
  Ga_Gen.AddChildFrame(V1a_Gen);
  Ga_Gen.AddChildFrame(V2a_Gen);
  Ga_Gen.AddChildFrame(Xa_Gen);
  Gb_Gen.AddChildFrame(V1b_Gen);
  Gb_Gen.AddChildFrame(V2b_Gen);
  Gb_Gen.AddChildFrame(Xb_Gen);

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  GG_Gen.SetVariableMass();
  // set gluino masses
  Ga_Gen.SetMass(mG);
  Gb_Gen.SetMass(mG);
  // set X masses
  Xa_Gen.SetMass(mX);
  Xb_Gen.SetMass(mX);
  // set jet pT and eta cuts
  V1a_Gen.SetPtCut(30.);                 V1a_Gen.SetEtaCut(2.5);  
  V1b_Gen.SetPtCut(30.);                 V1b_Gen.SetEtaCut(2.5);  
  V2a_Gen.SetPtCut(30.);                 V2a_Gen.SetEtaCut(2.5);  
  V2b_Gen.SetPtCut(30.);                 V2b_Gen.SetEtaCut(2.5);  

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
									       
  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  LabRecoFrame LAB("LAB","LAB");
  DecayRecoFrame     GG("GG","#tilde{g}#tilde{g}");
  DecayRecoFrame     Ga("Ga","#tilde{g}_{a}");
  DecayRecoFrame     Gb("Gb","#tilde{g}_{b}");
  DecayRecoFrame     Ca("Ca","C_{a}");
  DecayRecoFrame     Cb("Cb","C_{b}");
  VisibleRecoFrame   V1a("V1a","j_{1a}");
  VisibleRecoFrame   V2a("V2a","j_{2a}");
  InvisibleRecoFrame Xa("Xa","#tilde{#chi}_{a}");
  VisibleRecoFrame   V1b("V1b","j_{1b}");
  VisibleRecoFrame   V2b("V2b","j_{2b}");
  InvisibleRecoFrame Xb("Xb","#tilde{#chi}_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(GG);
  GG.AddChildFrame(Ga);
  GG.AddChildFrame(Gb);
  Ga.AddChildFrame(V1a);
  Ga.AddChildFrame(Ca);
  Ca.AddChildFrame(V2a);
  Ca.AddChildFrame(Xa);
  Gb.AddChildFrame(V1b);
  Gb.AddChildFrame(Cb);
  Cb.AddChildFrame(V2b);
  Cb.AddChildFrame(Xb);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Set up 'background-like' analysis tree
  LabRecoFrame LAB_B("LAB_B","LAB");
  SelfAssemblingRecoFrame CM_B("CM_B","CM");
  VisibleRecoFrame V_B("V_B","Vis");
  InvisibleRecoFrame I_B("I_B","Inv");
  LAB_B.SetChildFrame(CM_B);
  CM_B.AddChildFrame(V_B);
  CM_B.AddChildFrame(I_B);
 
  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  InvisibleGroup INV("INV","WIMP Jigsaws");
  INV.AddFrame(Xa);
  INV.AddFrame(Xb);
  CombinatoricGroup VIS("VIS","Visible Object Jigsaws");

  // visible frames in first decay step must always have at least one element
  VIS.AddFrame(V1a);
  VIS.AddFrame(V1b);
  VIS.SetNElementsForFrame(V1a,1,false);
  VIS.SetNElementsForFrame(V1b,1,false);
  // visible frames in second decay step can have zero elements
  VIS.AddFrame(V2a);
  VIS.AddFrame(V2b);
  VIS.SetNElementsForFrame(V2a,0,false);
  VIS.SetNElementsForFrame(V2b,0,false);

  InvisibleGroup INV_B("INV_B","Invisible State Jigsaws");
  INV_B.AddFrame(I_B);
  CombinatoricGroup VIS_B("VIS_B","Visible Object Jigsaws");
  VIS_B.AddFrame(V_B);
  VIS_B.SetNElementsForFrame(V_B,1,false);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  // signal-like jigsaws
  SetMassInvJigsaw MinMassJigsaw("MINMASS", "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);
  SetRapidityInvJigsaw RapidityJigsaw("RAPIDITY", "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrames((LAB.GetListVisibleFrames()));
  ContraBoostInvJigsaw ContraBoostJigsaw("CONTRA","Contraboost invariant Jigsaw");
  INV.AddJigsaw(ContraBoostJigsaw);
  ContraBoostJigsaw.AddVisibleFrames((Ga.GetListVisibleFrames()), 0);
  ContraBoostJigsaw.AddVisibleFrames((Gb.GetListVisibleFrames()), 1);
  ContraBoostJigsaw.AddInvisibleFrames((Ga.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw.AddInvisibleFrames((Gb.GetListInvisibleFrames()), 1);
  MinMassesCombJigsaw HemiJigsaw("HEM_JIGSAW","Minimize m _{V_{a,b}} Jigsaw");
  VIS.AddJigsaw(HemiJigsaw);
  HemiJigsaw.AddFrame(V1a,0);
  HemiJigsaw.AddFrame(V1b,1);
  HemiJigsaw.AddFrame(V2a,0);
  HemiJigsaw.AddFrame(V2b,1);
  MinMassesCombJigsaw CaHemiJigsaw("CaHEM_JIGSAW","Minimize m _{C_{a}} Jigsaw");
  VIS.AddJigsaw(CaHemiJigsaw);
  CaHemiJigsaw.AddFrame(V1a,0);
  CaHemiJigsaw.AddFrame(V2a,1);
  CaHemiJigsaw.AddFrame(Xa,1);
  MinMassesCombJigsaw CbHemiJigsaw("CbHEM_JIGSAW","Minimize m _{C_{b}} Jigsaw");
  VIS.AddJigsaw(CbHemiJigsaw);
  CbHemiJigsaw.AddFrame(V1b,0);
  CbHemiJigsaw.AddFrame(V2b,1);
  CbHemiJigsaw.AddFrame(Xb,1);

  // background tree jigsaws
  SetMassInvJigsaw MinMassJigsaw_B("MINMASS_B","Zero Mass for invisible system");
  INV_B.AddJigsaw(MinMassJigsaw_B);
  SetRapidityInvJigsaw RapidityJigsaw_B("RAPIDITY_B","Invisible system rapidity Jigsaw");
  INV_B.AddJigsaw(RapidityJigsaw_B);
  RapidityJigsaw_B.AddVisibleFrames((LAB_B.GetListVisibleFrames()));

  // check reconstruction trees
  if(LAB.InitializeAnalysis() && LAB_B.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analyses" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analyses" << LogEnd;

  //////////////////////////////////////////////////////////////
  // draw some pictures of our trees
  //////////////////////////////////////////////////////////////

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_Gen);
  tree_plot->Draw("GenTree", "Generator Tree", true);

  // signal reco tree
  tree_plot->SetTree(LAB);
  tree_plot->AddJigsaw(ContraBoostJigsaw);
  tree_plot->AddJigsaw(HemiJigsaw);
  tree_plot->AddJigsaw(CaHemiJigsaw);
  tree_plot->AddJigsaw(CbHemiJigsaw);
  tree_plot->Draw("SigRecoTree", "Signal Reconstruction Tree");

  // background reco tree
  tree_plot->SetTree(LAB_B);
  tree_plot->Draw("BkgRecoTree", "Background Reconstruction Tree");

  // Invisible Jigsaws
  tree_plot->SetTree(INV);
  tree_plot->Draw("InvTree", "Invisible Objects Jigsaws");

  // Visible Jigsaws
  tree_plot->SetTree(VIS);
  tree_plot->Draw("VisTree", "Visible Objects Jigsaws");

  
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%((std::max(Ngen,10))/10) == 0)
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_Gen.ClearEvent();                             // clear the gen tree
 
    LAB_Gen.SetPToverM(LAB_Gen.GetRandom());             // give the di-gluinos some Pt
              
    LAB_Gen.AnalyzeEvent();                           // generate a new event

    // analyze event
    TVector3 MET = LAB_Gen.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    std::vector<TLorentzVector> JETS;                 // Get the Jets from gen tree
    JETS.push_back(V1a_Gen.GetFourVector());
    JETS.push_back(V2a_Gen.GetFourVector());
    JETS.push_back(V1b_Gen.GetFourVector());
    JETS.push_back(V2b_Gen.GetFourVector());

    // give the signal-like tree the event info and analyze
    LAB.ClearEvent();                              // clear the signal-like tree
    INV.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    std::vector<RFKey> jetID;                      // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    LAB.AnalyzeEvent();                            // analyze the event

    // give the background-like tree the event info and analyze
    LAB_B.ClearEvent();                                   // clear the bkg-like tree
    INV_B.SetLabFrameThreeVector(MET);                    // Set the MET in tree
    for(int i = 0; i < int(JETS.size()); i++){
      TLorentzVector jet = JETS[i];
      jet.SetPtEtaPhiM(jet.Pt(), 0., jet.Phi(), jet.M()); // only pass transverse info to bkg-like tree
      VIS_B.AddLabFrameFourVector(jet);
    }
    LAB_B.AnalyzeEvent(); // analyze the event

    DecayRecoFrame* G[2];
    DecayRecoFrame* C[2];
    VisibleRecoFrame* VS[2];
    VisibleRecoFrame* VC[2];
    InvisibleRecoFrame* X[2];
    // Randomize the two hemispheres
    int flip = (gRandom->Rndm() > 0.5);
    G[flip] = &Ga;
    G[(flip+1)%2] = &Gb;
    C[flip] = &Ca;
    C[(flip+1)%2] = &Cb;
    VS[flip] = &V1a;
    VS[(flip+1)%2] = &V1b;
    VC[flip] = &V2a;
    VC[(flip+1)%2] = &V2b;
    X[flip] = &Xa;
    X[(flip+1)%2] = &Xb;

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

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

  }
  
  TFile fout(output_name.c_str(),"RECREATE");
  fout.Close();
  tree_plot->WriteOutput(output_name);
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_DiGluino_to_bbXbbX();
  return 0;
}
#endif
