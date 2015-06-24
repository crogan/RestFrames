#include <iostream>
#include "RestFrames/RestFrames_config.h"
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFBase class methods
  ///////////////////////////////////////////////

  RFBase::RFBase(const string& sname, const string& stitle){
    Init(sname, stitle);
  }

  RFBase::~RFBase(){
    delete m_LogPtr;
  }

  void RFBase::Init(const string& sname, const string& stitle){
    m_Name = sname;
    m_Title = stitle;
    m_Body   = false;
    m_Mind   = false;
    m_Spirit = false;
    m_LogPtr = new FrameLog();
  }

  bool RFBase::IsSame(const RFBase& obj) const {
    return IsSame(&obj);
  }
     
  bool RFBase::IsSame(const RFBase* objPtr) const {
    if(!objPtr) return false;
    if(m_Key == objPtr->GetKey()) return true;
    return false;
  }

  void RFBase::SetKey(int key) {
    m_Key = key;
  }

  int RFBase::GetKey() const {
    return m_Key;
  }

  string RFBase::GetName() const {
    return m_Name;
  }

  string RFBase::GetTitle() const {
    return m_Title;
  }

  void RFBase::SetBody(bool body) const {
    m_Body = body;
    if(!body) SetMind(body);
  }

  void RFBase::SetMind(bool mind) const {
    m_Mind = mind;
    if(!mind) SetSpirit(mind);
  }

  void RFBase::SetSpirit(bool spirit) const {
    m_Spirit = spirit;
  }

  bool RFBase::IsSoundBody() const {
    return m_Body;
  }

  bool RFBase::IsSoundMind() const {
    return m_Mind;
  }

  bool RFBase::IsSoundSpirit() const {
    return m_Spirit;
  }

  void RFBase::Print(LogType type) const {
    string output = PrintString(type);
    m_Log << type << output << m_End;
  }

  string RFBase::PrintString(LogType type) const {
    string output = "\n";
    output += "   Name: "+GetName()+"\n";
    output += "   Title: "+GetTitle()+"\n";
    return output;
  }

  // Initializer.
  __attribute__((constructor))
  static void initializer(void){
    printf("\n" "\x1b[36m");
    printf(PACKAGE_NAME);
    printf(" v");
    printf(PACKAGE_VERSION);
    printf(" -- Developed by Christopher Rogan (crogan@cern.ch)\n");
    printf("                     ");
    printf("Copyright (c) 2014-2015, Christopher Rogan\n");
    printf("                     ");
    printf("http://RestFrames.com\n");
    printf("\x1b[0m" "\n");
  }

}
