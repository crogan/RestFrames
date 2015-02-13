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

  GVisibleFrame::~GVisibleFrame(){
   
  }

  void GVisibleFrame::Init(){
  
  }

}
