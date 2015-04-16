#ifndef LUA_INTERPRETER_INCLUDED
#define LUA_INTERPRETER_INCLUDED

#include <string>
#include <memory>
#include <liblua/lua.hpp>
#include <core/LuaConfigurator.hpp>

namespace Lua {

/** namespace variable for supporting C-signal handler in a C++ class */
extern lua_State* luaState;

/**
 * Provides an instance for a Lua interpreter. A lua script is specified
 * by a filename and merged with pre- and postprocessing-lua-scripts.
 */
class LuaInterpreter {
  static const std::string PREPROCESS_FILENAME;
  static const std::string POSTPROCESS_FILENAME;
  static const std::string PROGRAM_NAME;

  /** Traceback function if some error occurs during script-interpretation */
  static int traceback(lua_State* L);

  /** Releases signal handler after trigger */
  static void lstop(lua_State* L, lua_Debug* ar);
  /** Is called as signalhandler during a running script  */
  static void laction(int i);

  /** Output a string on std::out */
  static void l_message(const char* msg);

  /** Set result code of script interpretation */
  static int report(lua_State* L, int status);
  /** Output an error message, if something went wrong. */
  static void finalreport(lua_State* L, int status);

  /** Helps to print the current lua-stack. */
  static void stackinspect(lua_State* L, int depth);

  /** Start script interpretation in protected mode */
  static int inProtectedMode(lua_State* L);

  /** Reads all files and merges them into one script file for interpretation */
  void prepareScript(const std::string& userConfig, std::string& script);

  /** Stores the configurator of the configuration for CATE, which is given in the lua-script. */
  static std::shared_ptr<LuaConfigurator> configurator; 
 
  /** initially false, but becomes true, if an error occurs during interpretation*/
  static bool errorOccurred;

  /*** The following are private configuration input-functions: ***/

  /** Iterate over all benchmarks and fetch their configuration. */
  static void iterBenchmarks(lua_State* L, int index);

  /** Fetch the configuration of one benchmark. */
  static void fetchBenchmark(lua_State* L, int index);

  /** Fetch the configuration of an algorithm. */
  static void fetchAlgorithm(lua_State* L, int index); 

  /** Fetch the parameter for an algorithm. */
  static void fetchAlgorithmParam(lua_State* L, int index);

  /** Fetch the field-structure of rules and headers. */
  static void fetchStructure(lua_State* L, int index); 

  /** Iterate over all rules in the rule set. */
  static void iterRules(lua_State* L, int index);

  /** Fetch the configuration of one rule. */
  static void fetchRule(lua_State* L, int index); 

  /** Fetch the configuration of one rule atom. */
  static void fetchRuleAtom(lua_State* L, int index); 

  /** Iterate over header definition and determine its type. */
  static void iterHeaderDefinition(lua_State* L, int index);

  /** Iterate over all explicit headers in the header set. */
  static void iterExplicitHeaders(lua_State* L, int index);

  /** Fetch the configuration of one explicit header. */
  static void fetchHeader(lua_State* L, int index); 

  /** Iterate over all header random distributions. */
  static void iterDistributions(lua_State* L, int index); 

  /** Fetch the configuration of one random distribution. */ 
  static void fetchDistribution(lua_State* L, int index); 

public:
  LuaInterpreter();
  ~LuaInterpreter();

  /** Set the connection to the configuration */
  inline void setConfigurator(std::shared_ptr<LuaConfigurator> config) { configurator = config; }

  /** 
   * Enter protected mode and interpret given file. 
   * 
   * @param filename path to file for interpretation
   * @return true, if interpretation was successful, otw. false
   */
  bool interpretFile(const std::string& filename);
};

} // namespace Lua
#endif
