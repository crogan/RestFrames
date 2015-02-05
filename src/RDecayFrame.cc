#include "RestFrames/RDecayFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RDecayFrame class
  // Class for an intermediate state
  // inheriting from DecayFrame and RFrame
  ///////////////////////////////////////////////
  RDecayFrame::RDecayFrame(const string& sname, const string& stitle, int key) : 
    RestFrame(sname,stitle,key), 
    DecayFrame(sname,stitle,key),
    RFrame(sname, stitle, key)
  {
    Init();
  }
  RDecayFrame::RDecayFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle), 
    DecayFrame(sname,stitle),
    RFrame(sname, stitle)
  {
    Init();
  }
  
  RDecayFrame::~RDecayFrame(){ }

  void RDecayFrame::Init(){
    m_RType = RDVanilla;
  }

  bool RDecayFrame::IsSelfAssemblingFrame() const {
    return m_RType == RDSelfAssembling;
  }
}
