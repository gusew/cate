#ifndef BITVECTOR_10TPL_INCLUDED
#define BITVECTOR_10TPL_INCLUDED

#include <generics/Base.hpp>
#include <algorithms/common/Converter10tpl.hpp>
#include <algorithms/bitvector/DataBitvector.hpp>
#include <metering/time/ChronoManager.hpp>
#include <memory>
#include <cstdint>
#include <vector>

// Some typedefs for beauty
typedef std::unique_ptr<DataBitvector::DimensionRanges<uint32_t>> DimRange32Ptr;

/** Shortcut for type of each stored representation of a rule. */
typedef DataBitvector::Rule10tpl<uint32_t> RuleStorageItem;
/** Type of the storage vector for rule-representations. */
typedef std::vector<std::unique_ptr<RuleStorageItem>> StorageVector;


class Bitvector10tpl : public Base {
  /** inner representation of the rule set (copy for updates) */
  StorageVector _rules;
  
  /** Projections of ranges in each dimension with bitvectors. */
  DimRange32Ptr _dim1;
  DimRange32Ptr _dim2;
  DimRange32Ptr _dim3;
  DimRange32Ptr _dim4;
  DimRange32Ptr _dim5;
  DimRange32Ptr _dim6;
  DimRange32Ptr _dim7;
  DimRange32Ptr _dim8;
  DimRange32Ptr _dim9;
  DimRange32Ptr _dim10;

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
  Bitvector10tpl() : _rules(), 
    _dim1(), 
    _dim2(), 
    _dim3(), 
    _dim4(), 
    _dim5(), 
    _dim6(), 
    _dim7(), 
    _dim8(), 
    _dim9(), 
    _dim10(), 
    _settingHeadersPerCheckpoint(1), _cntHeadersAfterCheckpoint(0) {}
  inline ~Bitvector10tpl() override {}
  
  void setParameters(const std::vector<double>& params) override;

	void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

  void setRules(const Generic::RuleSet& ruleset) override;

	void ruleAdded(uint32_t index, const Generic::Rule& rule) override;
  
  void ruleRemoved(uint32_t index) override;

  void reset() override;
};

#endif

