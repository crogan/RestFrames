#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RFrame.hh"
#include "RestFrames/RLabFrame.hh"
#include "RestFrames/RDecayFrame.hh"
#include "RestFrames/RVisibleFrame.hh"
#include "RestFrames/RInvisibleFrame.hh"
#include "RestFrames/RSelfAssemblingFrame.hh"
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/MinimizeMassesCombinatoricJigsaw.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/FramePlot.hh"

using namespace std;
using namespace RestFrames;

void TestZeroLepton(){

 //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  // Set up RestFrames code
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // different 'Frames' of interest
  //////////////////////////////////////////////////////////////
  std::cout << " Initialize lists of visible, invisible particles and intermediate states " << endl;
  RLabFrame LAB("LAB","lab");
  RDecayFrame SS("SS","SS");
  RSelfAssemblingFrame S1("S1","#tilde{S}_{a}");
  RSelfAssemblingFrame S2("S2","#tilde{S}_{b}");
  RVisibleFrame V1("V1","V_{a}");
  RVisibleFrame V2("V2","V_{b}");
  RInvisibleFrame I1("I1","I_{a}");
  RInvisibleFrame I2("I2","I_{b}");

  //////////////////////////////////////////////////////////////
  // different 'groups' of particles 
  //////////////////////////////////////////////////////////////
  std::cout << " Define invisible and combinatoric groups " << endl;

  // The invisible group is all of the weakly interacting particles
  InvisibleGroup INV("INV","Invisible State Jigsaws");
  INV.AddFrame(I1);
  INV.AddFrame(I2);

  // the combinatoric group is the list of visible objects
  // that go into our hemispheres 
  CombinatoricGroup VIS("VIS","Visible Object Jigsaws");
  VIS.AddFrame(V1);
  VIS.SetNElementsForFrame(V1,1,false);
  VIS.AddFrame(V2);
  VIS.SetNElementsForFrame(V2,1,false);

  //////////////////////////////////////////////////////////////
  // define how all the frames connect in the decay tree
  //////////////////////////////////////////////////////////////
  std::cout << " Build decay tree " << endl;
  LAB.SetChildFrame(SS);

  SS.AddChildFrame(S1);
  SS.AddChildFrame(S2);

  S1.AddChildFrame(V1);
  S1.AddChildFrame(I1);
  S2.AddChildFrame(V2);
  S2.AddChildFrame(I2);

  //////////////////////////////////////////////////////////////
  // check to make sure the tree is acceptably connected 
  //////////////////////////////////////////////////////////////
  std::cout << "Is consistent tree topology? " << LAB.InitializeTree() << endl; 

  //////////////////////////////////////////////////////////////
  // now we define 'jigsaw rules' that tell the tree
  // how to define the objects in our groups 
  //////////////////////////////////////////////////////////////
  InvisibleMassJigsaw MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);

  InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));
  
  ContraBoostInvariantJigsaw ContraBoostJigsaw("CB_JIGSAW","Contraboost invariant Jigsaw");
  INV.AddJigsaw(ContraBoostJigsaw);
  ContraBoostJigsaw.AddVisibleFrame((S1.GetListVisibleFrames()), 0);
  ContraBoostJigsaw.AddVisibleFrame((S2.GetListVisibleFrames()), 1);
  ContraBoostJigsaw.AddInvisibleFrame((S1.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw.AddInvisibleFrame((S2.GetListInvisibleFrames()), 1);

  MinimizeMassesCombinatoricJigsaw HemiJigsaw("HEM_JIGSAW","Minimize m _{V_{a,b}} Jigsaw");
  VIS.AddJigsaw(HemiJigsaw);
  HemiJigsaw.AddFrame(V1,0);
  HemiJigsaw.AddFrame(V2,1);

  //////////////////////////////////////////////////////////////
  // check to make sure that all the jigsaws etc. are correctly connected
  //////////////////////////////////////////////////////////////
  std::cout << "Is consistent analysis tree? : " << LAB.InitializeAnalysis() << endl; 

  //////////////////////////////////////////////////////////////
  // draw some pictures of our tree
  //////////////////////////////////////////////////////////////
  FramePlot* TreePlot = new FramePlot("tree","Decay Tree");
  TreePlot->AddFrameTree(LAB);
  TreePlot->AddJigsaw(ContraBoostJigsaw);
  TreePlot->AddJigsaw(HemiJigsaw);
  TreePlot->DrawFramePlot();

  FramePlot* INVPlot = new FramePlot("inv","Invisible Objects Jigsaws");
  INVPlot->AddGroupTree(INV);
  INVPlot->DrawFramePlot();

  FramePlot* VISPlot = new FramePlot("vis","Visible Objects Jigsaws");
  VISPlot->AddGroupTree(VIS);
  VISPlot->DrawFramePlot();
  
  //////////////////////////////////////////////////////////////
  // Now, we make a 'background'-like, transverse momentum only, tree
  //////////////////////////////////////////////////////////////
  RLabFrame LAB_alt("LAB","lab");
  RSelfAssemblingFrame S_alt("CM","CM");
  RVisibleFrame V_alt("V_alt","Vis");
  RInvisibleFrame I_alt("I_alt","Iinv");
 
  InvisibleGroup INV_alt("INV_alt","Invisible State Jigsaws");
  INV_alt.AddFrame(I_alt);

  CombinatoricGroup VIS_alt("VIS_alt","Visible Object Jigsaws");
  VIS_alt.AddFrame(V_alt);
  VIS_alt.SetNElementsForFrame(V_alt,1,false);

  LAB_alt.SetChildFrame(S_alt);
  S_alt.AddChildFrame(V_alt);
  S_alt.AddChildFrame(I_alt);

  LAB_alt.InitializeTree(); 

  // Will just set invisible mass to zero
  InvisibleMassJigsaw MinMass_alt("MINMASS_JIGSAW_ALT", "Invisible system mass Jigsaw");
  INV_alt.AddJigsaw(MinMass_alt);

  // will set rapidity to zero
  InvisibleRapidityJigsaw Rapidity_alt("RAPIDITY_JIGSAW_ALT", "Invisible system rapidity Jigsaw");
  INV_alt.AddJigsaw(Rapidity_alt);
  Rapidity_alt.AddVisibleFrame((LAB_alt.GetListVisibleFrames()));

  LAB_alt.InitializeAnalysis(); 


  //
  // TEST EVENT Reconcstruction
  // 
  int Njetmax = 20;
  int Ntrial = 1000;
  for(int itrial = 0; itrial < Ntrial; itrial++){
    // generate some random jets
    int Njet = 2 + int((float(Njetmax)-2.)*gRandom->Rndm());
    vector<TLorentzVector> JETS;
    for(int ijet = 0; ijet < Njet; ijet++){
      float Px = gRandom->Rndm();
      float Py = gRandom->Rndm();
      float Pz = gRandom->Rndm();
      float M = gRandom->Rndm();
      float E = sqrt(Px*Px + Py*Py + Pz*Pz + M*M);
      TLorentzVector jet;
      jet.SetPxPyPzE(Px,Py,Pz,E);
      JETS.push_back(jet);
    }
    TVector3 MET(gRandom->Rndm(),gRandom->Rndm(),0.);

    LAB.ClearEvent();
    // can use this to see where 
    // each jet ended up in the tree
    vector<GroupElementID> jetID; 
   
    // add the jets to the 'VIS' group
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    // Set the 'INV' group momentum in 
    // the lab frame (the MET)
    INV.SetLabFrameThreeVector(MET);
    LAB.AnalyzeEvent();

    // Do the same for 'background' tree - set 
    // jets' z-momenta to zero before loading into tree
    LAB_alt.ClearEvent();
    for(int i = 0; i < int(JETS.size()); i++){
      TLorentzVector jet = JETS[i];
      jet.SetPtEtaPhiM(jet.Pt(), 0., jet.Phi(), jet.M());
      VIS_alt.AddLabFrameFourVector(jet);
    }
    INV_alt.SetLabFrameThreeVector(MET);
    LAB_alt.AnalyzeEvent();

    //////////////////////////////////////
    // Calculation of ZeroLepton variables
    //////////////////////////////////////

    // primary observables
    double shatR = SS.GetMass();
    double gaminvRp1 = SS.GetVisibleShape();
    
    // some other 'angle'-like things from the CM frame
    double costhetaSS = SS.GetCosDecayAngle();
    double gaminvRp1_T = SS.GetTransverseVisibleShape();
    double dphi12 = SS.GetDeltaPhiVisible();
    double scalarMomTransverseRatio = SS.GetTransverseScalarVisibleMomentum()/SS.GetScalarVisibleMomentum();

    // dphiR and Rptshat (formerly cosPT)
    // for QCD rejection
    double dphiR = SS.GetDeltaPhiBoostVisible();
    double PTCM = SS.GetFourVector(LAB).Pt();
    double Rptshat = PTCM / (PTCM + shatR/4.);

    // QCD rejection using the 'background tree'
    // MET 'sibling' in background tree auxillary calculations
    TLorentzVector Psib = I_alt.GetSiblingFrame()->GetFourVector(LAB_alt);
    TLorentzVector Pmet = I_alt.GetFourVector(LAB_alt);
    double Psib_dot_METhat = max(0., Psib.Vect().Dot(MET.Unit()));
    double Mpar2 = Psib.E()*Psib.E()-Psib.Vect().Dot(MET.Unit())*Psib.Vect().Dot(MET.Unit());
    double Msib2 = Psib.M2();
    double MB2 = 2.*(Pmet.E()*Psib.E()-MET.Dot(Psib.Vect()));
    TVector3 boostPsibM = (Pmet+Psib).BoostVector();

    // QCD rejection variables from 'background tree'
    double DepthBKG = S_alt.GetFrameDepth(I_alt);
    int Nsib = I_alt.GetSiblingFrame()->GetNDescendants();
    double cosBKG = I_alt.GetParentFrame()->GetCosDecayAngle();
    double dphiMsib = fabs(MET.DeltaPhi(Psib.Vect()));
    double RpsibM = Psib_dot_METhat / (Psib_dot_METhat + MET.Mag());
    double RmsibM = 1. / ( MB2/(Mpar2-Msib2) + 1.);
    Psib.Boost(-boostPsibM);
    double cosPsibM = -1.*Psib.Vect().Unit().Dot(boostPsibM.Unit());
    cosPsibM = (1.-cosPsibM)/2.;
    double DeltaQCD1 = (cosPsibM-RpsibM)/(cosPsibM+RpsibM);
    double DeltaQCD2 = (cosPsibM-RmsibM)/(cosPsibM+RmsibM);

    // Hemisphere variables - still studying to improve this part
    int NJ[2];
    double ND[2];
    double cosD[2][3];  // cos-decay angle at some depth
    double RATIO[2][3]; // momentum-ratio between depths
    RestFrame *V[2],*I[2],*S[2];
    V[0] = &V1;
    V[1] = &V2;
    I[0] = &I1;
    I[1] = &I2;
    S[0] = &S1;
    S[1] = &S2;
    
    for(int h = 0; h < 2; h++){
      NJ[h] = VIS.GetNElementsInFrame(V[h]);
      ND[h] = S[h]->GetFrameDepth(I[h]);
      for(int d = 0; d < 3; d++){
	if(d >= ND[h]){
	  cosD[h][d] = -2.;
	  RATIO[h][d] = -2.;
	  continue;
	}
        const RestFrame* frame = S[h]->GetFrameAtDepth(d+1, I[h]);
	const RestFrame* prod_frame = frame->GetProductionFrame();
	const RestFrame* prod_prod_frame = prod_frame->GetProductionFrame();
	  
	RATIO[h][d] = 2./sqrt(3.)*frame->GetMomentum(prod_frame)/prod_frame->GetMomentum(prod_prod_frame);
	cosD[h][d] = prod_frame->GetCosDecayAngle(frame);
      }
    }
  }
  
  
}

