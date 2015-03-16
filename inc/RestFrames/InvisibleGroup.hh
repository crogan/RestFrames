#ifndef InvisibleGroup_HH
#define InvisibleGroup_HH
#include <iostream>
#include <string>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/Group.hh"
#include "RestFrames/InvisibleState.hh"

using namespace std;

namespace RestFrames {

  class Group;
  class State;
  class InvisibleState;

  ///////////////////////////////////////////////
  // InvisibleGroup class
  ///////////////////////////////////////////////
  class InvisibleGroup : public Group {
  public:
    InvisibleGroup(const string& sname, const string& stitle);
    InvisibleGroup(const string& sname, const string& stitle, int ikey);
    virtual ~InvisibleGroup();
	
    virtual void AddFrame(RestFrame& frame);
    virtual void AddFrame(RestFrame* framePtr);
    virtual bool AddJigsaw(Jigsaw& jigsaw);
    virtual bool AddJigsaw(Jigsaw* jigsawPtr);

    // Event analysis functions
    virtual void SetLabFrameFourVector(const TLorentzVector& V);
    virtual void SetLabFrameThreeVector(const TVector3& V);
    virtual TLorentzVector GetLabFrameFourVector();
    virtual void ClearEvent();
    virtual bool AnalyzeEvent();

  protected:
    virtual State* InitializeGroupState();

  private:
    TLorentzVector m_Lab_P;
    void Init();

  };

}

#endif
