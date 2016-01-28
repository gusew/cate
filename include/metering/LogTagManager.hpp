#ifndef LOGTAGMANAGER_HPP
#define LOGTAGMANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include <chrono>

/** We use a simple system clock for the timestamps of a log tag. */
typedef std::chrono::system_clock Logclock;

/** Represents a log tag entry with timestamp as key and string as value. */
struct LogTag {
  /** timestamp of log entry */
  Logclock::time_point time;

  /** arbitrary text of log entry */
  std::string tag;

  void getTimeStr(std::string& result) const;
};

/** Smart pointer to LogTag instance */
typedef std::unique_ptr<LogTag> LogTagPtr;

/** Container type for log tags */
typedef std::vector<LogTagPtr> LogTagPtrVector; 

/**
 * Represents a manager class for storing of custom string-tags which
 * an algorithm programmer can use for logging purposes during a 
 * benchmark run.
 */
class LogTagManager {
  /** stores all log entries (implicitly sorted by log-time) */
  LogTagPtrVector _tags;

public:
  /**
   * Create a new tag for the log and assign the current timestamp to it.
   *
   * @param t arbitrary text description of this tag
   */
  void tag(const std::string& t);

  /**
   * Returns all stored log tags to the caller
   *
   * @return const reference to container with all available log tags
   */
  inline const LogTagPtrVector& getTags() const { return _tags; }

  /** Change back to initial state, all stored tags are deleted. */
  inline void reset() { _tags.clear(); }
};

#endif /* LOGTAGMANAGER_HPP */

