#ifndef ContraBoostInvariantJigsaw_HH
#define ContraBoostInvariantJigsaw_HH

#include "RestFrames/NVisibleMInvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {


  ///////////////////////////////////////////////
  // ContraBoostInvariantJigsaw class
  ///////////////////////////////////////////////
  class ContraBoostInvariantJigsaw : public NVisibleMInvisibleJigsaw {
  public:
    //constructor and destructor
    ContraBoostInvariantJigsaw(const string& sname, const string& stitle);
    ~ContraBoostInvariantJigsaw();

    virtual double GetMinimumMass();
    virtual bool AnalyzeEvent();

    virtual void FillInvisibleMassJigsawDependancies(RFList<Jigsaw>* jigsaws);

  protected:
    void CalcCoef();
    double GetC1(){ return m_C1; }
    double GetC2(){ return m_C2; }

  private:
    double m_C1;
    double m_C2;
    void Init();
  
  };

}

#endif
