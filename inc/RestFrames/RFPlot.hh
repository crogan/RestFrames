#ifndef RFPlot_HH
#define RFPlot_HH
#include <map>
#include <TCanvas.h>
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  class RFPlot : public RFBase {
  public:
    RFPlot(const string& sname, const string& stitle);
    ~RFPlot();

    virtual void Clear();

    void WriteOutput(const string& filename);

  protected:
    TCanvas* m_CanvasPtr;
    void AddCanvas(TCanvas* can);
    void AddTObject(TObject* obj);
    int GetNCanvases();
    
  private:
    vector<TObject*> m_TObjects;
    vector<TCanvas*> m_Canvases;

  };

}

#endif
