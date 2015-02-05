#include "RestFrames/NVisibleMInvisibleJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // NVisibleMInvisibleJigsaw class methods
  ///////////////////////////////////////////////
  NVisibleMInvisibleJigsaw::NVisibleMInvisibleJigsaw(const string& sname, const string& stitle, 
						     int N, int M) : 
    InvisibleJigsaw(sname, stitle)
  {
    Init(N, M);
  }
  NVisibleMInvisibleJigsaw::NVisibleMInvisibleJigsaw(const string& sname, const string& stitle, 
						     int ikey, int N, int M) : 
    InvisibleJigsaw(sname, stitle, ikey)
  {
    Init(N, M);
  }
  NVisibleMInvisibleJigsaw::~NVisibleMInvisibleJigsaw(){
  
  }

  void NVisibleMInvisibleJigsaw::Init(int N, int M){
    m_Nvis = N;
    m_Minv = M;
  }

  void NVisibleMInvisibleJigsaw::SetGroup(Group* groupPtr){
    Jigsaw::SetGroup(groupPtr);
    if(!groupPtr) return;
    if(m_Minv == 1) AddInvisibleFrame(groupPtr->GetFrames());
  }

  bool NVisibleMInvisibleJigsaw::IsSoundBody() const {
    Jigsaw::IsSoundBody();
    if(!m_Body) return m_Body;

    int checkNdep = 0;
    int checkNout = 0;
    int Ndep = m_DependancyFrames.size();
    int Nout = m_OutputFrames.size();

    if(Ndep != m_Nvis) m_Body = false;
    for(int i = 0; i < Ndep; i++){
      if(m_DependancyFrames[i]->GetN() > 0) checkNdep++;
    }
    if(checkNdep != m_Nvis) m_Body = false;

    if(Nout != m_Minv) m_Body = false;
    for(int i = 0; i < Nout; i++){
      if(m_OutputFrames[i]->GetN() > 0) checkNout++;
    }
    if(checkNout != m_Minv) m_Body = false;

    return m_Body;
  }

}
