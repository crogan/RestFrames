#ifndef GLabFrame_HH
#define GLabFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/LabFrame.hh"
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GLabFrame class
  ///////////////////////////////////////////////
  class GLabFrame : public LabFrame, public GFrame {
  public:
    GLabFrame(const string& sname, const string& stitle, int key);
    GLabFrame(const string& sname, const string& stitle);
    virtual ~GLabFrame();
  
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

    virtual void SetThreeVector(const TVector3& P);
    virtual void SetTransverseMomenta(double val);
    virtual void SetLongitudinalMomenta(double val);
    virtual void SetPhi(double val);
    
  protected:
    double m_PT;
    double m_PL;
    double m_Phi;
    double m_Theta;

    virtual void ResetFrame();
    virtual bool GenerateFrame();

    void ResetProductionAngles();

  private:
    void Init();
    
  };

}

#endif
