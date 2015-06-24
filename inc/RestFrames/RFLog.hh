#ifndef RFLog_HH
#define RFLog_HH
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
  // RFLog class
  ///////////////////////////////////////////////
  class RFLog {
  public:
    RFLog(const string& source, LogType def_type = LogInfo);
    RFLog();
    ~RFLog();

    void SetSource(const string& source_name){ m_Source = source_name; }
    friend void SetLogPrint(bool print);
    friend void SetLogPrint(LogType type, bool print);
    friend void SetLogStream(ostream* ostr);
    friend void SetLogMaxWidth(int NMAX);

    static RFLog& EndMessage(RFLog& log);
      
    RFLog& operator<< (LogType type);
    
    template <class T> RFLog& operator<< (T arg){
      m_Message << arg;
      return *this;
    }

    RFLog& operator<< (RFLog& (*_f)( RFLog&));
    RFLog& operator<< (ostream& (*_f)(ostream&));
    RFLog& operator<< (ios& (*_f)(ios&));

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

  template <> RFLog& RFLog::operator<< (RFBase* arg);

  inline RFLog& RFLog::operator<< (RFLog& (*_f)(RFLog&)){
    return (_f)(*this);
  }

  inline RFLog& RFLog::operator<< (ostream& (*_f)(ostream&)){
   (_f)(m_Message);
   return *this;
  }

  inline RFLog& RFLog::operator<< (std::ios& (*_f)(ios&)){
    (_f)(m_Message);
    return *this;
  }

  inline RFLog& RFLog::operator<< (LogType type){
    m_CurType = type;
    return *this;
  }

  extern RFLog g_Log;

  RFBase* Log(const RFBase& obj);
  RFBase* Log(RFBase* ptr);

#define m_Log (*m_LogPtr)
#define m_End RFLog::EndMessage  
#define g_End RFLog::EndMessage

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
