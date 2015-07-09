#ifndef DATA_HICUTS_4TPL_INCLUDED
#define DATA_HICUTS_4TPL_INCLUDED

#include <memory>
#include <vector>
#include <metering/memory/MemTrace.hpp>
#include <algorithms/hicuts/DataHiCuts.hpp>
#include <algorithms/common/Data4tpl.hpp>

namespace DataHiCuts4tpl {

/** Represents a 4-tuple rule with ranges for the rule checks. */
template <typename S>
struct RangeRule {
  Memory::MemTrace<unsigned int> index; // rule index
  DataHiCuts::Range<S> check1;
  DataHiCuts::Range<S> check2;
  DataHiCuts::Range<S> check3;
  DataHiCuts::Range<S> check4;

  RangeRule(unsigned int idx, const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4) : 
      index(idx), 
      check1(min1, max1), 
      check2(min2, max2), 
      check3(min3, max3), 
      check4(min4, max4) 
  {}

  bool overlaps(const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<S>& r2,
    const DataHiCuts::Range<S>& r3,
    const DataHiCuts::Range<S>& r4) const {
    return check1.overlaps(r1) && 
      check2.overlaps(r2) &&
      check3.overlaps(r3) &&
      check4.overlaps(r4);
  }

  inline bool match(const S& val1, 
    const S& val2,
    const S& val3,
    const S& val4) const {
    return check1.match(val1) && 
      check2.match(val2) &&
      check3.match(val3) &&
      check4.match(val4);
  }

  inline bool inside(const RangeRule<S>& oth, 
    const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<S>& r2,
    const DataHiCuts::Range<S>& r3,
    const DataHiCuts::Range<S>& r4) const {
    return check1.inside(oth.check1, r1) && 
      check2.inside(oth.check2, r2) &&
      check3.inside(oth.check3, r3) &&
      check4.inside(oth.check4, r4);
  }

};

/** Type of range-rules with used header structure. */
typedef RangeRule<uint32_t> Rule4tpl;

/** A vector with pointers to rules. */
typedef std::vector<std::shared_ptr<Rule4tpl>> RulesVector;

class TrieNode; // forward declaration

/** A smart-pointer to a single trie node. */
typedef std::unique_ptr<TrieNode> TrieNodePtr;

/** A collection of trie nodes with index-access support. */
typedef std::vector<TrieNodePtr> TrieNodesVector;

/** Represents one single node in a trie. */
class TrieNode {
  TrieNodesVector _children;
  RulesVector _rules;

  /** Contains decision, on which dimension the cutting is. */
  Memory::MemTrace<unsigned int> _cutDimension;
  /** Stores the range-length of a piece in each cutting. */
  Memory::MemTrace<unsigned int> _cutPieceSize;
  
  DataHiCuts::Range<uint32_t> _boxDim1;
  DataHiCuts::Range<uint32_t> _boxDim2;
  DataHiCuts::Range<uint32_t> _boxDim3;
  DataHiCuts::Range<uint32_t> _boxDim4;


  /** Calculates the correct size of a cutting piece. */
  bool calcPieceSize(unsigned int amountCuttings);
  
  /** Calculates the space measure value for terminating cut-size iteration. */
  unsigned int spaceMeasure();

public:
  TrieNode(uint32_t box1min, uint32_t box1max,
    uint32_t box2min, uint32_t box2max,
    uint32_t box3min, uint32_t box3max,
    uint32_t box4min, uint32_t box4max) : 
    _children(), _rules(), _cutDimension(0), _cutPieceSize(0),
    _boxDim1(box1min, box1max),
    _boxDim2(box2min, box2max),
    _boxDim3(box3min, box3max), 
    _boxDim4(box4min, box4max) 
  {}
  ~TrieNode() {}

  void build(const DataHiCuts::Parameters& params);
  void setRules(const RulesVector& rules);
  
  inline unsigned int getRuleAmount() { return _rules.size(); }

  bool search(const Data4tpl::HeaderTuple& header, unsigned int& index) const;

  void cut(unsigned int amount, bool& underrun);

  void removeRedundancy();
};

/** Iterates over all rules and determines how many disjunct rule check pairs in each dimension exist. */
void countDisjunctPairs(const RulesVector& rules, std::vector<unsigned int>& results);

/** Represents a whole trie with multiple nodes. */
class Trie {
  std::unique_ptr<TrieNode> _root;
  DataHiCuts::Parameters _params;

public:
  Trie() : _root(), _params() {}
  
  inline void setParameters(unsigned int binth, double spfac) { 
    _params.binth = binth; _params.spfac = spfac; 
  }

  /** Starts with the construction of a trie. */
  void construct(const RulesVector& rules);

  /** Returns true, if found a matching rule for the given header. */
  bool search(const Data4tpl::HeaderTuple& header, unsigned int& index) const;

  void reset() { _root.reset(); }
};
} // namespace DataHiCuts4tpl

#endif

