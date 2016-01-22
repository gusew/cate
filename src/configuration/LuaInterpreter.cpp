#include <configuration/LuaInterpreter.hpp>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <frontend/FilesysHelper.hpp>
#include <string>

lua_State* Lua::luaState = nullptr;

using namespace Lua;

const std::string LuaInterpreter::PREPROCESS_FILENAME = "preprocess.lua";
const std::string LuaInterpreter::POSTPROCESS_FILENAME = "postprocess.lua";
const std::string LuaInterpreter::PROGRAM_NAME = "lua(cate)";

std::shared_ptr<LuaConfigurator> LuaInterpreter::configurator;
bool LuaInterpreter::errorOccurred = false;

int LuaInterpreter::traceback(lua_State *L) {
  const char *msg = lua_tostring(L, 1);
  if (msg)
    luaL_traceback(L, L, msg, 1);
  else if (!lua_isnoneornil(L, 1)) {  /* is there an error object? */
    if (!luaL_callmeta(L, 1, "__tostring"))  /* try its 'tostring' metamethod */
      lua_pushliteral(L, "(no error message)");
  }
  return 1;
}

void LuaInterpreter::lstop(lua_State* L, lua_Debug* ar) {
  (void)ar;  /* unused arg. */
  lua_sethook(L, NULL, 0, 0);
  luaL_error(L, "interrupted!");
}
void LuaInterpreter::laction(int i) {
  signal(i, SIG_DFL); /* if another SIGINT happens before lstop,
                              terminate process (default action) */
  lua_sethook(luaState, &Lua::LuaInterpreter::lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}

void LuaInterpreter::l_message(const char* msg) {
  luai_writestringerror("%s: ", PROGRAM_NAME.c_str());   
  luai_writestringerror("%s\n", msg); 
}

int LuaInterpreter::report(lua_State* L, int status) {
  if (status != LUA_OK && !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    l_message(msg);
    lua_pop(L, 1);
    /* force a complete garbage collection in case of errors */
    lua_gc(L, LUA_GCCOLLECT, 0);
    errorOccurred = true;
  }
  return status;
}

void LuaInterpreter::finalreport(lua_State* L, int status) {
  if (status != LUA_OK) {
    const char *msg = (lua_type(L, -1) == LUA_TSTRING) ? lua_tostring(L, -1) : NULL;
    if (msg == NULL) msg = "(error object is not a string)";
    l_message(msg);
    lua_pop(L, 1);
    errorOccurred = true;
  }
}

void LuaInterpreter::stackinspect(lua_State* L, int depth) {
  int top = lua_gettop(L);
  std::cout << "--- STACK INSPECTOR ---" << std::endl;
  for (int i = top; i > top-depth && i > 0; --i) {
    std::cout << i << ": [" << lua_typename(L, lua_type(L, i)) << "]= ";
    
    if (lua_isboolean(L, i)) std::cout << lua_toboolean(L, i) << std::endl;
    else if (lua_isnumber(L, i)) std::cout << lua_tointeger(L, i) << std::endl;
    else if (lua_isnoneornil(L, i)) std::cout << "none or nil" << std::endl;
    else if (lua_isstring(L, i)) std::cout << lua_tostring(L, i) << std::endl;
    else std::cout << "(?)" << std::endl;
  }
  std::cout << "--- END ---" << std::endl;
}

int LuaInterpreter::inProtectedMode(lua_State* L) {
  const char* luascript = lua_tostring(L, 1);

  lua_pushboolean(L, 1);  /* signal for libraries to ignore env. vars. */
  lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");

  /* open standard libraries */
  luaL_checkversion(L);
  lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
  luaL_openlibs(L);  /* open libraries */
  lua_gc(L, LUA_GCRESTART, 0); // restart garbage collector

  int status = luaL_loadstring(L, luascript); // execute scriptfile
  lua_insert(L, -1);
  if (status == LUA_OK) {
    // set traceback function as error-handler for script
    int base = lua_gettop(L);  /* function index */
    lua_pushcfunction(L, &Lua::LuaInterpreter::traceback);  /* push traceback function */
    lua_insert(L, base);  /* put it under chunk and args */

    luaState = L;
    signal(SIGINT, laction);
    status = lua_pcall(L, 0, 1, base); // base for error-function
    signal(SIGINT, SIG_DFL);

    if (lua_istable(L, base+1)) { // check if configuration was returned
      iterBenchmarks(L, base+1); // iterate over all benchmarks in suite
    } else { // error: no configuration set
      errorOccurred = true;
    }

    if (status == LUA_OK) {
      lua_remove(L, base+1); // remove table from stack
      lua_remove(L, base);  // remove traceback function
    } // else keep error-msg on top of stack
  }

  if (report(L, status) != LUA_OK) return 0;
  
  return 1;
}

void LuaInterpreter::iterBenchmarks(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);
    // key is at index -2, value at index -1
    if (lua_istable(L, tblIdx)) {
      configurator->addBenchmark();
      fetchBenchmark(L, tblIdx);
    } else {
      l_message("Invalid benchmark definition found (not a table).");
      errorOccurred = true;
    }
    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchBenchmark(lua_State* L, int index) {
  lua_pushnil(L); // first key

  while(lua_next(L, index) != 0 && !errorOccurred) {  
    int key = lua_tointeger(L, -2); // key is at index -2, value at index -1
    int valIdx = lua_gettop(L);

    if (key == 1 && lua_isstring(L, valIdx)) // caption
      configurator->setCaption(lua_tostring(L, valIdx));
    else if (key == 1) {
      l_message("No caption for benchmark found.");
      errorOccurred = true;
    }
    else if (key == 2 && lua_istable(L, valIdx)) { // algorithm
      fetchAlgorithm(L, valIdx); 
    }
    else if (key == 2) {
      l_message("No algorithm definition for benchmark found.");
      errorOccurred = true;
    }
    else if (key == 3 && lua_istable(L, valIdx)) { // field-structure
      fetchStructure(L, valIdx); 
    }
    else if (key == 3) {
      l_message("No field structure definition for benchmark found.");
      errorOccurred = true;
    }
    else if (key == 4 && lua_istable(L, valIdx)) { // ruleset
      iterRules(L, valIdx);
    }
    else if (key == 4) {
      l_message("No ruleset for benchmark found.");
      errorOccurred = true;
    }
    else if (key == 5 && lua_istable(L, valIdx)) { // headers
      iterHeaderDefinition(L, valIdx);
    }
    else if (key == 5) {
      l_message("No headers for benchmark found.");
      errorOccurred = true;
    }
    else if (key == 6 && lua_isnumber(L, valIdx)) { // number of runs
      configurator->setNumberRuns(lua_tounsigned(L, valIdx));
    }
    else if (key == 6) {
      l_message("Number of runs is not defined. Set default value of 1 run.");
      configurator->setNumberRuns(1);
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchAlgorithm(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0 && !errorOccurred) {
    int key = lua_tointeger(L, -2); // key is at index -2, value at index -1
    int valIdx = lua_gettop(L);
    if (key == 1 && lua_isstring(L, valIdx)) // algorithm-filename
      configurator->setAlgFilename(lua_tostring(L, valIdx));
    else if (key == 1) {
      l_message("No filename for the algorithm found.");
      errorOccurred = true;
    }
    else if (key == 2 && lua_istable(L, valIdx)) { // algorithm-parameter
      fetchAlgorithmParam(L, valIdx);
    }
    else if (key == 2) {
      l_message("No parameter list for the algorithm found.");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchAlgorithmParam(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0) {
    int valIdx = lua_gettop(L);

    if (lua_isnumber(L, valIdx))
      configurator->addAlgParameter(lua_tonumber(L, valIdx));
    
    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchStructure(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0) {
    int valIdx = lua_gettop(L);
  
    if (lua_isnumber(L, valIdx))
      configurator->addFieldStructure(lua_tounsigned(L, valIdx));
    
    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::iterRules(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);

    if (lua_istable(L, tblIdx)) {
      configurator->addRule();
      fetchRule(L, tblIdx);
    } else {
      l_message("Invalid rule definition found (not a table).");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchRule(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);

    if (lua_istable(L, tblIdx)) {
      fetchRuleAtom(L, tblIdx);
    } else {
      l_message("Invalid rule atom definition found (not a table).");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchRuleAtom(lua_State* L, int index) {
  lua_pushnil(L); // first key
  unsigned int atomType=0;
  unsigned int atomVal1=0, atomVal2=0;
  std::string varVal1, varVal2; // atom values can also exceed 32bits

  while(lua_next(L, index) != 0 && !errorOccurred) {
    int key = lua_tointeger(L, -2); // key is at index -2, value at index -1
    int tblIdx = lua_gettop(L);

    if (key == 1 && lua_isnumber(L, tblIdx)) { // rule atom type
      atomType = lua_tounsigned(L, tblIdx);
    } else if (key == 1) {
      l_message("Invalid rule atom type found (not a number).");
      errorOccurred = true;
    }
    else if (key == 2 && lua_isnumber(L, tblIdx)) { // rule atom first value (as single number)
      atomVal1 = lua_tounsigned(L, tblIdx);

      if (atomType == 0) // atom-type is exact
        configurator->addRuleAtomExact(atomVal1);
    }
    else if (key == 2 && lua_isstring(L, tblIdx)) { // rule atom first value (in string representation)
      varVal1 = lua_tostring(L, tblIdx);

      if (atomType == 0) // atom-type is exact
        configurator->addRuleAtomExact(varVal1);
    }
    else if (key == 2) {
      l_message("Invalid rule atom first value found (not a number or string).");
      errorOccurred = true;
    }
    else if (key == 3 && atomType > 0 && lua_isnumber(L, tblIdx)) { // rule atom second value (as single number)
      atomVal2 = lua_tounsigned(L, tblIdx);

      if (atomType == 1) // atom-type is range
        configurator->addRuleAtomRange(atomVal1, atomVal2);
      else if (atomType == 2) // atom-type is prefix
        configurator->addRuleAtomPrefix(atomVal1, atomVal2);
      else {
        l_message("Invalid rule atom type found (not exact, range nor prefix).");
        errorOccurred = true;
      }
    }
    else if (key == 3 && atomType > 0 && lua_istable(L, tblIdx)) { // rule atom second value (in string representation)
      varVal2 = lua_tostring(L, tblIdx);

      if (atomType == 1) // atom-type is range
        configurator->addRuleAtomRange(varVal1, varVal2);
      else if (atomType == 2) // atom-type is prefix
        configurator->addRuleAtomPrefix(varVal1, varVal2);
      else {
        l_message("Invalid variable rule atom type found (not exact, range nor prefix).");
        errorOccurred = true;
      }
    } 
    else if (key == 3) {
      l_message("Invalid rule atom second value found (not a number or string).");
      errorOccurred = true;
    }
    
    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::iterHeaderDefinition(lua_State* L, int index) {
  lua_pushnil(L); // first key
  bool isRandomHeader = false;
  bool isExactHeader = false;

  while(lua_next(L, index) != 0 && !errorOccurred) {
    int key = lua_tointeger(L, -2); // key is at index -2, value at index -1
    int tblIdx = lua_gettop(L);

    if (key == 1 && lua_isstring(L, tblIdx)) { // type-marker
      std::string cmpExact("exact");
      std::string cmpRandom("random");

      if (cmpExact.compare(lua_tostring(L, tblIdx)) == 0) { 
        // get exact header values
        isExactHeader = true;
      } else if (cmpRandom.compare(lua_tostring(L, tblIdx)) == 0) {
        // get random header values
        isRandomHeader = true;
      } else {
        l_message("Invalid header definition found (not exact nor random values).");
        errorOccurred = true;
      }
    }
    else if (key == 2 && isExactHeader && lua_istable(L, tblIdx)) { 
      // analyze table structure with explicit header values
      iterExplicitHeaders(L, tblIdx);
    }
    else if (key == 2 && isRandomHeader && lua_isnumber(L, tblIdx)) { 
      // number of headers to generate
      configurator->setRandomHeaderNumber(lua_tounsigned(L, tblIdx));
    } 
    else if (key == 3 && isRandomHeader && lua_isboolean(L, tblIdx)) {
      // output headers to file
      configurator->setRandomHeaderOutput(lua_toboolean(L, tblIdx));
    }
    else if (key == 4 && isRandomHeader && lua_istable(L, tblIdx)) { 
      // distributions
      iterDistributions(L, tblIdx);
    } else {
      l_message("Invalid header definition found (table structure doesn't match expectations).");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::iterExplicitHeaders(lua_State* L, int index) {
  lua_pushnil(L); // first key

  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);

    if (lua_istable(L, tblIdx)) { 
      configurator->addHeader();
      fetchHeader(L, tblIdx);
    } else {
      l_message("Invalid explicit header definition found (not a table).");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchHeader(lua_State* L, int index) {
  lua_pushnil(L); // first key
  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);

    if (lua_isnumber(L, tblIdx)) { // header field as single number
      configurator->addHeaderValue(lua_tounsigned(L, tblIdx));
    } 
    else if (lua_isstring(L, tblIdx)) { // header field in string representation
      std::string varVal = lua_tostring(L, tblIdx);
      configurator->addHeaderValue(varVal);
    }
    else {
      l_message("Invalid header value found (not a number or string).");
      errorOccurred = true;
    }
    
    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::iterDistributions(lua_State* L, int index) {
  lua_pushnil(L); // first key

  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);

    if (lua_istable(L, tblIdx)) { 
      fetchDistribution(L, tblIdx);
    } else {
      l_message("Invalid header distribution found (not a table).");
      errorOccurred = true;
    }

    lua_pop(L, 1); // remove value, keep key for next iteration
  }
}

void LuaInterpreter::fetchDistribution(lua_State* L, int index) {
  lua_pushnil(L); // first key
  int distribId = -1;
  unsigned int seed=0, distVal1=0, distVal2=0;
  std::string distVarVal1, distVarVal2; // distrib. values can exceed 32bits
  double dblValue=0.0, dblValue2=0.0; // some distrib. parameters are doubles

  while(lua_next(L, index) != 0 && !errorOccurred) {
    int tblIdx = lua_gettop(L);
    int key = lua_tointeger(L, -2); // key is at index -2, value at index -1

    if (key == 1 && lua_isnumber(L, tblIdx)) { // id of distribution
      distribId = lua_tonumber(L, tblIdx);
    } 
    else if (key == 2 && distribId == 0) { // constant distrib.
      if (lua_isnumber(L, tblIdx)) { // 32 bit value
        configurator->addDistributionConstant(lua_tounsigned(L, tblIdx));
      } else if (lua_isstring(L, tblIdx)) { // in string representation
        distVarVal1 = lua_tostring(L, tblIdx);
        configurator->addDistributionConstant(distVarVal1);
      }
    }
    else if (distribId == 1) { // uniform distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // min
        if (lua_isnumber(L, tblIdx))
          distVal1 = lua_tounsigned(L, tblIdx);
        else if (lua_isstring(L, tblIdx))
          distVarVal1 = lua_tostring(L, tblIdx);
        else {  
          l_message("Invalid definition of uniform distribution min-value found.");
          errorOccurred = true;
        }
      } else if (key == 4) { // max
        if (lua_isnumber(L, tblIdx)) {
          distVal2 = lua_tounsigned(L, tblIdx);
          configurator->addDistributionUniform(seed, distVal1, distVal2);
        } else if (lua_isstring(L, tblIdx)) {
          distVarVal2 = lua_tostring(L, tblIdx);
          configurator->addDistributionUniform(seed, distVarVal1, distVarVal2); 
        } else {  
          l_message("Invalid definition of uniform distribution max-value found.");
          errorOccurred = true;
        }
      }
      else {
        l_message("Invalid definition of uniform distribution found.");
        errorOccurred = true;
      }
    }
    else if (distribId == 2) { // normal distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // mean
        if (lua_isnumber(L, tblIdx))
          distVal1 = lua_tounsigned(L, tblIdx);
        else if (lua_isstring(L, tblIdx))
          distVarVal1 = lua_tostring(L, tblIdx);
        else {  
          l_message("Invalid definition of normal distribution mean-value found.");
          errorOccurred = true;
        }
      } else if (key == 4) { // stddev
        if (lua_isnumber(L, tblIdx)) {
          distVal2 = lua_tounsigned(L, tblIdx);
          configurator->addDistributionNormal(seed, distVal1, distVal2);
        } 
        else if (lua_isstring(L, tblIdx)) {
          distVarVal2 = lua_tostring(L, tblIdx);
          configurator->addDistributionNormal(seed, distVarVal1, distVarVal2); 
        } else {  
          l_message("Invalid definition of normal distribution stddev-value found.");
          errorOccurred = true;
        }
      }
      else {
        l_message("Invalid definition of normal distribution found.");
        errorOccurred = true;
      }
    }
    else if (distribId == 3) { // lognormal distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // m
        if (lua_isnumber(L, tblIdx)) {
          distVal1 = lua_tounsigned(L, tblIdx);
        } 
        else if (lua_isstring(L, tblIdx)) {
          distVarVal1 = lua_tostring(L, tblIdx);
        } else {  
          l_message("Invalid definition of lognormal distribution m-value found.");
          errorOccurred = true;
        }
      } else if (key == 4) { // s
        if (lua_isnumber(L, tblIdx)) {
          distVal2 = lua_tounsigned(L, tblIdx);
          configurator->addDistributionLognormal(seed, distVal1, distVal2);
        } 
        else if (lua_isstring(L, tblIdx)) {
          distVarVal2 = lua_tostring(L, tblIdx);
          configurator->addDistributionLognormal(seed, distVarVal1, distVarVal2); 
        } else {  
          l_message("Invalid definition of lognormal distribution s-value found.");
          errorOccurred = true;
        }
      }
      else {
        l_message("Invalid definition of lognormal distribution found.");
        errorOccurred = true;
      }
    }
    else if (distribId == 4) { // exponential distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // lambda
        if (lua_isnumber(L, tblIdx)) {
          dblValue = lua_tonumber(L, tblIdx);
          configurator->addDistributionExponential(seed, dblValue);
        } else {  
          l_message("Invalid definition of exponential distribution lambda-value found.");
          errorOccurred = true;
        }
      } else {
        l_message("Invalid definition of exponential distribution found.");
        errorOccurred = true;
      }
    }
    else if (distribId == 5) { // cauchy distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // b
        if (lua_isnumber(L, tblIdx)) {
          dblValue = lua_tonumber(L, tblIdx);
        } else {  
          l_message("Invalid definition of cauchy distribution b-value found.");
          errorOccurred = true;
        }
      } else if (key == 4) { // a
        if (lua_isnumber(L, tblIdx)) {
          distVal1 = lua_tounsigned(L, tblIdx);
          configurator->addDistributionCauchy(seed, distVal1, dblValue);
        } 
        else if (lua_isstring(L, tblIdx)) {
          distVarVal1 = lua_tostring(L, tblIdx);
          configurator->addDistributionCauchy(seed, distVarVal1, dblValue);
        } else {  
          l_message("Invalid definition of cauchy distribution a-value found.");
          errorOccurred = true;
        }
      }
      else {
        l_message("Invalid definition of cauchy distribution found.");
        errorOccurred = true;
      }
    }
    else if (distribId == 6) { // pareto distrib.
      if (key == 2 && lua_isnumber(L, tblIdx)) { // seed
        seed = lua_tounsigned(L, tblIdx);
      } else if (key == 3) { // scale
        if (lua_isnumber(L, tblIdx)) {
          dblValue = lua_tonumber(L, tblIdx);
        } else {  
          l_message("Invalid definition of pareto distribution scale-value found.");
          errorOccurred = true;
        }
      } else if (key == 4) { // shape
        if (lua_isnumber(L, tblIdx)) {
          dblValue2 = lua_tonumber(L, tblIdx);
        } else {  
          l_message("Invalid definition of pareto distribution shape-value found.");
          errorOccurred = true;
        }
      } else if (key == 5) { // offset 
        if (lua_isnumber(L, tblIdx)) {
          distVal1 = lua_tounsigned(L, tblIdx);
          configurator->addDistributionPareto(seed, dblValue, dblValue2, distVal1);
        } 
        else if (lua_isstring(L, tblIdx)) {
          distVarVal1 = lua_tostring(L, tblIdx);
          configurator->addDistributionPareto(seed, dblValue, dblValue2, distVarVal1);
        } else {  
          l_message("Invalid definition of pareto distribution offset-value found.");
          errorOccurred = true;
        }
      }
      else {
        l_message("Invalid definition of pareto distribution found.");
        errorOccurred = true;
      }
    }
    else {
      l_message("Invalid distribution value found (not a valid number).");
      errorOccurred = true;
    }
    
    lua_pop(L, 1); // remove value, keep key for next iteration
  }

}

LuaInterpreter::LuaInterpreter() {
    if (luaState != nullptr) throw "Cannot use LuaInterpreter in more than one instance.";

    luaState = luaL_newstate();
    if (luaState == NULL) throw "Cannot create Lua-state: not enough memory.";

    errorOccurred = false; // initially no errors
}

LuaInterpreter::~LuaInterpreter() { 
  lua_close(luaState); 
  luaState = nullptr;
  configurator.reset();
}

void LuaInterpreter::prepareScript(const std::string& userConfig, std::string& script) {
  // create filepaths 
  std::string preFile, postFile;
  configurator->makeFullRelativePath(PREPROCESS_FILENAME, preFile);
  configurator->makeFullRelativePath(POSTPROCESS_FILENAME, postFile);

  // load preprocess & postprocess-files
  if (!FilesysHelper::checkFileExists(preFile)) throw "Couldn't find preprocessing lua-script.";
  if (!FilesysHelper::checkFileExists(postFile)) throw "Couldn't find postprocessing lua-script.";
  
  std::string pre, post, config;
  if (!FilesysHelper::readFile(preFile, pre)) throw "Error when reading preprocessing lua-script";
  if (!FilesysHelper::readFile(postFile, post)) throw "Error when reading preprocessing lua-script";
  // read file into string
  if (!FilesysHelper::readFile(userConfig, config)) throw "Error when reading user-configuration lua-script";

  // merge all files into complete script
  script = "-- This is the merged file for CATE-configuration.\n\n";
  script.append(pre);
  script.append(config);
  script.append(post);
}

bool LuaInterpreter::interpretFile(const std::string& filename) {
  if (!configurator) throw "Couldn't interpret lua-script as no configurator was set.";
  
  std::string script;
  prepareScript(filename, script);

  int status = 0;
  // handle over function for protected mode and script-filename
  lua_pushcfunction(luaState, &Lua::LuaInterpreter::inProtectedMode); 
  lua_pushstring(luaState, script.c_str());

  // enter protected mode
  status = lua_pcall(luaState, 1, 1, 0);
  if (status != LUA_OK) finalreport(luaState, status);

  return !errorOccurred;
}


