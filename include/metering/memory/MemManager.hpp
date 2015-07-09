#ifndef MEM_MANAGER_INCLUDED
#define MEM_MANAGER_INCLUDED

#include <memory>
#include <unordered_map>
#include <vector>
#include <metering/memory/Registry.hpp>
#include <metering/memory/MemSnapshot.hpp>
#include <utility>

namespace Memory {

/** A single memory result value (headerCnt, memsize). */
typedef std::pair<unsigned int, size_t> MemResultValue;

/** A single memory result-category (e.g. allocated). */
typedef std::vector<MemResultValue> MemResultCategory;

/** representation of a single group of memory results. */
struct MemResultGroup {
  MemResultCategory allocBytes;
  MemResultCategory allocMaxBytes;
  MemResultCategory accBytes;
  MemResultCategory accCount;
  MemResultCategory accWriteBytes;
  MemResultCategory accWriteCount;
};

/** Multiple different groups of the same result-category. */
typedef std::vector<std::unique_ptr<MemResultGroup>> MemResultGroups;


/**
 * Central unit for organization of collecting of memory-data and storing it
 * for until the end of a measurement. MemTrace-instances will register themselves
 * so that their memory usage is tracked. Groups of multiple MemTraces can be defined
 * and help to seperate concerns for benchmark-evaluation.
 */
class MemManager : public Registry {
  std::unordered_map<RegistryItem*, unsigned int> _mtraces;
  unsigned int _groupsTotal;
  unsigned int _currentGroupId;
  std::vector<MemSnapshotPtr> _history;
  MemSnapshotPtr _current;

public:
  MemManager() : _groupsTotal(1), _currentGroupId(0), _current(new MemSnapshot(0)) {}
  ~MemManager() {}

  void reg(RegistryItem& item) override;
  void dereg(RegistryItem& item) override;

  /**
	 * Update the amount of already processed packet header data and therefore
	 * create a new memory-snapshot and keep previous snapshot in trace.
	 *
	 * @param headers amount of header data that have been processed
	 */
  void checkpoint(unsigned int headers);

  /**
   * Create a new group where all next registered MemTrace-instances will belong to.
   */
  inline void groupCreate() { _currentGroupId = _groupsTotal++; _current->groupAdd(); }
  
  /**
   * Switch current group to given value, so that all upcoming MemTrace-instances
   * will belong to the given group.
   *
   * @param id unique identifier of group to switch to (must exist).
   */
  void groupSwitch(unsigned int id);

  /** Returns the current amount of groups in total. */
  inline unsigned int groupGetAmount() { return _groupsTotal; }

  /** Return the currently active group id. */
  inline unsigned int groupGetCurrentId() { return _currentGroupId; }

  /** Returns the current counter value for processed packet headers. */
  inline unsigned int getCurrentHeaderCount() const { return _current->headers; }

  /**
   * Sum up each value for all groups in the current snapshot and return as single group value.
   *
   * @param result pointer which is assigned by the function with global memory usage values 
   */
  void getCurrentTotal(MemGroupSnapshotPtr& result) const;

  /**
   * Calculates current memory usage for one single group and returns it.
   *
   * @param groupId unique identifier of group to calculate current memory snapshot for
   * @param result pointer which is assigned with group memory usage values
   */
  void getCurrentByGroup(unsigned int groupId, MemGroupSnapshotPtr& result) const;

  /** Returns amount of already stored snapshots from earlier checkpoints. */
  inline unsigned int getHistorySize() const { return _history.size(); }

  /**
   * Sum up each value for all groups in the history and returns as single group value.
   *
   * @param historyIdx index of required history entry in collection with all stored snapshots
   * @patam result pointer is assigned with sum of all groups at history-index
   */
  void getPastRecordTotal(unsigned int historyIdx, MemGroupSnapshotPtr& result) const;

  /**
   * Returns memory usage values for a single group from an earlier snapshot in history.
   *
   * @param historyIdx index of required history entry in collection with all stored snapshots
   * @param groupId unique identifier of group to get stored values for
   * @param result pointer is assigned with result values
   */
  void getPastRecordByGroup(unsigned int historyIdx, unsigned int groupId, MemGroupSnapshotPtr& result) const;

  /**
   * Copies all memory usage values for all groups to given results-container.
   *
   * @param results provided container for the results
   */
  void getMemResultGroups(MemResultGroups& results) const;

  /**
   * Do a complete reset so that afterwards this instance returns to an initial state.
   */
  void reset();
};

} // namespace Memory
#endif

