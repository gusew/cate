#ifndef LUA_CONFIGURATOR_INCLUDED
#define LUA_CONFIGURATOR_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <configuration/Configuration.hpp>

/**
 * Is used as a connector between LuaInterpreter, which goes through the
 * data structures in the given Lua configuration-script and stores the
 * converted information into a configuration of benchmarks to run after-
 * wards in the specified testing environment.
 */
class LuaConfigurator {
  /** Reference to the configuration object with a collection of benchmarks. */
  std::shared_ptr<Configuration> _config;

  /** helper function to avoid repeating same code in addDistribution* functions */
  void _pushBackRandomDistribution(unsigned int seed, const std::unique_ptr<RandomDistribution> dist);

  /** Returns the index of the current top most rule of the last benchmark. */
  int _getTopRuleIndex() const;
  /** Returns the configured width of the next to add rule atom. */
  size_t _getCurrentAtomWidth() const;

public:
  LuaConfigurator(std::shared_ptr<Configuration> cfg) : _config(cfg) {}
  ~LuaConfigurator() {}

  void addBenchmark();

  void setCaption(const char* caption);

  void setAlgFilename(const char* filename);
  void addAlgParameter(double value);

  void addFieldStructure(unsigned int bitsize);

  void addRule();
  void addRuleAtomExact(unsigned int value);
  void addRuleAtomExact(std::string& value);
  void addRuleAtomRange(unsigned int min, unsigned int max);
  void addRuleAtomRange(std::string& min, std::string& max);
  void addRuleAtomPrefix(unsigned int prefix, unsigned int mask);
  void addRuleAtomPrefix(std::string& prefix, std::string& mask);

  void addHeader();
  void addHeaderValue(unsigned int value);
  void addHeaderValue(std::string& value);

  void setRandomHeaderNumber(unsigned int number);
  void setRandomHeaderOutput(bool outputToFile);
  void addDistributionConstant(unsigned int value);
  void addDistributionConstant(std::string& value);
  void addDistributionUniform(unsigned int seed, unsigned int min, unsigned int max);
  void addDistributionUniform(unsigned int seed, std::string& min, std::string& max);
  void addDistributionNormal(unsigned int seed, unsigned int mean, unsigned int stddev);
  void addDistributionNormal(unsigned int seed, std::string& mean, std::string& stddev);
  void addDistributionLognormal(unsigned int seed, unsigned int m, unsigned int s);
  void addDistributionLognormal(unsigned int seed, std::string& m, std::string& s);
  void addDistributionExponential(unsigned int seed, double lambda);
  void addDistributionCauchy(unsigned int seed, unsigned int a, double b);
  void addDistributionCauchy(unsigned int seed, std::string& a, double b);
  void addDistributionPareto(unsigned int seed, double scale, double shape, unsigned int offset);
  void addDistributionPareto(unsigned int seed, double scale, double shape, std::string& offset);

  void setNumberRuns(unsigned int number);

  void makeFullRelativePath(const std::string& postfix, std::string& result);
};
#endif

