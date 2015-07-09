#include <metering/memory/MemSnapshot.hpp>

using namespace Memory;

void MemGroupSnapshot::update(size_t allocB, size_t accB, size_t accCnt, size_t accWriteB, size_t accWriteCnt) {
  allocBytes += allocB;
  if (allocMaxBytes < allocBytes) allocMaxBytes = allocBytes;
  accBytes += accB;
  accCount += accCnt;
  accWriteBytes += accWriteB;
  accWriteCount += accWriteCnt;
}

void MemGroupSnapshot::merge(const std::unique_ptr<MemGroupSnapshot>& other) { 
  if (!other) throw "No MemGroupSnapshot object for merging passed (MemSnapshot::merge).";

  // extra: set allocation maximum manually
  allocMaxBytes += other->allocMaxBytes;

  // handle variables with simple update
  update(other->allocBytes, other->accBytes, other->accCount, other->accWriteBytes, other->accWriteCount); 
}

MemSnapshot::MemSnapshot(const MemSnapshot& other) : headers(other.headers) {
  // deep copy of all group-snapshots
  for (auto iter(other.groups.begin()); iter != other.groups.end(); ++iter) {
    MemGroupSnapshotPtr copyval(new MemGroupSnapshot(**iter));
    groups.push_back(std::move(copyval));
  }
}

void MemSnapshot::groupUpdate(unsigned int groupId, size_t allocB, size_t accB, size_t accCnt, size_t accWriteB, size_t accWriteCnt) {
  if (groupId >= groups.size()) throw "Group id in MemSnapshot-groupUpdate() exceeds maximum group id.";
  
  // delegate update to correct group
  groups[groupId]->update(allocB, accB, accCnt, accWriteB, accWriteCnt);
}

