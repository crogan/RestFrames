#ifndef InvisibleFrame_HH
#define InvisibleFrame_HH
#include <iostream>
#include <string>
#include <vector>
#include "RestFrames/RestFrame.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;

  ///////////////////////////////////////////////
  // InvisibleFrame class
  ///////////////////////////////////////////////
  class InvisibleFrame : public virtual RestFrame{
  public:
    //constructor and destructor
    InvisibleFrame(const string& sname, const string& stitle, int key);
    InvisibleFrame(const string& sname, const string& stitle);
    virtual ~InvisibleFrame();

  protected:
    virtual bool IsSoundBody() const;

  private:
    void Init();

  };

}

#endif
