#ifndef NVisibleMInvisibleJigsaw_HH
#define NVisibleMInvisibleJigsaw_HH
#include <iostream>
#include <string>
#include <vector>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrameList.hh"
#include "RestFrames/InvisibleJigsaw.hh"
#include "RestFrames/JigsawList.hh"
#include "RestFrames/State.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class RestFrameList;
  class Jigsaw;
  class JigsawList;
  class InvisibleJigsaw;
  class State;
  class StateList;

  ///////////////////////////////////////////////
  // NVisibleMInvisibleJigsaw class
  ///////////////////////////////////////////////
  class NVisibleMInvisibleJigsaw : public InvisibleJigsaw{
  public:
    NVisibleMInvisibleJigsaw(const string& sname, const string& stitle, int N, int M);
    NVisibleMInvisibleJigsaw(const string& sname, const string& stitle, int ikey, int N, int M);
    virtual ~NVisibleMInvisibleJigsaw();

    virtual bool IsSoundBody() const ;
    virtual void SetGroup(Group* groupPtr);

  private:
    int m_Nvis;
    int m_Minv;  
    void Init(int N, int M);

  };

}

#endif
