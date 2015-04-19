#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <iostream>
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

void TestSuperRazor(){

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

    // Can get our two visible hemispheres' four vectors in any
    // reference frame in our tree
    TLorentzVector HEM1_inLab = V1.GetFourVector(LAB);
    TLorentzVector HEM2_inLab = V2.GetFourVector(LAB);
    TLorentzVector HEM1_inS2 = V1.GetFourVector(S2);

    // Can get other variables
    cout << "SHATR = " << SS.GetMass() << endl;
    cout << "1/gamma_R+1 = " << SS.GetVisibleShape() << endl;
    cout << "Invisible 1 E in S2 = " << I1.GetEnergy(S2) << endl;
    
    // Can find out where all the jets ended up
    cout << "Number of jets in HEM 1 = " << VIS.GetNElementsInFrame(V1) << endl;
    cout << "Number of jets in HEM 2 = " << VIS.GetNElementsInFrame(V2) << endl;

    // Can find out where specific jets ended up
    cout << "Jet #1 is in HEM 1? " << V1.IsSame(VIS.GetFrame(jetID[0])) << endl;

    // Can get decay angles of different frames
    cout << "cos theta_SS = " << SS.GetCosDecayAngle() << endl;

    // Can find out the 'depth' of the WIMPS in each decay hemisphere
    cout << "WIMP 'depth' in S1 = " << S1.GetFrameDepth(I1) << endl;
    cout << "WIMP 'depth' in S2 = " << S2.GetFrameDepth(I2) << endl;
      
    // Can get the frame each WIMP is produced in and calculate things from it
    const RestFrame* I1_prodframe = I1.GetProductionFrame();
    cout << "P_{I1}^{prod} / M_{S1} = "; 
    cout << I1.GetMomentum(I1_prodframe)/S1.GetMass() << endl;
    
  }
  TreePlot->AddFrameTree(SS);
  TreePlot->DrawFramePlot();
  
}

