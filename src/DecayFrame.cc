#include "RestFrames/DecayFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // DecayFrame class
  ///////////////////////////////////////////////
  DecayFrame::DecayFrame(const string& sname,const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  DecayFrame::DecayFrame(const string& sname,const string& stitle,int ikey) : 
    RestFrame(sname, stitle, ikey)
  {
    Init();
  }

  DecayFrame::~DecayFrame(){
  
  }
  void DecayFrame::Init(){
    m_Type = FDecay;
  }

  bool DecayFrame::IsSoundBody() const {
    RestFrame::IsSoundBody();
    int Nchild = GetNChildren();
    if(Nchild < 1 || !m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

}
