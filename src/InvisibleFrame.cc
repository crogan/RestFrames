#include "RestFrames/InvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleFrame class
  ///////////////////////////////////////////////
  InvisibleFrame::InvisibleFrame(const string& sname, const string& stitle, int ikey) : 
    RestFrame(sname, stitle, ikey)
  {
    Init();
  }
  InvisibleFrame::InvisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }
  InvisibleFrame::~InvisibleFrame(){
    
  }
  void InvisibleFrame::Init(){
    m_Type = FInvisible;
  }
  
  bool InvisibleFrame::IsSoundBody() const {
    RestFrame::IsSoundBody();
    int Nchild = GetNChildren();
    if(Nchild > 0 || !m_ParentLinkPtr) m_Body = false;
    return m_Body;
  }

}


