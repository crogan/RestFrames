#include "RestFrames/RInvisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RInvisibleFrame class
  ///////////////////////////////////////////////
  RInvisibleFrame::RInvisibleFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    InvisibleFrame(sname, stitle, key),
    RFrame(sname, stitle, key)
  {
    Init();
  }

  RInvisibleFrame::RInvisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle),
    InvisibleFrame(sname, stitle),
    RFrame(sname, stitle)
  {
    Init();
  }

  RInvisibleFrame::~RInvisibleFrame(){
    
  }

  void RInvisibleFrame::Init(){
    minMASS = 0.;
  }

}
