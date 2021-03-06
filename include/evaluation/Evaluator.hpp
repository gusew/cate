#ifndef EVALUATOR_INCLUDED
#define EVALUATOR_INCLUDED

#include <configuration/Benchmark.hpp>
#include <evaluation/Results.hpp>

/** 
 * Evaluates the data of benchmarks in a benchmark suite.
*/
class Evaluator {
  /** Build a data structure with benchmark information. */
  static void generateBenchmarkInfo(BenchmarkPtr b, BenchmarkInfoVector& info);

  /** Transform chronograph measurements in a table structure. */
  static void createChronoMeasurements(const BenchmarkResults& res, ChronoEvaluation& chrono);
  
  /** Calculate statistic values for each chrono-category. */
  static void createChronoStatistics(ChronoEvaluation& chrono);

  /** Divides each chrono-category mean value by amount of headers. */
  static void calcChronoRelMean(ChronoEvaluation& chrono, unsigned int headers, unsigned int rules);

  /** Calculate statistic values for each memory-group. */
  static void createMemoryStatistics(const BenchmarkResults& res, MemEvaluationGroups& mem);

  /** Check if each testrun produced the same matching-indices. */
  static bool compareIndices(const BenchmarkResults& res);

  /** Creates a histogram for rule-index-matches, generated by the algorithm. */
  static void createHistogram(const Generic::RuleIndexSet& indices, unsigned int amountRules, std::vector<HistogramPair>& hist);
  
  /** Copy all logged tags from each testrun to one single container. */
  static void joinLogTags(const BenchmarkResults& res, LogTagVector& tags);

public:

  /** Evaluate a given benchmark with one or multiple testruns. */
  static void evalBenchmark(BenchmarkPtr b, const BenchmarkResults& res, BenchmarkEvaluation& eval);
};

#endif
