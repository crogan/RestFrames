#ifndef InvisibleRapidityJigsaw_HH
#define InvisibleRapidityJigsaw_HH
#include "RestFrames/NVisibleMInvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleRapidityJigsaw class
  ///////////////////////////////////////////////
  class InvisibleRapidityJigsaw : public NVisibleMInvisibleJigsaw {
  public:
    //constructor and destructor
    InvisibleRapidityJigsaw(const string& sname, const string& stitle);
    virtual ~InvisibleRapidityJigsaw();

    virtual bool AnalyzeEvent();

  protected:
    void Init();
  
  };

}

#endif
