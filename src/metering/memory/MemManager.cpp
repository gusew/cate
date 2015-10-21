#include <metering/memory/MemManager.hpp>
#include <metering/memory/MemTraceData.hpp>

using namespace Memory;

void MemManager::reg(RegistryItem& item) {
  _mtraces.insert(
    std::make_pair<RegistryItem*, unsigned int&>(&item, _currentGroupId)
  );

  // trace max. allocation size
  const MemTraceData& data = static_cast<const MemTraceData&>(item.getData());
  _current->groups[_currentGroupId]->allocMaxBytes += data.allocBytes; 
}

void MemManager::dereg(RegistryItem& item) {
  // check, if item is contained in collection
  if (_mtraces.count(&item) == 0) return; // nothing to do

  // get final values and add them to group values before removing item
  const MemTraceData& data = static_cast<const MemTraceData&>(item.getData());
  unsigned int groupId = _mtraces[&item];
  _current->groupUpdate(groupId, 0, data.totalBytes, data.count, data.wBytes, data.countWrite);
  _mtraces.erase(&item);
}

void MemManager::checkpoint(unsigned int headers) {
  _current->headers += headers;

  // prior to calculations: copy current snapshot for using it as next
  MemSnapshotPtr copySnapshot(new MemSnapshot(*_current));

  // update current snapshot with stats from all remaining items in collection
  for (auto iter(_mtraces.begin()); iter != _mtraces.end(); ++iter) {
    const MemTraceData& data = static_cast<const MemTraceData&>( iter->first->getData() );
    _current->groupUpdate(iter->second, data.allocBytes, data.totalBytes, data.count, data.wBytes, data.countWrite);
  }
  
  // save snapshot with aggregated values in history
  _history.push_back( std::move(_current) );
  
  // use previous copy as current snapshot
  _current = std::move(copySnapshot);
}

void MemManager::groupSwitch(unsigned int id) { 
  if (id < _groupsTotal) _currentGroupId = id;
  else throw "Given group id is invalid for switching groups.";
}

void MemManager::getCurrentTotal(MemGroupSnapshotPtr& result) const {
  // create new group for result of aggregation
  result.reset(new MemGroupSnapshot);
  
  // get all current group-values (values for _mtrace-objects are not included)
  for (auto iter(_current->groups.begin()); iter != _current->groups.end(); ++iter) 
    result->merge(*iter);

  // aggregate all current _mtrace-objects in result-group
  for (auto iter(_mtraces.begin()); iter != _mtraces.end(); ++iter) {
    const MemTraceData& data = static_cast<const MemTraceData&>( iter->first->getData() );
    result->update(data.allocBytes, data.totalBytes, data.count, data.wBytes, data.countWrite);
  }
}

void MemManager::getCurrentByGroup(unsigned int groupId, MemGroupSnapshotPtr& result) const {
  if (groupId >= _groupsTotal) throw "Given group id exceeds total number of groups (MemManager::getCurrentByGroup).";

  // create new group for result of aggregation
  result.reset(new MemGroupSnapshot);
  
  // get single group-value (values for _mtrace-objects are not included)
  result->merge( _current->groups[groupId] );

  // aggregate all current _mtrace-objects that belong to given group in result-group
  for (auto iter(_mtraces.begin()); iter != _mtraces.end(); ++iter) {
    if (iter->second == groupId) {
      const MemTraceData& data = static_cast<const MemTraceData&>( iter->first->getData() );
      result->update(data.allocBytes, data.totalBytes, data.count, data.wBytes, data.countWrite);
    }
  } 
}

void MemManager::getPastRecordTotal(unsigned int historyIdx, MemGroupSnapshotPtr& result) const {
  if (historyIdx >= _history.size()) throw "Given history index exceeds total history size (MemManager::getPastRecordTotal).";

  // create new group for result of aggregation
  result.reset(new MemGroupSnapshot);
  
  // get all group-values for history index (values for _mtrace-objects are included)
  for (auto iter(_history[historyIdx]->groups.begin()); iter != _history[historyIdx]->groups.end(); ++iter) 
    result->merge(*iter);
}

void MemManager::getPastRecordByGroup(unsigned int historyIdx, unsigned int groupId, MemGroupSnapshotPtr& result) const {
  if (groupId >= _groupsTotal) throw "Given group id exceeds total number of groups (MemManager::getPastRecordByGroup).";
  if (historyIdx >= _history.size()) throw "Given history index exceeds total history size (MemManager::getPastRecordTotal).";

  // create new group for result of aggregation
  result.reset(new MemGroupSnapshot);
  
  // get one single group-value from history index (values for _mtrace-objects are included)
  result->merge( _history[historyIdx]->groups[groupId] );
}

void MemManager::getMemResultGroups(MemResultGroups& results) const {
  // iterate over all available groups at current timepoint
  for (unsigned int group = 0; group < _groupsTotal; ++group) {
    std::unique_ptr<MemResultGroup> groupPtr(new MemResultGroup);
    
    // get values for this group in all previous timepoints
    for (unsigned int histIdx = 0; histIdx < getHistorySize(); ++histIdx) {
      // check if current group has existed at previous timepoint
      if (group < _history[histIdx]->groups.size()) {
        unsigned int headerCnt = _history[histIdx]->headers;

        groupPtr->allocBytes.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->allocBytes) );
        groupPtr->allocMaxBytes.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->allocMaxBytes) );
        groupPtr->accBytes.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->accBytes) );
        groupPtr->accCount.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->accCount) );
        groupPtr->accWriteBytes.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->accWriteBytes) );
        groupPtr->accWriteCount.push_back( std::make_pair(headerCnt, _history[histIdx]->groups[group]->accWriteCount) );
      }
    }
    results.push_back(std::move(groupPtr));
  }
}

void MemManager::reset() {
  _mtraces.clear();
  _groupsTotal = 1;
  _currentGroupId = 0;
  _history.clear();
  _current.reset(new MemSnapshot(0));
}

