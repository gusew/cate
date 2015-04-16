#include <libunittest/all.hpp>
#include <memory/MemManager.hpp>
#include <memory/MemSnapshot.hpp>
#include <memory/Registry.hpp>
#include <memory/MemTraceData.hpp>
#include <memory>
#include <limits>

using namespace unittest::assertions;
using namespace Memory;

struct TestItem : public RegistryItem {
  MemTraceData data;
  TestItem(size_t allocB, size_t totalB, size_t accCnt, size_t accWriteB, size_t accWriteCnt) {
    data.allocBytes = allocB;
    data.totalBytes = totalB;
    data.count = accCnt;
    data.wBytes = accWriteB;
    data.countWrite = accWriteCnt;
  }
  const RegistryData& getData() const override { return data; }
};

TEST(test_memmanager_reg_dereg)
{
  MemManager m;
  TestItem item1(0xa110c, 0xaccA11, 0xacc123, 64, 4);
  TestItem item2(16, 32, 2, 16, 1);

  m.reg(item1);
  
  MemGroupSnapshotPtr msnap;
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.reg(item2);

  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  m.dereg(item2);

  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  m.dereg(item1);
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  assert_equal(m.getCurrentHeaderCount(), (unsigned)0, SPOT); // no side effects
}

TEST(test_memmanager_checkpoint)
{
  MemManager m;
  TestItem item1(0xa110c, 0xaccA11, 0xacc123, 64, 4);

  m.reg(item1);
  
  MemGroupSnapshotPtr msnap;
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  assert_equal(m.getHistorySize(), (unsigned)0, SPOT);
  m.checkpoint(1);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)1, SPOT);

  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  assert_equal(m.getHistorySize(), (unsigned)1, SPOT);
  m.getPastRecordTotal(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.checkpoint(998);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)999, SPOT);

  m.dereg(item1);

  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.getPastRecordTotal(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  assert_equal(m.getHistorySize(), (unsigned)2, SPOT);
  m.getPastRecordTotal(1, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);
}

TEST(test_memmanager_groups)
{
  MemManager m;
  TestItem item1(0xa110c, 0xaccA11, 0xacc123, 64, 4);
  TestItem item2(16, 32, 2, 16, 1);

  assert_equal(m.groupGetAmount(), (unsigned)1, SPOT);

  m.groupCreate();
  assert_equal(m.groupGetAmount(), (unsigned)2, SPOT);
  
  m.reg(item1);
  
  MemGroupSnapshotPtr msnap;
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.getCurrentByGroup(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accCount, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)0, SPOT);

  m.getCurrentByGroup(1, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.groupSwitch(0);
  assert_equal(m.groupGetAmount(), (unsigned)2, SPOT);
  
  m.reg(item2);
 
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  m.getCurrentByGroup(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)32, SPOT);
  assert_equal(msnap->accCount, (unsigned)2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)1, SPOT);

  m.getCurrentByGroup(1, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.checkpoint(0xABCDEF0);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)0xABCDEF0, SPOT);
  assert_equal(m.getHistorySize(), (unsigned)1, SPOT);
  
  m.dereg(item1);
  
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  m.getCurrentByGroup(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)32, SPOT);
  assert_equal(msnap->accCount, (unsigned)2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)1, SPOT);

  m.getCurrentByGroup(1, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.getPastRecordTotal(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c + 16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11 + 32, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123 + 2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64 + 16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4 + 1, SPOT);

  m.getPastRecordByGroup(0, (unsigned)0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)16, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accBytes, (unsigned)32, SPOT);
  assert_equal(msnap->accCount, (unsigned)2, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)16, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)1, SPOT);

  m.getPastRecordByGroup(0, 1, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);
}

TEST(test_memmanager_results)
{
  MemManager m;
  TestItem item1(1, 1, 1, 1, 1);
  TestItem item2(4, 4, 2, 2, 1);
  TestItem item3(16, 32, 8, 16, 4);

  // history[0]: 1 group, just item1, (unsigned)0 headers
  m.reg(item1);
  m.checkpoint(0); 

  // history[1]: 1 group, item1 & item2, 10 headers
  m.reg(item2);
  m.checkpoint(10);

  // history[2]: 2 groups, items 1&2&3, 10 headers
  m.groupCreate();
  m.reg(item3);
  m.checkpoint(0);

  // history[3]: 2 groups, items 1&3, 11 headers
  m.dereg(item2);
  m.checkpoint(1);

  // history[4]: 2 groups, item 3, 12 headers
  m.dereg(item1);
  m.checkpoint(1);

  // history[5]: 2 groups, no items, 13 headers
  m.dereg(item3);
  m.checkpoint(1);

  MemResultGroups resgroups;
  m.getMemResultGroups(resgroups);
  
  assert_equal(resgroups.size(), (unsigned)2, SPOT);
  
  // examine group 0
  assert_equal(resgroups[0]->allocBytes.size(), (unsigned)6, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes.size(), (unsigned)6, SPOT);
  assert_equal(resgroups[0]->accBytes.size(), (unsigned)6, SPOT);
  assert_equal(resgroups[0]->accCount.size(), (unsigned)6, SPOT);
  assert_equal(resgroups[0]->accWriteBytes.size(), (unsigned)6, SPOT);
  assert_equal(resgroups[0]->accWriteCount.size(), (unsigned)6, SPOT);
 
  // history[0], headers
  assert_equal(resgroups[0]->allocBytes[0].first, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[0].first, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->accBytes[0].first, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->accCount[0].first, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[0].first, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->accWriteCount[0].first, (unsigned)0, SPOT);
  // history[1], headers
  assert_equal(resgroups[0]->allocBytes[1].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[1].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accBytes[1].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accCount[1].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[1].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accWriteCount[1].first, (unsigned)10, SPOT);
  // history[2], headers
  assert_equal(resgroups[0]->allocBytes[2].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[2].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accBytes[2].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accCount[2].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[2].first, (unsigned)10, SPOT);
  assert_equal(resgroups[0]->accWriteCount[2].first, (unsigned)10, SPOT);
  // history[3], headers
  assert_equal(resgroups[0]->allocBytes[3].first, (unsigned)11, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[3].first, (unsigned)11, SPOT);
  assert_equal(resgroups[0]->accBytes[3].first, (unsigned)11, SPOT);
  assert_equal(resgroups[0]->accCount[3].first, (unsigned)11, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[3].first, (unsigned)11, SPOT);
  assert_equal(resgroups[0]->accWriteCount[3].first, (unsigned)11, SPOT);
  // history[4], headers
  assert_equal(resgroups[0]->allocBytes[4].first, (unsigned)12, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[4].first, (unsigned)12, SPOT);
  assert_equal(resgroups[0]->accBytes[4].first, (unsigned)12, SPOT);
  assert_equal(resgroups[0]->accCount[4].first, (unsigned)12, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[4].first, (unsigned)12, SPOT);
  assert_equal(resgroups[0]->accWriteCount[4].first, (unsigned)12, SPOT);
  // history[5], headers
  assert_equal(resgroups[0]->allocBytes[5].first, (unsigned)13, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[5].first, (unsigned)13, SPOT);
  assert_equal(resgroups[0]->accBytes[5].first, (unsigned)13, SPOT);
  assert_equal(resgroups[0]->accCount[5].first, (unsigned)13, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[5].first, (unsigned)13, SPOT);
  assert_equal(resgroups[0]->accWriteCount[5].first, (unsigned)13, SPOT);

  // history[0], values
  assert_equal(resgroups[0]->allocBytes[0].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[0].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->accBytes[0].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->accCount[0].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[0].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->accWriteCount[0].second, (unsigned)1, SPOT);
  // history[1], values
  assert_equal(resgroups[0]->allocBytes[1].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[1].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accBytes[1].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accCount[1].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[1].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteCount[1].second, (unsigned)2, SPOT);
  // history[2], values
  assert_equal(resgroups[0]->allocBytes[2].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[2].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accBytes[2].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accCount[2].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[2].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteCount[2].second, (unsigned)2, SPOT);
  // history[3], values
  assert_equal(resgroups[0]->allocBytes[3].second, (unsigned)1, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[3].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accBytes[3].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accCount[3].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[3].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteCount[3].second, (unsigned)2, SPOT);
  // history[4], values
  assert_equal(resgroups[0]->allocBytes[4].second, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[4].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accBytes[4].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accCount[4].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[4].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteCount[4].second, (unsigned)2, SPOT);
  // history[5], values
  assert_equal(resgroups[0]->allocBytes[5].second, (unsigned)0, SPOT);
  assert_equal(resgroups[0]->allocMaxBytes[5].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accBytes[5].second, (unsigned)5, SPOT);
  assert_equal(resgroups[0]->accCount[5].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteBytes[5].second, (unsigned)3, SPOT);
  assert_equal(resgroups[0]->accWriteCount[5].second, (unsigned)2, SPOT);


  // examine group 1
  assert_equal(resgroups[1]->allocBytes.size(), (unsigned)4, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes.size(), (unsigned)4, SPOT);
  assert_equal(resgroups[1]->accBytes.size(), (unsigned)4, SPOT);
  assert_equal(resgroups[1]->accCount.size(), (unsigned)4, SPOT);
  assert_equal(resgroups[1]->accWriteBytes.size(), (unsigned)4, SPOT);
  assert_equal(resgroups[1]->accWriteCount.size(), (unsigned)4, SPOT);

  // history[0], headers
  assert_equal(resgroups[1]->allocBytes[0].first, (unsigned)10, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[0].first, (unsigned)10, SPOT);
  assert_equal(resgroups[1]->accBytes[0].first, (unsigned)10, SPOT);
  assert_equal(resgroups[1]->accCount[0].first, (unsigned)10, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[0].first, (unsigned)10, SPOT);
  assert_equal(resgroups[1]->accWriteCount[0].first, (unsigned)10, SPOT);
  // history[1], headers
  assert_equal(resgroups[1]->allocBytes[1].first, (unsigned)11, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[1].first, (unsigned)11, SPOT);
  assert_equal(resgroups[1]->accBytes[1].first, (unsigned)11, SPOT);
  assert_equal(resgroups[1]->accCount[1].first, (unsigned)11, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[1].first, (unsigned)11, SPOT);
  assert_equal(resgroups[1]->accWriteCount[1].first, (unsigned)11, SPOT);
  // history[2], headers
  assert_equal(resgroups[1]->allocBytes[2].first, (unsigned)12, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[2].first, (unsigned)12, SPOT);
  assert_equal(resgroups[1]->accBytes[2].first, (unsigned)12, SPOT);
  assert_equal(resgroups[1]->accCount[2].first, (unsigned)12, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[2].first, (unsigned)12, SPOT);
  assert_equal(resgroups[1]->accWriteCount[2].first, (unsigned)12, SPOT);
  // history[3], headers
  assert_equal(resgroups[1]->allocBytes[3].first, (unsigned)13, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[3].first, (unsigned)13, SPOT);
  assert_equal(resgroups[1]->accBytes[3].first, (unsigned)13, SPOT);
  assert_equal(resgroups[1]->accCount[3].first, (unsigned)13, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[3].first, (unsigned)13, SPOT);
  assert_equal(resgroups[1]->accWriteCount[3].first, (unsigned)13, SPOT);

  // history[0], values
  assert_equal(resgroups[1]->allocBytes[0].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[0].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accBytes[0].second, (unsigned)32, SPOT);
  assert_equal(resgroups[1]->accCount[0].second, (unsigned)8, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[0].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accWriteCount[0].second, (unsigned)4, SPOT);
  // history[1], values
  assert_equal(resgroups[1]->allocBytes[1].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[1].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accBytes[1].second, (unsigned)32, SPOT);
  assert_equal(resgroups[1]->accCount[1].second, (unsigned)8, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[1].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accWriteCount[1].second, (unsigned)4, SPOT);
  // history[2], values
  assert_equal(resgroups[1]->allocBytes[2].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[2].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accBytes[2].second, (unsigned)32, SPOT);
  assert_equal(resgroups[1]->accCount[2].second, (unsigned)8, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[2].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accWriteCount[2].second, (unsigned)4, SPOT);
  // history[3], values
  assert_equal(resgroups[1]->allocBytes[3].second, (unsigned)0, SPOT);
  assert_equal(resgroups[1]->allocMaxBytes[3].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accBytes[3].second, (unsigned)32, SPOT);
  assert_equal(resgroups[1]->accCount[3].second, (unsigned)8, SPOT);
  assert_equal(resgroups[1]->accWriteBytes[3].second, (unsigned)16, SPOT);
  assert_equal(resgroups[1]->accWriteCount[3].second, (unsigned)4, SPOT);
}

TEST(test_memmanager_reset)
{
  MemManager m;
  TestItem item1(0xa110c, 0xaccA11, 0xacc123, 64, 4);

  m.groupCreate();
  assert_equal(m.groupGetCurrentId(), (unsigned)1, SPOT);

  m.reg(item1);
  m.checkpoint(42);
  m.dereg(item1);

  m.groupCreate();
  assert_equal(m.groupGetCurrentId(), (unsigned)2, SPOT);
  m.groupSwitch(1);
  assert_equal(m.groupGetCurrentId(), (unsigned)1, SPOT);

  m.checkpoint(8);

  m.reset();

  assert_equal(m.groupGetAmount(), (unsigned)1, SPOT);
  assert_equal(m.groupGetCurrentId(), (unsigned)0, SPOT);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)0, SPOT);
  assert_equal(m.getHistorySize(), (unsigned)0, SPOT);
  
  MemGroupSnapshotPtr msnap;
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accCount, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)0, SPOT);

  m.getCurrentByGroup(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accCount, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)0, SPOT);
  
  // second run, to check if everything is functional
  m.groupCreate();
  assert_equal(m.groupGetCurrentId(), (unsigned)1, SPOT);
 
  m.reg(item1);
  m.checkpoint(42);
  m.dereg(item1);

  m.groupCreate();
  assert_equal(m.groupGetCurrentId(), (unsigned)2, SPOT);
  m.groupSwitch(1);
  assert_equal(m.groupGetCurrentId(), (unsigned)1, SPOT);

  m.checkpoint(8);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)50, SPOT);
  assert_equal(m.getHistorySize(), (unsigned)2, SPOT);
  assert_equal(m.groupGetAmount(), (unsigned)3, SPOT);

  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0xa110c, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0xaccA11, SPOT);
  assert_equal(msnap->accCount, (unsigned)0xacc123, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)64, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)4, SPOT);

  m.reset();

  assert_equal(m.groupGetAmount(), (unsigned)1, SPOT);
  assert_equal(m.groupGetCurrentId(), (unsigned)0, SPOT);
  assert_equal(m.getCurrentHeaderCount(), (unsigned)0, SPOT);
  assert_equal(m.getHistorySize(), (unsigned)0, SPOT);
  
  m.getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accCount, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)0, SPOT);

  m.getCurrentByGroup(0, msnap);
  assert_equal(msnap->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap->allocMaxBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accCount, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteBytes, (unsigned)0, SPOT);
  assert_equal(msnap->accWriteCount, (unsigned)0, SPOT);
}


