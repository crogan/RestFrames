#include "RestFrames/GDecayFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GDecayFrame class
  ///////////////////////////////////////////////
  GDecayFrame::GDecayFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname,stitle,key), 
    DecayFrame(sname,stitle,key),
    GFrame(sname, stitle, key)
  {
    Init();
  }
  GDecayFrame::GDecayFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle), 
    DecayFrame(sname,stitle),
    GFrame(sname, stitle)
  {
    Init();
  }
  
  GDecayFrame::~GDecayFrame(){ }

  void GDecayFrame::Init(){
    m_Mass = -1.;
    m_MassSet = false;
    m_ChildP = -1.;
    m_ChildGamma = -1.;
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;
  }

  bool GDecayFrame::IsSoundBody() const{
    RestFrame::IsSoundBody();
    int Nchild = GetNChildren();
    if(Nchild < 2 || !m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

  void GDecayFrame::SetMass(double val){
    if(val < 0.) return;

    m_Mass = val;
    m_MassSet = true;
    m_ChildP = -1.;
    m_ChildGamma = 0.;
  }

  void GDecayFrame::SetChildMomentum(double val){
    if(val < 0.) return;

    m_Mass = -1.;
    m_MassSet = false;
    m_ChildP = val;
    m_ChildGamma = 0.;
  }

  void GDecayFrame::SetChildGamma(double val){
    if(val < 1.) return;

    m_Mass = -1.;
    m_MassSet = false;
    m_ChildP = -1.;
    m_ChildGamma = val;
  }

  void GDecayFrame::SetCosDecayAngle(double val){
    if(fabs(val) > 1.) return;

    m_CosDecayAngle = val;
  }

  void GDecayFrame::SetDeltaPhiDecayPlane(double val){
    while(val > acos(-1.)*2.) val -= acos(-1.)*2.;
    while(val < 0.) val += acos(-1.)*2.;
    m_DeltaPhiDecayPlane = val;
  }

  void GDecayFrame::ResetFrame(){
    m_Spirit = false;
    if(m_ChildP > 0. || m_ChildGamma >= 1.) m_MassSet = false;
    ResetDecayAngles();
  }

  void GDecayFrame::ResetDecayAngles(){
    m_CosDecayAngle = -2.;
    m_DeltaPhiDecayPlane = -2.;
  }

  double GDecayFrame::GetMass() const{
    if(m_MassSet) return m_Mass;

    int Nchild = GetNChildren();
    if(Nchild != 2) return -1.;
    double m[2];
    for(int i = 0; i < 2; i++) m[i] = GetChildFrame(i)->GetMass();
    double Mass;
    if(m_ChildP >= 0.){
      Mass = 0.;
      for(int i = 0; i < 2; i++) 
	Mass += sqrt(m_ChildP*m_ChildP + m[i]*m[i]);
    }
    if(m_ChildGamma >= 0.){
      Mass = sqrt(4.*m[0]*m[1]*m_ChildGamma*m_ChildGamma
		  + (m[0]-m[1])*(m[0]-m[1]));
    }
    m_Mass = Mass;
    m_MassSet = true;
    return m_Mass;
  }

  bool GDecayFrame::GenerateFrame(){
    if(!m_Body) return false;
    int Nchild = GetNChildren();

    vector<double> ChildMasses;
    double ChildMassTOT = 0.;
    for(int i = 0; i < Nchild; i++){
      double ChildMass = max(0.,GetChildFrame(i)->GetMass());
      ChildMasses.push_back(ChildMass);
      ChildMassTOT += ChildMass;
    }

    double Mass = GetMass();
    if(Mass <= ChildMassTOT) return false;

    vector<TLorentzVector> ChildVectors;
    GenerateTwoBodyRecursive(Mass, ChildMasses, 
			     m_ParentLinkPtr->GetBoostVector(),
			     GetParentFrame()->GetDecayPlaneNormalVector(),
			     ChildVectors);
    
    SetChildren(ChildVectors);
    
    return true;
  }

  double GDecayFrame::GenerateTwoBodyRecursive(double M,const vector<double>& M_c,
					       const TVector3& axis_par, const TVector3& axis_perp,
					       vector<TLorentzVector>& P_c)
  {
    TVector3 n_par = axis_par.Unit();
    TVector3 n_perp = axis_perp.Unit();

    int N_c = M_c.size();
  
     double m[2];
     m[0] = M_c[0];
     if(N_c == 2){
       m[1] = M_c[1];
     } else {
       double mO = 0.;
       for(int i = 1; i < N_c; i++) mO += M_c[i];
       m[1] = mO + GetRandom()*(M - m[0] - mO);
     }
     double Pcm = sqrt((M*M-m[0]*m[0]-m[1]*m[1])*(M*M-m[0]*m[0]-m[1]*m[1])-4.*m[0]*m[0]*m[1]*m[1])/2./M;
     TVector3 V_c[2];
     V_c[0] = Pcm*n_par;
     V_c[1] = -Pcm*n_par;
     if(m_CosDecayAngle < -1.) m_CosDecayAngle = 1.-2.*GetRandom();
     if(m_DeltaPhiDecayPlane < 0.) m_DeltaPhiDecayPlane = 2.*acos(-1.)*GetRandom();
   
     for(int i = 0; i < 2; i++) V_c[i].Rotate(-acos(m_CosDecayAngle),n_perp);
     for(int i = 0; i < 2; i++) V_c[i].Rotate(-m_DeltaPhiDecayPlane,n_par);
     ResetDecayAngles();
     TLorentzVector P_child[2];
     for(int i = 0; i < 2; i++) P_child[i].SetVectM(V_c[i], m[i]);
     P_c.push_back(P_child[0]);
     
     if(N_c == 2){
       P_c.push_back(P_child[1]);
       return Pcm;
     }

     // Recursively generate other two-body decays
     vector<double> M_cR;
     for(int i = 1; i < N_c; i++) M_cR.push_back(M_c[i]);
     TVector3 boost = P_child[1].BoostVector();
     vector<TLorentzVector> P_cR;
     Pcm *= GenerateTwoBodyRecursive(m[1], M_cR, boost, V_c[0].Cross(axis_par), P_cR);
     for(int i = 0; i < N_c-1; i++) P_cR[i].Boost(boost);
     for(int i = 0; i < N_c-1; i++) P_c.push_back(P_cR[i]);
     
     return Pcm;
  }

}
