#ifndef WEB_INCLUDED
#define WEB_INCLUDED

#include <string>

namespace Frontend {

class Web {
  /** Relative startup path to executed program */
  std::string _relativePath;
  /** Name of executed program */
  std::string _programName;
  /** Port number for tcp-socket to listen for requests. */
  unsigned int _portNumber;
  
public:
	Web() : _relativePath(""), _portNumber(22839) {} 
	~Web() {}

  inline void setRelativePath(std::string path) { _relativePath = path; }
  inline void setProgramName(std::string name) { _programName = name; }
  inline void setPort(unsigned int port) { _portNumber = port; }
  void run();
};

} // namespace Frontend

#endif
