#ifndef FILESYS_HELPER_INCLUDED
#define FILESYS_HELPER_INCLUDED

#include <string>

class FilesysHelper {

public:

  /**
   * Extract from a full startup-path with program-name only the directory-path-prefix
   * with the current execution directory and the name of the program that is executed.
   *
   * @param argv0 full startup-path as given by first argv-parameter
   * @param startupPath reference to string where to place directory-prefix
   * @param name reference to string where to place name of program
   */
  static void parsePath(std::string argv0, std::string& startupPath, std::string& name);

  /**
   * Checks, if a specified directory exists under a given path and if so, 
   * set correct relative path to directory.
   *
   * @param dirpath reference to relative or absolute path to directory (can be set
   *    to full relative path)
   * @param relpath if dirpath is a relative path, take this parameter as prefix
   * @return true, if directory exists under given path
   */
  static bool checkAndSetDir(std::string& dirpath, std::string relpath); 

  /** 
   * Checks if a file with given filepath is present.
   *
   * @param filename full path to a file to check for existence
   * @return true, if file exists, otw. false
   */
  static bool checkFileExists(const std::string& filename);
  
  /**
   * Checks if a directory with given path is present.
   *
   * @param dirpath full path to a directory to check for existence
   * @return true, if directory exists, otw. false
   */
  static bool checkDirExists(const std::string& dirpath);

  /** 
   * Read from a file and copy content into output-string.
   * 
   * @param filename full path to a file which has to be read
   * @param reference to string to store the content of the file
   * @return true, if file was successfully read
   */
  static bool readFile(const std::string& filename, std::string& output);

  /**
   * Copy an existing file to another location.
   *
   * @param source location of file that needs to be copied
   * @param dest target, where file should be copied to
   * @return true, if file is copied successfully
   */
  static bool copyFile(const std::string& source, const std::string& dest);
};

#endif
