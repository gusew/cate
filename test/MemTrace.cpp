#include <libunittest/all.hpp>
#include <metering/memory/MemTrace.hpp>
#include <metering/memory/MemManager.hpp>
#include <metering/memory/MemSnapshot.hpp>
#include <metering/memory/MemTraceRegistry.hpp>
#include <memory>
#include <iostream>

using namespace unittest::assertions;
using namespace Memory;

/** Some helper functions to shorten assertions, if a measured value equals given value. */
void assert_allocBytes(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->allocBytes, s, spot);
}
void assert_allocMax(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->allocMaxBytes, s, spot);
}
void assert_accB(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->accBytes, s, spot);
}
void assert_accCnt(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->accCount, s, spot);
}
void assert_accWriteB(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->accWriteBytes, s, spot);
}
void assert_accWriteCnt(size_t s, std::shared_ptr<MemManager> manager, std::string spot) {
  MemGroupSnapshotPtr msnap;
  manager->getCurrentTotal(msnap);
  assert_equal(msnap->accWriteCount, s, spot);
}

#ifdef MEMTRACE_DISABLED
TEST(test_memtrace_disabled)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  std::cout << std::endl<<std::endl << "This test operates in restricted mode as MemTrace-functionality was disabled. To enable it, remove preprocessor define 'MEMTRACE_DISABLED'."<< std::endl<<std::endl;

  { // limit scope of var
    MemTrace<uint8_t> val(123);
    assert_allocBytes(0, manager, SPOT);
    assert_allocMax(0, manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(0, manager, SPOT);
  assert_accB(0, manager, SPOT);
  assert_accCnt(0, manager, SPOT);
  assert_accWriteB(0, manager, SPOT);
  assert_accWriteCnt(0, manager, SPOT);
}
#endif

#ifndef MEMTRACE_DISABLED
TEST(test_memtrace_podtype) 
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  { // limit scope of var
    MemTrace<uint8_t> val(123);
    assert_allocBytes(sizeof(uint8_t), manager, SPOT);
    assert_allocMax(sizeof(uint8_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint8_t), manager, SPOT);
  assert_accB(sizeof(uint8_t), manager, SPOT);
  assert_accCnt(1, manager, SPOT);
  assert_accWriteB(sizeof(uint8_t), manager, SPOT);
  assert_accWriteCnt(1, manager, SPOT);

  manager->reset();
  {
    MemTrace<uint32_t> val(42);
    assert_allocBytes(sizeof(uint32_t), manager, SPOT);
    assert_allocMax(sizeof(uint32_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint32_t), manager, SPOT);
  assert_accCnt(1, manager, SPOT);
  assert_accWriteB(sizeof(uint32_t), manager, SPOT);
  assert_accWriteCnt(1, manager, SPOT);
  assert_accB(sizeof(uint32_t), manager, SPOT);

  manager->reset();
  {
    MemTrace<uint64_t> val(0xFFFF1234ABCD);
    assert_allocBytes(sizeof(uint64_t), manager, SPOT);
    assert_allocMax(sizeof(uint64_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint64_t), manager, SPOT);
  assert_accCnt(1, manager, SPOT);
  assert_accWriteB(sizeof(uint64_t), manager, SPOT);
  assert_accWriteCnt(1, manager, SPOT);
  assert_accB(sizeof(uint64_t), manager, SPOT);

  manager->reset();
  {
    MemTrace<uint8_t> val1(0xCD);
    MemTrace<uint16_t> val2(0xABCD);
    MemTrace<uint32_t> val3(0x1234ABCD);
    MemTrace<uint64_t> val4(0xFFFF1234ABCD);
    assert_allocBytes(sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint64_t), manager, SPOT);
    assert_allocMax(sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint64_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint64_t), manager, SPOT);
  assert_accCnt(4, manager, SPOT);
  assert_accWriteB(sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint64_t), manager, SPOT);
  assert_accWriteCnt(4, manager, SPOT);
  assert_accB(sizeof(uint8_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint64_t), manager, SPOT);
}
#endif

#ifndef MEMTRACE_DISABLED
TEST(test_memtrace_usrtype)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  { // limit scope of var
    MemTrace<MemManager> val;
    val.groupCreate();
    assert_equal(val.groupGetNumber(), (unsigned)2, SPOT);

    assert_allocBytes(sizeof(MemManager), manager, SPOT);
    assert_allocMax(sizeof(MemManager), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(MemManager), manager, SPOT);
  assert_accCnt(0, manager, SPOT);
  assert_accWriteB(0, manager, SPOT);
  assert_accWriteCnt(0, manager, SPOT);
  assert_accB(0, manager, SPOT);
}
#endif

#ifndef MEMTRACE_DISABLED
TEST(test_memtrace_assign_podtype)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  { // limit scope of var
    MemTrace<uint32_t> val(0x1234E0E0);
    assert_equal(val, (unsigned)0x1234E0E0, SPOT);
    assert_accCnt(2, manager, SPOT);
    assert_accWriteB(sizeof(uint32_t), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(sizeof(uint32_t)*2, manager, SPOT);

    uint32_t other = 22222;
    val = other;
    assert_accCnt(3, manager, SPOT);
    assert_accWriteB(2*sizeof(uint32_t), manager, SPOT);
    assert_accWriteCnt(2, manager, SPOT);
    assert_accB(sizeof(uint32_t)*3, manager, SPOT);
    assert_equal(val, other, SPOT);
    assert_equal(val, (unsigned)22222, SPOT);
    assert_accCnt(5, manager, SPOT);
    assert_accWriteB(2*sizeof(uint32_t), manager, SPOT);
    assert_accWriteCnt(2, manager, SPOT);
    assert_accB(sizeof(uint32_t)*5, manager, SPOT);

    val = 42;
    assert_equal(val, (unsigned)42, SPOT);
    other = val;
    assert_accCnt(8, manager, SPOT);
    assert_accWriteB(3*sizeof(uint32_t), manager, SPOT);
    assert_accWriteCnt(3, manager, SPOT);
    assert_accB(sizeof(uint32_t)*8, manager, SPOT);

    assert_equal(val, (unsigned)42, SPOT);
    assert_equal(other, (unsigned)42, SPOT);
    assert_accCnt(9, manager, SPOT);
    assert_accWriteB(3*sizeof(uint32_t), manager, SPOT);
    assert_accWriteCnt(3, manager, SPOT);
    assert_accB(sizeof(uint32_t)*9, manager, SPOT);

    assert_allocBytes(sizeof(uint32_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint32_t), manager, SPOT);
  assert_accCnt(9, manager, SPOT);
  assert_accWriteB(3*sizeof(uint32_t), manager, SPOT);
  assert_accWriteCnt(3, manager, SPOT);
  assert_accB(sizeof(uint32_t)*9, manager, SPOT); 
}
#endif

#ifndef MEMTRACE_DISABLED
bool fct() { return true; }
TEST(test_memtrace_rvalue)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  {
    MemTrace<bool> val(fct());
    assert_accCnt(1, manager, SPOT);
    assert_accWriteB(sizeof(bool), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(sizeof(bool), manager, SPOT);
    
    assert_true(val, SPOT);
    assert_accCnt(2, manager, SPOT);
    assert_accWriteB(sizeof(bool), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(2*sizeof(bool), manager, SPOT);
    assert_allocMax(sizeof(bool), manager, SPOT);

    val = false;
    assert_accCnt(3, manager, SPOT);
    assert_accWriteB(2*sizeof(bool), manager, SPOT);
    assert_accWriteCnt(2, manager, SPOT);
    assert_accB(3*sizeof(bool), manager, SPOT);
    
    assert_true(!val, SPOT);
    assert_allocBytes(sizeof(bool), manager, SPOT);
    assert_allocMax(2*sizeof(bool), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(2*sizeof(bool), manager, SPOT);
}
#endif

TEST(test_memtrace_cast_podtype)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  MemTrace<char> val1;
  MemTrace<int> val2(999222333);
  val1 = 42;
  val2 = val1;
  char val3 = val1; // works too, thanx to implicit conversion

  assert_equal(val1, 42, SPOT);
  assert_equal(val2, 42, SPOT);
  assert_equal(val3, 42, SPOT);
}

TEST(test_memtrace_getptr)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  MemTrace<uint8_t> val1;
  val1 = 42;
  assert_equal(val1, (unsigned)42, SPOT);

  uint8_t* ptr = val1.getPtr();
  *ptr = 88;
  assert_equal(val1, (unsigned)88, SPOT);
}


TEST(test_memtrace_op_comparison)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  MemTrace<uint8_t> val1(42);
  MemTrace<uint16_t> val2(42);
  MemTrace<uint32_t> val3(0xABCD);
  MemTrace<uint64_t> val4(0);

  assert_true(val1 == 42, SPOT);
  assert_true(val1 == val2, SPOT);
  assert_true(val2 == val1, SPOT);
  assert_true(val1 <= val2, SPOT);
  assert_true(val2 <= val1, SPOT);
  assert_true(val1 <= 88, SPOT);
  assert_true(val1 >= val2, SPOT);
  assert_true(val2 >= val1, SPOT);
  assert_true(val2 >= 27, SPOT);

  assert_true(val1 != 17, SPOT);
  assert_true(val1 != val3, SPOT);
  assert_true(val3 != val1, SPOT);
  assert_true(val1 < val3, SPOT);
  assert_true(val3 > val1, SPOT);
  assert_true(val3 > val2, SPOT);
  assert_true(val1 < 255, SPOT);
  assert_true(val1 > 5, SPOT);
  assert_true(val2 > val4, SPOT);
  assert_true(val4 < val3, SPOT);
}

TEST(test_memtrace_op_arithmetic_binary)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  MemTrace<uint32_t> val1(2);
  MemTrace<uint32_t> val2(100);
  uint32_t val3 = 1000;
  uint32_t val4 = 10;

  val1 += 10;
  val2 -= 10;
  assert_equal(val1, (unsigned)12, SPOT);
  assert_equal(val2, (unsigned)90, SPOT);

  val1 /= 6;
  val2 *= 2;
  assert_equal(val1, (unsigned)2, SPOT);
  assert_equal(val2, (unsigned)180, SPOT);

  val1 += val3;
  val2 -= val4;
  assert_equal(val1, (unsigned)1002, SPOT);
  assert_equal(val2, (unsigned)170, SPOT);

  val1 /= val3;
  val2 *= val4;
  assert_equal(val1, (unsigned)1, SPOT);
  assert_equal(val2, (unsigned)1700, SPOT);

  val1 = val2 - val1;
  val2 = val2 + val1; 
  assert_equal(val1, (unsigned)1699, SPOT);
  assert_equal(val2, (unsigned)3399, SPOT);

  val1 = val2 + 1;
  val2 = val1 - val3;
  assert_equal(val1, (unsigned)3400, SPOT);
  assert_equal(val2, (unsigned)2400, SPOT);

  val1 = val2 % val3;
  val2 = val2 % val1;
  assert_equal(val1, (unsigned)400, SPOT);
  assert_equal(val2, (unsigned)0, SPOT);

  val1 = val1 / val4;
  val2 = val1 * val3;
  assert_equal(val1, (unsigned)40, SPOT);
  assert_equal(val2, (unsigned)40000, SPOT);

  val1 = val2 / val1;
  val2 = val1 * val2;
  assert_equal(val1, (unsigned)1000, SPOT);
  assert_equal(val2, (unsigned)40000000, SPOT);

  val1 = 12;
  val2 = 43;
  val2 %= val1;
  val1 %= val4;
  assert_equal(val1, (unsigned)2, SPOT);
  assert_equal(val2, (unsigned)7, SPOT);

  val1 = val1 & val2;
  val2 = val1 & val4;
  assert_equal(val1, (unsigned)2 & 7, SPOT);
  assert_equal(val2, (unsigned)2 & 10, SPOT);
  
  // and so forth: ^ | && ||  %= <<= >>= << >> &= ^= |= 
}

TEST(test_memtrace_op_arithmetic_unary)
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  MemTrace<uint16_t> val1(42);
  MemTrace<uint16_t> val2(val1--);
  MemTrace<uint16_t> val3(val1++);

  assert_equal(val1, (unsigned)42, SPOT);
  assert_equal(val2, (unsigned)42, SPOT);
  assert_equal(val3, (unsigned)41, SPOT);

  ++val3;
  --val2;

  assert_equal(val2, (unsigned)41, SPOT);
  assert_equal(val3, (unsigned)42, SPOT);
}

#ifndef MEMTRACE_DISABLED
struct ExampleClass { uint32_t value; };

TEST(test_memtrace_pointer) 
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  uint32_t val0 = 42;
  { // limit scope of var
    MemTrace<uint32_t*> valptr(&val0);
    assert_accCnt(1, manager, SPOT);
    assert_accWriteB(sizeof(uint32_t*), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(sizeof(uint32_t*), manager, SPOT);
    *valptr = 99;
    assert_allocBytes(sizeof(uint32_t*), manager, SPOT);
    assert_allocMax(sizeof(uint32_t*), manager, SPOT);
    assert_accCnt(2, manager, SPOT);
    assert_accWriteB(sizeof(uint32_t*), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(2*sizeof(uint32_t*), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint32_t*), manager, SPOT);
  assert_accCnt(2, manager, SPOT);
  assert_accWriteB(sizeof(uint32_t*), manager, SPOT);
  assert_accWriteCnt(1, manager, SPOT);
  assert_accB(2*sizeof(uint32_t*), manager, SPOT);
  assert_equal(val0, (unsigned)99, SPOT);

  manager->reset();
  {
    MemTrace<uint64_t*> valptr(new uint64_t(0xABCD));
    *valptr = 123456;
    assert_equal(*valptr, (unsigned)123456, SPOT);
    delete valptr;
    assert_allocBytes(sizeof(uint64_t*), manager, SPOT);
    assert_allocMax(sizeof(uint64_t*), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint64_t*), manager, SPOT);

  manager->reset();
  {
    MemTrace<MemTrace<uint16_t>*> valptr(new MemTrace<uint16_t>(42));
    *valptr = 7;
    assert_equal(*valptr, (unsigned)7, SPOT);
    delete valptr;
    assert_allocBytes(sizeof(MemTrace<uint16_t>*), manager, SPOT);
    assert_allocMax(sizeof(MemTrace<uint16_t>*)+sizeof(uint16_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(MemTrace<uint16_t>*)+sizeof(uint16_t), manager, SPOT);

  manager->reset();
  {
    std::shared_ptr<uint16_t> shared1(new uint16_t(642));
    MemTrace<std::shared_ptr<uint16_t>> valptr(shared1);
    *valptr = 7;
    assert_equal(*shared1, 7, SPOT);

    assert_allocBytes(sizeof(std::shared_ptr<uint16_t>), manager, SPOT);
    assert_allocMax(sizeof(std::shared_ptr<uint16_t>), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(std::shared_ptr<uint16_t>), manager, SPOT);

  manager->reset();
  {
    ExampleClass excl;
    excl.value = 42;
    MemTrace<ExampleClass*> excl_ptr(&excl);
    excl_ptr->value = 90;
    assert_accCnt(2, manager, SPOT);
    assert_accWriteB(sizeof(ExampleClass*), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(2*sizeof(ExampleClass*), manager, SPOT);
    excl_ptr->value += 10;
    assert_accCnt(3, manager, SPOT);
    assert_accWriteB(sizeof(ExampleClass*), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(3*sizeof(ExampleClass*), manager, SPOT);

    assert_equal(excl_ptr->value, (unsigned)100, SPOT);

    assert_allocBytes(sizeof(ExampleClass*), manager, SPOT);
    assert_allocMax(sizeof(ExampleClass*), manager, SPOT);
    assert_accCnt(4, manager, SPOT);
    assert_accWriteB(sizeof(ExampleClass*), manager, SPOT);
    assert_accWriteCnt(1, manager, SPOT);
    assert_accB(4*sizeof(ExampleClass*), manager, SPOT);
  }
  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(ExampleClass*), manager, SPOT);
}
#endif

#ifndef MEMTRACE_DISABLED
TEST(test_memtrace_array) 
{
  std::shared_ptr<MemManager> manager = std::make_shared<MemManager>();
  std::shared_ptr<MemTraceRegistry> memreg = std::make_shared<MemTraceRegistry>(manager);
  memRegistryPtr = memreg; // for registering MemTrace-instances

  {
    MemTrace<int[4]> arr1;
    MemTrace<MemTrace<int>[4]> arr2;

    for (int i = 0; i < 4; ++i) {
      arr1[i] = i * 100;
      arr2[i] = i * 1000;
    }

    for (int i = 0; i < 4; ++i) {
      assert_equal(arr1[i], i*100, SPOT);
      assert_equal(arr2[i], i*1000, SPOT);
    }

    assert_allocBytes(4*sizeof(MemTrace<int>) + 8*sizeof(int), manager, SPOT);
    assert_allocMax(4*sizeof(MemTrace<int>) + 8*sizeof(int), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(4*sizeof(MemTrace<int>) + 8*sizeof(int), manager, SPOT);

  manager->reset();
  {
    MemTrace<uint16_t*> valptr(new uint16_t[16]); // measure only pointer to first element
    
    for (int i = 0; i < 16; ++i) {
      valptr[i] = i * 100;
    }

    for (int i = 0; i < 16; ++i) {
      assert_equal(valptr[i], i*100, SPOT);
    }

    delete[] valptr;
    assert_allocBytes(sizeof(uint16_t*), manager, SPOT);
    assert_allocMax(sizeof(uint16_t*), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(uint16_t*), manager, SPOT);

  manager->reset();
  {
    MemTrace<MemTrace<uint16_t>*> valptr(new MemTrace<uint16_t>[16]); // measure whole array
    
    for (int i = 0; i < 16; ++i) {
      valptr[i] = i * 100;
    }

    for (int i = 0; i < 16; ++i) {
      assert_equal(valptr[i], i*100, SPOT);
    }

    assert_allocBytes(sizeof(MemTrace<uint16_t>*) + 16*sizeof(uint16_t), manager, SPOT);
    delete[] valptr;
    assert_allocBytes(sizeof(MemTrace<MemTrace<uint16_t>>*), manager, SPOT);
    assert_allocMax(sizeof(MemTrace<uint16_t>*) + 16*sizeof(uint16_t), manager, SPOT);
  }

  assert_allocBytes(0, manager, SPOT);
  assert_allocMax(sizeof(MemTrace<uint16_t>*) + 16*sizeof(uint16_t), manager, SPOT);
}
#endif

