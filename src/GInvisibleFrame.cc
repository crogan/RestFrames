#include "RestFrames/GInvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GInvisibleFrame class
  ///////////////////////////////////////////////
  GInvisibleFrame::GInvisibleFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    InvisibleFrame(sname, stitle, key),
    GFrame(sname, stitle, key)
  {
    Init();
  }

  GInvisibleFrame::GInvisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle),
    InvisibleFrame(sname, stitle),
    GFrame(sname, stitle)
  {
    Init();
  }

  GInvisibleFrame::~GInvisibleFrame(){ }

  void GInvisibleFrame::Init(){
    m_Mass = 0.;
  }

  void GInvisibleFrame::SetMass(double val){
    if(val >= 0.) m_Mass = val;
  }

  void GInvisibleFrame::ResetFrame(){ }

  bool GInvisibleFrame::GenerateFrame(){ 
    return true;
  }

}
