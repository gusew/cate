#include <libunittest/all.hpp>
#include <memory/MemSnapshot.hpp>
#include <memory>

using namespace unittest::assertions;
using namespace Memory;

TEST(test_memsnapshot_ctor)
{
  MemSnapshot msnap1(0);
  assert_equal(msnap1.headers, (unsigned)0, SPOT);
  
  MemSnapshot msnap2(42);
  assert_equal(msnap2.headers, (unsigned)42, SPOT);
  
  MemSnapshot msnap3(0xFFFFFFFF);
  assert_equal(msnap3.headers, (unsigned)0xFFFFFFFF, SPOT);
}

TEST(test_memsnapshot_groupadd)
{
  MemSnapshot msnap(0);
  assert_equal(msnap.groups.size(), (unsigned)1, SPOT);

  msnap.groupAdd();
  assert_equal(msnap.groups.size(), (unsigned)2, SPOT);

  msnap.groupAdd();
  assert_equal(msnap.groups.size(), (unsigned)3, SPOT);

  assert_equal(msnap.headers, (unsigned)0, SPOT); // no side effects
}

TEST(test_memsnapshot_groupupdate1)
{
  MemSnapshot msnap(0);
  msnap.groupUpdate(0, 0xA110C, 0xACCB, 0xACCc4, 0xAC3E, 0xAC3Ec4);

  assert_equal(msnap.groups[0]->allocBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->allocMaxBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->accBytes, (unsigned)0xACCB, SPOT);
  assert_equal(msnap.groups[0]->accCount, (unsigned)0xACCc4, SPOT);
  assert_equal(msnap.groups[0]->accWriteBytes, (unsigned)0xAC3E, SPOT);
  assert_equal(msnap.groups[0]->accWriteCount, (unsigned)0xAC3Ec4, SPOT);

  msnap.groupUpdate(0, -0xC, 100, 2000, 42, 3333);

  assert_equal(msnap.groups[0]->allocBytes, (unsigned)0xA1100, SPOT);
  assert_equal(msnap.groups[0]->allocMaxBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->accBytes, (unsigned)(0xACCB + 100), SPOT);
  assert_equal(msnap.groups[0]->accCount, (unsigned)(0xACCc4 + 2000), SPOT);
  assert_equal(msnap.groups[0]->accWriteBytes, (unsigned)(0xAC3E + 42), SPOT);
  assert_equal(msnap.groups[0]->accWriteCount, (unsigned)(0xAC3Ec4 + 3333), SPOT);

  msnap.groupUpdate(0, -0xA1100, 11, 222, 42, 6667);

  assert_equal(msnap.groups[0]->allocBytes, (unsigned)0, SPOT);
  assert_equal(msnap.groups[0]->allocMaxBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->accBytes, (unsigned)(0xACCB + 111), SPOT);
  assert_equal(msnap.groups[0]->accCount, (unsigned)(0xACCc4 + 2222), SPOT);
  assert_equal(msnap.groups[0]->accWriteBytes, (unsigned)(0xAC3E + 2*42), SPOT);
  assert_equal(msnap.groups[0]->accWriteCount, (unsigned)(0xAC3Ec4 + 10000), SPOT);

  assert_equal(msnap.headers, (unsigned)0, SPOT); // no side effects
}

TEST(test_memsnapshot_groupupdate2)
{
  MemSnapshot msnap(0);
  msnap.groupAdd();
  msnap.groupUpdate(0, 0xA110C, 0xACCB, 0xACCc4, 0xAC3E, 0xAC3Ec4);
  msnap.groupUpdate(1, 98, 76, 54, 10, 0);
  
  assert_equal(msnap.groups[0]->allocBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->allocMaxBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap.groups[0]->accBytes, (unsigned)0xACCB, SPOT);
  assert_equal(msnap.groups[0]->accCount, (unsigned)0xACCc4, SPOT);
  assert_equal(msnap.groups[0]->accWriteBytes, (unsigned)0xAC3E, SPOT);
  assert_equal(msnap.groups[0]->accWriteCount, (unsigned)0xAC3Ec4, SPOT);

  assert_equal(msnap.groups[1]->allocBytes, (unsigned)98, SPOT);
  assert_equal(msnap.groups[1]->allocMaxBytes, (unsigned)98, SPOT);
  assert_equal(msnap.groups[1]->accBytes, (unsigned)76, SPOT);
  assert_equal(msnap.groups[1]->accCount, (unsigned)54, SPOT);
  assert_equal(msnap.groups[1]->accWriteBytes, (unsigned)10, SPOT);
  assert_equal(msnap.groups[1]->accWriteCount, (unsigned)0, SPOT);

  assert_equal(msnap.headers, (unsigned)0, SPOT); // no side effects
}

TEST(test_memsnapshot_copyctor)
{
  MemSnapshot msnap1(999);

  msnap1.groupAdd();
  msnap1.groupUpdate(0, 0xA110C, 0xACCB, 0xACCc4, 0xAC3E, 0xAC3Ec4);
  msnap1.groupUpdate(1, 98, 76, 54, 10, 0);
 
  MemSnapshot msnap2(msnap1);

  assert_equal(msnap1.headers, msnap2.headers, SPOT);

  assert_equal(msnap2.groups[0]->allocBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap2.groups[0]->allocMaxBytes, (unsigned)0xA110C, SPOT);
  assert_equal(msnap2.groups[0]->accBytes, (unsigned)0xACCB, SPOT);
  assert_equal(msnap2.groups[0]->accCount, (unsigned)0xACCc4, SPOT);
  assert_equal(msnap2.groups[0]->accWriteBytes, (unsigned)0xAC3E, SPOT);
  assert_equal(msnap2.groups[0]->accWriteCount, (unsigned)0xAC3Ec4, SPOT);

  assert_equal(msnap2.groups[1]->allocBytes, (unsigned)98, SPOT);
  assert_equal(msnap2.groups[1]->allocMaxBytes, (unsigned)98, SPOT);
  assert_equal(msnap2.groups[1]->accBytes, (unsigned)76, SPOT);
  assert_equal(msnap2.groups[1]->accCount, (unsigned)54, SPOT);
  assert_equal(msnap2.groups[1]->accWriteBytes, (unsigned)10, SPOT);
  assert_equal(msnap2.groups[1]->accWriteCount, (unsigned)0, SPOT);
}

