#include <iostream>
#include <string>
#include <frontend/FilesysHelper.hpp>
#include <frontend/Shell.hpp>
#include <frontend/Web.hpp>


#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR 1
#endif


void printVersion() {
  unsigned int version_major = VERSION_MAJOR;
  unsigned int version_minor = VERSION_MINOR;

  std::cout << "#-- CATE (version " << version_major << "." << version_minor << ") --#" << std::endl;
}

void printUsage(const std::string& progname) {
  std::cerr << "Usage:\t" << progname << " <configuration-file> <results-dir>" << std::endl;

  //std::cerr << "   or:\t" << progname << " -w <port>" << std::endl;
  //std::cerr << "\t-w\tstart as web-server on specified tcp-port <port>" << std::endl;
}

int main(int argc, char *argv[]) {
  printVersion();
  
  // get relative path to main program
  std::string relPath, progname;
  FilesysHelper::parsePath( (argc > 0 ? argv[0] : ""), relPath, progname );

//  std::string paramWebServer("-w");

//  if (argc >= 2 && paramWebServer.compare(argv[1]) == 0) { // start in web-mode
//    unsigned int webPort = 0;
//
//    webPort = std::stoul(argv[2], nullptr, 0);
//    if (webPort > 0 && webPort <= 65535) { // port is valid
//      // start web-server
//      std::cout << "starting web-server on port " << webPort << "..." << std::endl;
//      
//      Frontend::Web web;
//      web.setRelativePath(relPath);
//      web.setProgramName(progname);
//      web.setPort(webPort);
//      web.run();
//    }
//    else
//      std::cerr << "Critical error! Port is invalid. Please specify a port-number between 1 and 65535." << std::endl;
//  }
  if (argc >= 3) { // try in shell-mode
    // get config-file
    std::string configFile(argv[1]);
		std::cout << "# configuration file: " << configFile << std::endl;

    unsigned int idxResultDir = 2;
    // get results-directory
    std::string resultDir;
    if (argc >= (int)(idxResultDir + 1)) {
      resultDir = argv[idxResultDir];
		  std::cout << "# directory for results: " << resultDir << std::endl;

      if (!FilesysHelper::checkAndSetDir(resultDir, relPath)) {
        std::cerr << "Critical error! Specified directory for results does not exist." << std::endl;
        return EXIT_FAILURE;
      }
    }
    else {
      std::cerr << "Critical error! No directory for results was specified." << std::endl;
      return EXIT_FAILURE;
    }

    // start in shell-mode
    std::cout << "starting in shell mode..." << std::endl;
    Frontend::Shell sh;
    sh.setRelativePath(relPath);
    sh.setProgramName(progname);
    sh.setConfigFile(configFile);
    sh.setResultsDir(resultDir);
    sh.run();
  }
  else // wrong usage
    printUsage(progname);

  return EXIT_SUCCESS;
}

