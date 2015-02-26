#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TSystem.h>
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

void TestSelfAssembling(){

  //////////////////////////////////////////////////////////////
  // Setup rest frames code
  //////////////////////////////////////////////////////////////
  std::cout << " Initialize lists of visible, invisible particles and intermediate states " << endl;
  RestFrames::RLabFrame LAB("LAB","lab");
  RSelfAssemblingFrame S1("S1","CM");
  RVisibleFrame V1("V1","#vec{p}");
  RInvisibleFrame I1("I1","#vec{E}_{T}^{miss}");

  std::cout << " Define invisible and combinatoric groups " << endl;
  InvisibleGroup INV("INV","Invisible State Jigsaws");
  INV.AddFrame(I1);

  CombinatoricGroup VIS("VIS","Visible Object Jigsaws");
  VIS.AddFrame(V1);
  VIS.SetNElementsForFrame(V1,1,false);

  std::cout << " Build decay tree " << endl;
  LAB.SetChildFrame(S1);

  S1.AddChildFrame(V1);
  S1.AddChildFrame(I1);

  //check that tree topology is consistent
  std::cout << "Is consistent topology: " << LAB.InitializeTree() << endl; 

  std::cout << "Initializing jigsaw rules" << endl; 

  InvisibleMassJigsaw MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);

  InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));

  cout << "Initializing the tree for analysis : " << endl;
  cout << LAB.InitializeAnalysis() << endl; 

  //draw tree with jigsaws
  FramePlot* TreePlot = new FramePlot("tree","Decay Tree");
  TreePlot->AddFrameTree(S1);
  //TreePlot->AddJigsaw(RapidityJigsaw);
  TreePlot->DrawFramePlot();
  
  //
  // TEST EVENT Reconcstruction
  // 
  int Njetmax = 12;
  int Ntrial = 1;
  for(int itrial = 0; itrial < Ntrial; itrial++){
    cout << "Event #" << itrial << " : " << Ntrial << endl;

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
    vector<GroupElementID> jetID;
    for(int i = 0; i < Njet; i++) jetID.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    INV.SetLabFrameThreeVector(MET);
    LAB.AnalyzeEvent();
   
    int N = 0;
    for(int i = 0; i < Njet; i++){
      const RestFrame* frame = S1.GetFrame(jetID[i]);
      const RestFrame* prod = frame->GetProductionFrame();
      if(frame) cout << frame->GetName().c_str() << " " << S1.GetFrameDepth(frame) << " " << prod->GetName().c_str() << endl;
    }
    const RestFrame* prod = I1.GetProductionFrame();
    cout << "Invis " << S1.GetFrameDepth(I1) << " " << prod->GetName().c_str() << endl;

    const RestFrame* depth2 = S1.GetFrameAtDepth(2, I1);
    if(depth2) cout << "Invis (2) " << depth2->GetName().c_str() << endl;
  }
  TreePlot->AddFrameTree(S1);
  //TreePlot->AddJigsaw(RapidityJigsaw);
  TreePlot->DrawFramePlot();
  
}

