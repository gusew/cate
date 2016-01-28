#ifndef BASE_INCLUDED
#define BASE_INCLUDED

#include <cstdint>
#include <vector>
#include <generics/PacketHeader.hpp>
#include <generics/RuleSet.hpp>
#include <metering/memory/MemManager.hpp>
#include <metering/time/ChronoManager.hpp>
#include <metering/LogTagManager.hpp>

/**
 * Base abstract class for all implemented filtering algorithms.
 */
class Base {

protected:
  std::shared_ptr<Memory::MemManager> _mmanager;
  std::shared_ptr<ChronoManager> _chronomgr;
  std::shared_ptr<LogTagManager> _logger;

  /** Checks, if MemManager instance is assigned. */
  inline void checkMemMgr() { if (!_mmanager) throw "Set MemManager before using a filtering algorithm."; }
  /** Checks, if ChronoManager instance is assigned. */
  inline void checkChronoMgr() { if (!_chronomgr) throw "Set ChronoManager before using a filtering algorithm."; }
  /** Checks, if LogTagManager instance is assigned. */
  inline void checkLogTagMgr() { if (!_logger) throw "Set LogTagManager before using a filtering algorithm."; }
  
public:
	virtual ~Base() {};

	/**
	 * Classify incoming packet header data by finding the best matching rule
	 * in the given rule set with a specific algorithm.
	 *
	 * @param data container with packet header data
	 * @param indices container with indices of matched rules in rule set
	 */
	virtual void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) = 0;

  /**
   * Set rules for algorithm and let it convert them to a native implementation, 
   * if needed. 
   *
   * @param ruleset reference to a generic ruleset structure
   */
  virtual void setRules(const Generic::RuleSet& ruleset) = 0;

  /**
   * Add a new rule to the rule set at a specified index.
   *
   * @param index position, at which the new rule has to be added
   * @param rule a reference to a rule instance to add
   */
	virtual void ruleAdded(uint32_t index, const Generic::Rule& rule) = 0;
	
  /**
   * Remove an already existing rule from the rule set at a given index.
   *
   * @param index position, from where an existing rule has to be removed
   */
  virtual void ruleRemoved(uint32_t index) = 0;

  /**
   * Set algorithm parameters as specified by a benchmark configuration.
   *
   * @param params collection with algorithm-specific parameters
   */
  virtual void setParameters(const std::vector<double>& params) = 0;

  /**
   * Reset algorithm instance to initial state.
   */
  virtual void reset() = 0;

  /**
   * Set MemManager instance for tracing memory usage of the algorithm.
   *
   * @param m smart-pointer that points to a created MemManager instance.
   */
  inline void setMemManager(std::shared_ptr<Memory::MemManager> m) { _mmanager = m; checkMemMgr(); }

  /**
   * Set ChronoManager instance for tracing timespans for performance metering.
   *
   * @param c smart-pointer that points to a created ChronoManager instance.
   */
  inline void setChronoManager(std::shared_ptr<ChronoManager> c) { _chronomgr = c; checkChronoMgr(); }

  /**
   * Set LogTagManager instance for storing custom log messages for debug or benchmarking.
   *
   * @param l smart-pointer that points to a created LogTagManager instance.
   */
  inline void setLogTagManager(std::shared_ptr<LogTagManager> l) { _logger = l; checkLogTagMgr(); }
};

#endif
