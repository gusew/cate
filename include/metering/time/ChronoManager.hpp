#ifndef CHRONO_MANAGER_INCLUDED
#define CHRONO_MANAGER_INCLUDED

#include <unordered_map>
#include <string>
#include <vector>
#include <metering/time/Chronograph.hpp>

/** one single chronometer group-result */
typedef std::pair<std::string, unsigned int> ChronoResult; 
/** a collection of group results */
typedef std::vector<ChronoResult> ChronoResults; 

/**
 * Represents an interface to multiple stopwatches, which can be used in parallel.
 * Each stopwatch gets a string as the tag to use it. Measured timespans can be
 * fetched in different dimensions (seconds, milliseconds nad microseconds).
 */
class ChronoManager {
  /** holds all stopwatches in use */
  std::unordered_map<std::string, ChronographPtr> _chronos;

public:
  /**
   * Starts the stopwatch with given name.
   *
   * @param key name of the stopwatch to start as string.
   */
  void start(std::string key);

  /**
   * Stops the stopwatch with given name. Second immediate call of stop
   * (without a start-call in between) leads to a timespan between previous 
   * stop-call and current stop.
   *
   * @param key name of the stopwatch to stop as string.
   */
  void stop(std::string key);

  /** Returns the total timespan of all stopwatch-instances in seconds */
  unsigned int getTimeSec();
  /** Returns the total timespan of all stopwatch-instances in milliseconds */
  unsigned int getTimeMilli();
  /** Returns the total timespan of all stopwatch-instances in microseconds */
  unsigned int getTimeMicro();
  /** Returns the total timespan if all stopwatch-instances in nanoseconds */
  double getTimeNano();

  /** Returns the total timespan of a single stopwatch-instance in seconds */
  unsigned int getTimeSec(std::string key);
  /** Returns the total timespan of a single stopwatch-instance in milliseconds */
  unsigned int getTimeMilli(std::string key);
  /** Returns the total timespan of a single stopwatch-instance in microseconds */
  unsigned int getTimeMicro(std::string key);
  /** Returns the total timespan of a single stopwatch-instance in nanoseconds */
  double getTimeNano(std::string key);

  /**
   * Pack each result value of all groups in the given container and return them.
   * As dimension microseconds are chosen, but can be customized with the help of 
   * the divisor parameter.
   *
   * @param results container, where result entries have to be inserted
   * @param divisor an optional divisor by which all microsecond-values are divided
   */
  void getAllResults(ChronoResults& results, unsigned int divisor = 1);

  /** Change back to initial state, all recorded times are deleted. */
  void reset() { _chronos.clear(); }
};

#endif

