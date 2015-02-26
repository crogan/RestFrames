//////////////////////////////////////////////////////////////
// Example RestFrames code for analyzing 
// di-leptonic ttbar events
// C. Rogan 29-01-15
//////////////////////////////////////////////////////////////

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
#include "RestFrames/InvisibleMassJigsaw.hh"
#include "RestFrames/InvisibleRapidityJigsaw.hh"
#include "RestFrames/ContraBoostInvariantJigsaw.hh"
#include "RestFrames/MinimizeMassesCombinatoricJigsaw.hh"
#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/FramePlot.hh"

using namespace std;
using namespace RestFrames;

void TestDiLeptonicTop(){
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  // Setup RestFrames topology code
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
 
  //////////////////////////////////////////////////////////////
  // Declare the 'frames' in your decay tree
  //////////////////////////////////////////////////////////////
  RLabFrame LAB("LAB","lab");
  RDecayFrame TT("TT","t #bar{t}");
  RDecayFrame T1("T1","t_{a}");
  RDecayFrame T2("T2","t_{b}");
  RVisibleFrame Bjet1("B1","b_{a}");
  RVisibleFrame Bjet2("B2","b_{b}");
  RDecayFrame W1("W1","W_{a}");
  RDecayFrame W2("W2","W_{b}");
  RVisibleFrame Lep1("L1","#it{l}_{a}");
  RVisibleFrame Lep2("L2","#it{l}_{b}");
  RInvisibleFrame Neu1("NU1","#nu_{a}");
  RInvisibleFrame Neu2("NU2","#nu_{b}");

  //////////////////////////////////////////////////////////////
  // Declare the 'groups' of frames
  //////////////////////////////////////////////////////////////
  // Invisible Frame Group
  InvisibleGroup INV("INV","Invisible Frame Jigsaws");
  INV.AddFrame(Neu1);
  INV.AddFrame(Neu2);

  CombinatoricGroup BTAGS("BTAGS","B-tagged jet Jigsaws");
  BTAGS.AddFrame(Bjet1);
  BTAGS.AddFrame(Bjet2);
  BTAGS.SetNElementsForFrame(Bjet1,1,true);
  BTAGS.SetNElementsForFrame(Bjet2,1,true);

  //////////////////////////////////////////////////////////////
  // Assemble your decay tree from your frames
  //////////////////////////////////////////////////////////////
  LAB.SetChildFrame(TT);

  TT.AddChildFrame(T1);
  TT.AddChildFrame(T2);
  
  T1.AddChildFrame(Bjet1);
  T1.AddChildFrame(W1);
  T2.AddChildFrame(Bjet2);
  T2.AddChildFrame(W2);
  
  W1.AddChildFrame(Lep1);
  W1.AddChildFrame(Neu1);
  W2.AddChildFrame(Lep2);
  W2.AddChildFrame(Neu2);


  //check that tree topology is consistent
  cout << "Is consistent decay topology? : " << LAB.InitializeTree() << endl; 

 

  InvisibleMassJigsaw MinMassJigsaw("MINMASS_JIGSAW", 
				    "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);

  InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", 
					 "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));
  // Same as this
  RapidityJigsaw.AddVisibleFrame(T1.GetListVisibleFrames());
  RapidityJigsaw.AddVisibleFrame(T2.GetListVisibleFrames());
  // Same as this
  RapidityJigsaw.AddVisibleFrame(Bjet1);
  RapidityJigsaw.AddVisibleFrame(Bjet2);
  RapidityJigsaw.AddVisibleFrame(Lep1);
  RapidityJigsaw.AddVisibleFrame(Lep2);
  
  ContraBoostInvariantJigsaw TTJigsaw("TT_JIGSAW","Contraboost invariant Jigsaw");
  INV.AddJigsaw(TTJigsaw);
  TTJigsaw.AddVisibleFrame((T1.GetListVisibleFrames()), 0);
  TTJigsaw.AddInvisibleFrame((T1.GetListInvisibleFrames()), 0);

  TTJigsaw.AddInvisibleFrame((T2.GetListInvisibleFrames()), 1);
  TTJigsaw.AddVisibleFrame((T2.GetListVisibleFrames()), 1);

  MinimizeMassesCombinatoricJigsaw BLJigsaw("BL_JIGSAW",
					    "Minimize m_{b#it{l}}'s Jigsaw");
  BTAGS.AddJigsaw(BLJigsaw);
  BLJigsaw.AddFrame(Bjet1,0);
  BLJigsaw.AddFrame(Lep1,0);
  
  BLJigsaw.AddFrame(Bjet2,1);
  BLJigsaw.AddFrame(Lep2,1);

  cout << "Is the tree ok for analysis? : " << LAB.InitializeAnalysis() << endl; 

  // draw your decay tree
  FramePlot* DecayTree_plot = new FramePlot("tree","Decay Tree");
  DecayTree_plot->AddFrameTree(LAB);
  DecayTree_plot->DrawFramePlot();

  // draw your decay tree with jigsaws
  FramePlot* JigsawTree_plot = new FramePlot("tree","Decay Tree with Jigsaws");
  JigsawTree_plot->AddFrameTree(TT);
  JigsawTree_plot->AddJigsaw(TTJigsaw);
  JigsawTree_plot->AddJigsaw(BLJigsaw);
  JigsawTree_plot->DrawFramePlot();

  //draw invisible group tree
  FramePlot* INVgroup_plot = new FramePlot("INVtree","Invisible Group");
  INVgroup_plot->AddGroupTree(INV);
  INVgroup_plot->DrawFramePlot();

  //draw b-tag group tree
  FramePlot* BTAGgroup_plot = new FramePlot("BTAGtree","B-tagged Jet Group");
  BTAGgroup_plot->AddGroupTree(BTAGS);
  BTAGgroup_plot->DrawFramePlot();
  

  // TEST EVENT
  TLorentzVector lepA, lepB, b1, b2, MET;
  lepA.SetPxPyPzE(1.,0.,0.,1.);
  lepB.SetPxPyPzE(0.,1.,0.,1.);
  b1.SetPxPyPzE(1.,0.,0.,1.);
  b2.SetPxPyPzE(0.,1.,0.,1.);
  MET.SetPxPyPzE(-.5,-.5, 0., 1.);

  LAB.ClearEvent();
  
  Lep1.SetLabFrameFourVector(lepA);
  Lep2.SetLabFrameFourVector(lepB);
  GroupElementID b1ID = BTAGS.AddLabFrameFourVector(b1);
  GroupElementID b2ID = BTAGS.AddLabFrameFourVector(b2);
  INV.SetLabFrameFourVector(MET);
  // same as
  INV.SetLabFrameThreeVector(MET.Vect());
  
  cout << "Analyzed event ok? : " << LAB.AnalyzeEvent() << endl;

  // example analysis methods
  double TTmass     = TT.GetMass();                  // mass
  double costhetaT1 = T1.GetCosDecayAngle();         // decay angle
  double P_T2_inTT  = T2.GetMomentum(TT);            // momentum in some frame
  TLorentzVector V_W1_in_W2 = W1.GetFourVector(W2);  // 4-vector in some frame
  double dphi_T1_W1 = T1.GetDeltaPhiDecayPlanes(W1); // angle between decay planes
  
  // object tracking in combinatoric groups
  cout << "Did b1 end up as Bjet1? : ";
  cout << BTAGS.GetFrame(b1ID)->IsSame(Bjet1) << endl;

}
