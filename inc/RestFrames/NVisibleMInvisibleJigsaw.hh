#ifndef NVisibleMInvisibleJigsaw_HH
#define NVisibleMInvisibleJigsaw_HH
#include <string>
#include "RestFrames/InvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // NVisibleMInvisibleJigsaw class
  ///////////////////////////////////////////////
  class NVisibleMInvisibleJigsaw : public InvisibleJigsaw {
  public:
    NVisibleMInvisibleJigsaw(const string& sname, const string& stitle, int N, int M);
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
