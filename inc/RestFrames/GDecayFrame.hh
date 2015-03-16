#ifndef GDecayFrame_HH
#define GDecayFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/DecayFrame.hh"
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GDecayFrame class
  ///////////////////////////////////////////////
  class GDecayFrame : public DecayFrame, public GFrame{
  public:
    GDecayFrame(const string& sname, const string& stitle, int key);
    GDecayFrame(const string& sname, const string& stitle);
    virtual ~GDecayFrame();

    virtual void SetMass(double val);
    virtual double GetMass() const;
    
    // For two-body decays
    virtual void SetChildMomentum(double val);
    virtual void SetChildGamma(double val);
    virtual void SetCosDecayAngle(double val);
    virtual void SetDeltaPhiDecayPlane(double val);

  protected:
    mutable double m_Mass;
    mutable bool m_MassSet;

    // For two-body decays
    double m_ChildP;
    double m_ChildGamma;
    double m_CosDecayAngle;
    double m_DeltaPhiDecayPlane;

    virtual bool IsSoundBody() const;

    virtual void ResetFrame();
    virtual bool GenerateFrame();

    void ResetDecayAngles();
    double GenerateTwoBodyRecursive(double M, const vector<double>& M_children, 
				    const TVector3& axis_par, const TVector3& axis_perp,
				    vector<TLorentzVector>& P_children);

  private:
    void Init();

  };

}

#endif
