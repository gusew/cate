#include <algorithms/Bitvector2tpl.hpp>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new Bitvector2tpl;
}

void Bitvector2tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Bitvector (2tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // no further parameters in use
}

void Bitvector2tpl::setRules(const Generic::RuleSet& ruleset) {
  using namespace DataBitvector;

  _rules.clear(); // empty rule storage

  _chronomgr->start("convert classifier");

  // save each range in rule storage (for updates)
	for (unsigned int ruleItr = 0; ruleItr < ruleset.size(); ++ruleItr) {
    convertRuleAndStore(*(ruleset[ruleItr].get()), _rules.end());
  }

  // now construct search data structure
  constructProjections();

  _chronomgr->stop("convert classifier");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void Bitvector2tpl::constructProjections() {
  using namespace DataBitvector;

  unsigned int rulesSize = _rules.size();

  // create dimensions
  _dimIpSrc.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dimIpDest.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));

  // iterate over rules and construct lookup-structure
  unsigned int ruleItr = 0;
  _chronomgr->start("convert classifier/addRange");
	for (StorageVector::const_iterator iter(_rules.cbegin()); iter != _rules.cend(); ++iter) {
    // get ranges of each rule atom
    _dimIpSrc->addRange((*iter)->check1.min, (*iter)->check1.max, ruleItr); 
    _dimIpDest->addRange((*iter)->check2.min, (*iter)->check2.max, ruleItr); 

    ++ruleItr;
  }
  _chronomgr->stop("convert classifier/addRange");
}

void Bitvector2tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
  using namespace DataBitvector;

	unsigned int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  if (!_dimIpSrc || !_dimIpDest)
    throw "Bitvector2tpl: One of the lookup structures is not set.";

  Data2tpl::HeaderTuple tpl(0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set

  Bitvector bv0(_rules.size());
  Bitvector bv1(_rules.size());

  Range<uint32_t> searchKey32(0, 0);

	for(Generic::PacketHeaderSet::const_iterator lineItr(data.cbegin()); lineItr != data.cend(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter2tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");

    searchKey32.min = tpl.addrSrc;
    searchKey32.max = tpl.addrSrc;
    bv0 = _dimIpSrc->search(searchKey32);
    
    searchKey32.min = tpl.addrDest;
    searchKey32.max = tpl.addrDest;
    bv1 = _dimIpDest->search(searchKey32);

    bv0 &= bv1;

    matchIndex = bv0.getFirstSetBit();
    _chronomgr->stop("classify");

    indices.push_back(matchIndex);

    headerProcessed(); // set recurrent checkpoints
	}
}

void Bitvector2tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void Bitvector2tpl::convertRuleAndStore(const Generic::Rule& rule, StorageVector::iterator pos) {
  if (rule.size() < 2)
    throw "Bitvector2tpl: Not enough rule checks for two-tuple algorithms in given rule found.";

  // iterate over each rule atom and get ranges
  Generic::VarValue minConv, maxConv;
  uint32_t min1=0, max1=0, min2=0, max2=0;

  for (unsigned int atomItr = 0; atomItr < 2; ++atomItr) {
    // special case: wildcard prefix
    if (rule[atomItr]->isWildcard()) {
      minConv = 0;
      if (atomItr < 2) maxConv = 0xFFFFFFFF;
      else if (atomItr < 4) maxConv = 0xFFFF;
      else maxConv = 0xFF;
    } else { // usual case: not a wildcard prefix
      rule[atomItr]->toRange(minConv, maxConv);
    }

    switch (atomItr) { // store ranges in each dimension
      case 0:
        min1 = minConv.get_ui();
        max1 = maxConv.get_ui(); 
        break;

      case 1:
        min2 = minConv.get_ui();
        max2 = maxConv.get_ui();
        break;

      default: throw "Bitvector2tpl: Out of bounds in setRules!";
    }
  }

  // create local rule-representation and add to storage
  std::unique_ptr<RuleStorageItem> convRule(new RuleStorageItem(min1, max1, min2, max2));
  _rules.insert(pos, std::move(convRule));
}

void Bitvector2tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  if (index > _rules.size())
    throw "Bitvector2tpl: Added a new rule at a position out of bounds of the existing classifier.";
  
  _chronomgr->start("add rule");
  convertRuleAndStore(rule, _rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("add rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}
  
void Bitvector2tpl::ruleRemoved(uint32_t index) {
  if (index >= _rules.size())
    throw "Bitvector2tpl: Index for rule to remove is outside bounds of the existing classifier.";
  
  _chronomgr->start("remove rule");
  _rules.erase(_rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("remove rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void Bitvector2tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _rules.clear();
  _dimIpSrc.reset();
  _dimIpDest.reset();
}
