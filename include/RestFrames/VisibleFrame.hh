#ifndef VisibleFrame_HH
#define VisibleFrame_HH
#include <iostream>
#include <string>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // VisibleFrame class
  ///////////////////////////////////////////////
  class VisibleFrame : public virtual RestFrame {
  public:
    //constructor and destructor
    VisibleFrame(const string& sname, const string& stitle, int key);
    VisibleFrame(const string& sname, const string& stitle);
    virtual ~VisibleFrame();

    virtual void SetLabFrameFourVector(const TLorentzVector& V);
    virtual TLorentzVector GetLabFrameFourVector() const;

  protected:
    virtual bool IsSoundBody() const;

  private:
    TLorentzVector m_Lab_P;

    void Init();

  };

}

#endif
