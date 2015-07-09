#ifndef OUTPUTRESULTS_INCLUDED
#define OUTPUTRESULTS_INCLUDED

#include <string>
#include <memory>
#include <data/Benchmark.hpp>
#include <generics/RuleSet.hpp>
#include <core/ChronoManager.hpp>
#include <memory/MemManager.hpp>
#include <generics/PacketHeader.hpp>
#include <data/Results.hpp>

class OutputResults {
  /** Benchmark to output results for. */
  BenchmarkPtr _benchmark;

  /** Path to directory where result-files should be stored. */
  std::string _resultsDir;
  /** Path to directory where the dependency-files are stored. */
  std::string _relativeDir;

  /** Output a name of the created file and a summary of its content to stdout. */
  void filenameNotification(const std::string& filename, const std::string& description)const;

  /** Copies a file to the result directory. */
  void _copyDependency(const std::string& filename) const;

  /** Generate the header for an evaluation html-summary. */
  void _htmlHeader(std::ostringstream& str) const;
  
  /** Generate the footer for an evaluation html-summary. */
  void _htmlFooter(std::ostringstream& str, const std::string& id) const;

  /** Generate a table with general info on benchmark. */
  void _htmlInfo(std::ostringstream& str, const std::string& id, const BenchmarkInfoVector& info) const;

  /** Generate a table with statistics on chronograph-results. */
  void _htmlChronoTable(std::ostringstream& str, const std::string& id, const ChronoEvaluation& chrono) const;

  /** Generate plots with chronograph-results. */
  void _htmlChronoPlots(std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const ChronoEvaluation& chrono) const;

  /** Generate tables with memory-trace statistics. */
  void _htmlMemTables(std::ostringstream& html, const std::string& id, const MemEvaluationGroups& mem) const;

  /** Generate plots with memory-results. */
  void _htmlMemPlots(std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const MemEvaluationGroups& mem) const;

  /** Generate a header-rules-histogram. */
  void _htmlHistogram(const std::string& id, std::ostringstream& html, std::ostringstream& data, std::ostringstream& plots, const std::vector<HistogramPair>& hist) const;

  /** Set theme settings for jqplot. */
  void _jsSetTheme(std::ostringstream& str) const;

  /** Add js-code for saving jqplots as png-image. */
  void _jsPlotFooter(std::ostringstream& str) const;

  /** Add js-code for a bar-plot with a single series. */
  void _jsBarPlot(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const;

  /** Add js-code for a histogram-plot. */
  void _jsHistogramPlot(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& xlabel, const std::string& ylabel) const;

  /** Add js-code for a normal line-plot with a single series. */
  void _jsLinePlot1(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const;

  /** Add js-code for a normal line-plot with two series. */
  void _jsLinePlot2(std::ostringstream& str, const std::string& divContainer, const std::string& dataVar1, const std::string& dataVar2, const std::string& s1label, const std::string& s2label, const std::string& title, const std::string& tickVar, const std::string& xlabel, const std::string& ylabel) const;

  /** Write js-code for an 1-dim. array-declaration. */
  void _jsArray1(std::ostringstream& str, const std::vector<std::string>& items, const std::string& name) const;

  /** Write js-code for a 2-dim. array-declaration. */
  void _jsArray2(std::ostringstream& str, const std::vector<std::string>& items1, const std::vector<std::string>& items2, const std::string& name) const;

  /** Write the content out to a specified file. */
  void _writeFile(std::ostringstream& str, const std::string& filename) const;

  /** Dump chronograph results in plain text to string. */
  void _csvChronograph(std::ostringstream& str, const ChronoEvaluation& chrono) const;

  /** Dump rule-matching results in plain text to string. */
  void _csvMatches(std::ostringstream& str, const Generic::RuleIndexSet& indices) const;

  /** Dump memory results in plain text to string. */
  void _csvMemory(std::ostringstream& str, const MemEvaluationGroups& mem) const;

public:
  OutputResults() : _resultsDir(""), _relativeDir("") {}
  ~OutputResults() {}

  /** Set directory for result-files. */
  inline void setResultsDirectory(const std::string& dir) { _resultsDir = dir; }
  /** Set directory for dependency files. */
  inline void setRelativeDirectory(const std::string& dir) { _relativeDir = dir; }

  /** Set pointer to benchmark object, for which results should be output. */
  inline void setBenchmark(BenchmarkPtr b) { _benchmark = b; }

  /** Appends generated header data to a file with all headers. */
  void headers(const Generic::PacketHeaderSet& headers) const;
  
  /** Creates a summary of a benchmark execution with evaluation. */
  void evaluation(const BenchmarkEvaluation& eval) const;
};

#endif

