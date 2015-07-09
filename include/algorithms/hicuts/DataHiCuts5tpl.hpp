#ifndef DATA_HICUTS_5TPL_INCLUDED
#define DATA_HICUTS_5TPL_INCLUDED

#include <memory>
#include <vector>
#include <metering/memory/MemTrace.hpp>
#include <algorithms/hicuts/DataHiCuts.hpp>
#include <algorithms/common/Data5tpl.hpp>

namespace DataHiCuts5tpl {

/** Represents a five-tuple rule with ranges for the rule checks. */
template <typename S, typename T, typename U, typename V, typename W>
struct RangeRule {
  Memory::MemTrace<unsigned int> index; // rule index
  DataHiCuts::Range<S> check1;
  DataHiCuts::Range<T> check2;
  DataHiCuts::Range<U> check3;
  DataHiCuts::Range<V> check4;
  DataHiCuts::Range<W> check5;

  RangeRule(unsigned int idx, const S& min1, const S& max1, 
    const T& min2, const T& max2,
    const U& min3, const U& max3,
    const V& min4, const V& max4,
    const W& min5, const W& max5) : 
      index(idx), check1(min1, max1), check2(min2, max2), check3(min3, max3),
      check4(min4, max4), check5(min5, max5) {}

  bool overlaps(const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<T>& r2, 
    const DataHiCuts::Range<U>& r3, 
    const DataHiCuts::Range<V>& r4, 
    const DataHiCuts::Range<W>& r5) const {
    return check1.overlaps(r1) && check2.overlaps(r2) && check3.overlaps(r3) && check4.overlaps(r4) && check5.overlaps(r5);
  }

  inline bool match(const S& val1, const T& val2, const U& val3, const V& val4, const W& val5) const {
    return check1.match(val1) && check2.match(val2) && check3.match(val3) && check4.match(val4) && check5.match(val5);
  }

  inline bool inside(const RangeRule<S, T, U, V, W>& oth, 
    const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<T>& r2, 
    const DataHiCuts::Range<U>& r3, 
    const DataHiCuts::Range<V>& r4, 
    const DataHiCuts::Range<W>& r5) const {
    return check1.inside(oth.check1, r1) && 
      check2.inside(oth.check2, r2) &&
      check3.inside(oth.check3, r3) &&
      check4.inside(oth.check4, r4) &&
      check5.inside(oth.check5, r5);
  }
};

/** Type of range-rules with used header structure. */
typedef RangeRule<uint32_t, uint32_t, uint16_t, uint16_t, uint8_t> Rule5tpl;

/** A vector with pointers to rules. */
typedef std::vector<std::shared_ptr<Rule5tpl>> RulesVector;

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
  DataHiCuts::Range<uint16_t> _boxDim3;
  DataHiCuts::Range<uint16_t> _boxDim4;
  DataHiCuts::Range<uint8_t> _boxDim5;

  /** Calculates the correct size of a cutting piece. */
  bool calcPieceSize(unsigned int amountCuttings);
  
  /** Calculates the space measure value for terminating cut-size iteration. */
  unsigned int spaceMeasure();

public:
  TrieNode(uint32_t box1min, uint32_t box1max,
    uint32_t box2min, uint32_t box2max,
    uint16_t box3min, uint16_t box3max,
    uint16_t box4min, uint16_t box4max,
    uint8_t box5min, uint8_t box5max) : 
    _children(), _rules(), _cutDimension(0), _cutPieceSize(0),
    _boxDim1(box1min, box1max),
    _boxDim2(box2min, box2max), 
    _boxDim3(box3min, box3max), 
    _boxDim4(box4min, box4max), 
    _boxDim5(box5min, box5max) {}
  ~TrieNode() {}

  void build(const DataHiCuts::Parameters& params);
  void setRules(const RulesVector& rules);
  
  inline unsigned int getRuleAmount() { return _rules.size(); }

  bool search(const Data5tpl::HeaderTuple& header, unsigned int& index) const;

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
  bool search(const Data5tpl::HeaderTuple& header, unsigned int& index) const;

  void reset() { _root.reset(); }
};
} // namespace DataHiCuts5tpl

#endif

