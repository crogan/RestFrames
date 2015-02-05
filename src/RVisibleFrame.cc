#include "RestFrames/RVisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RVisibleFrame class
  ///////////////////////////////////////////////
  RVisibleFrame::RVisibleFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname, stitle, key),
    VisibleFrame(sname, stitle, key),
    RFrame(sname, stitle, key)
  {
    Init();
  }

  RVisibleFrame::RVisibleFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle),
    VisibleFrame(sname, stitle),
    RFrame(sname, stitle)
  {
    Init();
  }

  RVisibleFrame::~RVisibleFrame(){
   
  }

  void RVisibleFrame::Init(){
  
  }

}
