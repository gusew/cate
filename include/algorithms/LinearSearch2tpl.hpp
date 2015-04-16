#ifndef LINEARSEARCH_2TPL_INCLUDED
#define LINEARSEARCH_2TPL_INCLUDED

#include <algorithms/Base.hpp>
#include <algorithms/Converter2tpl.hpp>
#include <core/ChronoManager.hpp>
#include <memory>

class LinearSearch2tpl : public Base {
  /** inner representation of the rule set */
  std::unique_ptr<Data2tpl::RuleSet> _rules;

  /** Specifies, after how many headers a new memory usage checkpoint is set. */
  uint32_t _settingHeadersPerCheckpoint;

  /** Counts the current amount of processed headers after the last checkpoint. */
  uint32_t _cntHeadersAfterCheckpoint;

  /**
   * Is called each time a packet header was processed in order to determine, if a new 
   * memory-manager checkpoint should be set.
   */
  void headerProcessed();

public:
	LinearSearch2tpl() : _settingHeadersPerCheckpoint(1), _cntHeadersAfterCheckpoint(0) { }
	inline ~LinearSearch2tpl() override {}

  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif
