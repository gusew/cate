#include <libunittest/all.hpp>
#include <algorithms/DataBitvector.hpp>
#include <algorithms/AlgTestFixtures.hpp>

using namespace unittest::assertions;
using namespace DataBitvector;

TEST(test_databitvector_bv_size)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(4);
  assert_equal(bv1.getSize(), (unsigned)(unsigned)4, SPOT);
  
  Bitvector bv2(16);
  assert_equal(bv2.getSize(), (unsigned)(unsigned)16, SPOT);

  Bitvector bv3(0); 
  assert_equal(bv3.getSize(), (unsigned)0, SPOT); 
  
  Bitvector bv4(33);
  assert_equal(bv4.getSize(), (unsigned)33, SPOT); 

  Bitvector bv5(3300);
  assert_equal(bv5.getSize(), (unsigned)3300, SPOT); 

  Bitvector bv6(0xFFFF);
  assert_equal(bv6.getSize(), (unsigned)0xFFFF, SPOT); 

  //Bitvector bv6(0xFFFFFFFF); // long duration ...
  //assert_equal(bv6.getSize(), (unsigned)0xFFFFFFFF, SPOT); 
}

TEST(test_databitvector_bv_access)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(4);
  for (int i = 0; i < 4; ++i) assert_false(bv1.getBit(i), SPOT);
  bv1.setBit(3);
  for (int i = 0; i < 3; ++i) assert_false(bv1.getBit(i), SPOT);
  assert_true(bv1.getBit(3), SPOT);

  try {
    bv1.setBit(4);
    assert_true(false, SPOT);
  } catch (const char* ex) {}
  try {
    bv1.getBit(4);
    assert_true(false, SPOT);
  } catch (const char* ex) {}


  Bitvector bv2(0);
  try {
    bv2.setBit(0);
    assert_true(false, SPOT);
  } catch (const char* ex) {}
  try {
    bv2.getBit(0);
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  Bitvector bv3(0xFFFF);
  for (int i = 0; i < 0xFFFF; ++i) assert_false(bv3.getBit(i), SPOT);
  bv3.setBit(0xAFFE);
  for (int i = 0; i < 0xAFFE; ++i) assert_false(bv3.getBit(i), SPOT);
  assert_true(bv3.getBit(0xAFFE), SPOT);
  for (int i = 0xAFFF; i < 0xFFFF; ++i) assert_false(bv3.getBit(i), SPOT);
  bv3.setBit(0x666);
  for (int i = 0; i < 0x666; ++i) assert_false(bv3.getBit(i), SPOT);
  assert_true(bv3.getBit(0x666), SPOT);
  for (int i = 0x667; i < 0xAFFE; ++i) assert_false(bv3.getBit(i), SPOT);
  assert_true(bv3.getBit(0xAFFE), SPOT);
  for (int i = 0xAFFF; i < 0xFFFF; ++i) assert_false(bv3.getBit(i), SPOT);

  try {
    bv1.setBit(0x1FFFF);
    assert_true(false, SPOT);
  } catch (const char* ex) {}
  try {
    bv1.getBit(0x1FFFF);
    assert_true(false, SPOT);
  } catch (const char* ex) {}
}

TEST(test_databitvector_bv_firstbit)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(4);
  assert_equal(bv1.getFirstSetBit(), noBitSetInBitvector(), SPOT);
  bv1.setBit(3);
  assert_equal(bv1.getFirstSetBit(), (unsigned)3, SPOT);
  bv1.setBit(0);
  assert_equal(bv1.getFirstSetBit(), (unsigned)0, SPOT);

  Bitvector bv2(0);
  assert_equal(bv2.getFirstSetBit(), noBitSetInBitvector(), SPOT);

  Bitvector bv3(0xFFFF); 
  assert_equal(bv3.getFirstSetBit(), noBitSetInBitvector(), SPOT);
  bv3.setBit(0xFFFF); // max-size is limited to 0xFFFFFFFF - 1 
  assert_equal(bv3.getFirstSetBit(), (unsigned)0xFFFF, SPOT);

  bv3.setBit(12345);
  assert_equal(bv3.getFirstSetBit(), (unsigned)12345, SPOT);
}

TEST(test_databitvector_bv_copy)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector orig(8);
  orig.setBit(3);
  orig.setBit(6);

  Bitvector copy(orig);
  assert_false(copy.getBit(0), SPOT);
  assert_false(copy.getBit(1), SPOT);
  assert_false(copy.getBit(2), SPOT);
  assert_true(copy.getBit(3), SPOT);
  assert_false(copy.getBit(4), SPOT);
  assert_false(copy.getBit(5), SPOT);
  assert_true(copy.getBit(6), SPOT);
  assert_false(copy.getBit(7), SPOT);
}

TEST(test_databitvector_bv_and)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(64);
  Bitvector bv2(64);
  Bitvector bv3(70);

  bv1.setBit(4);
  bv1.setBit(16);
  bv1.setBit(32);
  bv1.setBit(40);

  bv2.setBit(16);
  bv2.setBit(32);

  bv3.setBit(32);
  bv3.setBit(40);
  bv3.setBit(64);

  // and-combinations (check also symmetry) 
  Bitvector bv1and2 = bv1 & bv2;
  Bitvector bv2and1 = bv2 & bv1;
  Bitvector bv1and3 = bv1 & bv3;
  Bitvector bv3and1 = bv3 & bv1;
  Bitvector bv2and3 = bv2 & bv3;
  Bitvector bv3and2 = bv3 & bv2;
  Bitvector bv1and2and3 = bv1 & bv2 & bv3;
  Bitvector bv3and1and2 = bv3 & bv1 & bv2;

  // check if size is correct
  assert_equal(bv1and2.getSize(), (unsigned)64, SPOT);
  assert_equal(bv2and1.getSize(), (unsigned)64, SPOT);
  assert_equal(bv1and3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3and1.getSize(), (unsigned)70, SPOT);
  assert_equal(bv2and3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3and2.getSize(), (unsigned)70, SPOT);
  assert_equal(bv1and2and3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3and1and2.getSize(), (unsigned)70, SPOT);

  // check for correct bit setting
  assert_true(bv1and2.getBit(16), SPOT);
  assert_true(bv1and2.getBit(32), SPOT);
  assert_false(bv1and2.getBit(4), SPOT);
  assert_false(bv1and2.getBit(40), SPOT);

  assert_true(bv2and1.getBit(16), SPOT);
  assert_true(bv2and1.getBit(32), SPOT);
  assert_false(bv2and1.getBit(4), SPOT);
  assert_false(bv2and1.getBit(40), SPOT);

  assert_true(bv1and3.getBit(32), SPOT);
  assert_true(bv1and3.getBit(40), SPOT);
  assert_false(bv1and3.getBit(4), SPOT);
  assert_false(bv1and3.getBit(16), SPOT);
  assert_false(bv1and3.getBit(64), SPOT);

  assert_true(bv3and1.getBit(32), SPOT);
  assert_true(bv3and1.getBit(40), SPOT);
  assert_false(bv3and1.getBit(4), SPOT);
  assert_false(bv3and1.getBit(16), SPOT);
  assert_false(bv3and1.getBit(64), SPOT);

  assert_true(bv2and3.getBit(32), SPOT);
  assert_false(bv2and3.getBit(16), SPOT);
  assert_false(bv2and3.getBit(40), SPOT);
  assert_false(bv2and3.getBit(64), SPOT);

  assert_true(bv3and2.getBit(32), SPOT);
  assert_false(bv3and2.getBit(16), SPOT);
  assert_false(bv3and2.getBit(40), SPOT);
  assert_false(bv3and2.getBit(64), SPOT);

  assert_true(bv1and2and3.getBit(32), SPOT);
  assert_false(bv1and2and3.getBit(4), SPOT);
  assert_false(bv1and2and3.getBit(16), SPOT);
  assert_false(bv1and2and3.getBit(40), SPOT);
  assert_false(bv1and2and3.getBit(64), SPOT);

  assert_true(bv3and1and2.getBit(32), SPOT);
  assert_false(bv3and1and2.getBit(4), SPOT);
  assert_false(bv3and1and2.getBit(16), SPOT);
  assert_false(bv3and1and2.getBit(40), SPOT);
  assert_false(bv3and1and2.getBit(64), SPOT);

  // check identities
  Bitvector bv1and1 = bv1 & bv1;
  Bitvector bv2and2 = bv2 & bv2;
  Bitvector bv3and3 = bv3 & bv3;

  assert_equal(bv1and1.getSize(), (unsigned)64, SPOT);
  assert_equal(bv2and2.getSize(), (unsigned)64, SPOT);
  assert_equal(bv3and3.getSize(), (unsigned)70, SPOT);

  assert_true(bv1and1.getBit(4), SPOT);
  assert_true(bv1and1.getBit(16), SPOT);
  assert_true(bv1and1.getBit(32), SPOT);
  assert_true(bv1and1.getBit(40), SPOT);

  assert_true(bv2and2.getBit(16), SPOT);
  assert_true(bv2and2.getBit(32), SPOT);

  assert_true(bv3and3.getBit(32), SPOT);
  assert_true(bv3and3.getBit(40), SPOT);
  assert_true(bv3and3.getBit(64), SPOT);

  // now check if there are any side effects on original values
  assert_true(bv1.getBit(4), SPOT);
  assert_true(bv1.getBit(16), SPOT);
  assert_true(bv1.getBit(32), SPOT);
  assert_true(bv1.getBit(40), SPOT);

  assert_true(bv2.getBit(16), SPOT);
  assert_true(bv2.getBit(32), SPOT);
  assert_false(bv2.getBit(4), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  assert_true(bv3.getBit(32), SPOT);
  assert_true(bv3.getBit(40), SPOT);
  assert_true(bv3.getBit(64), SPOT);
  assert_false(bv3.getBit(4), SPOT);
  assert_false(bv3.getBit(16), SPOT);
}

TEST(test_databitvector_bv_and_unary)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(48);
  Bitvector bv2(64);
  Bitvector bv3(8);

  bv1.setBit(2); bv1.setBit(30); bv1.setBit(40);
  bv2.setBit(6); bv2.setBit(10); bv2.setBit(30);
  bv3.setBit(2); bv3.setBit(4); bv3.setBit(6);

  bv1 &= bv2;
  assert_equal(bv1.getSize(), (unsigned)48, SPOT);

  assert_true(bv1.getBit(30), SPOT);
  assert_false(bv1.getBit(2), SPOT);
  assert_false(bv1.getBit(40), SPOT);
  assert_false(bv1.getBit(6), SPOT);
  assert_false(bv1.getBit(10), SPOT);

  // no side effects on bv2:
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  bv3 &= bv2;
  assert_equal(bv3.getSize(), (unsigned)8, SPOT);

  assert_true(bv3.getBit(6), SPOT);
  assert_false(bv3.getBit(2), SPOT);
  assert_false(bv3.getBit(4), SPOT);
 
  // no side effects on bv2:
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  bv1 &= bv3;
  assert_equal(bv1.getSize(), (unsigned)48, SPOT);

  assert_false(bv1.getBit(30), SPOT);
  assert_false(bv1.getBit(2), SPOT);
  assert_false(bv1.getBit(40), SPOT);
  assert_false(bv1.getBit(6), SPOT);
  assert_false(bv1.getBit(10), SPOT);

  // no side effects on bv3
  assert_equal(bv3.getSize(), (unsigned)8, SPOT);

  assert_true(bv3.getBit(6), SPOT);
  assert_false(bv3.getBit(2), SPOT);
  assert_false(bv3.getBit(4), SPOT);

  // check self-reference
  bv2 &= bv2;
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);
}

TEST(test_databitvector_bv_or)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(64);
  Bitvector bv2(64);
  Bitvector bv3(70);

  bv1.setBit(4); bv1.setBit(16); bv1.setBit(32); bv1.setBit(40);
  bv2.setBit(16); bv2.setBit(32);
  bv3.setBit(32); bv3.setBit(40); bv3.setBit(64);

  // or-combinations (check also symmetry) 
  Bitvector bv1or2 = bv1 | bv2;
  Bitvector bv2or1 = bv2 | bv1;
  Bitvector bv1or3 = bv1 | bv3;
  Bitvector bv3or1 = bv3 | bv1;
  Bitvector bv2or3 = bv2 | bv3;
  Bitvector bv3or2 = bv3 | bv2;
  Bitvector bv1or2or3 = bv1 | bv2 | bv3;
  Bitvector bv3or1or2 = bv3 | bv1 | bv2;

  // check if size is correct
  assert_equal(bv1or2.getSize(), (unsigned)64, SPOT);
  assert_equal(bv2or1.getSize(), (unsigned)64, SPOT);
  assert_equal(bv1or3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3or1.getSize(), (unsigned)70, SPOT);
  assert_equal(bv2or3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3or2.getSize(), (unsigned)70, SPOT);
  assert_equal(bv1or2or3.getSize(), (unsigned)70, SPOT);
  assert_equal(bv3or1or2.getSize(), (unsigned)70, SPOT);

  // check for correct bit setting
  assert_true(bv1or2.getBit(4), SPOT);
  assert_true(bv1or2.getBit(16), SPOT);
  assert_true(bv1or2.getBit(32), SPOT);
  assert_true(bv1or2.getBit(40), SPOT);

  assert_true(bv2or1.getBit(4), SPOT);
  assert_true(bv2or1.getBit(16), SPOT);
  assert_true(bv2or1.getBit(32), SPOT);
  assert_true(bv2or1.getBit(40), SPOT);

  assert_true(bv1or3.getBit(32), SPOT);
  assert_true(bv1or3.getBit(40), SPOT);
  assert_true(bv1or3.getBit(4), SPOT);
  assert_true(bv1or3.getBit(16), SPOT);
  assert_true(bv1or3.getBit(64), SPOT);

  assert_true(bv3or1.getBit(32), SPOT);
  assert_true(bv3or1.getBit(40), SPOT);
  assert_true(bv3or1.getBit(4), SPOT);
  assert_true(bv3or1.getBit(16), SPOT);
  assert_true(bv3or1.getBit(64), SPOT);

  assert_true(bv2or3.getBit(32), SPOT);
  assert_true(bv2or3.getBit(16), SPOT);
  assert_true(bv2or3.getBit(40), SPOT);
  assert_true(bv2or3.getBit(64), SPOT);

  assert_true(bv3or2.getBit(32), SPOT);
  assert_true(bv3or2.getBit(16), SPOT);
  assert_true(bv3or2.getBit(40), SPOT);
  assert_true(bv3or2.getBit(64), SPOT);

  assert_true(bv1or2or3.getBit(32), SPOT);
  assert_true(bv1or2or3.getBit(4), SPOT);
  assert_true(bv1or2or3.getBit(16), SPOT);
  assert_true(bv1or2or3.getBit(40), SPOT);
  assert_true(bv1or2or3.getBit(64), SPOT);

  assert_true(bv3or1or2.getBit(32), SPOT);
  assert_true(bv3or1or2.getBit(4), SPOT);
  assert_true(bv3or1or2.getBit(16), SPOT);
  assert_true(bv3or1or2.getBit(40), SPOT);
  assert_true(bv3or1or2.getBit(64), SPOT);

  // check identities
  Bitvector bv1or1 = bv1 & bv1;
  Bitvector bv2or2 = bv2 & bv2;
  Bitvector bv3or3 = bv3 & bv3;

  assert_equal(bv1or1.getSize(), (unsigned)64, SPOT);
  assert_equal(bv2or2.getSize(), (unsigned)64, SPOT);
  assert_equal(bv3or3.getSize(), (unsigned)70, SPOT);

  assert_true(bv1or1.getBit(4), SPOT);
  assert_true(bv1or1.getBit(16), SPOT);
  assert_true(bv1or1.getBit(32), SPOT);
  assert_true(bv1or1.getBit(40), SPOT);

  assert_true(bv2or2.getBit(16), SPOT);
  assert_true(bv2or2.getBit(32), SPOT);

  assert_true(bv3or3.getBit(32), SPOT);
  assert_true(bv3or3.getBit(40), SPOT);
  assert_true(bv3or3.getBit(64), SPOT);

  // now check if there are any side effects on original values
  assert_true(bv1.getBit(4), SPOT);
  assert_true(bv1.getBit(16), SPOT);
  assert_true(bv1.getBit(32), SPOT);
  assert_true(bv1.getBit(40), SPOT);

  assert_true(bv2.getBit(16), SPOT);
  assert_true(bv2.getBit(32), SPOT);
  assert_false(bv2.getBit(4), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  assert_true(bv3.getBit(32), SPOT);
  assert_true(bv3.getBit(40), SPOT);
  assert_true(bv3.getBit(64), SPOT);
  assert_false(bv3.getBit(4), SPOT);
  assert_false(bv3.getBit(16), SPOT);
}

TEST(test_databitvector_bv_or_unary)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Bitvector bv1(48);
  Bitvector bv2(64);
  Bitvector bv3(8);

  bv1.setBit(2); bv1.setBit(30); bv1.setBit(40);
  bv2.setBit(6); bv2.setBit(10); bv2.setBit(30);
  bv3.setBit(2); bv3.setBit(4); bv3.setBit(6);

  bv1 |= bv2;
  assert_equal(bv1.getSize(), (unsigned)48, SPOT);

  assert_true(bv1.getBit(30), SPOT);
  assert_true(bv1.getBit(2), SPOT);
  assert_true(bv1.getBit(40), SPOT);
  assert_true(bv1.getBit(6), SPOT);
  assert_true(bv1.getBit(10), SPOT);

  // no side effects on bv2:
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  bv3 |= bv2;
  assert_equal(bv3.getSize(), (unsigned)8, SPOT);

  assert_true(bv3.getBit(6), SPOT);
  assert_true(bv3.getBit(2), SPOT);
  assert_true(bv3.getBit(4), SPOT);
 
  // no side effects on bv2:
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);

  bv1 |= bv3;
  assert_equal(bv1.getSize(), (unsigned)48, SPOT);

  assert_true(bv1.getBit(30), SPOT);
  assert_true(bv1.getBit(2), SPOT);
  assert_true(bv1.getBit(40), SPOT);
  assert_true(bv1.getBit(6), SPOT);
  assert_true(bv1.getBit(10), SPOT);

  // no side effects on bv3
  assert_equal(bv3.getSize(), (unsigned)8, SPOT);

  assert_true(bv3.getBit(6), SPOT);
  assert_true(bv3.getBit(2), SPOT);
  assert_true(bv3.getBit(4), SPOT);

  // check self-reference
  bv2 |= bv2;
  assert_equal(bv2.getSize(), (unsigned)64, SPOT);

  assert_true(bv2.getBit(6), SPOT);
  assert_true(bv2.getBit(10), SPOT);
  assert_true(bv2.getBit(30), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_false(bv2.getBit(40), SPOT);
}

TEST(test_databitvector_range)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  Range<unsigned int> r1(17, 30);
  Range<unsigned int> r2(30, 20000);
  Range<unsigned int> r3(0, 0xFFFFFFFF);
  Range<unsigned int> r4(2, 16);
  Range<unsigned int> r5(2, 16);

  // test some relations
  assert_true(r1 < r2, SPOT);
  assert_false(r2 < r1, SPOT);

  assert_true(r3 < r1, SPOT);
  assert_true(r3 < r2, SPOT);

  assert_true(r4 < r1, SPOT);
  assert_true(r5 < r1, SPOT);
  
  assert_true(r4 < r2, SPOT);
  assert_true(r5 < r2, SPOT);

  assert_false(r4 < r5, SPOT);
  assert_false(r5 < r4, SPOT);
  assert_true(r4 == r5, SPOT);

  // test some overlappings
  assert_true(r1.overlaps(r2), SPOT);
  assert_true(r2.overlaps(r1), SPOT);
  assert_true(r1.overlaps(r3), SPOT);
  assert_true(r3.overlaps(r1), SPOT);
  assert_true(r2.overlaps(r3), SPOT);
  assert_true(r3.overlaps(r2), SPOT);

  assert_true(r3.overlaps(r4), SPOT);
  assert_true(r3.overlaps(r5), SPOT);
  assert_true(r4.overlaps(r3), SPOT);
  assert_true(r5.overlaps(r3), SPOT);

  assert_true(r4.overlaps(r5), SPOT);
  assert_true(r5.overlaps(r4), SPOT);

  assert_false(r1.overlaps(r4), SPOT);
  assert_false(r4.overlaps(r1), SPOT);
  assert_false(r2.overlaps(r4), SPOT);
  assert_false(r4.overlaps(r2), SPOT);

  assert_false(r1.overlaps(r5), SPOT);
  assert_false(r5.overlaps(r1), SPOT);
  assert_false(r2.overlaps(r5), SPOT);
  assert_false(r5.overlaps(r2), SPOT);
}

TEST(test_databitvector_range_search)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  DimensionRanges<unsigned short> dim(4, 0, 15);
  dim.addRange(0, 15, 0);
  dim.addRange(6, 7, 1);
  dim.addRange(10, 13, 2);
  dim.addRange(2, 8, 3);

  Range<unsigned short> rs(0, 0);
  Bitvector bv0 = dim.search(rs); rs.min = 1; rs.max = 1;
  Bitvector bv1 = dim.search(rs); rs.min = 2; rs.max = 2;
  Bitvector bv2 = dim.search(rs); rs.min = 3; rs.max = 3;
  Bitvector bv3 = dim.search(rs); rs.min = 4; rs.max = 4;
  Bitvector bv4 = dim.search(rs); rs.min = 5; rs.max = 5;
  Bitvector bv5 = dim.search(rs); rs.min = 6; rs.max = 6;
  Bitvector bv6 = dim.search(rs); rs.min = 7; rs.max = 7;
  Bitvector bv7 = dim.search(rs); rs.min = 8; rs.max = 8;
  Bitvector bv8 = dim.search(rs); rs.min = 9; rs.max = 9;
  Bitvector bv9 = dim.search(rs); rs.min = 10; rs.max = 10;
  Bitvector bv10 = dim.search(rs); rs.min = 11; rs.max = 11;
  Bitvector bv11 = dim.search(rs); rs.min = 12; rs.max = 12;
  Bitvector bv12 = dim.search(rs); rs.min = 13; rs.max = 13;
  Bitvector bv13 = dim.search(rs); rs.min = 14; rs.max = 14;
  Bitvector bv14 = dim.search(rs); rs.min = 15; rs.max = 15;
  Bitvector bv15 = dim.search(rs);
  
  // check each vector seperately
  assert_true(bv0.getBit(0), SPOT);
  assert_false(bv0.getBit(1), SPOT);
  assert_false(bv0.getBit(2), SPOT);
  assert_false(bv0.getBit(3), SPOT);
  assert_true(bv1.getBit(0), SPOT);
  assert_false(bv1.getBit(1), SPOT);
  assert_false(bv1.getBit(2), SPOT);
  assert_false(bv1.getBit(3), SPOT);

  assert_true(bv2.getBit(0), SPOT);
  assert_true(bv2.getBit(3), SPOT);
  assert_false(bv2.getBit(1), SPOT);
  assert_false(bv2.getBit(2), SPOT);
  assert_true(bv3.getBit(0), SPOT);
  assert_true(bv3.getBit(3), SPOT);
  assert_false(bv3.getBit(1), SPOT);
  assert_false(bv3.getBit(2), SPOT);
  assert_true(bv4.getBit(0), SPOT);
  assert_true(bv4.getBit(3), SPOT);
  assert_false(bv4.getBit(1), SPOT);
  assert_false(bv4.getBit(2), SPOT);
  assert_true(bv5.getBit(0), SPOT);
  assert_true(bv5.getBit(3), SPOT);
  assert_false(bv5.getBit(1), SPOT);
  assert_false(bv5.getBit(2), SPOT);

  assert_true(bv6.getBit(0), SPOT);
  assert_true(bv6.getBit(1), SPOT);
  assert_true(bv6.getBit(3), SPOT);
  assert_false(bv6.getBit(2), SPOT);
  assert_true(bv7.getBit(0), SPOT);
  assert_true(bv7.getBit(1), SPOT);
  assert_true(bv7.getBit(3), SPOT);
  assert_false(bv7.getBit(2), SPOT);

  assert_true(bv8.getBit(0), SPOT);
  assert_true(bv8.getBit(3), SPOT);
  assert_false(bv8.getBit(1), SPOT);
  assert_false(bv8.getBit(2), SPOT);

  assert_true(bv9.getBit(0), SPOT);
  assert_false(bv9.getBit(1), SPOT);
  assert_false(bv9.getBit(2), SPOT);
  assert_false(bv9.getBit(3), SPOT);
  
  assert_true(bv10.getBit(0), SPOT);
  assert_true(bv10.getBit(2), SPOT);
  assert_false(bv10.getBit(1), SPOT);
  assert_false(bv10.getBit(3), SPOT);
  assert_true(bv11.getBit(0), SPOT);
  assert_true(bv11.getBit(2), SPOT);
  assert_false(bv11.getBit(1), SPOT);
  assert_false(bv11.getBit(3), SPOT);
  assert_true(bv12.getBit(0), SPOT);
  assert_true(bv12.getBit(2), SPOT);
  assert_false(bv12.getBit(1), SPOT);
  assert_false(bv12.getBit(3), SPOT);
  assert_true(bv13.getBit(0), SPOT);
  assert_true(bv13.getBit(2), SPOT);
  assert_false(bv13.getBit(1), SPOT);
  assert_false(bv13.getBit(3), SPOT);

  assert_true(bv14.getBit(0), SPOT);
  assert_false(bv14.getBit(1), SPOT);
  assert_false(bv14.getBit(2), SPOT);
  assert_false(bv14.getBit(3), SPOT);
  assert_true(bv15.getBit(0), SPOT);
  assert_false(bv15.getBit(1), SPOT);
  assert_false(bv15.getBit(2), SPOT);
  assert_false(bv15.getBit(3), SPOT);
}

TEST(test_databitvector_range_clear)
{
  MemChronoSetup setup;
  AlgTestFixtures::setupMemChrono(setup);

  DimensionRanges<unsigned int> dim(3, 0, 0xFFFFF);
  dim.addRange(0xABC, 0xFFFFF, 0);
  dim.addRange(0xFF, 0xAFFE, 1);
  dim.addRange(10, 100000, 2);

  Range<unsigned int> rs(0, 0);

  Bitvector bv(dim.search(rs));
  assert_false(bv.getBit(0), SPOT);
  assert_false(bv.getBit(1), SPOT);
  assert_false(bv.getBit(2), SPOT);
 
  rs.min = 0xBBB; rs.max = 0xBBB;
  bv = dim.search(rs);
  assert_true(bv.getBit(0), SPOT);
  assert_true(bv.getBit(1), SPOT);
  assert_true(bv.getBit(2), SPOT);
 
  dim.reset();

  rs.min = 0; rs.max = 0;
  bv = dim.search(rs);
  assert_false(bv.getBit(0), SPOT);
  assert_false(bv.getBit(1), SPOT);
  assert_false(bv.getBit(2), SPOT);
 
  rs.min = 0xBBB; rs.max = 0xBBB;
  bv = dim.search(rs);
  assert_false(bv.getBit(0), SPOT);
  assert_false(bv.getBit(1), SPOT);
  assert_false(bv.getBit(2), SPOT);
}

