#include <configuration/LuaConfigurator.hpp>
#include <limits>
#include <configuration/RandomHeaderConfiguration.hpp>

using namespace Generic;

void LuaConfigurator::addBenchmark() { 
  _config->getBenchmarkSet().push_back(std::make_shared<Benchmark>());
}

/*** Handle general information of Benchmark. */
void LuaConfigurator::setCaption(const char* caption) { 
  _config->getBenchmarkSet().back()->caption = caption;
}
void LuaConfigurator::setAlgFilename(const char* filename) { 
  _config->getBenchmarkSet().back()->algFilename = filename;
}

void LuaConfigurator::addAlgParameter(double value) { 
  _config->getBenchmarkSet().back()->algParameter.push_back(value);
}

void LuaConfigurator::addFieldStructure(unsigned int bitsize) { 
  _config->getBenchmarkSet().back()->fieldStructure.push_back(bitsize);
}

/*** Handle rules and rule atoms. */
void LuaConfigurator::addRule() { 
  std::unique_ptr<Rule> rule(new Rule);
  _config->getBenchmarkSet().back()->rules.push_back(std::move(rule));
}

int LuaConfigurator::_getTopRuleIndex() const {
  return _config->getBenchmarkSet().back()->rules.size() - 1;
}

size_t LuaConfigurator::_getCurrentAtomWidth() const {
  // lookup width of current rule atom
  int topRuleAtomIdx = _config->getBenchmarkSet().back()->rules[_getTopRuleIndex()]->size();
  return _config->getBenchmarkSet().back()->fieldStructure[topRuleAtomIdx];
}

void LuaConfigurator::addRuleAtomExact(unsigned int value) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomExact> atom(new RuleAtomExact(value, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

void LuaConfigurator::addRuleAtomExact(std::string& value) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomExact> atom(new RuleAtomExact(value, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

void LuaConfigurator::addRuleAtomRange(unsigned int min, unsigned int max) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomRange> atom(new RuleAtomRange(min, max, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

void LuaConfigurator::addRuleAtomRange(std::string& min, std::string& max) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomRange> atom(new RuleAtomRange(min, max, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

void LuaConfigurator::addRuleAtomPrefix(unsigned int prefix, unsigned int mask) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomPrefix> atom(new RuleAtomPrefix(prefix, mask, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

void LuaConfigurator::addRuleAtomPrefix(std::string& prefix, std::string& mask) {
  int topRuleIdx = _getTopRuleIndex();
  size_t width = _getCurrentAtomWidth();

  std::unique_ptr<RuleAtomPrefix> atom(new RuleAtomPrefix(prefix, mask, width));
  _config->getBenchmarkSet().back()->rules[topRuleIdx]->push_back(std::move(atom));
}

/*** Handle headers and random header generation. */
void LuaConfigurator::addHeader() { 
  std::unique_ptr<PacketHeaderLine> line(new PacketHeaderLine);
  _config->getBenchmarkSet().back()->headers.push_back(std::move(line));
}

void LuaConfigurator::addHeaderValue(unsigned int value) { 
  std::unique_ptr<PacketHeaderAtom> headerAtom(new PacketHeaderAtom(value));
  _config->getBenchmarkSet().back()->headers.back()->push_back(std::move(headerAtom));
}

void LuaConfigurator::addHeaderValue(std::string& value) { 
  std::unique_ptr<PacketHeaderAtom> headerAtom(new PacketHeaderAtom(value));

  _config->getBenchmarkSet().back()->headers.back()->push_back(std::move(headerAtom));
}

void LuaConfigurator::setRandomHeaderAmount(unsigned int amount) {
  if (amount > 0)
    _config->getBenchmarkSet().back()->generateHeaders = true;

  _config->getBenchmarkSet().back()->rndHeaderConfig.totalHeaders = amount;
}

void LuaConfigurator::setRandomHeaderOutput(bool outputToFile) {
  _config->getBenchmarkSet().back()->rndHeaderConfig.outputToFile = outputToFile;
}

void LuaConfigurator::_pushBackRandomDistribution(unsigned int seed, std::unique_ptr<RandomDistribution> dist) {
  _config->getBenchmarkSet().back()->rndHeaderConfig.distributions.push_back(std::move(dist));

  _config->getBenchmarkSet().back()->rndHeaderConfig.seeds.push_back(seed);
}

void LuaConfigurator::addDistributionConstant(unsigned int value) {
  VarValue val(value);
  std::unique_ptr<RandomDistConstant> dist(new RandomDistConstant(val));
  _pushBackRandomDistribution(0, std::move(dist));
}

void LuaConfigurator::addDistributionConstant(std::string& value) {
  VarValue val(value);
  std::unique_ptr<RandomDistConstant> dist(new RandomDistConstant(val));
  _pushBackRandomDistribution(0, std::move(dist));
}

void LuaConfigurator::addDistributionUniform(unsigned int seed, unsigned int min, unsigned int max) {
  VarValue minval(min);
  VarValue maxval(max);
  std::unique_ptr<RandomDistUniform> dist(new RandomDistUniform(minval, maxval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionUniform(unsigned int seed, std::string& min, std::string& max) {
  VarValue minval(min);
  VarValue maxval(max);
  std::unique_ptr<RandomDistUniform> dist(new RandomDistUniform(minval, maxval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionNormal(unsigned int seed, unsigned int mean, unsigned int stddev) {
  VarValue meanval(mean);
  VarValue stddevval(stddev);
  std::unique_ptr<RandomDistNormal> dist(new RandomDistNormal(meanval, stddevval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionNormal(unsigned int seed, std::string& mean, std::string& stddev) {
  VarValue meanval(mean);
  VarValue stddevval(stddev);
  std::unique_ptr<RandomDistNormal> dist(new RandomDistNormal(meanval, stddevval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionLognormal(unsigned int seed, unsigned int m, unsigned int s) {
  VarValue mval(m);
  VarValue sval(s);
  std::unique_ptr<RandomDistLognormal> dist(new RandomDistLognormal(mval, sval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionLognormal(unsigned int seed, std::string& m, std::string& s) {
  VarValue mval(m);
  VarValue sval(s);
  std::unique_ptr<RandomDistLognormal> dist(new RandomDistLognormal(mval, sval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionExponential(unsigned int seed, double lambda) {
  std::unique_ptr<RandomDistExponential> dist(new RandomDistExponential(lambda));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionCauchy(unsigned int seed, unsigned int a, double b) {
  VarValue aval(a);
  std::unique_ptr<RandomDistCauchy> dist(new RandomDistCauchy(aval, b));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionCauchy(unsigned int seed, std::string& a, double b) {
  VarValue aval(a);
  std::unique_ptr<RandomDistCauchy> dist(new RandomDistCauchy(aval, b));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionPareto(unsigned int seed, double scale, double shape, unsigned int offset) {
  VarValue offsval(offset);
  std::unique_ptr<RandomDistPareto> dist(new RandomDistPareto(scale, shape, offsval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

void LuaConfigurator::addDistributionPareto(unsigned int seed, double scale, double shape, std::string& offset) {
  VarValue offsval(offset);
  std::unique_ptr<RandomDistPareto> dist(new RandomDistPareto(scale, shape, offsval));
  _pushBackRandomDistribution(seed, std::move(dist));
}

/*** Handle amount of runs and path string. */
void LuaConfigurator::setAmountRuns(unsigned int amount) {
  _config->getBenchmarkSet().back()->amountRuns = amount;
}

void LuaConfigurator::makeFullRelativePath(const std::string& postfix, std::string& result) {
  result = _config->getProgRelativePath() + postfix;
}

