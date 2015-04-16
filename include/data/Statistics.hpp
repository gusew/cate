#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED

#include <vector>
#include <numeric> // std::accumulate
#include <algorithm> // std::min_element, ...
#include <cmath> // sqrt
#include <memory>
#include <utility> // std::pair

/** 
 * Contains elements of a typical measurement data series. 
 * Performs statistical evaluations on this data.
 */
template <typename T>
struct Series {
  std::vector<T> data;

  Series() : data() {}
  ~Series() {}
 
  T calcMinValue() const {
    auto res = std::min_element(data.cbegin(), data.cend());
    if (res == data.cend())
      throw "Error in calcMinValue(): Series does not contain any data!";
    else 
      return *res;
  }

  T calcMaxValue() const {
    auto res = std::max_element(data.cbegin(), data.cend());
    if (res == data.cend())
      throw "Error in calcMaxValue(): Series does not contain any data!";
    else 
      return *res;
  }

  double calcMeanValue() const {
    if (data.size() <= 0) throw "Error in calcMeanValue(): Series does not contain any data!";
    return calcSum() / data.size();
  }

  T calcMedianValue() const {
    if (data.size() <= 0) throw "Error in calcMedianValue(): Series does not contain any data!";
    if (data.size() == 1) return data[0];

    // create a copy for sorting
    std::vector<T> copy(data);
    std::sort(copy.begin(), copy.end());
    return copy[copy.size() / 2 - 1];
  }

  double calcStdDev() const {
    if (data.size() <= 0) throw "Error in calcStdDev(): Series does not contain any data!";
    if (data.size() == 1) return 0;

    T mean = calcMeanValue();
    double sum = 0;
    // calculate sum(1,n) of (val_i - mean)^2
    std::for_each(data.cbegin(), data.cend(), [&sum, mean] (const T& val) { sum += (val-mean) * (val-mean); });
    
    return sqrt(sum / (data.size() - 1) );
  }

  double calcSum() const {
    return std::accumulate(data.cbegin(), data.cend(), 0.0);
  }
};

/** Container with all (except sum) statistic data of one series. */
template <typename T>
struct StatValues {
  T minimum;
  T maximum;
  double mean;
  T median;
  double stddev;
  
  StatValues() {}
  StatValues(const Series<T>& series) : minimum(series.calcMinValue()), maximum(series.calcMaxValue()), mean(series.calcMeanValue()), median(series.calcMedianValue()), stddev(series.calcStdDev()) {}
};

/** Container with mean and stddev statistic data of one series. */
struct MeanValue {
  double mean;
  double stddev;
  
  MeanValue() {}
  template <typename T>
  MeanValue(const Series<T>& series) : mean(series.calcMeanValue()), stddev(series.calcStdDev()) {}
};


/** Type for the user of a Histogram to get results. */
typedef std::pair<unsigned int, unsigned int> HistogramPair;

/** Represents one histogram bin-object. */
struct HistogramBin {
  /** ID of the histogram bin (e.g. index of an element) */
  unsigned int id;
  /** Counts the occurence of the elements in this bin for the histogram. */
  unsigned int counter;

  HistogramBin(unsigned int idVal) : id(idVal), counter(0) {}
  HistogramBin(const HistogramBin& other) : id(other.id), counter(other.counter) {}
  ~HistogramBin() {}

  inline bool operator<(const HistogramBin& rhs) const {
    return (counter < rhs.counter || (counter == rhs.counter && id > rhs.id) );
  }
  // no common sematic here: always equal, if same IDs
  inline bool operator==(const HistogramBin& rhs) const {
    return (id == rhs.id);
  }

  // ++HistogramBin
  inline HistogramBin& operator++() { ++counter; return *this; }
  // HistogramBin++
  inline HistogramBin operator++(int) { HistogramBin res(*this); ++counter; return res; }

  /** For returning results to the user. */
  inline HistogramPair getPair() { return std::make_pair(id, counter); }
};

/** A helper for the construction of histograms. */
class Histogram {
  /** One entry for each histogram-bin */
  std::vector<std::unique_ptr<HistogramBin>> _bins;

public:
  Histogram(unsigned int amountBins);
  ~Histogram() {}

  /** Increment occurence counter for specified bin-id. */
  void occured(unsigned int id);

  /** Return a container with sorted histogram results. */
  void getSorted(std::vector<HistogramPair>& result) const;

  /** Return a container with histogram results (unsorted by id). */
  void getUnsorted(std::vector<HistogramPair>& result) const;
};

#endif

