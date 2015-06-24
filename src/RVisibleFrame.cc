#include "RestFrames/RVisibleFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RVisibleFrame class
  ///////////////////////////////////////////////
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
