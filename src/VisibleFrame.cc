#include "RestFrames/VisibleFrame.hh"

using namespace std;

namespace RestFrames {
  ///////////////////////////////////////////////
  // VisibleFrame class methods
  ///////////////////////////////////////////////
  VisibleFrame::VisibleFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key)
  {
    Init();
  }

  VisibleFrame::VisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  VisibleFrame::~VisibleFrame(){
    
  }

  void VisibleFrame::Init(){
    m_Type = FVisible;
  }

  bool VisibleFrame::IsSoundBody() const {
    RestFrame::IsSoundBody();
    int Nchild = GetNChildren();
    if(Nchild > 0 || !m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

  void VisibleFrame::SetLabFrameFourVector(const TLorentzVector& V){
    m_Lab_P.SetVectM(V.Vect(),V.M());
  }

  TLorentzVector VisibleFrame::GetLabFrameFourVector() const{
    TLorentzVector V;
    V.SetVectM(m_Lab_P.Vect(),m_Lab_P.M());
    return V;
  }

}
