#include <algorithms/hicuts/DataHiCuts2tpl.hpp>
#include <iterator> // for std::distance
#include <cmath>
#include <cstdlib> // srand, rand

using namespace DataHiCuts2tpl;

void TrieNode::build(const DataHiCuts::Parameters& params) {
  // decide, if already leaf or cutting is necessary
  if (_rules.size() <= params.binth) return; // already done

  // decide which dimension to cut
  std::vector<unsigned int> disjunctRules;
  countDisjunctPairs(_rules, disjunctRules);

  // select maximum value
  unsigned int maxVal = 0;
  std::vector<unsigned int> maxIndices;

  for (unsigned int index = 0; index < disjunctRules.size(); ++index) {
    if (disjunctRules[index] > maxVal) {
      maxIndices.clear();
      maxVal = disjunctRules[index];
      maxIndices.push_back(index);
    } 
    else if (disjunctRules[index] == maxVal) {
      maxIndices.push_back(index);
    }
  }
   
  if (maxIndices.size() == 0) // if no dimension with disjunct rules, terminate here 
    return; 
  else if (maxIndices.size() > 1) { // decide randomly if more than one maximum dimension
    _cutDimension = maxIndices[rand() % maxIndices.size()] + 1;
  }
  else { // exactly one maximum value found
    _cutDimension = maxIndices[0] + 1;
  }

  // calculate amount of equidistant cuts
  unsigned int cuts = 16; // 16 terminates faster
  
  // perform cuts and calc space and threshold
  bool amountTooHigh = false;
  cut(cuts, amountTooHigh);
  unsigned int spaceMeas = spaceMeasure();
  unsigned int threshold = params.spfac * params.totalRules;

  // stop, if space measurement exceeds threshold 
  // or if dimension-ranges are smaller than amount of cuttings
  while (spaceMeas < threshold && !amountTooHigh) {
    cuts <<= 1; // multiply by 2
    _children.clear();
    cut(cuts, amountTooHigh);
    spaceMeas = spaceMeasure();
  }
  // keep cuts and children afterwards

  // perform some memory-optimizations
  for (TrieNodesVector::iterator itr(_children.begin()); itr != _children.end(); ++itr) {
    if ((*itr)->getRuleAmount() == 0)
      itr->reset(); // free pointer to empty trie
    else 
      (*itr)->removeRedundancy(); // remove redundant rules
  }

  // now build each child recursively (depth-first)
  if (_cutPieceSize > 1) { // check, if more cuts possible
    for (TrieNodesVector::iterator itr(_children.begin()); itr != _children.end(); ++itr) {
      if (*itr) (*itr)->build(params);
    }
  }

  // if built children, clear own rule-pointer
  _rules.clear();
}

void TrieNode::removeRedundancy() {
  // outer loop for rule with higher priority
  for (RulesVector::iterator higher(_rules.begin()); higher < _rules.end() - 1; ++higher) {
    RulesVector::iterator lower(higher + 1); 

    // inner loop for rules with lower priority, which will be compared to higher priority rule
    while (lower != _rules.end()) {
      if ((*lower)->inside(**higher, _boxDim1, _boxDim2))
        lower = _rules.erase(lower); // lower rule is completely overlapped by higher rule, so remove
      else
        ++lower;
    }
  }
}

bool TrieNode::calcPieceSize(unsigned int amount) {
  unsigned int partSize = 0;
  switch (_cutDimension) {
    case 1: partSize = _boxDim1.size() / amount; break;
    case 2: partSize = _boxDim2.size() / amount; break;
  }

  // check, if amount of cuts is reasonable
  if (partSize == 0) {
    _cutPieceSize = 1; // set to minimum size for a cut
    return true;
  }
  else {
    _cutPieceSize = partSize;
    return false;
  }
}

void TrieNode::cut(unsigned int amount, bool& underrun) {
  // determine how big each cutting piece in one dimension will be
  underrun = calcPieceSize(amount);

  uint32_t child1min = _boxDim1.min; // copy box size from this node
  uint32_t child1max = _boxDim1.max;
  uint32_t child2min = _boxDim2.min;
  uint32_t child2max = _boxDim2.max;
  
  // generate child nodes
  for (unsigned int c = 0; c < amount; ++c) {
    TrieNodePtr newChild;

    switch (_cutDimension) {
      case 1: { 
        uint32_t boxStart = child1min + c*_cutPieceSize;
        newChild = TrieNodePtr(new TrieNode(boxStart, boxStart + _cutPieceSize-1, child2min, child2max));
        break;
      }
      case 2: {
        uint32_t boxStart = child2min + c*_cutPieceSize;
        newChild = TrieNodePtr(new TrieNode(child1min, child1max, boxStart, boxStart + _cutPieceSize-1));
        break;
      }
    }

    _children.push_back(std::move(newChild));
  }

  // set rules for each child
  for (TrieNodesVector::iterator itr(_children.begin()); itr != _children.end(); ++itr) {
    (*itr)->setRules(_rules);
  }
}

unsigned int TrieNode::spaceMeasure() {
  unsigned int space = 0;

  for (TrieNodesVector::const_iterator itr(_children.cbegin()); itr != _children.cend(); ++itr) {
    space += (*itr)->getRuleAmount();
  }

  return space + (_children.size() == 0 ? 0 : _children.size() - 1);
}

void TrieNode::setRules(const RulesVector& rules) {
  _rules.clear();

  // check which rules lie inside the hypercube of trie
  for (RulesVector::const_iterator itr(rules.cbegin()); itr != rules.cend(); ++itr) {
    if ((*itr)->overlaps(_boxDim1, _boxDim2)) {
      
      _rules.push_back(*itr); // place copy in own rule-collection
    }
  }
}

bool TrieNode::search(const Data2tpl::HeaderTuple& header, unsigned int& index) const {
  if (_children.empty()) { // this is a leaf
    // iterate over each rule (linear search)
    for (RulesVector::const_iterator itr(_rules.cbegin()); itr != _rules.cend(); ++itr) {
      if ((*itr)->match(header.addrSrc, header.addrDest)) {
        index = (*itr)->index;
        return true; // first match
      }
    }

    return false; // nothing found
  }
  else { // search in appropriate child
    unsigned int hdrInDim = 0;
    unsigned int boxMinInDim = 0;

    switch (_cutDimension) {
      case 1: 
        hdrInDim = header.addrSrc;
        boxMinInDim = _boxDim1.min;
        break;
      case 2: 
        hdrInDim = header.addrDest;
        boxMinInDim = _boxDim2.min;
        break;
      default: throw "Error in DataHiCuts2tpl: Invalid value for cutting dimension!";
    }
    if (_cutPieceSize == 0) throw "Error in DataHiCuts2tpl: Cut piece size in cutting dimension is zero!";

    unsigned int childIdx = (hdrInDim - boxMinInDim) / _cutPieceSize;

    // select index of child where search is continued
    if (childIdx < _children.size()) {
      if (_children[childIdx]) // check if child contains something
        return _children[childIdx]->search(header, index);
      else
        return false; // child has no rules
    }
    else
      throw "Error in DataHiCuts2tpl: Wrong calculation of child-index for recursive search!";
  }
}

namespace DataHiCuts2tpl {
void countDisjunctPairs(const RulesVector& rules, std::vector<unsigned int>& results) {
  // contain disjunct pair counters per dimension (results)
  unsigned int disjunctPairs1 = 0;
  unsigned int disjunctPairs2 = 0;

  // iterate over all rule-pairs
  RulesVector::const_iterator rule2Itr(rules.cbegin());
  for (RulesVector::const_iterator rule1Itr(rules.cbegin()); rule1Itr != rules.cend() - 1; ++rule1Itr) {
    for (rule2Itr = rule1Itr + 1; rule2Itr != rules.cend(); ++rule2Itr) {
      // determine for each check, if it is disjunct from previous checks in same dimension
      if (!(*rule1Itr)->check1.overlaps((*rule2Itr)->check1)) ++disjunctPairs1;
      if (!(*rule1Itr)->check2.overlaps((*rule2Itr)->check2)) ++disjunctPairs2;
    }
  }

  // push results into return-vector
  results.push_back(disjunctPairs1);
  results.push_back(disjunctPairs2);
}
} // namespace DataHiCuts2tpl

void Trie::construct(const RulesVector& rules) {
  _root.reset(new TrieNode(0, 0xFFFFFFFF, 0, 0xFFFFFFFF));
  
  _params.totalRules = rules.size();
  _root->setRules(rules);
  srand(0x12345678); // init with same pseudo random numbers for each trie
  _root->build(_params); // creates tree structure recursively
}

bool Trie::search(const Data2tpl::HeaderTuple& header, unsigned int& index) const {
  if (_root) 
    return _root->search(header, index);
  else 
    return false; // no trie was constructed before
}

