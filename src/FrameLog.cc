#include <sstream>
#include "RestFrames/FrameLog.hh"

using namespace std;

namespace RestFrames {

  bool FrameLog::m_print = true;
  ostream* FrameLog::m_default_ostr = &cerr;

  ///////////////////////////////////////////////
  // FrameLog class methods
  ///////////////////////////////////////////////
  FrameLog::FrameLog(const string& message, bool error){
    m_error = error;
    ErrorMessage();
    AddMessage(message);
    PrintMessage();
  }  

  FrameLog::FrameLog(const string& message, RestFrame* frame_ptr, bool error){
    m_error = error;
    ErrorMessage();
    FrameMessage(frame_ptr);
    AddMessage(message);
    PrintMessage();
  }  

  FrameLog::FrameLog(const string& message, Jigsaw* jigsaw_ptr, bool error){
    m_error = error;
    ErrorMessage();
    JigsawMessage(jigsaw_ptr);
    AddMessage(message);
    PrintMessage();
  }  

  FrameLog::FrameLog(const string& message, Group* group_ptr, bool error){
    m_error = error;
    ErrorMessage();
    GroupMessage(group_ptr);
    AddMessage(message);
    PrintMessage();
  }  
  
  void FrameLog::ErrorMessage(){
    if(m_error)
      m_message = "\x1b[31m RestFrames::Error: \x1b[0m";
    else
      m_message = "\x1b[35m RestFrames::Warning: \x1b[0m";
  }

  void FrameLog::AddMessage(const string& message){
    m_message += "\x1b[37m";
    m_message += message; 
    m_message += "\x1b[0m";
  }

  void FrameLog::PrintMessage(){
    if (m_print && m_default_ostr){
      ostringstream oss;
      oss << m_message << endl;
      *m_default_ostr << oss.str();
      m_default_ostr->flush();     
    }
  }

  void FrameLog::FrameMessage(RestFrame* frame_ptr){
    if(frame_ptr) 
	m_message += "From\x1b[36m RestFrame " + frame_ptr->GetName() + "\x1b[0m => ";
  }

  void FrameLog::JigsawMessage(Jigsaw* jigsaw_ptr){
    if(jigsaw_ptr) 
	m_message += "From\x1b[36m Jigsaw " + jigsaw_ptr->GetName() + "\x1b[0m => ";
  }

  void FrameLog::GroupMessage(Group* group_ptr){
    if(group_ptr) 
	m_message += "From\x1b[36m Group " + group_ptr->GetName() + "\x1b[0m => ";
  }

}
