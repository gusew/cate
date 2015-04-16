#include <libunittest/all.hpp>
#include <data/VarValue.hpp>

using namespace unittest::assertions;
using namespace Generic;

TEST(test_varvalue_assign)
{
	VarValue var8(0xAB);
	VarValue var16(0xABCD);
	VarValue var32(0xABCD1234);
	VarValue var64("0xABCD12345678FEDA");
	VarValue var128("0xABCD12345678FEDA1234567890123456");
  
  assert_true(var8.fits_ushort_p(), SPOT);
  assert_equal(var8, 0xAB, SPOT);
  assert_true(var16.fits_ushort_p(), SPOT);
  assert_equal(var16, 0xABCD, SPOT);
  assert_true(var32.fits_uint_p(), SPOT);
  assert_equal(var32, 0xABCD1234, SPOT);

  assert_false(var64.fits_uint_p(), SPOT);
  double cmp64 = 1.2379570966709731E19;
  assert_approx_equal(var64.get_d(), cmp64, 3000, SPOT);

  assert_false(var128.fits_uint_p(), SPOT);
  assert_false(var128.fits_ulong_p(), SPOT);
  double cmp128 = 1.8577920920438034E20;
  assert_approx_equal(var128.get_d(), cmp128, 1, SPOT);
}

TEST(test_varvalue_comparison)
{
  VarValue var8(0x86);
  VarValue var8eq(var8);
  VarValue var8lt(0x85);
	
  VarValue var16(0xA1B2);
	VarValue var16eq8(0x86);
	VarValue var16eq(var16);

	VarValue var32(0xA1B2C3D4);
	VarValue var32eq(var32);
	VarValue var32eq8(0x86);
	VarValue var32eq16(0xA1B2);

	VarValue var64("0x1111222233334444");
	VarValue var64eq(var64);
	VarValue var64eq8(0x86);
	VarValue var64eq16(0xA1B2);
	VarValue var64eq32(0xA1B2C3D4);
	
  VarValue var128("0xAAAABBBBCCCCDDDD1234123412349999");
  VarValue var128eq(var128);
  VarValue var128eq8(0x86);
  VarValue var128eq16(0xA1B2);
  VarValue var128eq32(0xA1B2C3D4);
  VarValue var128eq64("0x1111222233334444");
  VarValue var128gr("0xAAAABBBBCCCCDDDD12341234123F9999");
  VarValue var128lt8(0x85);
 
  assert_true(var8 == var8eq, SPOT);
  assert_true(var8 == var8, SPOT);
  assert_true(var8 > var8lt, SPOT);
  assert_true(var8 >= var8lt, SPOT);
  assert_true(var8 <= var8eq, SPOT);
  assert_true(var8 <= var16, SPOT);
  assert_true(var8 < var16, SPOT);
  assert_true(var8 == var16eq8, SPOT);
  assert_true(var8 == var32eq8, SPOT);
  assert_true(var8 == var64eq8, SPOT);
  assert_true(var8 == var128eq8, SPOT);
  
  assert_true(var16 == var16, SPOT);
  assert_true(var16 == var16eq, SPOT);
  assert_true(var16 > var8, SPOT);
  assert_true(var16 >= var8, SPOT);
  assert_true(var16 <= var32, SPOT);
  assert_true(var16 < var32, SPOT);
  assert_true(var16 == var32eq16, SPOT);
  assert_true(var16 == var64eq16, SPOT);
  assert_true(var16 == var128eq16, SPOT);

  assert_true(var32 == var32, SPOT);
  assert_true(var32 == var32eq, SPOT);
  assert_true(var32 > var16, SPOT);
  assert_true(var32 >= var16, SPOT);
  assert_true(var32 <= var64, SPOT);
  assert_true(var32 < var64, SPOT);
  assert_true(var32 == var64eq32, SPOT);
  assert_true(var32 == var128eq32, SPOT);

  assert_true(var64 == var64, SPOT);
  assert_true(var64 == var64eq, SPOT);
  assert_true(var64 > var32, SPOT);
  assert_true(var64 >= var32, SPOT);
  assert_true(var64 <= var128, SPOT);
  assert_true(var64 < var128, SPOT);
  assert_true(var64 == var128eq64, SPOT);

  assert_true(var128 == var128, SPOT);
  assert_true(var128 == var128eq, SPOT);
  assert_true(var128 > var64, SPOT);
  assert_true(var128 >= var64, SPOT);
  assert_true(var128 <= var128gr, SPOT);
  assert_true(var128 < var128gr, SPOT);
  assert_true(var128lt8 < var8, SPOT);
}

