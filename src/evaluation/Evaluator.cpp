#include <evaluation/Evaluator.hpp>
#include <utility>
#include <string>
#include <evaluation/Statistics.hpp>

void Evaluator::generateBenchmarkInfo(BenchmarkPtr b, BenchmarkInfoVector& info) {
  info.push_back(std::make_pair("caption", b->caption));
  info.push_back(std::make_pair("algorithm", b->algFilename));

  // parameters handed over to the algorithm
  std::string algParameterString("");
  if (b->algParameter.size() > 0) {
    for (AlgorithmParameterSet::const_iterator iter(b->algParameter.cbegin()); iter != b->algParameter.cend(); ++iter)
      algParameterString += std::to_string(*iter) + "  ";
  } else {
    algParameterString = "none";
  }
  info.push_back(std::make_pair("parameters", algParameterString));

  // header- and rule-structure in bits per field
  std::string fieldStructure("(");
  int tupleSize = b->fieldStructure.size();
  int counter = 0;
  for (FieldStructureSet::const_iterator iter(b->fieldStructure.cbegin()); iter != b->fieldStructure.cend(); ++iter) 
    fieldStructure += std::to_string(*iter) + " bit" + (counter++ < (tupleSize-1) ? ", " : ")");
  info.push_back(std::make_pair("fields", fieldStructure));

  // amount of rules in classifier
  std::string classifierSize(std::to_string(b->rules.size()) + " rules");
  info.push_back(std::make_pair("classifier", classifierSize));

  // amount of explicit or random generated headers
  std::string amountHeaders = std::to_string(b->getHeaderAmount());
  if (b->generateHeaders) { // random header generation
    amountHeaders += " randomly generated";
  } else {
    amountHeaders += " items";
  }
  info.push_back(std::make_pair("headers", amountHeaders));

  info.push_back(std::make_pair( "testruns", std::to_string(b->amountRuns) ));
}

void Evaluator::createChronoMeasurements(const BenchmarkResults& res, ChronoEvaluation& chrono) {
  // first testrun in benchmark: create data-structures
  bool initSubStructure = true;
  for (BenchmarkResults::const_iterator trItr(res.cbegin()); trItr != res.cend(); ++trItr) {
    ChronoResults& chronoRes = ((*trItr)->chronoRes);

    // iterate over each chronometer-category
    unsigned int categoryCounter = 0;
    for (ChronoResults::const_iterator chrItr(chronoRes.cbegin()); chrItr != chronoRes.cend(); ++chrItr) {

      if (initSubStructure) {
        // get each category name
        chrono.categoryNames.push_back(chrItr->first);

        // create sub-structure (vector for exact values)
        std::unique_ptr<ChronoCategoryResults> categoryRes(new ChronoCategoryResults);
        chrono.measurements.push_back(std::move(categoryRes));
      }

      // get exact measurement
      chrono.measurements[categoryCounter++]->push_back(chrItr->second); 
    }

    initSubStructure = false; // constructed all necessary objects in first iteration
  }
}

void Evaluator::createChronoStatistics(ChronoEvaluation& chrono) {
  // iterate over each chrono-category
  for (ChronoResultTable::const_iterator iter(chrono.measurements.cbegin()); iter != chrono.measurements.cend(); ++iter) {
    // collect all category-results in a series
    Series<unsigned int> s;

    // iterate over each value in table-row and copy to series
    for (ChronoCategoryResults::const_iterator valItr((*iter)->cbegin()); valItr != (*iter)->cend(); ++valItr) {
      s.data.push_back(*valItr);
    }

    ChronoStatValuePtr statVal(new ChronoStatValue(s));
    chrono.statistics.push_back(std::move(statVal));
  }
}

void Evaluator::calcChronoRelMean(ChronoEvaluation& chrono, unsigned int headers, unsigned int rules) {
  // iterate over all chrono-categories
  for (auto iter(chrono.statistics.cbegin()); iter != chrono.statistics.cend(); ++iter) {
    double mean = (*iter)->mean;
    chrono.relativeHeaders.push_back( (headers > 0 ? mean/headers : mean) );
    chrono.relativeRules.push_back( (rules > 0 ? mean/rules : mean) );
  }
}


void Evaluator::createMemoryStatistics(const BenchmarkResults& res, MemEvaluationGroups& mem) {
  // get amount of testruns in total
  unsigned int amountRuns = res.size();

  // stop here, if no runs available
  if (amountRuns == 0) return;

  // get amount of groups in total
  unsigned int amountGroups = res[0]->memRes.size();

  // iterate over memory-groups
  for (unsigned int group = 0; group < amountGroups; ++group) {
    std::unique_ptr<MemEvaluationGroup> evalGroupPtr(new MemEvaluationGroup);
    
    // get amount of memory values in current group
    unsigned int amountValues = res[0]->memRes[group]->allocBytes.size();

    // iterate over memory-values in one group
    for (unsigned int valIdx = 0; valIdx < amountValues; ++valIdx) {
      // create statistic series with memory-values of one specific group
      Series<size_t> sAllocBytes;
      Series<size_t> sAllocMaxBytes;
      Series<size_t> sAccBytes;
      Series<size_t> sAccReadBytes;
      Series<size_t> sAccWriteBytes;
      Series<size_t> sAccCount;
      Series<size_t> sAccReadCount;
      Series<size_t> sAccWriteCount;
      Series<double> sMeanAccSize;
      Series<double> sMeanReadSize;
      Series<double> sMeanWriteSize;

      // get header counter value (from first testrun, as all are identic)
      unsigned int headerCount = res[0]->memRes[group]->allocBytes[valIdx].first;

      // iterate over testruns
      for (unsigned int runIdx = 0; runIdx < amountRuns; ++runIdx) {
        size_t allocBytes = res[runIdx]->memRes[group]->allocBytes[valIdx].second;
        size_t allocMaxBytes = res[runIdx]->memRes[group]->allocMaxBytes[valIdx].second;
        size_t accBytes = res[runIdx]->memRes[group]->accBytes[valIdx].second;
        size_t accWriteBytes = res[runIdx]->memRes[group]->accWriteBytes[valIdx].second;
        size_t accCount = res[runIdx]->memRes[group]->accCount[valIdx].second;
        size_t accWriteCount = res[runIdx]->memRes[group]->accWriteCount[valIdx].second;
        // calculate other values
        size_t accReadBytes = accBytes - accWriteBytes;
        size_t accReadCount = accCount - accWriteCount;
        double meanReadSize = (accReadCount > 0 ? (double)accReadBytes / accReadCount : 0);
        double meanWriteSize = (accWriteCount > 0 ? (double)accWriteBytes / accWriteCount : 0);
        double meanAccSize = (accCount > 0 ? (double)accBytes / accCount : 0);

        // place them in series-containers
        sAllocBytes.data.push_back(allocBytes);
        sAllocMaxBytes.data.push_back(allocMaxBytes);
        sAccBytes.data.push_back(accBytes);
        sAccReadBytes.data.push_back(accReadBytes);
        sAccWriteBytes.data.push_back(accWriteBytes);
        sAccCount.data.push_back(accCount);
        sAccReadCount.data.push_back(accReadCount);
        sAccWriteCount.data.push_back(accWriteCount);
        sMeanAccSize.data.push_back(meanAccSize);
        sMeanReadSize.data.push_back(meanReadSize);
        sMeanWriteSize.data.push_back(meanWriteSize);
      }

      // store series evaluation as single statistic value
      std::unique_ptr<MemStatValue> statAllocBytesPtr(new MemStatValue(sAllocBytes));
      std::unique_ptr<MemStatValue> statAllocMaxBytesPtr(new MemStatValue(sAllocMaxBytes));
      std::unique_ptr<MemStatValue> statAccBytesPtr(new MemStatValue(sAccBytes));
      std::unique_ptr<MemStatValue> statAccReadBytesPtr(new MemStatValue(sAccReadBytes));
      std::unique_ptr<MemStatValue> statAccWriteBytesPtr(new MemStatValue(sAccWriteBytes));
      std::unique_ptr<MemStatValue> statAccCountPtr(new MemStatValue(sAccCount));
      std::unique_ptr<MemStatValue> statAccReadCountPtr(new MemStatValue(sAccReadCount));
      std::unique_ptr<MemStatValue> statAccWriteCountPtr(new MemStatValue(sAccWriteCount));
      std::unique_ptr<MemStatValue> statMeanAccSizePtr(new MemStatValue(sMeanAccSize));
      std::unique_ptr<MemStatValue> statMeanReadSizePtr(new MemStatValue(sMeanReadSize));
      std::unique_ptr<MemStatValue> statMeanWriteSizePtr(new MemStatValue(sMeanWriteSize));

      evalGroupPtr->allocBytes.push_back(std::make_pair(headerCount, std::move(statAllocBytesPtr)));
      evalGroupPtr->allocMaxBytes.push_back(std::make_pair(headerCount, std::move(statAllocMaxBytesPtr)));
      evalGroupPtr->accBytes.push_back(std::make_pair(headerCount, std::move(statAccBytesPtr)));
      evalGroupPtr->accReadBytes.push_back(std::make_pair(headerCount, std::move(statAccReadBytesPtr)));
      evalGroupPtr->accWriteBytes.push_back(std::make_pair(headerCount, std::move(statAccWriteBytesPtr)));
      evalGroupPtr->accCount.push_back(std::make_pair(headerCount, std::move(statAccCountPtr)));
      evalGroupPtr->accReadCount.push_back(std::make_pair(headerCount, std::move(statAccReadCountPtr)));
      evalGroupPtr->accWriteCount.push_back(std::make_pair(headerCount, std::move(statAccWriteCountPtr)));
      evalGroupPtr->meanAccSize.push_back(std::make_pair(headerCount, std::move(statMeanAccSizePtr)));
      evalGroupPtr->meanReadSize.push_back(std::make_pair(headerCount, std::move(statMeanReadSizePtr)));
      evalGroupPtr->meanWriteSize.push_back(std::make_pair(headerCount, std::move(statMeanWriteSizePtr)));
    }

    mem.push_back(std::move(evalGroupPtr)); // store whole group in results
  }
}

bool Evaluator::compareIndices(const BenchmarkResults& res) {
  // trivial case: no testruns
  if (res.size() == 0) return true;

  // iterate over all runs and compare amount of indices
  unsigned int amountIndices = res[0]->indices.size();
  for (BenchmarkResults::const_iterator trItr(res.cbegin() + 1); trItr != res.cend(); ++trItr) {
    if ((*trItr)->indices.size() != amountIndices) return false;
  }

  // compare indices of first testrun with each other run
  for (unsigned int i = 0; i < amountIndices; ++i) {
    for (BenchmarkResults::const_iterator trItr(res.cbegin() + 1); trItr != res.cend(); ++trItr) {
      if (res[0]->indices[i] != (*trItr)->indices[i]) return false;
    }
  }

  return true;
}

void Evaluator::createHistogram(const Generic::RuleIndexSet& indices, unsigned int amountRules, std::vector<HistogramPair>& hist) {
  if (amountRules == 0) return; // no rules, no histogram

  Histogram h(amountRules);

  // iterate over all indices
  for (Generic::RuleIndexSet::const_iterator iter(indices.cbegin()); iter != indices.cend(); ++iter) {
    if ((*iter) < Generic::noRuleIsMatching()) h.occured((*iter));
  }

  h.getSorted(hist);
}

void Evaluator::evalBenchmark(BenchmarkPtr b, const BenchmarkResults& res, BenchmarkEvaluation& eval) {

  // gain all general information on benchmark and pack into container
  generateBenchmarkInfo(b, eval.benchmarkInfo);

  // Chronograph: create table with all results
  createChronoMeasurements(res, eval.chrono);

  // Chronograph: data series of each chrono-group
  createChronoStatistics(eval.chrono);

  // Chronograph: relative mean values
  calcChronoRelMean(eval.chrono, b->getHeaderAmount(), b->getRuleAmount());

  // Memory: mean values over all testruns + additional field
  createMemoryStatistics(res, eval.mem);
  
  // Indices: compare them between testruns
  eval.allIndicesMatch = compareIndices(res);

  // further index evaluation only make sense, if generated indices match
  if (eval.allIndicesMatch) { 
    // Indices: copy matches once to output them later
    if (res.size() > 0)
      eval.indices = res[0]->indices;

    // Indices: create histogram with distribution on rules
    createHistogram(eval.indices, b->rules.size(), eval.histogram);
  }

  // TODO calculate mean of matchings per rule 
}

