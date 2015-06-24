#include <iomanip>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "RestFrames/FrameLog.hh"
#include "RestFrames/RFBase.hh"

using namespace std;

namespace RestFrames {

  // default FrameLog parameters
  map<LogType,bool> FrameLog::m_PrintMap = InitPrintMap();
  ostream* FrameLog::m_Ostr = &cerr;
  int FrameLog::m_NMAX = 100;

  FrameLog::FrameLog(const string& source, LogType def_type){
    Init();
    SetSource(source);
  }

  FrameLog::FrameLog(){
    Init();
  }

  FrameLog g_Log("RestFrames Global");

  FrameLog::~FrameLog() { }

  void FrameLog::Init(){
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

  string FrameLog::GetFormattedSource() const {
    string source_name = m_Source;
    if (source_name.size() > 22){
      source_name = source_name.substr( 0, 22 - 3 );
      source_name += "...";
    }
    return source_name;
  }
  
  string FrameLog::GetFormattedMessage(const string& message) {
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

  void FrameLog::Send(){
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
	if(line == "") break;
	
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

  FrameLog& FrameLog::EndMessage(FrameLog& log){
    log.Send();
    return log;
  }

  void FrameLog::PrintObject(RFBase* objPtr){
    m_Message << objPtr->PrintString();
  }

  void SetLogPrint(LogType type, bool print){
    FrameLog::m_PrintMap[type] = print;
  }

  void SetLogPrint(bool print){
    for (map<LogType, bool>::iterator m = FrameLog::m_PrintMap.begin() ; m != FrameLog::m_PrintMap.end(); ++m)
      m->second = (m->second && print);
  }

  void SetLogStream(ostream* ostr){
    if(ostr) FrameLog::m_Ostr = ostr;
  }

  void SetLogMaxWidth(int NMAX){
    if(NMAX > 0) FrameLog::m_NMAX = NMAX;
  }

  RFBase* Log(const RFBase& obj){ return (RFBase*)&obj; }
  RFBase* Log(RFBase* ptr){ return (RFBase*)ptr; }

  template <> FrameLog& FrameLog::operator<< (RFBase* arg){
      PrintObject(arg);
      return *this;
    }

}
