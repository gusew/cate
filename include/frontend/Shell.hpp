#ifndef SHELL_INCLUDED
#define SHELL_INCLUDED

#include <string>

namespace Frontend {

class Shell {
  /** Relative startup path to executed program */
  std::string _relativePath;
  /** Name of executed program */
  std::string _programName;
  /** Relative path to configuration file */
  std::string _configFile;
  /** Relative path to directory for saving results */
  std::string _resultsDir;

public:
	Shell() : _relativePath(""), _programName("cate"), _configFile(""), _resultsDir("") {}
	~Shell() {}

  inline void setRelativePath(const std::string& path) { _relativePath = path; }
  inline void setProgramName(const std::string& name) { _programName = name; }
  inline void setConfigFile(const std::string& file) { _configFile = file; }
  inline void setResultsDir(const std::string& dir) { _resultsDir = dir; }
  
  void run();
};

} // namespace Frontend
#endif
