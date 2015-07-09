#ifndef HICUTS_10TPL_INCLUDED
#define HICUTS_10TPL_INCLUDED

#include <generics/Base.hpp>
#include <algorithms/common/Converter10tpl.hpp>
#include <algorithms/hicuts/DataHiCuts10tpl.hpp>
#include <metering/time/ChronoManager.hpp>
#include <memory>
#include <cstdint>

class HiCuts10tpl: public Base {
  /** inner representation of the rule set (copy for updates) */
  DataHiCuts10tpl::RulesVector _rules;

  /** Contains the rules with hierarchical cuttings. */
  DataHiCuts10tpl::Trie _searchTrie;
  unsigned int _binth;
  double _spfac;

  /** Specifies, after how many headers a new memory usage checkpoint is set. */
  uint32_t _settingHeadersPerCheckpoint;

  /** Counts the current amount of processed headers after the last checkpoint. */
  uint32_t _cntHeadersAfterCheckpoint;

  /** Converts a rule from the generic representation. */
  void convertRule(const Generic::Rule& rule, unsigned int index, DataHiCuts10tpl::RulesVector::iterator pos);

  /**
   * Is called each time a packet header was processed in order to determine, if a new 
   * memory-manager checkpoint should be set.
   */
  void headerProcessed();

public:
  HiCuts10tpl() : _rules(), _searchTrie(), _binth(2), _spfac(1.0), _settingHeadersPerCheckpoint(1), _cntHeadersAfterCheckpoint(0) {}
  inline ~HiCuts10tpl() override {}
  
  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif

