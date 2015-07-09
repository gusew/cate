#include <frontend/Shell.hpp>
#include <iostream>
#include <configuration/Configuration.hpp>
#include <configuration/LuaConfigurator.hpp>
#include <configuration/LuaInterpreter.hpp>
#include <core/BenchmarkExecutor.hpp>

using namespace Frontend;

void Shell::run() {
  // get configuration
  std::shared_ptr<Configuration> config = std::make_shared<Configuration>();
  config->setProgRelativePath(_relativePath);
  config->setProgName(_programName);

  Lua::LuaInterpreter lua;
  lua.setConfigurator(std::make_shared<LuaConfigurator>(config));

  try {
    if (lua.interpretFile(_configFile))
      std::cout << "interpretation of lua-configuration was successful" << std::endl;
    else {
      std::cout << "interpretation of lua-configuration failed" << std::endl;
      return;
    }
  }
  catch (const char* ex) {
    std::cerr << "Error while interpreting lua-configuration: " << ex << std::endl;
    return;
  }

  // assign benchmark ids
  config->setBenchmarkIds();

  // launch each benchmark separately
  BenchmarkExecutor texec(_relativePath, _resultsDir);
  for (BenchmarkSet::iterator iter(config->getBenchmarkSet().begin()); iter != config->getBenchmarkSet().end(); ++iter) {
    texec.configure(*iter);
    std::cout << std::endl << "starting execution of '" << (*iter)->caption << "'..." << std::endl;
    bool executionSuccess=false;
    try {
      executionSuccess = texec.execute();
    } catch (const char* ex) {
      std::cerr << "Exception when executing benchmark '" << (*iter)->caption << "':" << std::endl << "\t" << ex << std::endl;
    }

    if (!executionSuccess) 
      std::cout << "Failed to execute benchmark '" << (*iter)->caption << "'. Stop." << std::endl;
  }

  std::cout << std::endl << "All benchmarks were executed." << std::endl;
}

