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

void TestGqb(){

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
  RDecayFrame SS("SS","#tilde{g} #tilde{g}");
  RSelfAssemblingFrame S1("S1","#tilde{g}_{a}");
  RSelfAssemblingFrame S2("S2","#tilde{g}_{b}");
  RDecayFrame SB1("sB1","#tilde{b}_{a}");
  RDecayFrame SB2("sB2","#tilde{b}_{b}");
  RVisibleFrame V1("V1","V_{a}");
  RVisibleFrame V2("V2","V_{b}");
  RVisibleFrame B1("V1","b_{a}");
  RVisibleFrame B2("V2","b_{b}");
  RInvisibleFrame I1("I1","#tilde{#chi}_{a}");
  RInvisibleFrame I2("I2","#tilde{#chi}_{b}");

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
  CombinatoricGroup VIS("VIS","Light Jet Object Jigsaws");
  VIS.AddFrame(V1);
  VIS.SetNElementsForFrame(V1,1,false);
  VIS.AddFrame(V2);
  VIS.SetNElementsForFrame(V2,1,false);

  CombinatoricGroup BTAGS("BTAG","B-tagged jet Jigsaws");
  BTAGS.AddFrame(B1);
  BTAGS.SetNElementsForFrame(V1,1,true);
  BTAGS.AddFrame(B2);
  BTAGS.SetNElementsForFrame(V2,1,true);

  //////////////////////////////////////////////////////////////
  // define how all the frames connect in the decay tree
  //////////////////////////////////////////////////////////////
  std::cout << " Build decay tree " << endl;
  LAB.SetChildFrame(SS);

  SS.AddChildFrame(S1);
  SS.AddChildFrame(S2);

  S1.AddChildFrame(V1);
  S1.AddChildFrame(SB1);
  S2.AddChildFrame(V2);
  S2.AddChildFrame(SB2);

  SB1.AddChildFrame(B1);
  SB1.AddChildFrame(I1);
  SB2.AddChildFrame(B2);
  SB2.AddChildFrame(I2);

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
  RapidityJigsaw.AddVisibleFrame((SS.GetListVisibleFrames()));
  
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

  MinimizeMassesCombinatoricJigsaw BtagsJigsaw("BTAGS_JIGSAW","Minimize m _{#tilde{b}_{a,b}} Jigsaw");
  BTAGS.AddJigsaw(BtagsJigsaw);
  BtagsJigsaw.AddFrame(B1,0);
  BtagsJigsaw.AddFrame(I1,0);
  BtagsJigsaw.AddFrame(V1,0);
  BtagsJigsaw.AddFrame(B2,1);
  BtagsJigsaw.AddFrame(I2,1);
  BtagsJigsaw.AddFrame(V2,1);

  //////////////////////////////////////////////////////////////
  // check to make sure that all the jigsaws etc. are correctly connected
  //////////////////////////////////////////////////////////////
  std::cout << "Is consistent analysis tree? : " << LAB.InitializeAnalysis() << endl; 

  //////////////////////////////////////////////////////////////
  // draw some pictures of our tree
  //////////////////////////////////////////////////////////////
  FramePlot* TreePlot = new FramePlot("tree","Decay Tree");
  TreePlot->AddFrameTree(LAB);
  TreePlot->AddJigsaw(MinMassJigsaw);
  TreePlot->AddJigsaw(ContraBoostJigsaw);
  TreePlot->AddJigsaw(BtagsJigsaw);
  TreePlot->AddJigsaw(HemiJigsaw);
  TreePlot->DrawFramePlot();

  /*
  FramePlot* INVPlot = new FramePlot("inv","Invisible Objects Jigsaws");
  INVPlot->DrawGroupTree(INV);

  FramePlot* VISPlot = new FramePlot("vis","Visible Objects Jigsaws");
  VISPlot->DrawGroupTree(VIS);

  FramePlot* BTAGPlot = new FramePlot("btags","B-tagged Objects Jigsaws");
  BTAGPlot->DrawGroupTree(BTAGS);
  */
}
