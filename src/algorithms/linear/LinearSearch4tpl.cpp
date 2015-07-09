#include <algorithms/linear/LinearSearch4tpl.hpp>
#include <iostream>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new LinearSearch4tpl;
}

void LinearSearch4tpl::setRules(const Generic::RuleSet& ruleset) {
  uint32_t rulesSize = (uint32_t)ruleset.size();
  _rules.reset(new Data4tpl::RuleSet(rulesSize)); // initializing native ruleset

  _chronomgr->start("convert classifier");
	for (uint32_t ruleItr = 0; ruleItr < rulesSize; ++ruleItr)
	{
    std::unique_ptr<Data4tpl::Rule> ruleptr;
    Converter4tpl::convertRule(*(ruleset[ruleItr].get()), ruleptr);
    _rules->setRule(ruleItr, std::move(ruleptr));
  }
  _chronomgr->stop("convert classifier");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void LinearSearch4tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
	bool ruleMatched;
	uint32_t matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  Data4tpl::HeaderTuple tpl(0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set
	for(Generic::PacketHeaderSet::const_iterator lineItr(data.begin()); lineItr != data.end(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter4tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");
	  ruleMatched = _rules->match(tpl, matchIndex);
    _chronomgr->stop("classify");

    if (ruleMatched) indices.push_back(matchIndex);
    else indices.push_back(Generic::noRuleIsMatching());
    
    headerProcessed(); // set recurrent checkpoints
	}
}

void LinearSearch4tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void LinearSearch4tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Linear Search (4tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // no further parameters in use
}

void LinearSearch4tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  uint32_t sizeOldRuleSet = _rules->getSize();

  if (index <= sizeOldRuleSet && rule.size() > 0) {
   _chronomgr->start("add rule");
    std::unique_ptr<Data4tpl::Rule> newRulePtr;
    Converter4tpl::convertRule(rule, newRulePtr);
    
    // create new array and copy old rules from previous array
    std::unique_ptr<Data4tpl::RuleSet> newRuleSet(new Data4tpl::RuleSet(sizeOldRuleSet + 1));
    uint32_t idxCntOld = 0;
    uint32_t idxCntNew = 0;
    while (idxCntOld < sizeOldRuleSet) { // just copy old rules
      if (idxCntNew != index) { 
        std::unique_ptr<Data4tpl::Rule> mvRule;
        if (_rules->swapRule(idxCntOld, mvRule))
          newRuleSet->setRule(idxCntNew, std::move(mvRule));
        else
          throw "LinearSearch4tpl: Error when trying to copy old rules to a new rule set.";
         
        ++idxCntOld;
      } 
      // else: let placeholder for new rule 
      ++idxCntNew;
    }

    // set new rule at given index
    newRuleSet->setRule(index, std::move(newRulePtr));

    // switch to complete new rule set 
    _rules.swap(newRuleSet);
    _chronomgr->stop("add rule");
    
    // set memory-checkpoint
    _mmanager->checkpoint(0);
  }
  else
    throw "Invalid index or rule to add to rule set given.";
}
  
void LinearSearch4tpl::ruleRemoved(uint32_t index) {
  uint32_t sizeOldRuleSet = _rules->getSize();

  if (index < sizeOldRuleSet) {
    _chronomgr->start("remove rule");

    // create new array and copy old rules from previous array
    std::unique_ptr<Data4tpl::RuleSet> newRuleSet(new Data4tpl::RuleSet(sizeOldRuleSet - 1));
    uint32_t idxCntOld = 0;
    uint32_t idxCntNew = 0;
    while (idxCntOld < sizeOldRuleSet) { // just copy old rules
      if (idxCntOld != index) { 
        std::unique_ptr<Data4tpl::Rule> mvRule;
        if (_rules->swapRule(idxCntOld, mvRule))
          newRuleSet->setRule(idxCntNew, std::move(mvRule));
        else
          throw "LinearSearch4tpl: Error when trying to copy all, but removed old rules.";
         
        ++idxCntNew;
      } 
      // else: don't copy rule to remove
      ++idxCntOld;
    }

    // switch to new rule set 
    _rules.swap(newRuleSet);
    _chronomgr->stop("remove rule");
    
    // set memory-checkpoint
    _mmanager->checkpoint(0);
  }
  else
    throw "Invalid index for removing a rule from the rule set given.";
}

void LinearSearch4tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _rules.reset();
}
