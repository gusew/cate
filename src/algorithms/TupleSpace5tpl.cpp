#include <algorithms/TupleSpace5tpl.hpp>
#include <iostream>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new TupleSpace5tpl;
}

bool TupleSpace5tpl::_mapSortFunction(const DataTupleSpace5tpl::Map5ptr& map1, const DataTupleSpace5tpl::Map5ptr& map2) {
  if (map1 && map2) 
    return (*map1) < (*map2);
  else 
    return (bool)map1;
}

void TupleSpace5tpl::placeRuleInMap(DataTupleSpace5tpl::Rule5ptr rule) {
  using namespace DataTupleSpace5tpl;

  Map5tpl* map = nullptr;

  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap)
  {
    if ((*itrMap)->responsible(rule)) {
      map = itrMap->get();
      break;
    }
  }

  if (map == nullptr) {
    // create new map with tuple of rule
    Tuple5ptr tuple(new Tuple5(rule->tuplemap->bitwidthmap)); 
    Map5ptr mapptr(new Map5tpl(_settingCapacity, std::move(tuple), Generic::noRuleIsMatching()));
    map = mapptr.get();

    _maps.push_back(std::move(mapptr));
  }

  map->insert(std::move(rule));
}

void TupleSpace5tpl::convertRuleToMany(const Generic::Rule& varRule, std::vector<DataTupleSpace5tpl::Rule5ptr>& specRules, const size_t index) const {
  if (varRule.size() < 5) 
    throw "A rule inside the generic ruleset has not enough rule-atoms for five-tuple compatibility.";

  Generic::RuleAtom* addrSrcGeneric = varRule[0].get();
  Generic::RuleAtom* addrDestGeneric = varRule[1].get();
  Generic::RuleAtom* portSrcGeneric = varRule[2].get();
  Generic::RuleAtom* portDestGeneric = varRule[3].get();
  Generic::RuleAtom* protocolGeneric = varRule[4].get();

  using namespace Generic;

  std::vector<std::tuple<VarValue, VarValue>> addrSrcRes;
  std::vector<std::tuple<VarValue, VarValue>> addrDestRes;
  std::vector<std::tuple<VarValue, VarValue>> portSrcRes;
  std::vector<std::tuple<VarValue, VarValue>> portDestRes;
  std::vector<std::tuple<VarValue, VarValue>> protocolRes;

  addrSrcGeneric->toPrefixes(addrSrcRes);
  addrDestGeneric->toPrefixes(addrDestRes);
  portSrcGeneric->toPrefixes(portSrcRes);
  portDestGeneric->toPrefixes(portDestRes);
  protocolGeneric->toPrefixes(protocolRes);

  // create a rule for each possible combination of prefix-rules
  for (auto it1(addrSrcRes.begin()); it1 != addrSrcRes.end(); ++it1) {
    for (auto it2(addrDestRes.begin()); it2 != addrDestRes.end(); ++it2) {
      for (auto it3(portSrcRes.begin()); it3 != portSrcRes.end(); ++it3) {
        for (auto it4(portDestRes.begin()); it4 != portDestRes.end(); ++it4) {
          for (auto it5(protocolRes.begin()); it5 != protocolRes.end(); ++it5) {
            DataTupleSpace5tpl::Rule5ptr prefRule(new DataTupleSpace5tpl::Rule5tpl(index,
              (uint32_t)std::get<0>(*it1).get_ui(), (size_t)std::get<1>(*it1).get_ui(),
              (uint32_t)std::get<0>(*it2).get_ui(), (size_t)std::get<1>(*it2).get_ui(),
              (uint16_t)std::get<0>(*it3).get_ui(), (size_t)std::get<1>(*it3).get_ui(),
              (uint16_t)std::get<0>(*it4).get_ui(), (size_t)std::get<1>(*it4).get_ui(),
              (uint8_t)std::get<0>(*it5).get_ui(), (size_t)std::get<1>(*it5).get_ui()
            ));

            specRules.push_back(std::move(prefRule));
          }
        }
      }
    }
  }
}

void TupleSpace5tpl::setRules(const Generic::RuleSet& ruleset) {
  using namespace DataTupleSpace5tpl;

  size_t rulesSize = ruleset.size();

  _chronomgr->start("convert classifier");
	for (size_t ruleItr = 0; ruleItr < rulesSize; ++ruleItr)
	{
    std::vector<Rule5ptr> rules;
    convertRuleToMany(*(ruleset[ruleItr].get()), rules, ruleItr);
    
    if (rules.size() > 1) { // in case that rule was expanded
      std::unique_ptr<Data5tpl::Rule> ruleptr;
      Converter5tpl::convertRule(*(ruleset[ruleItr].get()), ruleptr);
      
      _expandRules.push_back(std::make_tuple(ruleItr, std::move(ruleptr)));
      if (_expandMinIdx > ruleItr) _expandMinIdx = ruleItr;
      continue;
    }

    // store resulting prefix rules in tuple maps
    for (std::vector<Rule5ptr>::iterator itrPrefx(rules.begin()); itrPrefx != rules.end(); ++itrPrefx) {
      placeRuleInMap(std::move(*itrPrefx));
    }
  }

  // sort hashmaps by minIndex
  std::sort(_maps.begin(), _maps.end(), TupleSpace5tpl::_mapSortFunction);
  _chronomgr->stop("convert classifier");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void TupleSpace5tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
	unsigned long int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  Data5tpl::HeaderTuple tpl(0, 0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set
	for(Generic::PacketHeaderSet::const_iterator lineItr(data.begin()); lineItr != data.end(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter5tpl::convertHeader(**lineItr, tpl); 
    _chronomgr->stop("convert header");

    _chronomgr->start("classify");
    matchIndex = Generic::noRuleIsMatching();
    unsigned long int currIndex = matchIndex;;

    //_chronomgr->start("classify(hash)");
	  for(auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap) {
      if (matchIndex > (*itrMap)->getMinIndex()) { // naive tuple pruning
        currIndex = (*itrMap)->lookup(tpl);

        if (currIndex < matchIndex) matchIndex = currIndex;
      }
    }
    //_chronomgr->stop("classify(hash)");
     
    // search also in list of "expanded rules"
    if (matchIndex > _expandMinIdx) {

      //_chronomgr->start("classify(lin)");
      for (auto it = _expandRules.begin(); it != _expandRules.end(); ++it) {
        if (std::get<1>(*it)->match(tpl)) {
          currIndex = std::get<0>(*it); 
          if (currIndex < matchIndex) {
            matchIndex = currIndex;
          }
          break;
        }
      }
      //_chronomgr->stop("classify(lin)");
    }

    _chronomgr->stop("classify");

    indices.push_back(matchIndex);
    
    headerProcessed(); // set recurrent checkpoints
	}
}

void TupleSpace5tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void TupleSpace5tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Tuple Space Search (5tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // second parameter might be hashmap capacity
  if (params.size() > 1) {
    _settingCapacity = (uint32_t)params[1];

    if (_settingCapacity < 1) { // forbid invalid values and inform user
      _settingCapacity = 100;
      std::cerr << "Error in Tuple Space Search (5tpl): Invalid parameter set for Capacity!" << std::endl;
    }
  }

  // no further parameters in use
}

void TupleSpace5tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  using namespace DataTupleSpace5tpl;

  // increment all indices of existing rules, if equal or greater index
  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap) {
    (*itrMap)->incrementIndex(index);
  }

  // convert and expand generic rule to prefix rules
  std::vector<Rule5ptr> rules;
  convertRuleToMany(rule, rules, index);

  // store resulting prefix rules in tuple maps
  for (std::vector<Rule5ptr>::iterator itrPrefx(rules.begin()); itrPrefx != rules.end(); ++itrPrefx) {
    placeRuleInMap(std::move(*itrPrefx));
  }

  // TODO consider expanded rules
}
  
void TupleSpace5tpl::ruleRemoved(uint32_t index) {
  // remove all prefix rules with given index from all maps
  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap) {
    (*itrMap)->remove(index);
    // decrement all indices of remaining rules, if greater
    (*itrMap)->decrementIndex(index);
  }

  // TODO consider expanded rules
}

void TupleSpace5tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _maps.clear();
  _expandRules.clear();
  _expandMinIdx = Generic::noRuleIsMatching();
}
