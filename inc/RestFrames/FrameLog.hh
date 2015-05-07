#ifndef FrameLog_HH
#define FrameLog_HH
#include <iostream>
#include <string>
#include <exception>
#include "RestFrames/RestFrame.hh"
#include "RestFrames/Jigsaw.hh"
#include "RestFrames/Group.hh"

using namespace std;

namespace RestFrames {

  class RestFrame;
  class Jigsaw;
  class Group;

  ///////////////////////////////////////////////
  // FrameLog class
  ///////////////////////////////////////////////
  class FrameLog {
  public:
    FrameLog(const string& message, bool error = false);
    FrameLog(const string& message, RestFrame* frame_ptr, bool error = false);
    FrameLog(const string& message, Jigsaw* jigsaw_ptr, bool error = false);
    FrameLog(const string& message, Group* group_ptr, bool error = false);

    virtual ~FrameLog() {}

    const string Message() const {
      return m_message;
    }

    void SetError(bool error = false){
      m_error = error;
    }

    void SetPrint(bool print = true){
      m_print = print;
    }

    void SetOutputStream(ostream* ostr) {
      m_default_ostr = ostr;
    }
	
  protected:
    string m_message;   
    bool m_error;

  private:
    static bool m_print;
    static std::ostream* m_default_ostr; 
    void ErrorMessage();
    void AddMessage(const string& message);
    void PrintMessage();
    void FrameMessage(RestFrame* frame_ptr);
    void JigsawMessage(Jigsaw* jigsaw_ptr);
    void GroupMessage(Group* group_ptr);
  };

  class RestFramesException : public exception, public FrameLog {
  public:
    RestFramesException(const string& message) : 
      FrameLog(message, true) {}
    RestFramesException(const string& message, RestFrame* frame_ptr) : 
      FrameLog(message, frame_ptr, true) {}
    RestFramesException(const string& message, Jigsaw* jigsaw_ptr) : 
      FrameLog(message, jigsaw_ptr, true) {}
    RestFramesException(const string& message, Group* group_ptr) : 
      FrameLog(message, group_ptr, true) {}

    virtual ~RestFramesException() throw() {}

    virtual const char* what() const throw(){
      return m_message.c_str();
    }
	
  };

}

#endif
