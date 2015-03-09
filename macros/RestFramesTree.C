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
#include "RestFrames/RSelfAssemblingFrame.hh"
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

int RestFramesTree(){
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  // Setup RestFrames topology code
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
 
  //////////////////////////////////////////////////////////////
  // Declare the 'frames' in your decay tree
  //////////////////////////////////////////////////////////////
  RLabFrame LAB("LAB","R");
  RDecayFrame TT("TT","E");
  RSelfAssemblingFrame T1("T1","S");
  RSelfAssemblingFrame T2("T2","T");
  RVisibleFrame Bjet1("B1","F");
  
  RVisibleFrame Lep1("L1","r");
RVisibleFrame Bjet2("B2","m");
  RVisibleFrame Lep2("L2","e");
  RInvisibleFrame Neu1("NU1","a");
  RInvisibleFrame Neu2("NU2","s");

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
  T1.AddChildFrame(Lep1);
  T2.AddChildFrame(Bjet2);
  T2.AddChildFrame(Lep2);
  T1.AddChildFrame(Neu1);
  T2.AddChildFrame(Neu2);


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
  JigsawTree_plot->AddFrameTree(LAB);
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
  
  

}
