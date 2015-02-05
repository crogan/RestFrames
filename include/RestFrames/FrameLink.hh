#ifndef FrameLink_HH
#define FrameLink_HH
#include <iostream>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // FrameLink class
  ///////////////////////////////////////////////
  class FrameLink {
  public:
    FrameLink();
    ~FrameLink();

    void SetParentFrame(RestFrame* framePtr);
    void SetChildFrame(RestFrame* framePtr);

    void Clear();
  
    RestFrame* GetChildFrame();
    RestFrame* GetParentFrame();
 
    void SetBoostVector(const TVector3& boost);
    TVector3 GetBoostVector();
	
  protected:
    TVector3 m_B; // 3-vector of the velocity of boost
    RestFrame* m_parent_framePtr; //link to parent frame
    RestFrame* m_child_framePtr; //link to child frame
  };

}

#endif
