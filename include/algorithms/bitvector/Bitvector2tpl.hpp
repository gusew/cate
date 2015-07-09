#ifndef BITVECTOR_2TPL_INCLUDED
#define BITVECTOR_2TPL_INCLUDED

#include <generics/Base.hpp>
#include <algorithms/common/Converter2tpl.hpp>
#include <algorithms/bitvector/DataBitvector.hpp>
#include <core/ChronoManager.hpp>
#include <memory>
#include <cstdint>
#include <vector>

// Some typedefs for beauty
typedef std::unique_ptr<DataBitvector::DimensionRanges<uint32_t>> DimRange32Ptr;

/** Shortcut for type of each stored representation of a rule. */
typedef DataBitvector::Rule2tpl<uint32_t, uint32_t> RuleStorageItem;
/** Type of the storage vector for rule-representations. */
typedef std::vector<std::unique_ptr<RuleStorageItem>> StorageVector;


class Bitvector2tpl : public Base {
  /** inner representation of the rule set (copy for updates) */
  StorageVector _rules;
  
  /** Projections of ranges in each dimension with bitvectors. */
  DimRange32Ptr _dimIpSrc;
  DimRange32Ptr _dimIpDest;

  /** Specifies, after how many headers a new memory usage checkpoint is set. */
  uint32_t _settingHeadersPerCheckpoint;

  /** Counts the current amount of processed headers after the last checkpoint. */
  uint32_t _cntHeadersAfterCheckpoint;

  /** Converts a rule from the generic representation and stores it in the rule container at given position. */
  void convertRuleAndStore(const Generic::Rule& rule, StorageVector::iterator pos);

  /** Resets all projections of ranges in each dimension and rebuilds them. */
  void constructProjections();

  /**
   * Is called each time a packet header was processed in order to determine, if a new 
   * memory-manager checkpoint should be set.
   */
  void headerProcessed();

public:
  Bitvector2tpl() : _rules(), _dimIpSrc(), _dimIpDest(), _settingHeadersPerCheckpoint(1), _cntHeadersAfterCheckpoint(0) {}
  inline ~Bitvector2tpl() override {}
  
  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif

