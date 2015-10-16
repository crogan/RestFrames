#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <iostream>
#include "RestFrames/RestFrames.hh"

using namespace std;
using namespace RestFrames;

void TestSelfAssembling(){

  //////////////////////////////////////////////////////////////
  // Setup rest frames code
  //////////////////////////////////////////////////////////////
  std::cout << " Initialize lists of visible, invisible particles and intermediate states " << endl;
  LabRecoFrame LAB("LAB","lab");
  SelfAssemblingRecoFrame S1("S1","CM");
  VisibleRecoFrame V1("V1","#vec{p}");
  InvisibleRecoFrame I1("I1","#vec{E}_{T}^{miss}");

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

  SetMassInvJigsaw MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);

  SetRapidityInvJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrames((LAB.GetListVisibleFrames()));

  cout << "Initializing the tree for analysis : " << endl;
  cout << LAB.InitializeAnalysis() << endl; 

  //draw tree with jigsaws
  TreePlot* treePlot = new TreePlot("tree","Decay Tree");
  treePlot->SetFrameTree(S1);
  //TreePlot->AddJigsaw(RapidityJigsaw);
  treePlot->Draw();
  
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
    vector<RFKey> jetID;
    for(int i = 0; i < Njet; i++) jetID.push_back(VIS.AddLabFrameFourVector(JETS[i]));
    INV.SetLabFrameThreeVector(MET);
    LAB.AnalyzeEvent();
   
    int N = 0;
    for(int i = 0; i < Njet; i++){
      const RestFrame& frame = S1.GetFrame(jetID[i]);
      const RestFrame& prod = frame.GetProductionFrame();
      if(!frame.IsEmpty()) cout << frame.GetName().c_str() << " " << S1.GetFrameDepth(frame) << " " << prod.GetName().c_str() << endl;
    }
    const RestFrame& prod = I1.GetProductionFrame();
    cout << "Invis " << S1.GetFrameDepth(I1) << " " << prod.GetName().c_str() << endl;

    const RestFrame& depth2 = S1.GetFrameAtDepth(2, I1);
    if(!depth2.IsEmpty()) cout << "Invis (2) " << depth2.GetName().c_str() << endl;
  }
  treePlot->SetFrameTree(S1);
  //TreePlot->AddJigsaw(RapidityJigsaw);
  treePlot->Draw();
  
}

