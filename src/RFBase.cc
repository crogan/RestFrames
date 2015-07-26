/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFBase.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/RestFrames_config.h"
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFBase class methods
  ///////////////////////////////////////////////

  RFBase::RFBase()
    : m_Log(), m_Key(-1)
  {
    m_Name  = "Empty";
    m_Title = "Empty";
    m_Body   = false;
    m_Mind   = false;
    m_Spirit = false;
  }

  RFBase::RFBase(const string& sname, const string& stitle, int key)
    : m_Log(), m_Key(key) {
    m_Name  = sname;
    m_Title = stitle;
    m_Body   = false;
    m_Mind   = false;
    m_Spirit = false;
  }

  RFBase::~RFBase(){
    RFBase::Clear();
  }

  void RFBase::Clear(){
    SetBody(false);
    int N = m_Owns.size();
    for(int i = 0; i < N; i++){
      delete m_Owns[i];
    }
    m_Owns.clear();
  }

  RFBase& RFBase::Empty(){
    return RFBase::m_Empty;
  }

  bool RFBase::IsEmpty() const {
    return m_Key == -1;
  }

  void RFBase::AddDependent(RFBase* dep){
    if(dep) m_Owns.push_back(dep);
  }

  bool RFBase::IsSame(const RFKey& key) const {
    return (m_Key == key);
  }
     
  bool RFBase::IsSame(const RFBase& obj) const {
    return obj == m_Key;
  }

  RFKey RFBase::GetKey() const {
    return RFKey(m_Key);
  }

  string RFBase::GetName() const {
    return m_Name;
  }

  string RFBase::GetTitle() const {
    return m_Title;
  }

  bool RFBase::SetBody(bool body) const {
    m_Body = body;
    if(!body) SetMind(body);
    return m_Body;
  }

  bool RFBase::SetMind(bool mind) const {
    m_Mind = mind;
    if(!mind) SetSpirit(mind);
    return m_Mind;
  }

  bool RFBase::SetSpirit(bool spirit) const {
    m_Spirit = spirit;
    return m_Spirit;
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

  void RFBase::UnSoundBody(const string& function) const {
    m_Log << LogWarning;
    m_Log << "Unable to evaluate function \"" << function << "\". ";
    m_Log << "Requires a successful call to \"InitializeTree()\" ";
    m_Log << "from the LabFrame associated with this tree.";
    m_Log << m_End;
  }

  void RFBase::UnSoundMind(const string& function) const {
    m_Log << LogWarning;
    m_Log << "Unable to evaluate function \"" << function << "\". ";
    m_Log << "Requires a successful call to \"InitializeAnalysis()\" ";
    m_Log << "from the LabFrame associated with this tree.";
    m_Log << m_End;
  }

  void RFBase::UnSoundSpirit(const string& function) const {
    m_Log << LogWarning;
    m_Log << "Unable to evaluate function \"" << function << "\". ";
    m_Log << "Requires a successful call to \"AnalyzeEvent()\" ";
    m_Log << "from the LabFrame associated with this tree.";
    m_Log << m_End;
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

  RFBase RFBase::m_Empty;

  double GetProb(double Mp, double Mc1, double Mc2){
    if(Mp <= 0.) return 0.;
    Mc1 = max(Mc1, 0.);
    Mc2 = max(Mc2, 0.);
    return sqrt(max(0., (Mp*Mp-Mc1*Mc1-Mc2*Mc2)*(Mp*Mp-Mc1*Mc1-Mc2*Mc2)-4.*Mc1*Mc1*Mc2*Mc2) )/2./Mp;
  }

}
