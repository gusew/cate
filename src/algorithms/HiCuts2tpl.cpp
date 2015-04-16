#include <algorithms/HiCuts2tpl.hpp>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new HiCuts2tpl;
}

void HiCuts2tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in HiCuts (2tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // second parameter is binth (bucket size for rules in leafs)
  if (params.size() > 1) {
    _binth = (unsigned int)params[1];

    if (_binth == 0) { // forbid invalid values
      _binth = 2;
      std::cerr << "Error in HiCuts (2tpl): Invalid parameter set for 'binth'!" << std::endl;
    }
  }

  // third parameter is spfac (relevant for space measure fct.)
  if (params.size() > 2) {
    _spfac = params[2];

    if (_spfac <= 0.0) { // forbid invalid values
      _spfac = 1.0;
      std::cerr << "Error in HiCuts (2tpl): Invalid parameter set for 'spfac'!" << std::endl;
    }
  }

  // no further parameters in use
}

void HiCuts2tpl::setRules(const Generic::RuleSet& ruleset) {
  using namespace DataHiCuts2tpl;
  
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

void HiCuts2tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
  using namespace DataHiCuts2tpl;

	unsigned int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  Data2tpl::HeaderTuple tpl(0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set
  bool foundMatch = false;

	for(Generic::PacketHeaderSet::const_iterator lineItr(data.cbegin()); lineItr != data.cend(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter2tpl::convertHeader(**lineItr, tpl); 
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

void HiCuts2tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void HiCuts2tpl::convertRule(const Generic::Rule& rule, unsigned int index, DataHiCuts2tpl::RulesVector::iterator pos) {
  if (rule.size() < 2)
    throw "HiCuts2tpl: Not enough rule checks for two-tuple algorithms in given rule found.";

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

      default: throw "HiCuts2tpl: Out of bounds in setRules!";
    }
  }

  using namespace DataHiCuts2tpl;
  // create local rule-representation and add to vector
  std::shared_ptr<Rule2tpl> convRule = std::make_shared<Rule2tpl>(index, min1, max1, min2, max2);
  _rules.insert(pos, convRule);
}

void HiCuts2tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  if (index > _rules.size())
    throw "HiCuts2tpl: Added a new rule at a position out of bounds of the existing classifier.";
  
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
  
void HiCuts2tpl::ruleRemoved(uint32_t index) {
  if (index >= _rules.size())
    throw "HiCuts2tpl: Index for rule to remove is outside bounds of the existing classifier.";
  
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

void HiCuts2tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _searchTrie.reset();
  _rules.clear();
}
