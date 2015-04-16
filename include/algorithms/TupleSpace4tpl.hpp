#ifndef TUPLESPACE_4tpl_INCLUDED
#define TUPLESPACE_4tpl_INCLUDED

#include <algorithms/Base.hpp>
#include <algorithms/Converter4tpl.hpp>
#include <data/VarValue.hpp>
#include <algorithms/DataTupleSpace4tpl.hpp>
#include <core/ChronoManager.hpp>
#include <memory>
#include <tuple>

class TupleSpace4tpl : public Base {
  /** holds all hash maps for each tuple structure */
  std::vector<DataTupleSpace4tpl::Map4ptr> _maps;

  /** Contains all rules, which need to be expanded. */
  std::vector<std::tuple<uint32_t, std::unique_ptr<Data4tpl::Rule>>> _expandRules;
  /** Stores the minimum index in expanded rules container. */
  Memory::MemTrace<unsigned long int> _expandMinIdx;

  /** Specifies bucket size of hash-map. */
  uint32_t _settingCapacity;

  /** Specifies, after how many headers a new memory usage checkpoint is set. */
  uint32_t _settingHeadersPerCheckpoint;

  /** Counts the current amount of processed headers after the last checkpoint. */
  uint32_t _cntHeadersAfterCheckpoint;

  /**
   * Is called each time a packet header was processed in order to determine, if a new 
   * memory-manager checkpoint should be set.
   */
  void headerProcessed();
  
  /** A converted prefix rule will be placed in a hashmap. */
  void placeRuleInMap(DataTupleSpace4tpl::Rule4ptr rule);

  /** Convert a generic rule to a prefix rule. */
  void convertRuleToMany(const Generic::Rule& varRule, std::vector<DataTupleSpace4tpl::Rule4ptr>& specRules, const size_t index) const;

  /** For sorting the maps according to minimum index. */
  static bool _mapSortFunction(const DataTupleSpace4tpl::Map4ptr& map1, const DataTupleSpace4tpl::Map4ptr& map2);

public:
	TupleSpace4tpl() : _maps(), 
    _expandRules(), 
    _expandMinIdx(Generic::noRuleIsMatching()),
    _settingHeadersPerCheckpoint(1), 
    _cntHeadersAfterCheckpoint(0) 
    {}

	inline ~TupleSpace4tpl() override {}

  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif
