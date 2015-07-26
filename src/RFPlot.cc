#include <TFile.h>

#include "RestFrames/RFPlot.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFPlot class methods
  // class which can plot RestFrame trees
  ///////////////////////////////////////////////
  RFPlot::RFPlot(const string& sname, const string& stitle)
    : RFBase(sname, stitle, -1){
    m_Log.SetSource("RFPlot "+GetName());
  }
  
  RFPlot::~RFPlot(){
    Clear();
  }

  void RFPlot::Clear(){
    int Ncan = m_Canvases.size();
    for(int i = 0; i < Ncan; i++)
      delete m_Canvases[i];
    m_Canvases.clear();
    int Nobj = m_TObjects.size();
    for(int i = 0; i < Nobj; i++)
      delete m_TObjects[i];
    m_TObjects.clear();
    RFBase::Clear();
  }

  void RFPlot::WriteOutput(const string& filename){
    int N = m_Canvases.size();
    if(N <= 0) return;

    TFile* file = new TFile(filename.c_str(),"UPDATE");
    file->cd();
    for(int i = 0; i < N; i++)
      m_Canvases[i]->Write("",TObject::kOverwrite);
    file->Close();
    delete file;
  }

  void RFPlot::AddCanvas(TCanvas* can){
    m_Canvases.push_back(can);
  }

  void RFPlot::AddTObject(TObject* obj){
    m_TObjects.push_back(obj);
  }

  int RFPlot::GetNCanvases(){
    return m_Canvases.size();
  }
}
