#include <algorithms/Bitvector4tpl.hpp>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new Bitvector4tpl;
}

void Bitvector4tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Bitvector (4tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // no further parameters in use
}

void Bitvector4tpl::setRules(const Generic::RuleSet& ruleset) {
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

void Bitvector4tpl::constructProjections() {
  using namespace DataBitvector;

  unsigned int rulesSize = _rules.size();

  // create dimensions
  _dim1.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim2.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim3.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim4.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));

  // iterate over rules and construct lookup-structure
  unsigned int ruleItr = 0;
  _chronomgr->start("convert classifier/addRange");
	for (StorageVector::const_iterator iter(_rules.cbegin()); iter != _rules.cend(); ++iter) {
    // get ranges of each rule atom
    _dim1->addRange((*iter)->check1.min, (*iter)->check1.max, ruleItr); 
    _dim2->addRange((*iter)->check2.min, (*iter)->check2.max, ruleItr); 
    _dim3->addRange((*iter)->check3.min, (*iter)->check3.max, ruleItr); 
    _dim4->addRange((*iter)->check4.min, (*iter)->check4.max, ruleItr); 

    ++ruleItr;
  }
  _chronomgr->stop("convert classifier/addRange");
}

void Bitvector4tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
  using namespace DataBitvector;

	unsigned int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  if (!_dim1 || !_dim1 || !_dim3 || !_dim4)
    throw "Bitvector4tpl: One of the lookup structures is not set.";

  Data4tpl::HeaderTuple tpl(0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set

  Bitvector bv0(_rules.size());
  Bitvector bv1(_rules.size());

  Range<uint32_t> searchKey32(0, 0);

	for(Generic::PacketHeaderSet::const_iterator lineItr(data.cbegin()); lineItr != data.cend(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter4tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");

    searchKey32.min = tpl.v1;
    searchKey32.max = tpl.v1;
    bv0 = _dim1->search(searchKey32);
    
    searchKey32.min = tpl.v2;
    searchKey32.max = tpl.v2;
    bv1 = _dim2->search(searchKey32);

    bv0 &= bv1;

    searchKey32.min = tpl.v3;
    searchKey32.max = tpl.v3;
    bv1 = _dim3->search(searchKey32);

    bv0 &= bv1;

    searchKey32.min = tpl.v4;
    searchKey32.max = tpl.v4;
    bv1 = _dim4->search(searchKey32);

    bv0 &= bv1;

    matchIndex = bv0.getFirstSetBit();
    _chronomgr->stop("classify");

    indices.push_back(matchIndex);

    headerProcessed(); // set recurrent checkpoints
	}
}

void Bitvector4tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void Bitvector4tpl::convertRuleAndStore(const Generic::Rule& rule, StorageVector::iterator pos) {
  if (rule.size() < 4)
    throw "Bitvector4tpl: Not enough rule checks for 4-tuple algorithms in given rule found.";

  // iterate over each rule atom and get ranges
  Generic::VarValue minConv, maxConv;
  uint32_t min1=0, max1=0, 
    min2=0, max2=0,
    min3=0, max3=0,
    min4=0, max4=0;

  for (unsigned int atomItr = 0; atomItr < 4; ++atomItr) {
    // special case: wildcard prefix
    if (rule[atomItr]->isWildcard()) {
      minConv = 0;
      maxConv = 0xFFFFFFFF;
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

      case 2:
        min3 = minConv.get_ui();
        max3 = maxConv.get_ui();
        break;

      case 3:
        min4 = minConv.get_ui();
        max4 = maxConv.get_ui();
        break;

      default: throw "Bitvector4tpl: Out of bounds in setRules!";
    }
  }

  // create local rule-representation and add to storage
  std::unique_ptr<RuleStorageItem> convRule(new RuleStorageItem(min1, max1, 
    min2, max2,
    min3, max3,
    min4, max4));
  _rules.insert(pos, std::move(convRule));
}

void Bitvector4tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  if (index > _rules.size())
    throw "Bitvector4tpl: Added a new rule at a position out of bounds of the existing classifier.";
  
  _chronomgr->start("add rule");
  convertRuleAndStore(rule, _rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("add rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}
  
void Bitvector4tpl::ruleRemoved(uint32_t index) {
  if (index >= _rules.size())
    throw "Bitvector4tpl: Index for rule to remove is outside bounds of the existing classifier.";
  
  _chronomgr->start("remove rule");
  _rules.erase(_rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("remove rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void Bitvector4tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _rules.clear();
  _dim1.reset();
  _dim2.reset();
  _dim3.reset();
  _dim4.reset();
}
