#ifndef InvisibleJigsaw_HH
#define InvisibleJigsaw_HH

#include "RestFrames/Jigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // InvisibleJigsaw class
  ///////////////////////////////////////////////

  class InvisibleJigsaw : public Jigsaw {
  public:
    InvisibleJigsaw(const string& sname, const string& stitle);
    virtual ~InvisibleJigsaw();

    void AddVisibleFrame(RestFrame& framePtr, int i = 0);
    void AddVisibleFrame(RestFrame* framePtr, int i = 0);
    void AddVisibleFrame(RFList<RestFrame>* framesPtr, int i = 0);
    void AddInvisibleFrame(RestFrame& framePtr, int i = 0);
    void AddInvisibleFrame(RestFrame* framePtr, int i = 0);
    void AddInvisibleFrame(RFList<RestFrame>* framesPtr, int i = 0);

    virtual double GetMinimumMass();
    virtual bool InitializeDependancyJigsaws();
    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsaws);

    virtual bool InitializeJigsawExecutionList(RFList<Jigsaw>* chain_jigsawPtr);

  protected:
    virtual State* NewOutputState();

  private:
    void Init();
  
  };

}

#endif
