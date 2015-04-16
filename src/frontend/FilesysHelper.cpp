#include <frontend/FilesysHelper.hpp>
#include <fstream>
#include <sys/stat.h> // for determining, if dir exists

void FilesysHelper::parsePath(std::string argv0, std::string& startupPath, std::string& name) {
  if (argv0.size() > 0) {
    // find last '/' and split in 2 parts
    size_t lastSlashIdx = argv0.find_last_of('/');

    if (lastSlashIdx != std::string::npos) {
      startupPath = argv0.substr(0, lastSlashIdx+1); 
      name = argv0.substr(lastSlashIdx+1);
    } else {
      startupPath = ".";
      name = argv0;
    }
  } else { // try with these...
    startupPath = ".";
    name = "cate";
  }
}

bool FilesysHelper::checkFileExists(const std::string& filename) {
  std::ifstream reader(filename);
  bool result = !(reader.fail() || reader.eof());
  reader.close();
  return result;
}

bool FilesysHelper::readFile(const std::string& filename, std::string& output) {
  std::ifstream reader(filename);
  bool result = !(reader.fail() || reader.eof());
  if (result) {
    reader.seekg(0, std::ios::end);
    output.resize(reader.tellg());
    reader.seekg(0, std::ios::beg);
    reader.read(&output[0], output.size());
  }

  reader.close();
  return result;
}

bool FilesysHelper::checkDirExists(const std::string& dirpath) {
  struct stat st;
  if (stat(dirpath.c_str(), &st) == 0) {
    if ( (st.st_mode & S_IFDIR) != 0) // it is a directory
      return true;
  }
  
  return false;
}

bool FilesysHelper::checkAndSetDir(std::string& dirpath, std::string relpath) {
  if (checkDirExists(dirpath)) 
    return true;
  else if (checkDirExists(relpath + dirpath)) {
    dirpath = (relpath + dirpath);
    return true;
  }
  else
    return false;
}

bool FilesysHelper::copyFile(const std::string& source, const std::string& dest) {
  try {
    std::ifstream from(source, std::ios::binary);
    if (from.fail()) return false;

    std::ofstream to(dest, std::ios::binary);
    if (to.fail()) return false;

    to << from.rdbuf();
    return true;
  }
  catch (const char* ex) {
    return false;
  }
}

