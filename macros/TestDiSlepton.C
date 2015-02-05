#include <TArc.h>
#include <TCanvas.h>
#include "src/RestFrame.cc"
#include "src/RestFrameList.cc"
#include "src/VisibleFrame.cc"
#include "src/InvisibleFrame.cc"
#include "src/DecayFrame.cc"
#include "src/LabFrame.cc"

#include "src/FrameLink.cc"

#include "src/RFrame.cc"
#include "src/RLabFrame.cc"
#include "src/RDecayFrame.cc"
#include "src/RVisibleFrame.cc"
#include "src/RInvisibleFrame.cc"

#include "src/Jigsaw.cc"
#include "src/JigsawList.cc"
#include "src/InvisibleJigsaw.cc"
#include "src/NVisibleMInvisibleJigsaw.cc"
#include "src/InvisibleMassJigsaw.cc"
#include "src/InvisibleRapidityJigsaw.cc"
#include "src/ContraBoostInvariantJigsaw.cc"

#include "src/Group.cc"
#include "src/GroupList.cc"
#include "src/InvisibleGroup.cc"

#include "src/State.cc"
#include "src/StateList.cc"

#include "src/FramePlot.cc"

void TestDiSlepton(){

  //////////////////////////////////////////////////////////////
  // Setup rest frames code
  //////////////////////////////////////////////////////////////
  cout << " Initialize lists of visible, invisible particles and intermediate states " << endl;
  RLabFrame *LAB = new RLabFrame("LAB","lab");
  RDecayFrame *SS = new RDecayFrame("SS","W^{+} W^{-}");
  RDecayFrame *S1 = new RDecayFrame("S1","W^{+}_{a}");
  RDecayFrame *S2 = new RDecayFrame("S2","W^{-}_{b}");
  RVisibleFrame *Lep1 = new RVisibleFrame("L1","#it{l}_{a}");
  RVisibleFrame *Lep2 = new RVisibleFrame("L2","#it{l}_{b}");
  //RInvisibleFrame *Neu1 = new RInvisibleFrame("NU1","(#nu+ #tilde{#chi} ^{0})_{a}");
  //RInvisibleFrame *Neu2 = new RInvisibleFrame("NU2","(#nu+ #tilde{#chi} ^{0})_{b}");
  RInvisibleFrame *Neu1 = new RInvisibleFrame("NU1","#nu_{a}");
  RInvisibleFrame *Neu2 = new RInvisibleFrame("NU2","#nu_{b}");


  cout << " Define invisible and combinatoric groups " << endl;
  InvisibleGroup *INV = new InvisibleGroup("INV","Invisible State Jigsaws");
  Neu1->SetGroup(INV);
  Neu2->SetGroup(INV);

  cout << " Build decay tree " << endl;
  LAB->AddChildFrame(SS);

  SS->AddChildFrame(S1);
  SS->AddChildFrame(S2);
  
  S1->AddChildFrame(Lep1);
  S1->AddChildFrame(Neu1);
  S2->AddChildFrame(Lep2);
  S2->AddChildFrame(Neu2);


  //check that tree topology is consistent
  cout << "Is consistent topology: " << LAB->IsConsistentTree() << endl; 

   cout << "Initializing jigsaw rules" << endl; 

  InvisibleMassJigsaw *MinMassJigsaw = new InvisibleMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
  INV->AddJigsaw(MinMassJigsaw);

  InvisibleRapidityJigsaw *RapidityJigsaw = new InvisibleRapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
  RapidityJigsaw->AddVisibleFrame(LAB->GetListVisibleFrames());
  INV->AddJigsaw(RapidityJigsaw);
  
  ContraBoostInvariantJigsaw *TopJigsaw = new ContraBoostInvariantJigsaw("TOP_JIGSAW","Contraboost invariant Jigsaw");
  INV->AddJigsaw(TopJigsaw);
  TopJigsaw->AddVisibleFrame(S1->GetListVisibleFrames(), 0);
  TopJigsaw->AddVisibleFrame(S2->GetListVisibleFrames(), 1);
  TopJigsaw->AddInvisibleFrame(S1->GetListInvisibleFrames(), 0);
  TopJigsaw->AddInvisibleFrame(S2->GetListInvisibleFrames(), 1);

  cout << "Initializing the tree for analysis : " << LAB->InitializeAnalysis() << endl; 
  
   //draw tree with jigsaws
  FramePlot *jigsaw_plot = new FramePlot();
  jigsaw_plot->SetCanvas("c_frames",600,600);
  jigsaw_plot->DrawRFrameTree(LAB,TopJigsaw);
  //jigsaw_plot->DrawFrameTree(LAB);
  TCanvas *c1 = jigsaw_plot->GetCanvas();
  c1->Draw();

   //draw tree with invisible group
  FramePlot *group_plot = new FramePlot();
  group_plot->SetCanvas("c_group",600,600);
  group_plot->DrawGroupTree(INV);
  TCanvas *c2 = group_plot->GetCanvas();
  c2->Draw();
  
}
