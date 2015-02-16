#include "TRandom.h"
#include "TH1D.h"
#include "RestFrames/GLabFrame.hh"
#include "RestFrames/GDecayFrame.hh"
#include "RestFrames/GVisibleFrame.hh"
#include "RestFrames/GInvisibleFrame.hh"
#include "RestFrames/FramePlot.hh"

using namespace RestFrames;

void TestHtoWWto2l2nu(){

  //////////////////////////////////////////////////////////////
  // Generator-level RestFrames Tree
  //////////////////////////////////////////////////////////////
  GLabFrame LAB_GEN("LAB_GEN","lab");
  GDecayFrame H_GEN("H_GEN","h");
  GDecayFrame Wa_GEN("Wa_GEN","W^{+}_{a}");
  GDecayFrame Wb_GEN("Wb_GEN","W^{-}_{b}");
  GVisibleFrame La_GEN("La_GEN","#it{l}_{a}");
  GVisibleFrame Lb_GEN("Lb_GEN","#it{l}_{b}");
  GInvisibleFrame NUa_GEN("NUa_GEN","#nu_{a}");
  GInvisibleFrame NUb_GEN("NUb_GEN","#nu_{b}");

  LAB_GEN.AddChildFrame(H_GEN);
  H_GEN.AddChildFrame(Wa_GEN);
  H_GEN.AddChildFrame(Wb_GEN);
  Wa_GEN.AddChildFrame(La_GEN);
  Wa_GEN.AddChildFrame(NUa_GEN);
  Wb_GEN.AddChildFrame(Lb_GEN);
  Wb_GEN.AddChildFrame(NUb_GEN);

  cout << "Is consistent generator topology: " << LAB_GEN.InitializeTree() << endl; 
  
  H_GEN.SetMass(200.);
  Wa_GEN.SetMass(80.);
  Wb_GEN.SetMass(80.);

  FramePlot* tree_GEN = new FramePlot("HtoWW","HtoWW");
  tree_GEN->AddFrameTree(LAB_GEN);
  tree_GEN->DrawFramePlot();

  TH1D *hist = new TH1D("h","h",100,0.,2.*acos(-1.));

  // Generate toy events
  int Nevent = 1000;
  for(int ievent = 0; ievent < Nevent; ievent++){
    LAB_GEN.ClearEvent();
    LAB_GEN.SetTransverseMomenta(10.);
       
    double dphi_Wa = gRandom->Rndm()*acos(-1.)*2.;
    Wa_GEN.SetDeltaPhiDecayPlane(dphi_Wa);

    double dphi_Wb = gRandom->Rndm()*acos(-1.)*2.;
    Wb_GEN.SetDeltaPhiDecayPlane(dphi_Wb);

    double dphi_H = gRandom->Rndm()*acos(-1.)*2.;
    H_GEN.SetDeltaPhiDecayPlane(dphi_H);
    
    double cos_Wa = 1.-2.*gRandom->Rndm();
    Wa_GEN.SetCosDecayAngle(cos_Wa);

    double cos_Wb = 1.-2.*gRandom->Rndm();
    Wb_GEN.SetCosDecayAngle(cos_Wb);

    double cos_H = 1.-2.*gRandom->Rndm();
    H_GEN.SetCosDecayAngle(cos_H);

    LAB_GEN.AnalyzeEvent();

    /*
    TLorentzVector H = H_GEN.GetFourVector();
    TLorentzVector W1 = Wa_GEN.GetFourVector();
    TLorentzVector W2 = Wb_GEN.GetFourVector();
    TLorentzVector L1 = La_GEN.GetFourVector();
    TLorentzVector L2 = Lb_GEN.GetFourVector();
    TLorentzVector NU1 = NUa_GEN.GetFourVector();
    TLorentzVector NU2 = NUb_GEN.GetFourVector();
    */
    
    if((fabs(H_GEN.GetDeltaPhiDecayPlanes(Wa_GEN)-dphi_Wa) > 0.00001)) cout << "dphi check: " <<  H_GEN.GetDeltaPhiDecayPlanes(Wa_GEN) << " " << dphi_Wa << endl;
    if((fabs(H_GEN.GetDeltaPhiDecayPlanes(Wb_GEN)-dphi_Wb) > 0.00001)) cout << "dphi check: " <<  H_GEN.GetDeltaPhiDecayPlanes(Wb_GEN) << " " << dphi_Wb << endl;
    if((fabs(LAB_GEN.GetDeltaPhiDecayPlanes(H_GEN)-dphi_H) > 0.00001)) cout << "dphi check: " <<  LAB_GEN.GetDeltaPhiDecayPlanes(H_GEN) << " " << dphi_H << endl;

    if((fabs(Wa_GEN.GetCosDecayAngle()-cos_Wa) > 0.00001)) cout << "costheta check: " <<  Wa_GEN.GetCosDecayAngle() << " " << cos_Wa << endl;
    if((fabs(Wb_GEN.GetCosDecayAngle()-cos_Wb) > 0.00001)) cout << "costheta check: " <<  Wb_GEN.GetCosDecayAngle() << " " << cos_Wb << endl;
    if((fabs(H_GEN.GetCosDecayAngle()-cos_H) > 0.00001)) cout << "costheta check: " <<  H_GEN.GetCosDecayAngle() << " " << cos_H << endl;

    /*
    cout << "H mass: " << H.M() << " " << (W1+W2).M() << " " << (L1+L2+NU1+NU2).M() << endl;
    cout << "W1 mass: " << W1.M() << " " << (L1+NU1).M() << endl;
    cout << "W2 mass: " << W2.M() << " " << (L2+NU2).M() << endl;
    cout << "lepton masses: " << L1.M() << " " << L2.M() << " " << NU1.M() << " " << NU2.M() << endl;
    cout << endl;
    */
  }
 
}
