#include <data/Configuration.hpp>
#include <chrono>

void Configuration::setBenchmarkIds() {
  using namespace std::chrono;

  // get current timestamp in ms
  unsigned int timestamp = time_point_cast<milliseconds>( system_clock::now() ).time_since_epoch().count(); 

  // assign to each benchmark the time and its index in the collection
  unsigned int index = 0;
  std::string prefix(std::to_string(timestamp));
  for (BenchmarkSet::iterator iter(_benchmarks.begin()); iter != _benchmarks.end(); ++iter) {
    (*iter)->id = prefix + "-" + std::to_string(index++);
  }
}

