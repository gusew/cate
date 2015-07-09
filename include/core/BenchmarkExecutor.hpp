#ifndef BENCHMARK_EXECUTOR_INCLUDED
#define BENCHMARK_EXECUTOR_INCLUDED

#include <string>
#include <memory>
#include <data/Benchmark.hpp>
#include <data/Results.hpp>
#include <generics/AlgFactory.hpp>
#include <memory/MemTrace.hpp>
#include <memory/MemManager.hpp>
#include <memory/MemTraceRegistry.hpp>
#include <frontend/OutputResults.hpp>
#include <core/Evaluator.hpp>

/** 
 * Prepares all relevant classes for the execution of a benchmark and triggers
 * the classification of the configured algorithm. After a benchmark is executed,
 * results are written to output files.
 */
class BenchmarkExecutor {
  /** holds the configuration of a single benchmark*/
  std::shared_ptr<Benchmark> _benchmark;
  /** relative path to the directory of cate-executable (needed to load the algorithm-library) */
  std::string _relativePath;
  /** path to the directory for results */
  std::string _resultsDir;
  /** handles the output of results during and after a benchmark-execution */
  OutputResults _resultsHandler;
  /** Stores all results of each run of a benchmark. */
  BenchmarkResults _results;

  /// Following are class-instances which will be constructed in call of "execute":

  /** Smart pointer to an instance of the algorithm to use. */
  std::unique_ptr<AlgWrapper> _algWrapper;
  /** Smart pointer to an instance of the MemManager to use. */
  std::shared_ptr<Memory::MemManager> _memManager;
  /** Smart Pointer to an instance of the MemRegistry to use. */
  std::shared_ptr<Memory::MemTraceRegistry> _memRegistry;
  /** Smart pointer to an instance of the ChronoManager to use. */
  std::shared_ptr<ChronoManager> _chrono;
  
  /** Create an instance of an algorithm by loading the specified library file of an algorithm. */
  bool _loadAlgorithm();

  /** Create an instance of a memory manager and assign it to the memory registry for tracing memory. */
  void _setupMemManager();

  /** Create a chronograph manager to measure runtimes. */
  void _setupChronoManager();

  /** Classify a given header set and return matching indices. */
  void _classifyHeaders(const Generic::PacketHeaderSet& headers, Generic::RuleIndexSet& indices);

  /** Copy all contained indices from src to dest container. */
  void _indicesPushBack(const Generic::RuleIndexSet& src, Generic::RuleIndexSet& dest);

  /** Generate headers and/or rules and classify headers with the instantiated algorithm. */
  void _classify(Generic::RuleIndexSet& indices);

  /** Output all given headers to file, if specified in benchmark configuration. */
  void _outputHeadersToFile(const Generic::PacketHeaderSet& headers) const;

  /** Reset all members in order to perform another repetition. */
  void _resetSetup();

public:
	BenchmarkExecutor(const std::string& relPath, const std::string& resultsDir) : _benchmark(), _relativePath(relPath), _resultsDir(resultsDir), _resultsHandler(), _results(), _algWrapper(), _memManager(), _memRegistry(), _chrono() {}
	~BenchmarkExecutor() {}

  /** Sets the benchmark configuration (necessary for execution of a benchmark) */
	void configure(std::shared_ptr<Benchmark> b);

  /** Setup all necessary instances, run specified benchmark, and output results. */
	bool execute();
};

#endif

