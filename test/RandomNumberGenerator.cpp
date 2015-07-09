#include <libunittest/all.hpp>
#include <generator/RandomNumberGenerator.hpp>
#include <memory>

using namespace unittest::assertions;

TEST(test_randomnumbergenerator_constant)
{
  using namespace Generic;

  VarValue val(255);
  std::unique_ptr<RandomDistribution> dist(new RandomDistConstant(val));
  RandomNumberGenerator gen(0, 4, dist);
  
  VarValue result;
  gen.getSample(result);
  assert_equal(result, 255, SPOT);

  result = 42;
  gen.getSample(result);
  assert_equal(result, 255, SPOT);
}

TEST(test_randomnumbergenerator_uniform)
{
  using namespace Generic;

  VarValue min(0xF00);
  VarValue max(0x10000);
  std::unique_ptr<RandomDistribution> dist(new RandomDistUniform(min, max));
  RandomNumberGenerator gen(0, 17, dist);
  
  VarValue res;
  for (int i = 0; i < 100000; ++i) { // all generated values should be in range
    gen.getSample(res);
    assert_true(res >= min && res <= max, SPOT);
  }
}

TEST(test_randomnumbergenerator_uniform_big)
{
  using namespace Generic;

  VarValue min("0x100000000");
  VarValue max("0xFFFF00000000");
  std::unique_ptr<RandomDistribution> dist(new RandomDistUniform(min, max));
  RandomNumberGenerator gen(1732, 48, dist);
  
  VarValue res;
  for (int i = 0; i < 100000; ++i) { // all generated values should be in range
    gen.getSample(res);
    assert_true(res >= min && res <= max, SPOT);
  }
}

TEST(test_randomnumbergenerator_uniform_seed)
{
  using namespace Generic;

  VarValue min(1024);
  VarValue max(0xFFFF);
  std::unique_ptr<RandomDistribution> dist(new RandomDistUniform(min, max));
  RandomNumberGenerator gen1(0xFEFE, 16, dist);
  RandomNumberGenerator gen2(0xABCD, 16, dist);
  RandomNumberGenerator gen3(0xFEFE, 32, dist);
 
  VarValue res1, res2, res3;
  for (int i = 0; i < 1024; ++i) { 
    gen1.getSample(res1);
    gen2.getSample(res2);
    gen3.getSample(res3);

    assert_equal(res1, res3, SPOT);
    assert_true(res1 != res2, SPOT); // very unlikely to be equal
  }
}

TEST(test_randomnumbergenerator_uniform_distribution)
{
  using namespace Generic;

  VarValue min(0);
  VarValue max(24);
  std::unique_ptr<RandomDistribution> dist(new RandomDistUniform(min, max));
  RandomNumberGenerator gen(42, 5, dist);
 
  VarValue res;
  unsigned int buckets[25];
  for (int i = 0; i < 25; buckets[i++] = 0);

  for (int i = 0; i < 250000; ++i) { 
    gen.getSample(res);
    assert_true(res >= 0 && res <= 24, SPOT);
    ++buckets[res.get_ui()];
  }

  for (int i = 0; i < 25; ++i)
    assert_approx_equal(buckets[i], (unsigned)10000, 1000, SPOT);
}

TEST(test_randomnumbergenerator_normal)
{
  using namespace Generic;

  VarValue mean(32000);
  VarValue stddev(2000);
  std::unique_ptr<RandomDistribution> dist(new RandomDistNormal(mean, stddev));
  RandomNumberGenerator gen(42, 16, dist);
 
  VarValue res;
  for (int i = 0; i < 10000; ++i) { 
    gen.getSample(res);

    assert_true(res > 22000 && res < 42000, SPOT);
  }
}

TEST(test_randomnumbergenerator_normal_ranges)
{
  using namespace Generic;

  VarValue mean(255);
  VarValue stddev(25);
  std::unique_ptr<RandomDistribution> dist(new RandomDistNormal(mean, stddev));
  RandomNumberGenerator gen(0, 8, dist);
 
  VarValue res;
  for (int i = 0; i < 10000; ++i) { 
    gen.getSample(res);
    assert_true(res >= 0 && res <= 0xFF, SPOT);
  }
}

TEST(test_randomnumbergenerator_lognormal)
{
  using namespace Generic;

  VarValue m(2);
  VarValue s(1);
  std::unique_ptr<RandomDistribution> dist(new RandomDistLognormal(m, s));
  RandomNumberGenerator gen(100, 8, dist);
 
  VarValue res;
  for (int i = 0; i < 1000; ++i) { 
    gen.getSample(res);
    assert_true(res >= 0 && res < 110, SPOT);
  }
}

TEST(test_randomnumbergenerator_exp)
{
  using namespace Generic;

  std::unique_ptr<RandomDistribution> dist(new RandomDistExponential(0.2));
  RandomNumberGenerator gen(0, 8, dist);
 
  VarValue res;
  for (int i = 0; i < 1000; ++i) { 
    gen.getSample(res);
    assert_true(res >= 0 && res < 35, SPOT);
  }
}

TEST(test_randomnumbergenerator_cauchy)
{
  using namespace Generic;

  VarValue a(128);
  std::unique_ptr<RandomDistribution> dist(new RandomDistCauchy(a, 3.14));
  RandomNumberGenerator gen(0, 8, dist);
 
  VarValue res;
  unsigned int outLimits = 0;
  for (int i = 0; i < 1000; ++i) { 
    gen.getSample(res);
    if (res < 64 || res > 192) ++outLimits;
  }
  assert_true(outLimits < 30, SPOT);
}

