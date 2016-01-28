#ifndef RESULTS_INCLUDED
#define RESULTS_INCLUDED

#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <metering/time/ChronoManager.hpp>
#include <metering/memory/MemManager.hpp>
#include <generics/RuleSet.hpp>
#include <evaluation/Statistics.hpp>

/** Contains logged tags as strings. */
typedef std::vector<std::string> LogTagVector;

/** Collects all results of one single testrun of a benchmark. */
struct TestrunResults {
  ChronoResults chronoRes;
  Memory::MemResultGroups memRes;
  Generic::RuleIndexSet indices;
  LogTagVector logTags;
};

/** Contains results of each run of a benchmark. */
typedef std::vector<std::unique_ptr<TestrunResults>> BenchmarkResults;

/** For general information on one single benchmark. */
typedef std::vector<std::pair<std::string, std::string>> BenchmarkInfoVector;

/** Contains results of each testrun in same chrono-category. */
typedef std::vector<unsigned int> ChronoCategoryResults;

/** Contains results of each testrun and each chrono-category. */
typedef std::vector<std::unique_ptr<ChronoCategoryResults>> ChronoResultTable;

/** Statistic summary of one whole chrono-category. */
typedef StatValues<unsigned int> ChronoStatValue;
typedef std::unique_ptr<StatValues<unsigned int>> ChronoStatValuePtr;

/** Container for all evaluation results of chronometer-data. */
struct ChronoEvaluation {
  /** Caption of each category stored. */
  std::vector<std::string> categoryNames;

  /** Holds the exact measurements of all chronos in each testrun. */
  ChronoResultTable measurements;
  
  /** Holds statistic values of each category. */
  std::vector<ChronoStatValuePtr> statistics;
  
  /** Holds statistic mean values divided by amount of headers. */ 
  std::vector<double> relativeHeaders;

  /** Holds statistic mean values divided by amount of rules. */ 
  std::vector<double> relativeRules;
};

/** Statistic summary of one memory-measurement-point. */
typedef MeanValue MemStatValue;

/** A single memory statistical result value (headerCnt, mean). */
typedef std::pair<unsigned int, std::unique_ptr<MemStatValue>> MemStatResult;

/** A single memory result-category (e.g. allocated). */
typedef std::vector<MemStatResult> MemStatCategory;

/** Container for all evaluation results of a memory-data group. */
struct MemEvaluationGroup {
  MemStatCategory allocBytes;
  MemStatCategory allocMaxBytes;
  MemStatCategory accBytes;
  MemStatCategory accReadBytes;
  MemStatCategory accWriteBytes;
  MemStatCategory accCount;
  MemStatCategory accReadCount;
  MemStatCategory accWriteCount;
  MemStatCategory meanAccSize;
  MemStatCategory meanReadSize;
  MemStatCategory meanWriteSize;
};
/** Multiple different groups of the same result-category. */
typedef std::vector<std::unique_ptr<MemEvaluationGroup>> MemEvaluationGroups;

/** Contains results of an evaluation of a single benchmark. */
struct BenchmarkEvaluation {
  /** Contains general information about processed benchmark. */
  BenchmarkInfoVector benchmarkInfo;

  /** Contains exact measured values and statistical evaluation. */
  ChronoEvaluation chrono;
  
  /** Contains statistical memory data, stacked by groups. */
  MemEvaluationGroups mem;
  
  /** True, if same indices produced during multiple testruns. */
  bool allIndicesMatch;

  /** If all indices match, this contains the data. */
  Generic::RuleIndexSet indices;

  /** Contains data to produce a histogram with index-rule matches (only if all indices match between testruns). */
  std::vector<HistogramPair> histogram;

  /** Contains logged tags during benchmark runs. */
  LogTagVector logTags;
};

#endif

