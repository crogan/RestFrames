#ifndef DecayFrame_HH
#define DecayFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // DecayFrame class
  // Class for an intermediate state
  // inheriting from RestFrame
  ///////////////////////////////////////////////
  class DecayFrame : public virtual RestFrame {
  public:
    //constructor and destructor
    DecayFrame(const string& sname, const string& stitle, int key);
    DecayFrame(const string& sname, const string& stitle);
    virtual ~DecayFrame();

  protected:
    virtual bool IsSoundBody() const;

  private:
    void Init();
  };

}

#endif
