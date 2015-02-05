#include "RestFrames/FrameLink.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // FrameLink class methods
  ///////////////////////////////////////////////
  FrameLink::FrameLink(){
    m_parent_framePtr = nullptr;
    m_child_framePtr = nullptr;
  }
  FrameLink::~FrameLink(){
  
  }

  void FrameLink::Clear(){
    m_parent_framePtr = nullptr;
    if(m_child_framePtr) m_child_framePtr->SetParentLink(nullptr);
    m_B.SetXYZ(0.,0.,0.);
  }

  void FrameLink::SetParentFrame(RestFrame* framePtr){
    m_parent_framePtr = framePtr;
  }

  void FrameLink::SetChildFrame(RestFrame* framePtr){
    m_child_framePtr = framePtr;
  }

  RestFrame* FrameLink::GetParentFrame(){
    return m_parent_framePtr;
  }

  RestFrame* FrameLink::GetChildFrame(){
    return m_child_framePtr;
  }

  void FrameLink::SetBoostVector(const TVector3& boost){
    m_B.SetXYZ(boost.X(),boost.Y(),boost.Z());
  }

  TVector3 FrameLink::GetBoostVector(){
    return m_B;
  }

}
