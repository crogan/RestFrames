#ifndef FrameLog_HH
#define FrameLog_HH
#include <string>
#include <sstream>
#include <map>
#include <exception>

using namespace std;

namespace RestFrames {

  class RFBase;

  /// Type of Log Message
  enum LogType { LogError, LogWarning, LogInfo, LogDebug, LogVerbose };

  ///////////////////////////////////////////////
  // FrameLog class
  ///////////////////////////////////////////////
  class FrameLog {
  public:
    FrameLog(const string& source, LogType def_type = LogInfo);
    FrameLog();
    ~FrameLog();

    void SetSource(const string& source_name){ m_Source = source_name; }
    friend void SetLogPrint(bool print);
    friend void SetLogPrint(LogType type, bool print);
    friend void SetLogStream(ostream* ostr);
    friend void SetLogMaxWidth(int NMAX);

    static FrameLog& EndMessage(FrameLog& log);
      
    FrameLog& operator<< (LogType type);
    
    template <class T> FrameLog& operator<< (T arg){
      m_Message << arg;
      return *this;
    }

    FrameLog& operator<< (FrameLog& (*_f)( FrameLog&));
    FrameLog& operator<< (ostream& (*_f)(ostream&));
    FrameLog& operator<< (ios& (*_f)(ios&));

  private:
    static ostream* m_Ostr;
    static map<LogType, bool> m_PrintMap;
    static int m_NMAX;
    map<LogType, string>      m_TypeMap;      
    map<LogType, string>      m_ColorMap;  

    void Send();
    void Init();
    string GetFormattedSource() const;
    string GetFormattedMessage(const string& message);

    void PrintObject(RFBase* objPtr);

    LogType m_CurType;
    string m_Source;
    ostringstream m_Message;

  };

  template <> FrameLog& FrameLog::operator<< (RFBase* arg);

  inline FrameLog& FrameLog::operator<< (FrameLog& (*_f)(FrameLog&)){
    return (_f)(*this);
  }

  inline FrameLog& FrameLog::operator<< (ostream& (*_f)(ostream&)){
   (_f)(m_Message);
   return *this;
  }

  inline FrameLog& FrameLog::operator<< (std::ios& (*_f)(ios&)){
    (_f)(m_Message);
    return *this;
  }

  inline FrameLog& FrameLog::operator<< (LogType type){
    m_CurType = type;
    return *this;
  }

  extern FrameLog g_Log;

  RFBase* Log(const RFBase& obj);
  RFBase* Log(RFBase* ptr);

#define m_Log (*m_LogPtr)
#define m_End FrameLog::EndMessage  

  class RestFramesException : public exception {

  public:
    RestFramesException(const string& message) : m_Message(message) {} 

    virtual ~RestFramesException() throw() {}

    virtual const char* what() const throw(){
      return m_Message.c_str();
    }
	
  private:
    string m_Message;
    
  };

  void SetLogPrint(bool print = true);
  void SetLogPrint(LogType type, bool print = true);
  void SetLogStream(ostream* ostr);
  void SetLogMaxWidth(int NMAX);
  map<LogType,bool> InitPrintMap();

}

#endif 
