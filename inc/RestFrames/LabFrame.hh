#ifndef LabFrame_HH
#define LabFrame_HH
#include <string>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // LabFrame class
  ///////////////////////////////////////////////
  class LabFrame : public virtual RestFrame{
  public:
    LabFrame(const string& sname, const string& stitle, int key);
    LabFrame(const string& sname, const string& stitle);
    virtual ~LabFrame();

    void SetChildFrame(RestFrame& frame);
    void SetChildFrame(RestFrame* framePtr);
  
    virtual bool InitializeTree() const;

    // Analysis functions
    TVector3 GetInvisibleMomentum() const;
  
    virtual double GetCosDecayAngle(const RestFrame* framePtr) const;
    virtual TVector3 GetDecayPlaneNormalVector() const;

  protected:
    virtual bool IsSoundBody() const;

  private:
    void Init();
  };

}

#endif
