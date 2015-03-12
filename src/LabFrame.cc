#include <iostream>
#include "RestFrames/LabFrame.hh"
#include "RestFrames/VisibleFrame.hh"
#include "RestFrames/RestFrameList.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // LabFrame class
  ///////////////////////////////////////////////
  LabFrame::LabFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key)
  {
    Init();
  }
  LabFrame::LabFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }
  LabFrame::~LabFrame(){
  
  }
  void LabFrame::Init(){
    m_Type = FLab;
  }

  bool LabFrame::IsSoundBody() const {
    RestFrame::IsSoundBody();
    int Nchild = GetNChildren();
    if(Nchild != 1 || m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

  void LabFrame::SetChildFrame(RestFrame& frame){
    SetChildFrame(&frame);
  }

  void LabFrame::SetChildFrame(RestFrame* framePtr){ 
    if(!framePtr) return;
    AddChildFrame(framePtr); 
  }

  bool LabFrame::InitializeTree() const {
    vector<int>* KEYS = new vector<int>;
    if(IsCircularTree(KEYS)){
      cout << endl << "Consistent Topology Failure: ";
      cout << "Tree is circular in construction" << endl;
      KEYS->clear();
      return false;
    }
    KEYS->clear();
 
    if(!IsConsistentAnaTree(m_Ana)){
      cout << endl << "Consistent Topology Failure: ";
      cout << "Tree contains mixture of node types (Reco, Gen)" << endl;
      KEYS->clear();
      return false;
    }

    if(!IsSoundBodyRecursive()){
      cout << endl << "Consistent Topology Failure: ";
      cout << "UnSound frame in tree" << endl;
      return false;
    }
    
    return true;
  }

  TVector3 LabFrame::GetInvisibleMomentum() const {
    return GetInvisibleFourVector().Vect();
  }

  double LabFrame::GetCosDecayAngle(const RestFrame* framePtr) const {
    if(m_ChildLinks.size() < 1) return 0.;
    TVector3 V1(0.,0.,1.);
    TVector3 V2;
    if(framePtr){
      V2 = framePtr->GetFourVector(this).Vect().Unit();
    } else {
      V2 = GetChildFrame(0)->GetFourVector(this).Vect().Unit();
    }
    return V1.Dot(V2);
  }

  TVector3 LabFrame::GetDecayPlaneNormalVector() const {
    TVector3 V(0.,0.,0.);
    if(m_ChildLinks.size() < 1) return V;
   
    TVector3 V1 = GetChildFrame(0)->GetFourVector(this).Vect().Unit();
    TVector3 V2(0.,0.,1.);
    return V1.Cross(V2).Unit();
  }

}
