#ifndef DATA_HICUTS_2TPL_INCLUDED
#define DATA_HICUTS_2TPL_INCLUDED

#include <memory>
#include <vector>
#include <memory/MemTrace.hpp>
#include <algorithms/hicuts/DataHiCuts.hpp>
#include <algorithms/common/Data2tpl.hpp>

namespace DataHiCuts2tpl {

/** Represents a five-tuple rule with ranges for the rule checks. */
template <typename S, typename T>
struct RangeRule {
  Memory::MemTrace<unsigned int> index; // rule index
  DataHiCuts::Range<S> check1;
  DataHiCuts::Range<T> check2;

  RangeRule(unsigned int idx, const S& min1, const S& max1, 
    const T& min2, const T& max2) : 
      index(idx), check1(min1, max1), check2(min2, max2) {}

  bool overlaps(const DataHiCuts::Range<S>& r1, const DataHiCuts::Range<T>& r2) const {
    return check1.overlaps(r1) && check2.overlaps(r2);
  }

  inline bool match(const S& val1, const T& val2) const {
    return check1.match(val1) && check2.match(val2);
  }

  inline bool inside(const RangeRule<S, T>& oth, 
    const DataHiCuts::Range<S>& r1, const DataHiCuts::Range<T>& r2) const {
    return check1.inside(oth.check1, r1) && check2.inside(oth.check2, r2);
  }

};

/** Type of range-rules with used header structure. */
typedef RangeRule<uint32_t, uint32_t> Rule2tpl;

/** A vector with pointers to rules. */
typedef std::vector<std::shared_ptr<Rule2tpl>> RulesVector;

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

  /** Calculates the correct size of a cutting piece. */
  bool calcPieceSize(unsigned int amountCuttings);
  
  /** Calculates the space measure value for terminating cut-size iteration. */
  unsigned int spaceMeasure();

public:
  TrieNode(uint32_t box1min, uint32_t box1max,
    uint32_t box2min, uint32_t box2max) : 
    _children(), _rules(), _cutDimension(0), _cutPieceSize(0),
    _boxDim1(box1min, box1max),
    _boxDim2(box2min, box2max) {}
  ~TrieNode() {}

  void build(const DataHiCuts::Parameters& params);
  void setRules(const RulesVector& rules);
  
  inline unsigned int getRuleAmount() { return _rules.size(); }

  bool search(const Data2tpl::HeaderTuple& header, unsigned int& index) const;

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
  bool search(const Data2tpl::HeaderTuple& header, unsigned int& index) const;

  void reset() { _root.reset(); }
};
} // namespace DataHiCuts2tpl

#endif

