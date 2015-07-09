#include <core/BenchmarkExecutor.hpp>
#include <generator/HeaderGenerator.hpp>
#include <iostream>

bool BenchmarkExecutor::_loadAlgorithm() {
  // create algorithms instance
  std::string pathToAlgorithm(_relativePath + _benchmark->algFilename);
  bool createRes = AlgFactory::createAlgorithm(pathToAlgorithm, _algWrapper);
  if (createRes && _algWrapper->isValid())
    std::cout << "created algorithm instance of '" << pathToAlgorithm << "'" << std::endl;
  else {
    std::cout << "failed to instantiate algorithm from '" << pathToAlgorithm << "'" << std::endl;
    return false;
  }

  return true;
}

void BenchmarkExecutor::_setupMemManager() {
  _memManager = std::make_shared<Memory::MemManager>();
  _memRegistry = std::make_shared<Memory::MemTraceRegistry>(_memManager);
  Memory::memRegistryPtr = _memRegistry; // for registering MemTrace-instances
  _algWrapper->getAlgorithm()->setMemManager(_memManager);
}

void BenchmarkExecutor::_setupChronoManager() {
  _chrono = std::make_shared<ChronoManager>();;
  _algWrapper->getAlgorithm()->setChronoManager(_chrono);
}

void BenchmarkExecutor::_classifyHeaders(const Generic::PacketHeaderSet& headers, Generic::RuleIndexSet& indices) {
  _chrono->start("total");
  _algWrapper->getAlgorithm()->classify(headers, indices);
  _chrono->stop("total");
}

void BenchmarkExecutor::_indicesPushBack(const Generic::RuleIndexSet& src, Generic::RuleIndexSet& dest) {
  for(Generic::RuleIndexSet::const_iterator iter(src.begin()); iter != src.end(); ++iter)
    dest.push_back(*iter);
}

void BenchmarkExecutor::_classify(Generic::RuleIndexSet& indices) {
  if (_benchmark->generateHeaders) { // generate header data
    const unsigned int BATCH_SIZE = 1024;

    Generic::PacketHeaderSet headers;
    Generic::RuleIndexSet indicesBatch;
    HeaderGenerator hdrGenerator;

    hdrGenerator.configure(_benchmark->fieldStructure, _benchmark->rndHeaderConfig);
    unsigned int headerCnt;

    for (headerCnt = 0; (headerCnt + BATCH_SIZE) < _benchmark->rndHeaderConfig.totalHeaders; headerCnt += BATCH_SIZE) {
      hdrGenerator.generateHeaders(BATCH_SIZE, headers);

      _classifyHeaders(headers, indicesBatch);

      // copy resulting indices to container for all indices
      _indicesPushBack(indicesBatch, indices);

      _outputHeadersToFile(headers); // output headers
    }

    // handle remaining headers
    unsigned int headerRemaining = (_benchmark->rndHeaderConfig.totalHeaders - headerCnt);

    if (headerRemaining > 0) {
      hdrGenerator.generateHeaders(headerRemaining, headers);
      _classifyHeaders(headers, indicesBatch);
      
      // copy resulting indices to container for all indices
      _indicesPushBack(indicesBatch, indices);

      _outputHeadersToFile(headers); // output headers
    }

  } else { // feed with given header data
    _classifyHeaders(_benchmark->headers, indices);
  }

  // feed with changes of rules
  // TODO
}

void BenchmarkExecutor::_outputHeadersToFile(const Generic::PacketHeaderSet& headers) const {
  if (_benchmark->rndHeaderConfig.outputToFile) {
    _resultsHandler.headers(headers);
  }
}

void BenchmarkExecutor::_resetSetup() {
  _memManager->reset();
  _chrono->reset();
  _algWrapper->getAlgorithm()->reset();
  _resultsHandler.setBenchmark(_benchmark); // create a new filename
}

void BenchmarkExecutor::configure(std::shared_ptr<Benchmark> b) { 
  _benchmark.swap(b);

  _resultsHandler.setBenchmark(_benchmark);
  _resultsHandler.setResultsDirectory(_resultsDir);
  _resultsHandler.setRelativeDirectory(_relativePath);
}

bool BenchmarkExecutor::execute() {
  if (!_loadAlgorithm()) return false; // create algorithm instance

  _results.clear(); // remove previous results
  _setupMemManager(); // setup MemManager
  _setupChronoManager(); // setup ChronoManager
  // set algorithm parameters
  _algWrapper->getAlgorithm()->setParameters(_benchmark->algParameter);

  for (unsigned int i = 0; i < _benchmark->amountRuns; ++i) {
    std::unique_ptr<TestrunResults> runResults(new TestrunResults);
    std::cout << "Run test " << std::to_string(i+1) << " of " << 
      std::to_string(_benchmark->amountRuns) << std::flush;
    
    // set rule set
    _algWrapper->getAlgorithm()->setRules(_benchmark->rules);

    // organize header data and classify header
    _classify(runResults->indices);

    // get results from chrono- and memory-manager
    _chrono->getAllResults(runResults->chronoRes);
    _memManager->getMemResultGroups(runResults->memRes);
    _results.push_back(std::move(runResults)); // save results

    _resetSetup(); // reset for next repetition

    // special case: output explicit headers only once
    if (_benchmark->generateHeaders) 
      _benchmark->rndHeaderConfig.outputToFile = false;

    std::cout << "\xd" << std::flush;
  }

  // evaluate all stored results
  BenchmarkEvaluation evaluation;
  Evaluator::evalBenchmark(_benchmark, _results, evaluation);
  _resultsHandler.evaluation(evaluation);

  return true;
}

