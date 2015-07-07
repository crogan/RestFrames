/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFLog.cc
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

#include <iomanip>
#include <stdlib.h>
#include "RestFrames/RFLog.hh"
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  class RFBase;

  // default RFLog parameters
  map<RestFrames::LogType,bool> RFLog::m_PrintMap = InitPrintMap();
  ostream* RFLog::m_Ostr = &cerr;
  int RFLog::m_NMAX = 100;

  RFLog::RFLog(const string& source, LogType def_type){
    Init();
    SetSource(source);
  }

  RFLog::RFLog(){
    Init();
  }

  RFLog g_Log("RestFrames Global");

  RFLog::~RFLog() { }

  void RFLog::Init(){
    m_Source = "Unknown"; 
    m_CurType = LogInfo;
    m_Message.str("");
    m_TypeMap[LogVerbose]  = "VERBOSE";
    m_TypeMap[LogDebug]    = "DEBUG";
    m_TypeMap[LogInfo]     = "INFO";
    m_TypeMap[LogWarning]  = "WARNING";
    m_TypeMap[LogError]    = "ERROR";

    m_ColorMap[LogVerbose]  = "\033[36m";
    m_ColorMap[LogDebug]    = "\033[33m";
    m_ColorMap[LogInfo]     = "\033[32m";
    m_ColorMap[LogWarning]  = "\033[35m";
    m_ColorMap[LogError]    = "\033[31m";
  }

  map<LogType,bool> InitPrintMap(){
    map<LogType,bool> m;
    m[LogVerbose]  = false;
    m[LogDebug]    = false;
    m[LogInfo]     = true;
    m[LogWarning]  = true;
    m[LogError]    = true;
    return m;
  }

  string RFLog::GetFormattedSource() const {
    string source_name = m_Source;
    if (source_name.size() > 22){
      source_name = source_name.substr( 0, 22 - 3 );
      source_name += "...";
    }
    return source_name;
  }
  
  string RFLog::GetFormattedMessage(const string& message) {
    string output = "";
    int N = message.size();
    double OFF = 18;
    if(N-OFF > m_NMAX){
      int Ncut = (N-OFF)/m_NMAX;
      string::size_type previous_pos = 0;
      for(int i = 0; i <= Ncut; i++){
	int off = m_NMAX;
	if(i == 0) off += OFF;
	string line = message.substr(previous_pos, off);
	if(i > 0) output += m_ColorMap[m_CurType]+"<...>\033[0m ...";
	output += line;
	previous_pos += off;
	if(previous_pos != N && i != Ncut) output += "...\n";
      }
    } else {
      output = message;
    }
    return output;
  }

  void RFLog::Send(){
    string source_name = GetFormattedSource();
    string message = m_Message.str();
    string::size_type previous_pos = 0, current_pos = 0;
    if(m_PrintMap[m_CurType] && m_Ostr){
      string prefix = m_ColorMap[m_CurType]+"<"+m_TypeMap[m_CurType]+">";
      for(int i = 0; i < 8-m_TypeMap[m_CurType].size(); i++){
	prefix += ' ';
      }
      prefix += source_name+": \033[0m";
      while (true) {
	current_pos = message.find( '\n', previous_pos );
	string line = message.substr( previous_pos, current_pos - previous_pos );
	
	ostringstream message_to_send;
	message_to_send.setf(std::ios::adjustfield, std::ios::left); 
	line = GetFormattedMessage(prefix+line);
	message_to_send << line << endl;
	
	*m_Ostr << message_to_send.str();
	m_Ostr->flush();
	
	if (current_pos == message.npos) break;
	previous_pos = current_pos + 1;
      }
    }
   
    if (m_CurType == LogError)
      throw RestFramesException(m_Message.str());
    
    m_Message.str("");
    return;
  }

  RFLog& RFLog::EndMessage(RFLog& log){
    log.Send();
    return log;
  }

  void RFLog::PrintObject(const RFBase* objPtr){
    m_Message << objPtr->PrintString();
  }

  template <class T>
  void RFLog::PrintList(const RFList<T>* listPtr){
    int N = listPtr->GetN();
    for(int i = 0; i < N; i++) 
      m_Message << listPtr->Get(i).GetName() << " ";
  }

  void SetLogPrint(LogType type, bool print){
    RFLog::m_PrintMap[type] = print;
  }

  void SetLogPrint(bool print){
    for (map<LogType, bool>::iterator m = RFLog::m_PrintMap.begin() ; m != RFLog::m_PrintMap.end(); ++m)
      m->second = (m->second && print);
  }

  void SetLogStream(ostream* ostr){
    if(ostr) RFLog::m_Ostr = ostr;
  }

  void SetLogMaxWidth(int NMAX){
    if(NMAX > 0) RFLog::m_NMAX = NMAX;
  }

  template <> RFLog& RFLog::operator<< (const RFBase* arg){
    PrintObject(arg);
    return *this;
  }

  template <> RFLog& RFLog::operator<< (const RFList<RFBase>* arg){
    PrintList(arg);
    return *this;
  }

  const RFBase* Log(const RFBase& obj){ return (const RFBase*)&obj; }
  const RFBase* Log(const RFBase* ptr){ return (const RFBase*)ptr; }

}
