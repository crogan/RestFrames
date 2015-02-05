#ifndef RDecayFrame_HH
#define RDecayFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/DecayFrame.hh"
#include "RestFrames/RFrame.hh"

using namespace std;

namespace RestFrames {

   enum RDecayType { RDVanilla, RDSelfAssembling };

  ///////////////////////////////////////////////
  // RDecayFrame class
  ///////////////////////////////////////////////
  class RDecayFrame : public DecayFrame, public RFrame{
  public:
    RDecayFrame(const string& sname, const string& stitle, int key);
    RDecayFrame(const string& sname, const string& stitle);
    virtual ~RDecayFrame();

    bool IsSelfAssemblingFrame() const;

  protected:
    RDecayType m_RType;

  private:
    void Init();

  };

}

#endif
