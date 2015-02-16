#include "RestFrames/GVisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GVisibleFrame class
  ///////////////////////////////////////////////
  GVisibleFrame::GVisibleFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    VisibleFrame(sname, stitle, key),
    GFrame(sname, stitle, key)
  {
    Init();
  }

  GVisibleFrame::GVisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle),
    VisibleFrame(sname, stitle),
    GFrame(sname, stitle)
  {
    Init();
  }

  GVisibleFrame::~GVisibleFrame(){ }

  void GVisibleFrame::Init(){
    m_Mass = 0.;
  }

  void GVisibleFrame::SetMass(double val){
    if(val >= 0.) m_Mass = val;
  }

  void GVisibleFrame::ResetFrame(){ }

  bool GVisibleFrame::GenerateFrame(){ 
    return true;
  }

}
