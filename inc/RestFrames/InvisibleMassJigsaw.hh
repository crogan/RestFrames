#ifndef InvisibleMassJigsaw_HH
#define InvisibleMassJigsaw_HH
#include "RestFrames/NVisibleMInvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleMassJigsaw class
  ///////////////////////////////////////////////
  class InvisibleMassJigsaw : public NVisibleMInvisibleJigsaw {
  public:
    InvisibleMassJigsaw(const string& sname, const string& stitle);
    virtual ~InvisibleMassJigsaw();

    virtual bool AnalyzeEvent();
    
  protected:
    void Init();
  
  };

}

#endif
