#include <evaluation/Statistics.hpp>

Histogram::Histogram(unsigned int numberBins) : _bins() {
  for (unsigned int id = 0; id < numberBins; ++id) {
    std::unique_ptr<HistogramBin> ptr(new HistogramBin(id));
    _bins.push_back(std::move(ptr));
  }
}

void Histogram::occured(unsigned int id) {
  if (id >= _bins.size()) throw "Histogram: Occurence id is outside bin-bounds.";

  ++(*_bins[id]);
}
  
void Histogram::getSorted(std::vector<HistogramPair>& result) const {
  // create a copy of current bins
  std::vector<std::unique_ptr<HistogramBin>> copy;
  for (auto iter(_bins.cbegin()); iter != _bins.cend(); ++iter) {
    std::unique_ptr<HistogramBin> ptr(new HistogramBin(**iter));
    copy.push_back(std::move(ptr));
  }

  // sort bins by counter
  std::sort(copy.begin(), copy.end(), 
    [] (const std::unique_ptr<HistogramBin>& lhs, const std::unique_ptr<HistogramBin>& rhs) { return *lhs < *rhs; });

  // output in reverse order (highest counters first)
  std::for_each(copy.crbegin(), copy.crend(), 
    [&result] (const std::unique_ptr<HistogramBin>& bin) { 
      result.push_back( bin->getPair() ); 
    }
  );
}

void Histogram::getUnsorted(std::vector<HistogramPair>& result) const {
  // create a copy of current bins
  std::vector<std::unique_ptr<HistogramBin>> copy;
  for (auto iter(_bins.cbegin()); iter != _bins.cend(); ++iter) {
    std::unique_ptr<HistogramBin> ptr(new HistogramBin(**iter));
    copy.push_back(std::move(ptr));
  }

  // output in normal order (lowest ids first)
  std::for_each(copy.cbegin(), copy.cend(), 
    [&result] (const std::unique_ptr<HistogramBin>& bin) { 
      result.push_back( bin->getPair() ); 
    }
  );
}

