#include <algorithms/bitvector/Bitvector10tpl.hpp>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new Bitvector10tpl;
}

void Bitvector10tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Bitvector (10tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // no further parameters in use
}

void Bitvector10tpl::setRules(const Generic::RuleSet& ruleset) {
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

void Bitvector10tpl::constructProjections() {
  using namespace DataBitvector;

  unsigned int rulesSize = _rules.size();

  // create dimensions
  _dim1.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim2.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim3.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim4.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim5.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim6.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim7.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim8.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim9.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));
  _dim10.reset(new DimensionRanges<uint32_t>(rulesSize, 0, 0xFFFFFFFF));

  // iterate over rules and construct lookup-structure
  unsigned int ruleItr = 0;
  _chronomgr->start("convert classifier/addRange");
	for (StorageVector::const_iterator iter(_rules.cbegin()); iter != _rules.cend(); ++iter) {
    // get ranges of each rule atom
    _dim1->addRange((*iter)->check1.min, (*iter)->check1.max, ruleItr); 
    _dim2->addRange((*iter)->check2.min, (*iter)->check2.max, ruleItr); 
    _dim3->addRange((*iter)->check3.min, (*iter)->check3.max, ruleItr); 
    _dim4->addRange((*iter)->check4.min, (*iter)->check4.max, ruleItr); 
    _dim5->addRange((*iter)->check5.min, (*iter)->check5.max, ruleItr); 
    _dim6->addRange((*iter)->check6.min, (*iter)->check6.max, ruleItr); 
    _dim7->addRange((*iter)->check7.min, (*iter)->check7.max, ruleItr); 
    _dim8->addRange((*iter)->check8.min, (*iter)->check8.max, ruleItr); 
    _dim9->addRange((*iter)->check9.min, (*iter)->check9.max, ruleItr); 
    _dim10->addRange((*iter)->check10.min, (*iter)->check10.max, ruleItr); 

    ++ruleItr;
  }
  _chronomgr->stop("convert classifier/addRange");
}

void Bitvector10tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
  using namespace DataBitvector;

	unsigned int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  if (!_dim1 || !_dim2 || !_dim3 || !_dim4 || !_dim5 || 
    !_dim6 || !_dim7 || !_dim8 || !_dim9 || !_dim10)
    throw "Bitvector10tpl: One of the lookup structures is not set.";

  Data10tpl::HeaderTuple tpl(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set

  Bitvector bv0(_rules.size());

  Range<uint32_t> searchKey32(0, 0);

	for(Generic::PacketHeaderSet::const_iterator lineItr(data.cbegin()); lineItr != data.cend(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter10tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");

    searchKey32.min = tpl.v1;
    searchKey32.max = tpl.v1;
    bv0 = _dim1->search(searchKey32); // initial copy
    
    searchKey32.min = tpl.v2;
    searchKey32.max = tpl.v2;
    bv0 &= _dim2->search(searchKey32);

    searchKey32.min = tpl.v3;
    searchKey32.max = tpl.v3;
    bv0 &= _dim3->search(searchKey32);

    searchKey32.min = tpl.v4;
    searchKey32.max = tpl.v4;
    bv0 &= _dim4->search(searchKey32);

    searchKey32.min = tpl.v5;
    searchKey32.max = tpl.v5;
    bv0 &= _dim5->search(searchKey32);

    searchKey32.min = tpl.v6;
    searchKey32.max = tpl.v6;
    bv0 &= _dim6->search(searchKey32);
    
    searchKey32.min = tpl.v7;
    searchKey32.max = tpl.v7;
    bv0 &= _dim7->search(searchKey32);

    searchKey32.min = tpl.v8;
    searchKey32.max = tpl.v8;
    bv0 &= _dim8->search(searchKey32);

    searchKey32.min = tpl.v9;
    searchKey32.max = tpl.v9;
    bv0 &= _dim9->search(searchKey32);

    searchKey32.min = tpl.v10;
    searchKey32.max = tpl.v10;
    bv0 &= _dim10->search(searchKey32);

    matchIndex = bv0.getFirstSetBit();
    _chronomgr->stop("classify");

    indices.push_back(matchIndex);

    headerProcessed(); // set recurrent checkpoints
	}
}

void Bitvector10tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void Bitvector10tpl::convertRuleAndStore(const Generic::Rule& rule, StorageVector::iterator pos) {
  if (rule.size() < 10)
    throw "Bitvector10tpl: Not enough rule checks for ten-tuple algorithms in given rule found.";

  // iterate over each rule atom and get ranges
  Generic::VarValue minConv, maxConv;
  uint32_t min1=0, max1=0, 
    min2=0, max2=0, 
    min3=0, max3=0, 
    min4=0, max4=0, 
    min5=0, max5=0, 
    min6=0, max6=0, 
    min7=0, max7=0,
    min8=0, max8=0,
    min9=0, max9=0,
    min10=0, max10=0;

  for (unsigned int atomItr = 0; atomItr < 10; ++atomItr) {
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

      case 4:
        min5 = minConv.get_ui();
        max5 = maxConv.get_ui(); 
        break;
      
      case 5:
        min6 = minConv.get_ui();
        max6 = maxConv.get_ui(); 
        break;

      case 6:
        min7 = minConv.get_ui();
        max7 = maxConv.get_ui();
        break;

      case 7:
        min8 = minConv.get_ui();
        max8 = maxConv.get_ui(); 
        break;

      case 8:
        min9 = minConv.get_ui();
        max9 = maxConv.get_ui();
        break;

      case 9:
        min10 = minConv.get_ui();
        max10 = maxConv.get_ui(); 
        break;

      default: throw "Bitvector10tpl: Out of bounds in setRules!";
    }
  }

  // create local rule-representation and add to storage
  std::unique_ptr<RuleStorageItem> convRule(new RuleStorageItem(min1, max1, 
    min2, max2, 
    min3, max3, 
    min4, max4, 
    min5, max5, 
    min6, max6, 
    min7, max7, 
    min8, max8, 
    min9, max9, 
    min10, max10));
  _rules.insert(pos, std::move(convRule));
}

void Bitvector10tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  if (index > _rules.size())
    throw "Bitvector10tpl: Added a new rule at a position out of bounds of the existing classifier.";
  
  _chronomgr->start("add rule");
  convertRuleAndStore(rule, _rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("add rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}
  
void Bitvector10tpl::ruleRemoved(uint32_t index) {
  if (index >= _rules.size())
    throw "Bitvector10tpl: Index for rule to remove is outside bounds of the existing classifier.";
  
  _chronomgr->start("remove rule");
  _rules.erase(_rules.begin() + index);
  constructProjections(); // reconstruct search structure
  _chronomgr->stop("remove rule");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void Bitvector10tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _rules.clear();
  _dim1.reset();
  _dim2.reset();
  _dim3.reset();
  _dim4.reset();
  _dim5.reset();
  _dim6.reset();
  _dim7.reset();
  _dim8.reset();
  _dim9.reset();
  _dim10.reset();
}
