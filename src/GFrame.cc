#include <math.h>
#include <TRandom3.h>
#include <TDatime.h>
#include "RestFrames/GFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // GFrame class methods
  ///////////////////////////////////////////////
  GFrame::GFrame(const string& sname, const string& stitle, int ikey) : 
    RestFrame(sname, stitle, ikey)
  {
    Init();
  }

  GFrame::GFrame(const string& sname, const string& stitle) : 
    RestFrame(sname, stitle)
  {
    Init();
  }

  GFrame::~GFrame(){
    delete m_Random;
  }

  void GFrame::Init(){
    m_Ana = FGen;
  
    TDatime *now = new TDatime();
    int today = now->GetDate();
    int clock = now->GetTime();
    delete now;
    int seed = today+clock+m_Key;
    m_Random = new TRandom3(seed);
    
  }

  void GFrame::ClearGFrame(){
   
  }

  void GFrame::ClearEventRecursive(){ 
    ResetFrame();
    if(!m_Body) return;
    
    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++)
      GetChildFrame(i)->ClearEventRecursive();
  }

  bool GFrame::AnalyzeEventRecursive(){
    m_Spirit = false;
    if(!GenerateFrame()) return false;

    int Nf =  GetNChildren();
    for(int i = 0; i < Nf; i++){
      if(!GetChildFrame(i)->AnalyzeEventRecursive()){
	return false;
      }
    }

    m_Spirit = true;
    return m_Spirit;
  }

  void GFrame::SetChildren(const vector<TLorentzVector>& P_children){
    int N = P_children.size();
    for(int i = 0; i < N; i++){
      TLorentzVector P = P_children[i];
      TVector3 B_child = P.BoostVector();

      m_ChildLinks[i]->SetBoostVector(B_child);
      dynamic_cast<GFrame*>(GetChildFrame(i))->SetFourVector(P,this);
    }
  }

  double GFrame::GetRandom(){
    return m_Random->Rndm();
  }

}
