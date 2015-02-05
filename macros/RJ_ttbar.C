#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TStyle.h>
#include <TF1.h>
#include <iostream>
#include <string>
#include <vector>

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

#include "src/CombinatoricJigsaw.cc"
#include "src/MinimizeMassesCombinatoricJigsaw.cc"

#include "src/Group.cc"
#include "src/GroupList.cc"
#include "src/InvisibleGroup.cc"
#include "src/CombinatoricGroup.cc"

#include "src/State.cc"
#include "src/StateList.cc"
#include "src/InvisibleState.cc"
#include "src/CombinatoricState.cc"

#include "src/FramePlot.cc"

using namespace std; 
using namespace RestFrames;

void setstyle();
TColor *icolor[9][2];
int color_list[10];

///////////stuff for getting gamma distribution from PDF's
int type; //0-q qbar 1-gg

double rootS, M, C;

double eta_1 = 0.27871;
double eta_2 = 3.3627;
double eps_u = 4.4343;
double g_u = 38.599;

double del_S = -0.11912;
double eta_S = 9.4189;
double eps_S = -2.6287;
double g_S = 18.065;

double A_g = 3.4055;
double del_g = -0.12178;
double eta_g = 2.9278;
double eps_g = -2.3210;
double g_g = 1.9233;
double A_g1 = -1.6189;
double del_g1 = -0.23999;
double eta_g1 = 24.792;

double PDF_v(double x);
double PDF_S(double x);
double PDF_g(double x);
double PDF_TOT(double x);
double Calc_dsigma_dgamma(double gamma);

///////// particle 4-vectors
TLorentzVector P[2]; //parent particles 
TLorentzVector C_1[2], C_2[2]; //first set of child particles, indexed by hemisphere
TLorentzVector C_1_1[2], C_1_2[2]; //second set of child particles, indexed by hemisphere
TLorentzVector C_1_1_1[2], C_1_1_2[2]; //third set of child particles, indexed by hemisphere
TLorentzVector C_1_1_1_1[2], C_1_1_1_2[2]; //fourth set of child particles, indexed by hemisphere
TLorentzVector vMiss[2];

void BoostToLabFrame(double gamma_eff);
void BoostHem(int iHem, TVector3 vBETA);
void PtBoost(double PT);

void GetLepTopHem(int iHem, double Mtop, double MW, double Mb, double Mlep, double Mnu);

TCanvas* Plot_Me_2D(char *titlecan, TH2D* histo, char *titleX, char *titleY);

void RJ_ttbar(){
  setstyle();
	
  //give transverse momenta to CM system in lab frame?
  double PT = 0.1; //In units of sqrt{shat}
	
  //gamma factor associated with 'off-threshold-ness' of tops
  double gamma = 1.2;
	
  //Now, we also have the option to take gamma, event-by-event, 
  //from a more realistic distribution
  //to do this, set 'b_gamma' to true and the rest
  //of the variables below appropriately
  bool b_gamma = true;
  rootS = 13.;
  type = 1; //0 quark-antiquark  1 gluon-gluon
  M = 175./1000.; //TeV units
	
  //Number of toy events to throw
  int N = 1000;

  //
  // Generate fake events taking flat ME's for all decay angles
  //
	
  //here, we set up the stuff for dynamic gamma
  TH1D *h_gamma = (TH1D*) new TH1D("newgamma","newgamma",500,1.0,10.0);
  if(b_gamma){
    cout << "generating gamma distribution" << endl;
    for(int ibin = 1; ibin <= 500; ibin++){
      double g = h_gamma->GetBinCenter(ibin);
      double entry = Calc_dsigma_dgamma(g);
    
      if(entry > 0.)
	h_gamma->SetBinContent(ibin, entry);
    }
    cout << "done" << endl;
  }
  
  //////////////////////////////////////////////////////////////
  // Setup rest frames code
  //////////////////////////////////////////////////////////////
  cout << " Initialize lists of visible, invisible particles and intermediate states " << endl;
  RLabFrame* LAB = new RLabFrame("LAB","lab");
  RDecayFrame* TT = new RDecayFrame("TT","t #bar{t}");
  RDecayFrame* T1 = new RDecayFrame("T1","t_{a}");
  RDecayFrame* T2 = new RDecayFrame("T2","t_{b}");
  RVisibleFrame* Bjet1 = new RVisibleFrame("B1","b_{a}");
  RVisibleFrame* Bjet2 = new RVisibleFrame("B2","b_{b}");
  RDecayFrame* W1 = new RDecayFrame("W1","W_{a}");
  RDecayFrame* W2 = new RDecayFrame("W2","W_{b}");
  RVisibleFrame* Lep1 = new RVisibleFrame("L1","#it{l}_{a}");
  RVisibleFrame* Lep2 = new RVisibleFrame("L2","#it{l}_{b}");
  RInvisibleFrame* Neu1 = new RInvisibleFrame("NU1","#nu_{a}");
  RInvisibleFrame* Neu2 = new RInvisibleFrame("NU2","#nu_{b}");

  cout << " Define invisible and combinatoric groups " << endl;
  InvisibleGroup INV("INV","Invisible State Jigsaws");
  INV.AddFrame(Neu1);
  INV.AddFrame(Neu2);

  CombinatoricGroup BTAGS("BTAGS","B-tagged jet Jigsaws");
  BTAGS.AddFrame(Bjet1);
  BTAGS.SetNElementsForFrame(Bjet1,1,true);
  BTAGS.AddFrame(Bjet2);
  BTAGS.SetNElementsForFrame(Bjet2,1,true);

  cout << " Build decay tree " << endl;
  LAB->SetChildFrame(TT);

  TT->AddChildFrame(T1);
  TT->AddChildFrame(T2);
  
  T1->AddChildFrame(Bjet1);
  T1->AddChildFrame(W1);
  T2->AddChildFrame(Bjet2);
  T2->AddChildFrame(W2);
  
  W1->AddChildFrame(Lep1);
  W1->AddChildFrame(Neu1);
  W2->AddChildFrame(Lep2);
  W2->AddChildFrame(Neu2);


  //check that tree topology is consistent
  cout << "Is consistent topology: " << LAB->InitializeTree() << endl; 

  cout << "Initializing jigsaw rules" << endl; 

  InvisibleMassJigsaw MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw");
  INV.AddJigsaw(MinMassJigsaw);

  InvisibleRapidityJigsaw RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw");
  INV.AddJigsaw(RapidityJigsaw);
  RapidityJigsaw.AddVisibleFrame((LAB->GetListVisibleFrames()));
  
  
  ContraBoostInvariantJigsaw TopJigsaw("TOP_JIGSAW","Contraboost invariant Jigsaw");
  INV.AddJigsaw(TopJigsaw);
  TopJigsaw.AddVisibleFrame((T1->GetListVisibleFrames()), 0);
  TopJigsaw.AddVisibleFrame((T2->GetListVisibleFrames()), 1);
  TopJigsaw.AddInvisibleFrame((T1->GetListInvisibleFrames()), 0);
  TopJigsaw.AddInvisibleFrame((T2->GetListInvisibleFrames()), 1);

  MinimizeMassesCombinatoricJigsaw BLJigsaw("BL_JIGSAW","Minimize m_{b#it{l}}'s Jigsaw");
  BTAGS.AddJigsaw(BLJigsaw);
  BLJigsaw.AddFrame(Bjet1,0);
  BLJigsaw.AddFrame(Lep1,0);
  BLJigsaw.AddFrame(Bjet2,1);
  BLJigsaw.AddFrame(Lep2,1);

  cout << "Initializing the tree for analysis : " << LAB->InitializeAnalysis() << endl; 

  //draw tree with jigsaws
  FramePlot* jigsaw_plot = new FramePlot("tree","Decay Tree");
  jigsaw_plot->AddFrameTree(LAB);
  jigsaw_plot->AddJigsaw(TopJigsaw);
  jigsaw_plot->AddJigsaw(BLJigsaw);
  jigsaw_plot->DrawFramePlot();
	
  for(int i = 0; i < N; i++){
    if(b_gamma){
      gamma = h_gamma->GetRandom();
    }
   	
    //////////////////////////////////////////////////////////////
    // BEGIN CODE TO generate toy GEN LEVEL events
    //////////////////////////////////////////////////////////////
    double Mt1 = 175.;
    double MW1 = 80.;
    double Mt2 = 175.;
    double MW2 = 80.;
		
    double Mnu1 = 0.;
    double Mnu2 = 0.;
		
    GetLepTopHem(0, Mt1, MW1, 5., 0.005, Mnu1);
    GetLepTopHem(1, Mt2, MW2, 5., 0.005, Mnu2);
		
    double EB1 = (Mt1*Mt1 - MW1*MW1)/(2.*Mt1);
    double EB2 = (Mt2*Mt2 - MW2*MW2)/(2.*Mt2);
		
    double EVIS1 = (Mt1*Mt1 - Mnu1*Mnu1)/(Mt1);
    double EVIS2 = (Mt2*Mt2 - Mnu2*Mnu2)/(Mt2);
		
    double EW1 = (Mt1*Mt1 + MW1*MW1)/(2.*Mt1);
    double EW2 = (Mt2*Mt2 + MW2*MW2)/(2.*Mt2);
		
    double EL1 = (MW1*MW1-Mnu1*Mnu1)/(2.*MW1);
    double EL2 = (MW2*MW2-Mnu2*Mnu2)/(2.*MW2);
		
    double gamma1 = EW1/MW1;
    double gamma2 = EW2/MW2;
    double beta1 = 1.;
    double beta2 = 1.;
		
    //put them in the lab frame
    BoostToLabFrame(gamma);
		
    // effective gamma factor for parents in CM frame
    double g_eff = (P[0]+P[1]).M()/(2.*sqrt(P[0].M()*P[1].M()));
		
    PtBoost(PT);
    //////////////////////////////////////////////////////////////
    // END CODE TO generate toy GEN LEVEL event
    //////////////////////////////////////////////////////////////
        
    //////////////////////////////////////////////////////////////
    // BEGIN CODE TO analyze GEN LEVEL events
    //////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////
    // First, we calculate approximate neutrino 4-vectors
    // in W frames using two different strategies
    //////////////////////////////////////////////////////////////
        
    TLorentzVector NU1_top, NU2_top, NU1_W, NU2_W;
      
    //
    // first, we will assign 4-vectos in the lab frame for
    // NU1_top and NU2_top by drawing a contraboost invariant symmetry 
    // between the two BL systems and neutrinos. 
    // Here, the tops are assumed to have the same boost factor in TT 
    // rest frame.
    //
    for(;;){
      // get MET in lab frame 
      TVector3 MET = (vMiss[0]+vMiss[1]).Vect();
      MET.SetZ(0.0);
      // get b-quarks in lab frame
      TLorentzVector B1, B2;
      B1 = C_2[0];
      B2 = C_2[1];
      // get leptons in lab frame
      TLorentzVector L1, L2;
      L1 = C_1_1[0];
      L2 = C_1_1[1];
      //
      // We separate the objects into decay hemispheres
      // practice we will need to assign BL pairs according to
      // metric (see commented below)
      //
      
      TLorentzVector VISTOT = B1+L1+B2+L2;
      TVector3 boostTOT = VISTOT.BoostVector();
      B1.Boost(-boostTOT);
      B2.Boost(-boostTOT);
      L1.Boost(-boostTOT);
      L2.Boost(-boostTOT);
      if( (B1+L2).P() > (B1+L1).P() ){
	TLorentzVector temp = B1;
	B1 = B2;
	B2 = temp;
      }
      B1.Boost(boostTOT);
      B2.Boost(boostTOT);
      L1.Boost(boostTOT);
      L2.Boost(boostTOT);

      //
      // visible hemispheres of top decays
      //
      TLorentzVector H1 = (B1+L1);
      TLorentzVector H2 = (B2+L2);

      //
      // We will now attempt to travel through approximations of each of the rest frames 
      // of interest, beginning in the lab frame and ending in the TT rest frame, where
      // we will specify the neutrino 4-momenta according to a contraboost invariant 
      // symmetry between the visible and invisible decay products of the two tops
      //
      
      //
      // first, we boost from the lab frame to intermediate 'CMz' frame
      // i.e. with the visible hemispheres' vectoral sum zero
      //
      TVector3 BL = H1.Vect()+H2.Vect();
      BL.SetX(0.0);
      BL.SetY(0.0);
      BL = (1./(H1.E()+H2.E()))*BL;
		
      B1.Boost(-BL);
      L1.Boost(-BL);
      B2.Boost(-BL);
      L2.Boost(-BL);
      H1.Boost(-BL);
      H2.Boost(-BL);
		 
      //
      // Now, we need to 'guess' the invariant mass of the weakly interacting system
      // as a Lorentz-invariant quantity which is a function of the visible system
      // masses, and large enough to accomodate the contraboost invariant 
      // solution we will use in the following TT CM frame. This allows us to write down
      // the transverse part of the boost from lab to TT CM frame.
      //
      
      double Minv2 = (H1+H2).M2() - 4.*H1.M()*H2.M();
      //double Minv2 = (H1+H2).M2() - 4.*min(H1.M2(),H2.M2());

      //
      // transverse momentum of total TT system in lab frame
      // NOTE: (H1+H2).Pz() == 0 due to previous longitudinal boost
      //
      TVector3 PT = MET+(H1+H2).Vect();
      double Einv2 = MET.Mag2() + Minv2;
      PT.SetZ(0.0); // redundant
      //
      // transverse boost from 'CMz' frame to TT CM frame
      //
      TVector3 BT = (1./( (H1+H2).E() + sqrt(Einv2) ))*PT;
      B1.Boost(-BT);
      L1.Boost(-BT);
      B2.Boost(-BT);
      L2.Boost(-BT);
      H1.Boost(-BT);
      H2.Boost(-BT);
      
      //
      // Now, in TT CM approx frame we will make a contraboost invariant choise for the assymmetric boost, then specifying 
      // neutrino 4-vectors
      // 
      //
      // contra-boost invariant quantities (for this boost)
      //
      double m1 = H1.M();
      double m2 = H2.M();
      double MC2 = 2.*( H1.E()*H2.E() + H1.Vect().Dot(H2.Vect()) );
     
      //
      // contra-boost invariant coefficients
      //
      double k1 = m1*m1-m2*m2 + MC2 - 2.*m1*m2;
      double k2 = m2*m2-m1*m1 + MC2 - 2.*m1*m2;

      double N = ( fabs(k1*m1*m1 - k2*m2*m2) - 0.5*fabs(k2-k1)*MC2 + 0.5*(k1+k2)*sqrt(MC2*MC2-4.*m1*m1*m2*m2) )/(k1*k1*m1*m1+k2*k2*m2*m2+k1*k2*MC2);

      double c1 = 0.5*(1.+N*k1);
      double c2 = 0.5*(1.+N*k2);

      //c1 = 1;
      //c2 = 1;
		
      // 
      // adjust coefficients such that di-neutrino invariant mass is equal to the Lorentz-invariant value we chose earlier,
      // maintaining a contra-boost invariant ratio
      //
      double A = ( H1.E()+H2.E() + sqrt( (H1.E()+H2.E())*(H1.E()+H2.E()) -(H1+H2).M2() + Minv2 ))/(c1*H1.E()+c2*H2.E())/2.;

      c1 *= A;
      c2 *= A;
                
      double Enu1 = (c1-1.)*H1.E() + c2*H2.E();
      double Enu2 = c1*H1.E() + (c2-1.)*H2.E();
      TVector3 pnu1 = (c1-1.)*H1.Vect() - c2*H2.Vect();
      TVector3 pnu2 = (c2-1.)*H2.Vect() - c1*H1.Vect();

      //
      // define neutrino 4-vectors
      //
      NU1_top.SetPxPyPzE(pnu1.X(),pnu1.Y(),pnu1.Z(),Enu1);
      NU2_top.SetPxPyPzE(pnu2.X(),pnu2.Y(),pnu2.Z(),Enu2);

      //
      // now, transform neutrinos back into lab frame
      //
      NU1_top.Boost(BT);
      NU2_top.Boost(BT);
      NU1_top.Boost(BL);
      NU2_top.Boost(BL);
            
      break;
    }
		
    //
    // Next, we will assign 4-vectos in the lab frame for
    // NU1_W and NU2_W by drawing a contraboost invariant symmetry 
    // between the two L systems and neutrinos. 
    // Here, the W's are assumed to have the same boost factor in the WW  
    // rest frame.
    //
    for(;;){
      TVector3 MET = (vMiss[0]+vMiss[1]).Vect();
      MET.SetZ(0.0);
      //get b-quarks in lab frame
      TLorentzVector B1, B2;
      B1 = C_2[0];
      B2 = C_2[1];
      //get leptons in lab frame
      TLorentzVector L1, L2;
      L1 = C_1_1[0];
      L2 = C_1_1[1];
      //
      // We separate the objects into decay hemispheres
      // practice we will need to assign BL pairs according to
      // metric (see commented below)
      //
      TLorentzVector VISTOT = B1+L1+B2+L2;
      TVector3 boostTOT = VISTOT.BoostVector();
      B1.Boost(-boostTOT);
      B2.Boost(-boostTOT);
      L1.Boost(-boostTOT);
      L2.Boost(-boostTOT);
      if( (B1+L2).P() > (B1+L1).P() ){
	TLorentzVector temp = B1;
	B1 = B2;
	B2 = temp;
      }
      B1.Boost(boostTOT);
      B2.Boost(boostTOT);
      L1.Boost(boostTOT);
      L2.Boost(boostTOT);
      //
      // visible hemispheres of top decays
      //
      TLorentzVector H1 = (B1+L1);
      TLorentzVector H2 = (B2+L2);
            
      //
      // We will now attempt to travel through approximations of each of the rest frames 
      // of interest, beginning in the lab frame and ending in the WW rest frame, where
      // we will specify the neutrino 4-momenta according to a contraboost invariant 
      // symmetry between the visible and invisible decay products of the two W's
      //
      
      //
      // first, we boost from the lab frame to intermediate 'CMz' frame
      // i.e. with the visible hemispheres' vectoral sum zero
      //
      TVector3 BL = H1.Vect()+H2.Vect();
      BL.SetX(0.0);
      BL.SetY(0.0);
      BL = (1./(H1.E()+H2.E()))*BL;
            
      B1.Boost(-BL);
      L1.Boost(-BL);
      B2.Boost(-BL);
      L2.Boost(-BL);
      H1.Boost(-BL);
      H2.Boost(-BL);
            
      //
      // Now, we need to 'guess' the invariant mass of the weakly interacting system
      // as a Lorentz-invariant quantity which is a function of the visible system
      // masses, and large enough to accomodate the contraboost invariant 
      // solution we will use in the following WW CM frame. This allows us to write down
      // the transverse part of the boost from lab to WW CM frame.
      //
      double Minv2 = (L1+L2).M2();
      TVector3 PT = MET+(L1+L2).Vect();
      double Einv2 = MET.Mag2() + Minv2;
     
      TVector3 BT = (1./( (L1+L2).E() + sqrt(Einv2) ))*PT;
      L1.Boost(-BT);
      L2.Boost(-BT);
      //
      // Now, in WW CM approx frame we will make a contraboost invariant choise for the assymmetric boost, then specifying 
      // neutrino 4-vectors
      // 
      //
      // contra-boost invariant coefficients are both 1, scaling factor to fix di-neutrino invariant mass
      // is 1, since (L1+L2).M2() = Minv2
      //
      double c = ( L1.E()+L2.E() + sqrt( (L1.E()+L2.E())*(L1.E()+L2.E()) -(L1+L2).M2() + Minv2 ))/(L1.E()+L2.E())/2.; //obviously redundant
            
      double Enu1 = (c-1.)*L1.E() + c*L2.E();
      double Enu2 = c*L1.E() + (c-1.)*L2.E();
      TVector3 pnu1 = (c-1.)*L1.Vect() - c*L2.Vect();
      TVector3 pnu2 = (c-1.)*L2.Vect() - c*L1.Vect();
            
      //define neutrino 4-vectors
      NU1_W.SetPxPyPzE(pnu1.X(),pnu1.Y(),pnu1.Z(),Enu1);
      NU2_W.SetPxPyPzE(pnu2.X(),pnu2.Y(),pnu2.Z(),Enu2);
            
      //now, go back to lab frame
      NU1_W.Boost(BT);
      NU2_W.Boost(BT);
      NU1_W.Boost(BL);
      NU2_W.Boost(BL);
            
      break;
    }
    //
    // Now we have our guess for the neutrino 4-vectors in the lab frame
    // using two different approaches (top or W symmetry, NUi_top and NUi_W
    // 4-vectors, respectively), along with the 'truth' values.
    // We will now go through all of the relevant reference frames in our approximate 
    // reconstructions and in truth simultaenously to calculate observables
    //
    TLorentzVector B1, B2;
    B1 = C_2[0];
    B2 = C_2[1];
    //get leptons in lab frame
    TLorentzVector L1, L2;
    L1 = C_1_1[0];
    L2 = C_1_1[1];
    //get truth neutrinos in lab frame
    TLorentzVector NU1, NU2;
    NU1 = C_1_2[0];
    NU2 = C_1_2[1];

    TVector3 MET = (vMiss[0]+vMiss[1]).Vect();
    MET.SetZ(0.0);

    INV.ClearEvent();
    BTAGS.ClearEvent();

    Lep1->SetLabFrameFourVector(L1);
    Lep2->SetLabFrameFourVector(L2);
    GroupElementID B1_ID = BTAGS.AddLabFrameFourVector(B1);
    GroupElementID B2_ID = BTAGS.AddLabFrameFourVector(B2);
    INV.SetLabFrameThreeVector(MET);

    LAB->AnalyzeEvent();

    //
    // Now we have two different sets of neutrino guesses in lab frame - define matching visible particles
    //
    TLorentzVector B1_top = B1;
    TLorentzVector B2_top = B2;
    TLorentzVector L1_top = L1;
    TLorentzVector L2_top = L2;
    TLorentzVector B1_W = B1;
    TLorentzVector B2_W = B2;
    TLorentzVector L1_W = L1;
    TLorentzVector L2_W = L2;
    //
    // We separate the objects into decay hemispheres
    // practice we will need to assign BL pairs according to
    // metric for 'reconstructed' events (see commented below)
    //
    
      TLorentzVector VISTOT_top = B1_top+L1_top+B2_top+L2_top;
      TVector3 boostTOT_top = VISTOT_top.BoostVector();
      B1_top.Boost(-boostTOT_top);
      B2_top.Boost(-boostTOT_top);
      L1_top.Boost(-boostTOT_top);
      L2_top.Boost(-boostTOT_top);
      if( (B1_top+L2_top).P() > (B1_top+L1_top).P() ){
	TLorentzVector temp = B1_top;
	B1_top = B2_top;
	B2_top = temp;
      }
      B1_top.Boost(boostTOT_top);
      B2_top.Boost(boostTOT_top);
      L1_top.Boost(boostTOT_top);
      L2_top.Boost(boostTOT_top);

      /*
      if( (B1_W+L1_W).M2()+(B2_W+L2_W).M2() > (B1_W+L2_W).M2()+(B2_W+L1_W).M2() ){
	TLorentzVector temp = L1_W;
	L1_W = L2_W;
	L2_W = temp;
      }
    */
    //
    // visible hemispheres of top decays
    //
    TLorentzVector H1 = L1+B1+NU1;
    TLorentzVector H2 = L2+B2+NU2;
    TLorentzVector H1_top = L1_top+B1_top+NU1_top;
    TLorentzVector H2_top = L2_top+B2_top+NU2_top;
    TLorentzVector H1_W = L1+B1+NU1_W;
    TLorentzVector H2_W = L2+B2+NU2_W;
    
    
    cout << "TT mass: " << (H1_top+H2_top).M() << " " << TT->GetMass() << endl;
    cout << "T1 mass: " << H1_top.M() << " " << T1->GetMass() << endl;
    cout << "T2 mass: " << H2_top.M() << " " << T2->GetMass() << endl;
    cout << "W1 mass: " << (L1_top+NU1_top).M() << " " << W1->GetMass() << endl;
    cout << "W2 mass: " << (L2_top+NU2_top).M() << " " << W2->GetMass() << endl;
    cout << "NU1 mass: " << NU1_top.M() << " " << Neu1->GetMass() << endl;
    cout << "NU2 mass: " << NU2_top.M() << " " << Neu2->GetMass() << endl;
    cout << "MET: " << (Neu1->GetFourVector(LAB)+Neu2->GetFourVector(LAB)).Pt() << " " << MET.Mag() << endl;
    
    /*
    cout << "TT mass: " << (H1_W+H2_W).M() << " " << TT->GetMass() << endl;
    cout << "T1 mass: " << H1_W.M() << " " << T1->GetMass() << endl;
    cout << "T2 mass: " << H2_W.M() << " " << T2->GetMass() << endl;
    cout << "W1 mass: " << (L1+NU1_W).M() << " " << W1->GetMass() << endl;
    cout << "W2 mass: " << (L2+NU2_W).M() << " " << W2->GetMass() << endl;
    cout << "NU1 mass: " << NU1_W.M() << " " << Neu1->GetMass() << endl;
    cout << "NU2 mass: " << NU2_W.M() << " " << Neu2->GetMass() << endl;
    */
    //
    // In the lab frame - let's move now to the ttbar CM frame
    // boosts
    //
    TVector3 BltoCM = (H1+H2).BoostVector();
    TVector3 BltoCM_top = (H1_top+H2_top).BoostVector();
    TVector3 BltoCM_W = (H1_W+H2_W).BoostVector();

    H1.Boost(-BltoCM);
    H2.Boost(-BltoCM);
    L1.Boost(-BltoCM);
    L2.Boost(-BltoCM);
    NU1.Boost(-BltoCM);
    NU2.Boost(-BltoCM);
    B1.Boost(-BltoCM);
    B2.Boost(-BltoCM);
    H1_top.Boost(-BltoCM_top);
    H2_top.Boost(-BltoCM_top);
    L1_top.Boost(-BltoCM_top);
    L2_top.Boost(-BltoCM_top);
    B1_top.Boost(-BltoCM_top);
    B2_top.Boost(-BltoCM_top);
    NU1_top.Boost(-BltoCM_top);
    NU2_top.Boost(-BltoCM_top);
    H1_W.Boost(-BltoCM_W);
    H2_W.Boost(-BltoCM_W);
    L1_W.Boost(-BltoCM_W);
    L2_W.Boost(-BltoCM_W);
    B1_W.Boost(-BltoCM_W);
    B2_W.Boost(-BltoCM_W);
    NU1_W.Boost(-BltoCM_W);
    NU2_W.Boost(-BltoCM_W);

    //
    // angles in the TT CM frames, approximate and true
    //
    double costhetaTT = H1.Vect().Unit().Dot( BltoCM.Unit() );
    double dphiTT = fabs(H1.Vect().DeltaPhi( BltoCM ));
    double dphiM  = fabs( (B1+B2+L1+L2).Vect().DeltaPhi( BltoCM ));
    double costhetaTT_top = fabs( H1_top.Vect().Unit().Dot( BltoCM_top.Unit() ));
    double dphiTT_top = fabs(H1_top.Vect().DeltaPhi( BltoCM_top ));
    double dphiM_top  = fabs( (B1_top+B2_top+L1_top+L2_top).Vect().DeltaPhi( BltoCM_top ));
    double costhetaTT_W = fabs( H1_W.Vect().Unit().Dot( BltoCM_W.Unit() ));
    double dphiTT_W = fabs(H1_W.Vect().DeltaPhi( BltoCM_W ));
    double dphiM_W  = fabs( (B1_W+B2_W+L1_W+L2_W).Vect().DeltaPhi( BltoCM_W ));
    //scale
    double MTT = (H1+H2).M();
    double MTT_top = (H1_top+H2_top).M();
    double MTT_W = (H1_W+H2_W).M();
    
    // vector normal to decay plane of CM frame (for later azimuthal angles)
    TVector3 vNORM_CM_T1 = B1.Vect().Cross((L1+NU1).Vect());
    TVector3 vNORM_CM_T2 = B2.Vect().Cross((L2+NU2).Vect());
    double dphi_T1_T2 = vNORM_CM_T1.Angle(vNORM_CM_T2);
    double dot_dphi_T1_T2 = B1.Vect().Dot(vNORM_CM_T2);
    if(dot_dphi_T1_T2 < 0.0 && dphi_T1_T2 > 0.0){
      dphi_T1_T2 = TMath::Pi()*2. - dphi_T1_T2;
    }
    TVector3 vNORM_CM_T1_top = B1_top.Vect().Cross((L1_top+NU1_top).Vect());
    TVector3 vNORM_CM_T2_top = B2_top.Vect().Cross((L2_top+NU2_top).Vect());
    double dphi_T1_T2_top = vNORM_CM_T1_top.Angle(vNORM_CM_T2_top);
    double dot_dphi_T1_T2_top = B1_top.Vect().Dot(vNORM_CM_T2_top);
    if(dot_dphi_T1_T2_top < 0.0 && dphi_T1_T2_top > 0.0){
      dphi_T1_T2_top = TMath::Pi()*2. - dphi_T1_T2_top;
    }
    TVector3 vNORM_CM_T1_W = B1_W.Vect().Cross((L1_W+NU1_W).Vect());
    TVector3 vNORM_CM_T2_W = B2_W.Vect().Cross((L2_W+NU2_W).Vect());
    double dphi_T1_T2_W = vNORM_CM_T1_W.Angle(vNORM_CM_T2_W);
    double dot_dphi_T1_T2_W = B1.Vect().Dot(vNORM_CM_T2_W);
    if(dot_dphi_T1_T2_W < 0.0 && dphi_T1_T2_W > 0.0){
      dphi_T1_T2_W = TMath::Pi()*2. - dphi_T1_T2_W;
    }
    
    double ddphi_T1_T2_top = sin(dphi_T1_T2_top-dphi_T1_T2);
    double ddphi_T1_T2_W = sin(dphi_T1_T2_W-dphi_T1_T2);

    //
    // To the next frames!!!!
    // now, to 'top' CM frame approxs and truth
    //
    TVector3 BCMtoT1 = H1.BoostVector();
    TVector3 BCMtoT2 = H2.BoostVector();
    TVector3 BCMtoT1_top = H1_top.BoostVector();
    TVector3 BCMtoT2_top = H2_top.BoostVector();
    TVector3 BCMtoT1_W = H1_W.BoostVector();
    TVector3 BCMtoT2_W = H2_W.BoostVector();

    B1.Boost(-BCMtoT1);
    B2.Boost(-BCMtoT2);
    L1.Boost(-BCMtoT1);
    L2.Boost(-BCMtoT2);
    NU1.Boost(-BCMtoT1);
    NU2.Boost(-BCMtoT2);
    B1_top.Boost(-BCMtoT1_top);
    B2_top.Boost(-BCMtoT2_top);
    L1_top.Boost(-BCMtoT1_top);
    L2_top.Boost(-BCMtoT2_top);
    NU1_top.Boost(-BCMtoT1_top);
    NU2_top.Boost(-BCMtoT2_top);
    B1_W.Boost(-BCMtoT1_W);
    B2_W.Boost(-BCMtoT2_W);
    L1_W.Boost(-BCMtoT1_W);
    L2_W.Boost(-BCMtoT2_W);
    NU1_W.Boost(-BCMtoT1_W);
    NU2_W.Boost(-BCMtoT2_W);

    cout <<  W1->GetFourVector(T1).E() << " " << (L1_top+NU1_top).E() << endl;
    cout <<  W2->GetFourVector(T2).E() << " " << (L2_top+NU2_top).E() << endl;

    //
    // decay angles in top frames
    //
    double costhetaT1 = B1.Vect().Unit().Dot(BCMtoT1.Unit());
    double costhetaT2 = B2.Vect().Unit().Dot(BCMtoT2.Unit());
    double costhetaT1_top = B1_top.Vect().Unit().Dot(BCMtoT1_top.Unit());
    double costhetaT2_top = B2_top.Vect().Unit().Dot(BCMtoT2_top.Unit());
    double costhetaT1_W = B1_W.Vect().Unit().Dot(BCMtoT1_W.Unit());
    double costhetaT2_W = B2_W.Vect().Unit().Dot(BCMtoT2_W.Unit());
    double dcosthetaT1_top = costhetaT1_top*sqrt(1.-costhetaT1*costhetaT1)-costhetaT1*sqrt(1.-costhetaT1_top*costhetaT1_top);
    double dcosthetaT2_top = costhetaT2_top*sqrt(1.-costhetaT2*costhetaT2)-costhetaT2*sqrt(1.-costhetaT2_top*costhetaT2_top);
    double dcosthetaT1_W = costhetaT1_W*sqrt(1.-costhetaT1*costhetaT1)-costhetaT1*sqrt(1.-costhetaT1_W*costhetaT1_W);
    double dcosthetaT2_W = costhetaT2_W*sqrt(1.-costhetaT2*costhetaT2)-costhetaT2*sqrt(1.-costhetaT2_W*costhetaT2_W);

    //vectors normal to decay planes of T frames
    TVector3 vNORM_T1_B = B1.Vect().Cross(BCMtoT1);
    TVector3 vNORM_T2_B = B2.Vect().Cross(BCMtoT2);
    TVector3 vNORM_T1_B_top = B1_top.Vect().Cross(BCMtoT1_top);
    TVector3 vNORM_T2_B_top = B2_top.Vect().Cross(BCMtoT2_top);
    TVector3 vNORM_T1_B_W = B1_W.Vect().Cross(BCMtoT1_W);
    TVector3 vNORM_T2_B_W = B2_W.Vect().Cross(BCMtoT2_W);
    //vectors normal to W decay planes in T frames
    TVector3 vNORM_T1_W = L1.Vect().Cross(NU1.Vect());
    TVector3 vNORM_T2_W = L2.Vect().Cross(NU2.Vect());
    TVector3 vNORM_T1_W_top = L1_top.Vect().Cross(NU1_top.Vect());
    TVector3 vNORM_T2_W_top = L2_top.Vect().Cross(NU2_top.Vect());
    TVector3 vNORM_T1_W_W = L1_W.Vect().Cross(NU1_W.Vect());
    TVector3 vNORM_T2_W_W = L2_W.Vect().Cross(NU2_W.Vect());

    double dphi_W_T1 = vNORM_T1_W.Angle(vNORM_T1_B);
    double dphi_W_T2 = vNORM_T2_W.Angle(vNORM_T2_B);
    double dot_dphi_W_T1 = L1.Vect().Dot(vNORM_T1_B);
    double dot_dphi_W_T2 = L2.Vect().Dot(vNORM_T2_B);
    if(dot_dphi_W_T1 < 0.0 && dphi_W_T1 > 0.0){
      dphi_W_T1 = TMath::Pi()*2. - dphi_W_T1;
    }
    if(dot_dphi_W_T2 < 0.0 && dphi_W_T2 > 0.0){
      dphi_W_T2 = TMath::Pi()*2. - dphi_W_T2;
    }
    double dphi_W_T1_top = vNORM_T1_W_top.Angle(vNORM_T1_B_top);
    double dphi_W_T2_top = vNORM_T2_W_top.Angle(vNORM_T2_B_top);
    double dot_dphi_W_T1_top = L1_top.Vect().Dot(vNORM_T1_B_top);
    double dot_dphi_W_T2_top = L2_top.Vect().Dot(vNORM_T2_B_top);
    if(dot_dphi_W_T1_top < 0.0 && dphi_W_T1_top > 0.0){
      dphi_W_T1_top = TMath::Pi()*2. - dphi_W_T1_top;
    }
    if(dot_dphi_W_T2_top < 0.0 && dphi_W_T2_top > 0.0){
      dphi_W_T2_top = TMath::Pi()*2. - dphi_W_T2_top;
    }
    double dphi_W_T1_W = vNORM_T1_W_W.Angle(vNORM_T1_B_W);
    double dphi_W_T2_W = vNORM_T2_W_W.Angle(vNORM_T2_B_W);
    double dot_dphi_W_T1_W = L1_W.Vect().Dot(vNORM_T1_B_W);
    double dot_dphi_W_T2_W = L2_W.Vect().Dot(vNORM_T2_B_W);
    if(dot_dphi_W_T1_W < 0.0 && dphi_W_T1_W > 0.0){
      dphi_W_T1_W = TMath::Pi()*2. - dphi_W_T1_W;
    }
    if(dot_dphi_W_T2_W < 0.0 && dphi_W_T2_W > 0.0){
      dphi_W_T2_W = TMath::Pi()*2. - dphi_W_T2_W;
    }

    //
    // differences between true and reco azimuthal angles
    //
    double ddphi_W_T1_top = sin(dphi_W_T1_top-dphi_W_T1);
    double ddphi_W_T2_top = sin(dphi_W_T2_top-dphi_W_T2);
    double ddphi_W_T1_W = sin(dphi_W_T1_W-dphi_W_T1);
    double ddphi_W_T2_W = sin(dphi_W_T2_W-dphi_W_T2);
    
    //
    // gamma for asymmetric boost
    //
    double gammaT = 1./pow( (1.-BCMtoT1.Mag2())*(1.-BCMtoT2.Mag2()),1./4. );
    double gammaT_top = 1./sqrt(1.-BCMtoT1_top.Mag2());
    double gammaT_W = 1./pow( (1.-BCMtoT1_W.Mag2())*(1.-BCMtoT2_W.Mag2()),1./4. );
    //
    // scale variables
    //
    double MT1 = H1.M();
    double MT2 = H2.M();
    double MT_top = H1_top.M();
    double MT1_W = H1_W.M();
    double MT2_W = H2_W.M();
  
    double Eb1 = B1.E();
    double Eb2 = B2.E();
    double Eb1_top = B1_top.E();
    double Eb2_top = B2_top.E();
    double Eb1_W = B1_W.E();
    double Eb2_W = B2_W.E();

    //
    // Heading to the last frames!!!!!
    // from the T rest frames to the W rest frames
    //
    TVector3 BTtoW1 = (L1+NU1).BoostVector();
    TVector3 BTtoW2 = (L2+NU2).BoostVector();
    TVector3 BTtoW1_top = (L1_top+NU1_top).BoostVector();
    TVector3 BTtoW2_top = (L2_top+NU2_top).BoostVector();
    TVector3 BTtoW1_W = (L1_W+NU1_W).BoostVector();
    TVector3 BTtoW2_W = (L2_W+NU2_W).BoostVector();

    L1.Boost(-BTtoW1);
    NU1.Boost(-BTtoW1);
    L2.Boost(-BTtoW2);
    NU2.Boost(-BTtoW2);
    L1_top.Boost(-BTtoW1_top);
    NU1_top.Boost(-BTtoW1_top);
    L2_top.Boost(-BTtoW2_top);
    NU2_top.Boost(-BTtoW2_top);
    L1_W.Boost(-BTtoW1_W);
    NU1_W.Boost(-BTtoW1_W);
    L2_W.Boost(-BTtoW2_W);
    NU2_W.Boost(-BTtoW2_W);

    //
    // scales in the W rest frame
    //
    double El1 = L1.E();
    double El2 = L2.E();
    double El1_top = L1_top.E();
    double El2_top = L2_top.E();
    double El1_W = L1_W.E();
    double El2_W = L2_W.E();
    
    //calculate some angles
    double costhetaW1 = L1.Vect().Unit().Dot(BTtoW1.Unit());
    double costhetaW2 = L2.Vect().Unit().Dot(BTtoW2.Unit());
    double costhetaW1_top = L1_top.Vect().Unit().Dot(BTtoW1_top.Unit());
    double costhetaW2_top = L2_top.Vect().Unit().Dot(BTtoW2_top.Unit());
    double costhetaW1_W = L1_W.Vect().Unit().Dot(BTtoW1_W.Unit());
    double costhetaW2_W = L2_W.Vect().Unit().Dot(BTtoW2_W.Unit());
    double dcosthetaW1_top = costhetaW1*sqrt(1.-costhetaW1_top*costhetaW1_top) - costhetaW1_top*sqrt(1.-costhetaW1*costhetaW1);
    double dcosthetaW2_top = costhetaW2*sqrt(1.-costhetaW2_top*costhetaW2_top) - costhetaW2_top*sqrt(1.-costhetaW2*costhetaW2);
    double dcosthetaW1_W = costhetaW1*sqrt(1.-costhetaW1_W*costhetaW1_W) - costhetaW1_W*sqrt(1.-costhetaW1*costhetaW1);
    double dcosthetaW2_W = costhetaW2*sqrt(1.-costhetaW2_W*costhetaW2_W) - costhetaW2_W*sqrt(1.-costhetaW2*costhetaW2);

    // define different scale sensitive ratios
    double El1Eb1_top = El1_top/(El1_top+Eb1_top);
    double El1Eb1_W = El1_W/(El1_W+Eb1_W);
    double El1Eb2_top = El1_top/(El1_top+Eb2_top);
    double El1Eb2_W = El1_W/(El1_W+Eb2_W);

    
    cout << "TT costheta: " << costhetaTT_top << " " << TT->GetCosDecayAngle() << endl;
    cout << "T1 costheta: " << costhetaT1_top << " " << T1->GetCosDecayAngle() << endl;
    cout << "T2 costheta: " << costhetaT2_top << " " << T2->GetCosDecayAngle() << endl;
    cout << "dphi_T1_T2_top: " << dphi_T1_T2_top << " " << T1->GetDeltaPhiDecayPlanes(T2) << endl;
    cout << "dphi_W_T1_top: " << dphi_W_T1_top << " " << T1->GetDeltaPhiDecayPlanes(W1) << endl;
   
    
    TLorentzVector newB1 = Bjet1->GetFourVector(T1);
    TLorentzVector newB2 = Bjet2->GetFourVector(T2);
    TLorentzVector newL1 = Lep1->GetFourVector(W1);
    TLorentzVector newL2 = Lep2->GetFourVector(W2);
    TLorentzVector newNU1 = Neu1->GetFourVector(W1);
    TLorentzVector newNU2 = Neu2->GetFourVector(W2);
    cout << "Eb1: " << Eb1_top << " " << newB1.E() << endl;
    cout << "Eb2: " << Eb2_top << " " << newB2.E() << endl;
    cout << "El1: " << El1_top << " " << newL1.E() << endl;
    cout << "El2: " << El2_top << " " << newL2.E() << endl;
    cout << "ENU1: " << NU1_top.E() << " " << newNU1.E() << endl;
    cout << "ENU2: " << NU2_top.E() << " " << newNU2.E() << endl;
    
  }
}

void BoostToLabFrame(double gamma_eff){
  double M1 = P[0].M();
  double M2 = P[1].M();
  double shat = 4.*M1*M2*gamma_eff*gamma_eff + (M1-M2)*(M1-M2);
  double gamma_1 = (sqrt(shat)/(2.*M1))*(1.+ (M1*M1-M2*M2)/shat);
  double gamma_2 = (sqrt(shat)/(2.*M2))*(1.+ (M2*M2-M1*M1)/shat);
  double beta_1 = sqrt(1. - 1./(gamma_1*gamma_1));
  double beta_2 = sqrt(1. - 1./(gamma_2*gamma_2));
  double c_S = 1.-2.*gRandom->Rndm();
  double s_S = sqrt(1.-c_S*c_S);
  double phi_S = TMath::Pi()*2.*gRandom->Rndm();
  TVector3 vBETA1, vBETA2;
  vBETA1.SetXYZ(beta_1*cos(phi_S)*s_S,beta_1*sin(phi_S)*s_S,beta_1*c_S);
  vBETA2.SetXYZ(-beta_2*cos(phi_S)*s_S,-beta_2*sin(phi_S)*s_S,-beta_2*c_S);
	
  BoostHem(0, vBETA1);
  BoostHem(1, vBETA2);
	
  return;
	
}

void PtBoost(double PT){
  double phi_pt = TMath::Pi()*2.*gRandom->Rndm();
  TVector3 B_pt;
  B_pt.SetPtEtaPhi(PT/sqrt(PT*PT+1.),0.0,phi_pt);
			 
  BoostHem(0,B_pt);
  BoostHem(1,B_pt);
}

void BoostHem(int iHem, TVector3 vBETA){
  P[iHem].Boost(vBETA);
  vMiss[iHem].Boost(vBETA);
  C_1[iHem].Boost(vBETA);
  C_2[iHem].Boost(vBETA);
  C_1_1[iHem].Boost(vBETA);
  C_1_2[iHem].Boost(vBETA);
  C_1_1_1[iHem].Boost(vBETA);
  C_1_1_2[iHem].Boost(vBETA);
	
}
void GetLepTopHem(int iHem, double Mtop, double MW, double Mb, double Mlep, double Mnu){
  //last decay
  double PL = sqrt((MW*MW-(Mnu-Mlep)*(Mnu-Mlep))*(MW*MW-(Mnu+Mlep)*(Mnu+Mlep)))/(2.*MW);
  double EL = (MW*MW-Mnu*Mnu+Mlep*Mlep)/(2.*MW);
  double Enu = (MW*MW+Mnu*Mnu-Mlep*Mlep)/(2.*MW);
	
  //second to last
  double Pb = sqrt((Mtop*Mtop-(Mb-MW)*(Mb-MW))*(Mtop*Mtop-(Mb+MW)*(Mb+MW)))/(2.*Mtop);
  double Eb = (Mtop*Mtop-MW*MW+Mb*Mb)/(2.*Mtop);
  double EW = (Mtop*Mtop+MW*MW-Mb*Mb)/(2.*Mtop);
	
  //first, we do leptons/neutrinos in W frames:
  double c1_L = 1.-2.*gRandom->Rndm();
  double s1_L = sqrt(1.-c1_L*c1_L);
  double phi1_L = TMath::Pi()*2.*gRandom->Rndm();
	
  //lepton
  C_1_1[iHem].SetPxPyPzE(PL*cos(phi1_L)*s1_L,PL*sin(phi1_L)*s1_L,PL*c1_L,EL);
  //neutrino
  C_1_2[iHem].SetPxPyPzE(-PL*cos(phi1_L)*s1_L,-PL*sin(phi1_L)*s1_L,-PL*c1_L,Enu);
	
	
  //Now, we do b's and W's in T frames
  double c1_B = 1.-2.*gRandom->Rndm();
  double s1_B = sqrt(1.-c1_B*c1_B);
  double phi1_B = TMath::Pi()*2.*gRandom->Rndm();

  //b-quark
  C_2[iHem].SetPxPyPzE(Pb*cos(phi1_B)*s1_B,Pb*sin(phi1_B)*s1_B,Pb*c1_B,Eb);
  //W
  C_1[iHem].SetPxPyPzE(-Pb*cos(phi1_B)*s1_B,-Pb*sin(phi1_B)*s1_B,-Pb*c1_B,EW);
	
  //Now, put the leptons and nu's into T-frames
  TVector3 W1boost = C_1[iHem].BoostVector();
	
  C_1_1[iHem].Boost(W1boost);
  C_1_2[iHem].Boost(W1boost);
	
  //system of weakly interacting particles from this hemisphere
  vMiss[iHem] = C_1_2[iHem];
	
  //total system of particles = parent
  P[iHem] = C_1[iHem]+C_2[iHem];
	
  return;
	
}


void setstyle() {
  
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(300); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.065);
  gStyle->SetPadRightMargin(0.22);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.17);

  // use large Times-Roman fonts
  gStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  gStyle->SetTitleFont(132," ");    // set the pad title font
  gStyle->SetTitleSize(0.06,"xyz"); // set the 3 axes title size
  gStyle->SetTitleSize(0.06," ");   // set the pad title size
  gStyle->SetLabelFont(132,"xyz");
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetTextFont(132);
  gStyle->SetTextSize(0.08);
  gStyle->SetStatFont(132);

  // use bold lines and markers
  gStyle->SetMarkerStyle(8);
  gStyle->SetHistLineWidth(1.85);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  //..Get rid of X error bars
  gStyle->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  const Int_t NRGBs = 5;
  const Int_t NCont = 25;
	
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };   
  Double_t red[NRGBs] =   { 1.0,   0.95,  0.95,  0.65,   0.15 };
  Double_t green[NRGBs] = { 1.0,  0.85, 0.7, 0.5,  0.3 };
  Double_t blue[NRGBs] =  { 0.95, 0.6 , 0.3,  0.45, 0.65 };
  double mr[NRGBs],mg[NRGBs],mb[NRGBs];
  for(int i = 0; i < NRGBs; i++){
    mr[i] = red[NRGBs-1-i];
    mg[i] = green[NRGBs-1-i];
    mb[i] = blue[NRGBs-1-i];
  }
  TColor::CreateGradientColorTable(NRGBs, stops, mr, mg, mb, NCont);
	
  gStyle->SetNumberContours(NCont);
	
  gStyle->cd();

}

double PDF_v(double x){
	
  double ret;
	
  ret = (1/x)*pow(x,eta_1)*pow(1-x,eta_2)*(1.+eps_u*sqrt(x)+g_u*x);
	
  return ret;
}
double PDF_S(double x){
	
  double ret;
	
  ret = (1/x)*pow(x,del_S)*pow(1-x,eta_S)*(1.+eps_S*sqrt(x)+g_S*x);
	
  return ret;
}
double PDF_g(double x){
	
  double ret;
	
  ret = (1/x)*(A_g*pow(x,del_g)*pow(1-x,eta_g)*(1.+eps_g*sqrt(x)+g_g*x)+A_g1*pow(x,del_g1)*pow(1-x,eta_g1));
	
  return ret;
}
double PDF_TOT(double x){
	
  double ret;
	
  if(type == 0){ // q qbar
    ret = (1/x)*(PDF_v(x)*PDF_S(C/x)+PDF_S(x)*PDF_v(C/x));
  } else { // gg
    ret = (1/x)*(PDF_g(x)*PDF_g(C/x));
  }
	
  return ret;
}

double Calc_dsigma_dgamma(double gamma){
	
  C = 4.*gamma*gamma*M*M/(rootS*rootS);
  TF1 *func = new TF1("func","PDF_TOT(x)",0,10);
	
  double ret;
	
  //ret = (1./pow(gamma,5.))*sqrt(1.-1./(gamma*gamma))*func->Integral(C,1.);
  ret = (1./gamma)*sqrt(1.-1./(gamma*gamma))*func->Integral(C,1.);
	
  return ret;
}

TCanvas* Plot_Me_2D(char *titlecan, TH2D* histo, char *titleX, char *titleY){
  TCanvas *c1 = new TCanvas(titlecan,titlecan,600,500);
  c1->Draw();
  c1->SetGridx();
  c1->SetGridy();
  histo->Scale(1./histo->Integral());
  histo->GetXaxis()->SetTitle(titleX);
  histo->GetYaxis()->SetTitle(titleY);
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->CenterTitle();
  histo->Draw("COLZ");
  char *sname = new char[200];
  sprintf(sname,"fig_%s.pdf",histo->GetTitle());
  //c1->Print(sname);	
  return c1;
	
}


