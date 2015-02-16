#include "RestFrames/GLabFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GLabFrame class
  ///////////////////////////////////////////////
  GLabFrame::GLabFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    LabFrame(sname, stitle, key), 
    GFrame(sname, stitle, key)
  {
    Init();
  }

  GLabFrame::GLabFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle), 
    LabFrame(sname, stitle), 
    GFrame(sname, stitle)
  {
    Init();
  }

  GLabFrame::~GLabFrame(){
    
  }

  void GLabFrame::Init(){
    m_PT = 0.;
    m_PL = 0.;
    m_Phi = -1.;
    m_Theta = -1.;
  }

  void GLabFrame::SetThreeVector(const TVector3& P){
    m_PT = P.Pt();
    m_PL = P.Z();
    SetPhi(P.Phi());
  }

  void GLabFrame::SetTransverseMomenta(double val){
    if(val >= 0.) m_PT = val;
  }

  void GLabFrame::SetLongitudinalMomenta(double val){
    m_PL = val;
  }

  void GLabFrame::SetPhi(double val){
    while(val > acos(-1.)*2.) val -= acos(-1.)*2.;
    while(val < 0.) val += acos(-1.)*2.;
    m_Phi = val;
  }

  void GLabFrame::ResetProductionAngles(){
     m_Phi = -1.;
     m_Theta = -1.;
  }

  void GLabFrame::ResetFrame(){
    m_Spirit = false;
    ResetProductionAngles();
  }
  
  bool GLabFrame::GenerateFrame(){
    if(!m_Body) return false;

    TLorentzVector P;
    double M = GetChildFrame(0)->GetMass();
    if(m_Phi < 0.) m_Phi = 2.*acos(-1.)*GetRandom();

    P.SetPxPyPzE(m_PT*cos(m_Phi), m_PT*sin(m_Phi), m_PL, sqrt(m_PT*m_PT+m_PL*m_PL+M*M));

    vector<TLorentzVector> ChildVector;
    ChildVector.push_back(P);
    SetChildren(ChildVector);
    ResetProductionAngles();
    
    return true;
  }

  void GLabFrame::ClearEvent(){
    m_Spirit = false;
    if(!m_Body) return;
    
    ClearEventRecursive();
  }

  bool GLabFrame::AnalyzeEvent(){
    m_Spirit = false;

    if(!AnalyzeEventRecursive()) return false;
    m_Spirit = true;
    return m_Spirit;
  }

}
