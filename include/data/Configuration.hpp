#ifndef CONFIGURATION_INCLUDED
#define CONFIGURATION_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <data/Benchmark.hpp>

typedef std::vector<std::shared_ptr<Benchmark>> BenchmarkSet;

/**
 * Represents a whole configuration for a CATE run. It contains a number of
 * benchmarks with each containing definitions for an algorithm, a field-
 * structure, ruleset, headers and match-indices between headers and rules.
 */
class Configuration {
  /** Relative path to program from point of execution */
  std::string _progRelPath;
  /** Name of the program that is executed */
  std::string _progName;
  /** Collection of benchmark-configurations to run */
  BenchmarkSet _benchmarks;

public:
	Configuration() : _progRelPath(""), _progName("cate"), _benchmarks() {}
	~Configuration() {}

  inline void setProgRelativePath(const std::string& path) { _progRelPath = path; }
  inline void setProgName(const std::string& name) { _progName = name; }
  
  inline std::string getProgRelativePath() { return _progRelPath; }
  inline std::string getProgName() { return _progName; }

  inline BenchmarkSet& getBenchmarkSet() { return _benchmarks; }

  /** Assign a unique identifier to each benchmark. */
  void setBenchmarkIds();
};

#endif
