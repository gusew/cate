#ifndef DATA_HICUTS_10TPL_INCLUDED
#define DATA_HICUTS_10TPL_INCLUDED

#include <memory>
#include <vector>
#include <memory/MemTrace.hpp>
#include <algorithms/hicuts/DataHiCuts.hpp>
#include <algorithms/common/Data10tpl.hpp>

namespace DataHiCuts10tpl {

/** Represents a five-tuple rule with ranges for the rule checks. */
template <typename S>
struct RangeRule {
  Memory::MemTrace<unsigned int> index; // rule index
  DataHiCuts::Range<S> check1;
  DataHiCuts::Range<S> check2;
  DataHiCuts::Range<S> check3;
  DataHiCuts::Range<S> check4;
  DataHiCuts::Range<S> check5;
  DataHiCuts::Range<S> check6;
  DataHiCuts::Range<S> check7;
  DataHiCuts::Range<S> check8;
  DataHiCuts::Range<S> check9;
  DataHiCuts::Range<S> check10;

  RangeRule(unsigned int idx, const S& min1, const S& max1, 
    const S& min2, const S& max2,
    const S& min3, const S& max3,
    const S& min4, const S& max4,
    const S& min5, const S& max5,
    const S& min6, const S& max6, 
    const S& min7, const S& max7,
    const S& min8, const S& max8,
    const S& min9, const S& max9,
    const S& min10, const S& max10) : index(idx), 
    check1(min1, max1), 
    check2(min2, max2), 
    check3(min3, max3),
    check4(min4, max4), 
    check5(min5, max5),
    check6(min6, max6), 
    check7(min7, max7), 
    check8(min8, max8),
    check9(min9, max9), 
    check10(min10, max10) 
  {}

  bool overlaps(const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<S>& r2, 
    const DataHiCuts::Range<S>& r3, 
    const DataHiCuts::Range<S>& r4, 
    const DataHiCuts::Range<S>& r5, 
    const DataHiCuts::Range<S>& r6, 
    const DataHiCuts::Range<S>& r7, 
    const DataHiCuts::Range<S>& r8, 
    const DataHiCuts::Range<S>& r9, 
    const DataHiCuts::Range<S>& r10) const {
    return check1.overlaps(r1) && 
      check2.overlaps(r2) && 
      check3.overlaps(r3) && 
      check4.overlaps(r4) && 
      check5.overlaps(r5) && 
      check6.overlaps(r6) && 
      check7.overlaps(r7) && 
      check8.overlaps(r8) && 
      check9.overlaps(r9) && 
      check10.overlaps(r10);
  }

  inline bool match(const S& val1, const S& val2, 
    const S& val3, const S& val4, 
    const S& val5, const S& val6, 
    const S& val7, const S& val8, 
    const S& val9, const S& val10) const {
    return check1.match(val1) && check2.match(val2) && 
      check3.match(val3) && check4.match(val4) && 
      check5.match(val5) && check6.match(val6) && 
      check7.match(val7) && check8.match(val8) && 
      check9.match(val9) && check10.match(val10);
  }

  inline bool inside(const RangeRule<S>& oth, 
    const DataHiCuts::Range<S>& r1, 
    const DataHiCuts::Range<S>& r2, 
    const DataHiCuts::Range<S>& r3, 
    const DataHiCuts::Range<S>& r4, 
    const DataHiCuts::Range<S>& r5, 
    const DataHiCuts::Range<S>& r6, 
    const DataHiCuts::Range<S>& r7, 
    const DataHiCuts::Range<S>& r8, 
    const DataHiCuts::Range<S>& r9, 
    const DataHiCuts::Range<S>& r10) const {
    return check1.inside(oth.check1, r1) && 
      check2.inside(oth.check2, r2) &&
      check3.inside(oth.check3, r3) &&
      check4.inside(oth.check4, r4) &&
      check5.inside(oth.check5, r5) &&
      check6.inside(oth.check6, r6) &&
      check7.inside(oth.check7, r7) &&
      check8.inside(oth.check8, r8) &&
      check9.inside(oth.check9, r9) &&
      check10.inside(oth.check10, r10);
  }
};

/** Type of range-rules with used header structure. */
typedef RangeRule<uint32_t> Rule10tpl;

/** A vector with pointers to rules. */
typedef std::vector<std::shared_ptr<Rule10tpl>> RulesVector;

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
  DataHiCuts::Range<uint32_t> _boxDim5;
  DataHiCuts::Range<uint32_t> _boxDim6;
  DataHiCuts::Range<uint32_t> _boxDim7;
  DataHiCuts::Range<uint32_t> _boxDim8;
  DataHiCuts::Range<uint32_t> _boxDim9;
  DataHiCuts::Range<uint32_t> _boxDim10;

  /** Calculates the correct size of a cutting piece. */
  bool calcPieceSize(unsigned int amountCuttings);
  
  /** Calculates the space measure value for terminating cut-size iteration. */
  unsigned int spaceMeasure();

public:
  TrieNode(uint32_t box1min, uint32_t box1max,
    uint32_t box2min, uint32_t box2max,
    uint32_t box3min, uint32_t box3max,
    uint32_t box4min, uint32_t box4max,
    uint32_t box5min, uint32_t box5max,
    uint32_t box6min, uint32_t box6max,
    uint32_t box7min, uint32_t box7max,
    uint32_t box8min, uint32_t box8max,
    uint32_t box9min, uint32_t box9max,
    uint32_t box10min, uint32_t box10max) : 
    _children(), _rules(), _cutDimension(0), _cutPieceSize(0),
    _boxDim1(box1min, box1max),
    _boxDim2(box2min, box2max), 
    _boxDim3(box3min, box3max), 
    _boxDim4(box4min, box4max), 
    _boxDim5(box5min, box5max),
    _boxDim6(box6min, box6max),
    _boxDim7(box7min, box7max), 
    _boxDim8(box8min, box8max), 
    _boxDim9(box9min, box9max), 
    _boxDim10(box10min, box10max) {}
  ~TrieNode() {}

  void build(const DataHiCuts::Parameters& params);
  void setRules(const RulesVector& rules);
  
  inline unsigned int getRuleAmount() { return _rules.size(); }

  bool search(const Data10tpl::HeaderTuple& header, unsigned int& index) const;

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
  bool search(const Data10tpl::HeaderTuple& header, unsigned int& index) const;

  void reset() { _root.reset(); }
};
} // namespace DataHiCuts10tpl

#endif

