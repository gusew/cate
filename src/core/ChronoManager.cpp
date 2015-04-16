#include <core/ChronoManager.hpp>

void ChronoManager::start(std::string key) {
  if (_chronos.count(key) == 0) {
    ChronographPtr chrono(new Chronograph);
    _chronos.insert(
      std::make_pair( key, std::move(chrono) )
    );
  }
  _chronos[key]->start();
}

void ChronoManager::stop(std::string key) {
  if (_chronos.count(key) > 0) _chronos[key]->stop();
  else throw "Stopwatch with the given key doesn't exist (ChronoManager::stop).";
}

unsigned int ChronoManager::getTimeSec() {
  unsigned int total = 0;
  for (auto iter(_chronos.begin()); iter != _chronos.end(); ++iter)
    total += iter->second->getTotalSec();
  return total;
}

unsigned int ChronoManager::getTimeMilli() {
  unsigned int total = 0;
  for (auto iter(_chronos.begin()); iter != _chronos.end(); ++iter)
    total += iter->second->getTotalMillisec();
  return total;
}

unsigned int ChronoManager::getTimeMicro() {
  unsigned int total = 0;
  for (auto iter(_chronos.begin()); iter != _chronos.end(); ++iter)
    total += iter->second->getTotalMicrosec();
  return total;
}

double ChronoManager::getTimeNano() {
  double total = 0;
  for (auto iter(_chronos.begin()); iter != _chronos.end(); ++iter)
    total += iter->second->getTotalNanosec();
  return total;
}

unsigned int ChronoManager::getTimeSec(std::string key) {
  if (_chronos.count(key) > 0) return _chronos[key]->getTotalSec();
  else throw "Stopwatch with the given key doesn't exist (ChronoManager::getTimeSec).";
}

unsigned int ChronoManager::getTimeMilli(std::string key) {
  if (_chronos.count(key) > 0) return _chronos[key]->getTotalMillisec();
  else throw "Stopwatch with the given key doesn't exist (ChronoManager::getTimeMilli).";
}

unsigned int ChronoManager::getTimeMicro(std::string key) {
  if (_chronos.count(key) > 0) return _chronos[key]->getTotalMicrosec();
  else throw "Stopwatch with the given key doesn't exist (ChronoManager::getTimeMicro).";
}

double ChronoManager::getTimeNano(std::string key) {
  if (_chronos.count(key) > 0) return _chronos[key]->getTotalNanosec();
  else throw "Stopwatch with the given key doesn't exist (ChronoManager::getTimeNano).";
}

void ChronoManager::getAllResults(ChronoResults& results, unsigned int divisor) {
  if (results.size() > 0) results.clear();
  if (divisor == 0) throw "The divisor can't be zero (ChronoManager::getAllResults).";

  unsigned int groupRes = 0;
  for (auto iter(_chronos.begin()); iter != _chronos.end(); ++iter) {
    groupRes = iter->second->getTotalMicrosec();
    if (divisor != 1) groupRes /= divisor;
    
    results.push_back(std::make_pair( iter->first, groupRes ));
  }
}

