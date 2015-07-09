#ifndef CHRONOGRAPH_INCLUDED
#define CHRONOGRAPH_INCLUDED

#include <chrono>
#include <memory>
#include <generics/VarValue.hpp>

typedef std::chrono::high_resolution_clock Chronoclock;
/** 
 * Represents a single stopwatch for measuring the performance of a 
 * specified aspect of an algorithm's benchmark run.
 */
class Chronograph {
  /** holds the time-point of stopwatch-start */
  Chronoclock::time_point _start;

  /** holds result time of the stopwatch in nanoseconds (multi-precision type) */
  Generic::VarValue _total;

public:
  Chronograph() : _total(0) {}
  /** Start the stopwatch */
  void start() { _start = Chronoclock::now(); }
  /** Stop the stopwatch and sum previous timespanup in total duration. */
  void stop();

  /** Return total measured time in seconds. */
  inline unsigned int getTotalSec() { return (unsigned int)(getTotalMillisec() / 1000.0); }
  /** Return total measured time in milliseconds. */
  inline unsigned int getTotalMillisec() { return (unsigned int)(getTotalMicrosec() / 1000.0); }
  /** Return total measured time in microseconds. */
  inline unsigned int getTotalMicrosec() { return (unsigned int)(getTotalNanosec() / 1000.0); }
  /** Return total measured time in nanoseconds. */ 
  inline double getTotalNanosec() { return _total.get_d(); }
};

typedef std::unique_ptr<Chronograph> ChronographPtr;

#endif
