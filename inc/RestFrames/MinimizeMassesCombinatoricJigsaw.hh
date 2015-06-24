#ifndef MinimizeMassesCombinatoricJigsaw_HH
#define MinimizeMassesCombinatoricJigsaw_HH

#include "RestFrames/CombinatoricJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // MinimizeMassesCombinatoricJigsaw class
  ///////////////////////////////////////////////
  class MinimizeMassesCombinatoricJigsaw : public CombinatoricJigsaw{
  public:
    MinimizeMassesCombinatoricJigsaw(const string& sname, const string& stitle);
    virtual ~MinimizeMassesCombinatoricJigsaw();

    virtual bool AnalyzeEvent();

  private:
    void Init();

  };

}

#endif
