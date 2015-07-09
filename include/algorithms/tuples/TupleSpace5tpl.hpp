#ifndef TUPLESPACE_5TPL_INCLUDED
#define TUPLESPACE_5TPL_INCLUDED

#include <generics/Base.hpp>
#include <algorithms/common/Converter5tpl.hpp>
#include <generics/VarValue.hpp>
#include <algorithms/tuples/DataTupleSpace5tpl.hpp>
#include <core/ChronoManager.hpp>
#include <memory>
#include <tuple>

class TupleSpace5tpl : public Base {
  /** holds all hash maps for each tuple structure */
  std::vector<DataTupleSpace5tpl::Map5ptr> _maps;

  /** Contains all rules, which need to be expanded. */
  std::vector<std::tuple<uint32_t, std::unique_ptr<Data5tpl::Rule>>> _expandRules;
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
  void placeRuleInMap(DataTupleSpace5tpl::Rule5ptr rule);

  /** Convert a generic rule to a prefix rule. */
  void convertRuleToMany(const Generic::Rule& varRule, std::vector<DataTupleSpace5tpl::Rule5ptr>& specRules, const size_t index) const;

  /** For sorting the maps according to minimum index. */
  static bool _mapSortFunction(const DataTupleSpace5tpl::Map5ptr& map1, const DataTupleSpace5tpl::Map5ptr& map2);

public:
	TupleSpace5tpl() : _maps(), 
    _expandRules(), 
    _expandMinIdx(Generic::noRuleIsMatching()),
    _settingHeadersPerCheckpoint(1), 
    _cntHeadersAfterCheckpoint(0) 
    {}

	inline ~TupleSpace5tpl() override {}

  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif
