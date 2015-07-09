#include <algorithms/hicuts/HiCuts10tpl.hpp>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new HiCuts10tpl;
}

void HiCuts10tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in HiCuts (10tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // second parameter is binth (bucket size for rules in leafs)
  if (params.size() > 1) {
    _binth = (unsigned int)params[1];

    if (_binth == 0) { // forbid invalid values
      _binth = 2;
      std::cerr << "Error in HiCuts (10tpl): Invalid parameter set for 'binth'!" << std::endl;
    }
  }

  // third parameter is spfac (relevant for space measure fct.)
  if (params.size() > 2) {
    _spfac = params[2];

    if (_spfac <= 0.0) { // forbid invalid values
      _spfac = 1.0;
      std::cerr << "Error in HiCuts (10tpl): Invalid parameter set for 'spfac'!" << std::endl;
    }
  }

  // no further parameters in use
}

void HiCuts10tpl::setRules(const Generic::RuleSet& ruleset) {
  using namespace DataHiCuts10tpl;
  
  // convert rules
  _chronomgr->start("convert classifier");
	for (unsigned int ruleItr = 0; ruleItr < ruleset.size(); ++ruleItr) {
    convertRule(*(ruleset[ruleItr].get()), ruleItr, _rules.end());
  }

  _searchTrie.setParameters(_binth, _spfac);

  _chronomgr->start("construct trie");
  _searchTrie.construct(_rules);
  _chronomgr->stop("construct trie");

  _chronomgr->stop("convert classifier");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void HiCuts10tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
  using namespace DataHiCuts10tpl;

	unsigned int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  Data10tpl::HeaderTuple tpl(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set
  bool foundMatch = false;

	for(Generic::PacketHeaderSet::const_iterator lineItr(data.cbegin()); lineItr != data.cend(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter10tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");
    foundMatch = _searchTrie.search(tpl, matchIndex);
    _chronomgr->stop("classify");

    if (foundMatch)
      indices.push_back(matchIndex);
    else
      indices.push_back(Generic::noRuleIsMatching());

    headerProcessed(); // set recurrent checkpoints
	}
}

void HiCuts10tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void HiCuts10tpl::convertRule(const Generic::Rule& rule, unsigned int index, DataHiCuts10tpl::RulesVector::iterator pos) {
  if (rule.size() < 10)
    throw "HiCuts10tpl: Not enough rule checks for ten-tuple algorithms in given rule found.";

  // iterate over each rule atom and get ranges
  Generic::VarValue minConv, maxConv;
  uint32_t min1=0, max1=0, min2=0, max2=0;
  uint32_t min3=0, max3=0, min4=0, max4=0;
  uint32_t min5=0, max5=0, min6=0, max6=0;
  uint32_t min7=0, max7=0, min8=0, max8=0;
  uint32_t min9=0, max9=0, min10=0, max10=0;

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

      default: throw "HiCuts10tpl: Out of bounds in setRules!";
    }
  }

  using namespace DataHiCuts10tpl;
  // create local rule-representation and add to vector
  std::shared_ptr<Rule10tpl> convRule = std::make_shared<Rule10tpl>(index, 
    min1, max1, min2, max2, 
    min3, max3, min4, max4, 
    min5, max5, min6, max6, 
    min7, max7, min8, max8, 
    min9, max9, min10, max10);
  _rules.insert(pos, convRule);
}

void HiCuts10tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  if (index > _rules.size())
    throw "HiCuts10tpl: Added a new rule at a position out of bounds of the existing classifier.";
  
  _chronomgr->start("add rule");
  
  // convert and store rule
  convertRule(rule, index, _rules.begin() + index);

  // change index of stored rules after the inserted one
  for (auto iter(_rules.begin() + index + 1); iter != _rules.end(); ++iter)
    (*iter)->index += 1;
  
  _searchTrie.reset();
  _searchTrie.construct(_rules); // reconstruct search structure

  _chronomgr->stop("add rule");
    
  // set memory-checkpoint
  _mmanager->checkpoint(0);
}
  
void HiCuts10tpl::ruleRemoved(uint32_t index) {
  if (index >= _rules.size())
    throw "HiCuts10tpl: Index for rule to remove is outside bounds of the existing classifier.";
  
  _chronomgr->start("remove rule");
  _rules.erase(_rules.begin() + index);
 
  // change index of stored rules after the removed one
  for (auto iter(_rules.begin() + index); iter != _rules.end(); ++iter)
    (*iter)->index -= 1;

  _searchTrie.reset();
  _searchTrie.construct(_rules); // reconstruct search structure
  _chronomgr->stop("remove rule");
    
  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void HiCuts10tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _searchTrie.reset();
  _rules.clear();
}
