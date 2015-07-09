#ifndef MEM_SNAPSHOT_INCLUDED
#define MEM_SNAPSHOT_INCLUDED

#include <cstddef>
#include <vector>
#include <memory>

namespace Memory {

/**
 * Represents a memory snapshot for a whole group at a given moment.
 * Contains all group-dependent counter-variables.
 */
struct MemGroupSnapshot {
  size_t allocBytes; // total currently allocated bytes
  size_t allocMaxBytes; // maximum value of allocated bytes
  size_t accBytes; // total amount of accessed bytes
  size_t accCount; // amount of accesses (+1 on each)
  size_t accWriteBytes; // amount of bytes on writes access
  size_t accWriteCount; // amount of write accesses (+1 on each)

  MemGroupSnapshot() : allocBytes(0), allocMaxBytes(0), accBytes(0), accCount(0), accWriteBytes(0), accWriteCount(0) {}

  /**
   * Updates values with values taken from a traced memory-object.
   *
   * @param allocB allocated bytes by instance
   * @param accB accessed bytes by instance
   * @param accCnt amount of accesses in numbers
   * @param accWriteB amount of bytes written
   * @param accWriteCnt amount of write accesses in numbers
   */
   void update(size_t allocB, size_t accB, size_t accCnt, size_t accWriteB, size_t accWriteCnt);
   
   /**
    * Updates this group snapshot with values from other group snapshot.
    *
    * @param other group snapshot where values for update are got from
    */
   void merge(const std::unique_ptr<MemGroupSnapshot>& other);
};

typedef std::unique_ptr<MemGroupSnapshot> MemGroupSnapshotPtr;

/**
 * Represents a complete memory tracing snapshot with all group-snapshots 
 * and a representation for the time (headers-counter).
 */
struct MemSnapshot {
  /** counts current processed packet-headers */
  unsigned int headers;
  /** contains all present group-data at this moment */
  std::vector<MemGroupSnapshotPtr> groups;

  MemSnapshot(unsigned int h) : headers(h) { groupAdd(); }
  MemSnapshot(const MemSnapshot& other);

  /** Add a new group to collection of already existing groups. */
  inline void groupAdd() { groups.emplace_back(new MemGroupSnapshot); }
 
  /**
   * Updates values of a specified group in collection with values taken 
   * from a traced memory-object.
   *
   * @param groupId id of the group to update
   * @param allocB allocated bytes by instance
   * @param accB accessed bytes by instance
   * @param accCnt amount of accesses in numbers
   * @param accWriteB amount of bytes written
   * @param accWriteCnt amount of write accesses in numbers
   */
  void groupUpdate(unsigned int groupId, size_t allocB, size_t accB, size_t accCnt, size_t accWriteB, size_t accWriteCnt); 
};

typedef std::unique_ptr<MemSnapshot> MemSnapshotPtr;

} // namespace Memory
#endif
