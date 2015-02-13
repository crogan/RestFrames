#include "RestFrames/GDecayFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GDecayFrame class
  ///////////////////////////////////////////////
  GDecayFrame::GDecayFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname,stitle,key), 
    DecayFrame(sname,stitle,key),
    GFrame(sname, stitle, key)
  {
    Init();
  }
  GDecayFrame::GDecayFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle), 
    DecayFrame(sname,stitle),
    GFrame(sname, stitle)
  {
    Init();
  }
  
  GDecayFrame::~GDecayFrame(){ }

  void GDecayFrame::Init(){
   
  }

}
