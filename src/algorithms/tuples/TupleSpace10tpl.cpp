#include <algorithms/tuples/TupleSpace10tpl.hpp>
#include <iostream>

/**
 * Provides a constructor for dynamic library binding at runtime.
 * ('extern C' as workaround to prevent name magling)
 */
extern "C" Base* create_algorithm() {
	return new TupleSpace10tpl;
}

bool TupleSpace10tpl::_mapSortFunction(const DataTupleSpace10tpl::Map10ptr& map1, const DataTupleSpace10tpl::Map10ptr& map2) {
  if (map1 && map2) 
    return (*map1) < (*map2);
  else 
    return (bool)map1;
}

void TupleSpace10tpl::placeRuleInMap(DataTupleSpace10tpl::Rule10ptr rule) {
  using namespace DataTupleSpace10tpl;

  Map10tpl* map = nullptr;

  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap)
  {
    if ((*itrMap)->responsible(rule)) {
      map = itrMap->get();
      break;
    }
  }

  if (map == nullptr) {
    // create new map with tuple of rule
    Tuple10ptr tuple(new Tuple10(rule->tuplemap->bitwidthmap)); 
    Map10ptr mapptr(new Map10tpl(_settingCapacity, std::move(tuple), Generic::noRuleIsMatching()));
    map = mapptr.get();

    _maps.push_back(std::move(mapptr));
  }

  map->insert(std::move(rule));
}

void TupleSpace10tpl::convertRuleToMany(const Generic::Rule& varRule, std::vector<DataTupleSpace10tpl::Rule10ptr>& specRules, const size_t index) const {
  if (varRule.size() < 10) 
    throw "A rule inside the generic ruleset has not enough rule-atoms for 10-tuple compatibility.";

  Generic::RuleAtom* c1 = varRule[0].get();
  Generic::RuleAtom* c2 = varRule[1].get();
  Generic::RuleAtom* c3 = varRule[2].get();
  Generic::RuleAtom* c4 = varRule[3].get();
  Generic::RuleAtom* c5 = varRule[4].get();
  Generic::RuleAtom* c6 = varRule[5].get();
  Generic::RuleAtom* c7 = varRule[6].get();
  Generic::RuleAtom* c8 = varRule[7].get();
  Generic::RuleAtom* c9 = varRule[8].get();
  Generic::RuleAtom* c10 = varRule[9].get();

  using namespace Generic;

  std::vector<std::tuple<VarValue, VarValue>> vec1; 
  std::vector<std::tuple<VarValue, VarValue>> vec2;
  std::vector<std::tuple<VarValue, VarValue>> vec3;
  std::vector<std::tuple<VarValue, VarValue>> vec4;
  std::vector<std::tuple<VarValue, VarValue>> vec5;
  std::vector<std::tuple<VarValue, VarValue>> vec6;
  std::vector<std::tuple<VarValue, VarValue>> vec7;
  std::vector<std::tuple<VarValue, VarValue>> vec8;
  std::vector<std::tuple<VarValue, VarValue>> vec9;
  std::vector<std::tuple<VarValue, VarValue>> vec10;

  c1->toPrefixes(vec1);
  c2->toPrefixes(vec2);
  c3->toPrefixes(vec3);
  c4->toPrefixes(vec4);
  c5->toPrefixes(vec5);
  c6->toPrefixes(vec6);
  c7->toPrefixes(vec7);
  c8->toPrefixes(vec8);
  c9->toPrefixes(vec9);
  c10->toPrefixes(vec10);

  // create a rule for each possible combination of prefix-rules
  for (auto it1(vec1.begin()); it1 != vec1.end(); ++it1) {
    for (auto it2(vec2.begin()); it2 != vec2.end(); ++it2) {
      for (auto it3(vec3.begin()); it3 != vec3.end(); ++it3) {
        for (auto it4(vec4.begin()); it4 != vec4.end(); ++it4) {
          for (auto it5(vec5.begin()); it5 != vec5.end(); ++it5) {
            for (auto it6(vec6.begin()); it6 != vec6.end(); ++it6) {
              for (auto it7(vec7.begin()); it7 != vec7.end(); ++it7) {
                for (auto it8(vec8.begin()); it8 != vec8.end(); ++it8) {
                  for (auto it9(vec9.begin()); it9 != vec9.end(); ++it9) {
                    for (auto it10(vec10.begin()); it10 != vec10.end(); ++it10) {
            DataTupleSpace10tpl::Rule10ptr prefRule(new DataTupleSpace10tpl::Rule10tpl(index,
              (uint32_t)std::get<0>(*it1).get_ui(), (size_t)std::get<1>(*it1).get_ui(),
              (uint32_t)std::get<0>(*it2).get_ui(), (size_t)std::get<1>(*it2).get_ui(),
              (uint32_t)std::get<0>(*it3).get_ui(), (size_t)std::get<1>(*it3).get_ui(),
              (uint32_t)std::get<0>(*it4).get_ui(), (size_t)std::get<1>(*it4).get_ui(),
              (uint32_t)std::get<0>(*it5).get_ui(), (size_t)std::get<1>(*it5).get_ui(),
              (uint32_t)std::get<0>(*it6).get_ui(), (size_t)std::get<1>(*it6).get_ui(),
              (uint32_t)std::get<0>(*it7).get_ui(), (size_t)std::get<1>(*it7).get_ui(),
              (uint32_t)std::get<0>(*it8).get_ui(), (size_t)std::get<1>(*it8).get_ui(),
              (uint32_t)std::get<0>(*it9).get_ui(), (size_t)std::get<1>(*it9).get_ui(),
              (uint32_t)std::get<0>(*it10).get_ui(), (size_t)std::get<1>(*it10).get_ui()
            ));

            specRules.push_back(std::move(prefRule));
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void TupleSpace10tpl::setRules(const Generic::RuleSet& ruleset) {
  using namespace DataTupleSpace10tpl;

  size_t rulesSize = ruleset.size();

  _chronomgr->start("convert classifier");
	for (size_t ruleItr = 0; ruleItr < rulesSize; ++ruleItr)
	{
    std::vector<Rule10ptr> rules;
    convertRuleToMany(*(ruleset[ruleItr].get()), rules, ruleItr);
    
    if (rules.size() > 1) { // in case that rule was expanded
      std::unique_ptr<Data10tpl::Rule> ruleptr;
      Converter10tpl::convertRule(*(ruleset[ruleItr].get()), ruleptr);
      
      _expandRules.push_back(std::make_tuple(ruleItr, std::move(ruleptr)));
      if (_expandMinIdx > ruleItr) _expandMinIdx = ruleItr;
      continue;
    }

    // store resulting prefix rules in tuple maps
    for (std::vector<Rule10ptr>::iterator itrPrefx(rules.begin()); itrPrefx != rules.end(); ++itrPrefx) {
      placeRuleInMap(std::move(*itrPrefx));
    }
  }

  // sort hashmaps by minIndex
  std::sort(_maps.begin(), _maps.end(), TupleSpace10tpl::_mapSortFunction);
  _chronomgr->stop("convert classifier");

  // set memory-checkpoint
  _mmanager->checkpoint(0);
}

void TupleSpace10tpl::classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) {
	unsigned long int matchIndex;
 
  checkMemMgr(); // assert that MemManager instance is referenced
  
  Data10tpl::HeaderTuple tpl(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // for conversion
	if (!indices.empty()) indices.clear(); // if caller forgot to empty set
	for(Generic::PacketHeaderSet::const_iterator lineItr(data.begin()); lineItr != data.end(); ++lineItr) {
    _chronomgr->start("convert header");
    Converter10tpl::convertHeader(**lineItr, tpl); 
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

void TupleSpace10tpl::headerProcessed() {
  ++_cntHeadersAfterCheckpoint;

  if (_cntHeadersAfterCheckpoint >= _settingHeadersPerCheckpoint) {
    _mmanager->checkpoint(_cntHeadersAfterCheckpoint);
    _cntHeadersAfterCheckpoint = 0;
  }
}

void TupleSpace10tpl::setParameters(const std::vector<double>& params) {
  // first parameter is checkpoint-set-span in headers
  if (params.size() > 0) {
    _settingHeadersPerCheckpoint = (uint32_t)params[0];
    
    if (_settingHeadersPerCheckpoint < 1) { // forbit invalid values and inform user
      _settingHeadersPerCheckpoint = 1;
      std::cerr << "Error in Tuple Space Search (10tpl): Invalid parameter set for Headers-per-Checkpoint!" << std::endl;
    }
  }

  // second parameter might be hashmap capacity
  if (params.size() > 1) {
    _settingCapacity = (uint32_t)params[1];

    if (_settingCapacity < 1) { // forbid invalid values and inform user
      _settingCapacity = 100;
      std::cerr << "Error in Tuple Space Search (10tpl): Invalid parameter set for Capacity!" << std::endl;
    }
  }

  // no further parameters in use
}

void TupleSpace10tpl::ruleAdded(uint32_t index, const Generic::Rule& rule) {
  using namespace DataTupleSpace10tpl;

  // increment all indices of existing rules, if equal or greater index
  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap) {
    (*itrMap)->incrementIndex(index);
  }

  // convert and expand generic rule to prefix rules
  std::vector<Rule10ptr> rules;
  convertRuleToMany(rule, rules, index);

  // store resulting prefix rules in tuple maps
  for (std::vector<Rule10ptr>::iterator itrPrefx(rules.begin()); itrPrefx != rules.end(); ++itrPrefx) {
    placeRuleInMap(std::move(*itrPrefx));
  }

  // TODO consider expanded rules
}
  
void TupleSpace10tpl::ruleRemoved(uint32_t index) {
  // remove all prefix rules with given index from all maps
  for (auto itrMap(_maps.begin()); itrMap != _maps.end(); ++itrMap) {
    (*itrMap)->remove(index);
    // decrement all indices of remaining rules, if greater
    (*itrMap)->decrementIndex(index);
  }

  // TODO consider expanded rules
}

void TupleSpace10tpl::reset() {
  _cntHeadersAfterCheckpoint = 0;
  _maps.clear();
  _expandRules.clear();
  _expandMinIdx = Generic::noRuleIsMatching();
}
