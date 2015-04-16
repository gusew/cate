#include <libunittest/all.hpp>
#include <core/HeaderGenerator.hpp>
#include <memory>

using namespace unittest::assertions;

TEST(test_headergenerator_init)
{
  using namespace Generic;

  HeaderGenerator generator;
  PacketHeaderSet headers;

  try { // not configured
    generator.generateHeaders(1000, headers);
    assert_true(false, SPOT);
  } catch (const char* ch) {}

  // create simplest configuration
  FieldStructureSet fields;
  fields.push_back(16);
  
  RandomHeaderConfiguration rndConfig;
  rndConfig.totalHeaders = 200;
  VarValue constant1 = 42;
  std::unique_ptr<RandomDistribution> distrib1(new RandomDistConstant(constant1));
  rndConfig.distributions.push_back(std::move(distrib1));
  rndConfig.seeds.push_back(0);

  assert_true(generator.configure(fields, rndConfig), SPOT);

  generator.generateHeaders(50, headers);
  assert_equal(headers.size(), (unsigned)50, SPOT);

  for (PacketHeaderSet::const_iterator hdr(headers.begin()); hdr != headers.end(); ++hdr) {
    assert_equal((*hdr)->size(), (unsigned)1, SPOT);

    for (PacketHeaderLine::const_iterator line((*hdr)->begin()); line != (*hdr)->end(); ++line) {
      assert_equal((*line)->value, 42, SPOT);
    }
  }

  // test if already existing elements in header set are cleared
  generator.generateHeaders(150, headers);
  assert_equal(headers.size(), (unsigned)150, SPOT);
  
  for (PacketHeaderSet::const_iterator hdr(headers.begin()); hdr != headers.end(); ++hdr) {
    assert_equal((*hdr)->size(), (unsigned)1, SPOT);
    for (PacketHeaderLine::const_iterator line((*hdr)->begin()); line != (*hdr)->end(); ++line) {
      assert_equal((*line)->value, 42, SPOT);
    }
  }
}

TEST(test_headergenerator_reconfigure)
{
  using namespace Generic;

  HeaderGenerator generator;
  PacketHeaderSet headers;

  // create simplest configuration
  FieldStructureSet fields;
  fields.push_back(32);
  
  RandomHeaderConfiguration rndConfig;
  rndConfig.totalHeaders = 20;
  VarValue constant1 = 0xFFFFABCD;
  VarValue constant2 = 0xF0000000;
  std::unique_ptr<RandomDistribution> distrib1(new RandomDistConstant(constant1));
  std::unique_ptr<RandomDistribution> distrib2(new RandomDistConstant(constant2));
  rndConfig.distributions.push_back(std::move(distrib1)); // only 1)
  rndConfig.seeds.push_back(0);

  assert_true(generator.configure(fields, rndConfig), SPOT);

  generator.generateHeaders(20, headers);
  assert_equal(headers.size(), (unsigned)20, SPOT);

  for (PacketHeaderSet::const_iterator hdr(headers.begin()); hdr != headers.end(); ++hdr) {
    assert_equal((*hdr)->size(), (unsigned)1, SPOT);

    for (PacketHeaderLine::const_iterator line((*hdr)->begin()); line != (*hdr)->end(); ++line) {
      assert_equal((*line)->value, 0xFFFFABCD, SPOT);
    }
  }

  // reconfigure
  rndConfig.distributions.clear();
  rndConfig.distributions.push_back(std::move(distrib2)); // now 2)
  assert_true(generator.configure(fields, rndConfig), SPOT);

  generator.generateHeaders(20, headers);
  assert_equal(headers.size(), (unsigned)20, SPOT);
  
  for (PacketHeaderSet::const_iterator hdr(headers.begin()); hdr != headers.end(); ++hdr) {
    assert_equal((*hdr)->size(), (unsigned)1, SPOT);
    for (PacketHeaderLine::const_iterator line((*hdr)->begin()); line != (*hdr)->end(); ++line) {
      assert_equal((*line)->value, 0xF0000000, SPOT);
    }
  }
}

TEST(test_headergenerator_seed)
{
  using namespace Generic;

  HeaderGenerator generator;
  PacketHeaderSet headers1;
  PacketHeaderSet headers2;

  // create uniform distribution configuration
  FieldStructureSet fields;
  fields.push_back(32);
  
  RandomHeaderConfiguration rndConfig;
  rndConfig.totalHeaders = 20;
  VarValue min(0);
  VarValue max("0xFFFFFFFFFFFFFFFF");
  std::unique_ptr<RandomDistribution> distrib1(new RandomDistUniform(min, max));
  rndConfig.distributions.push_back(std::move(distrib1));
  rndConfig.seeds.push_back(888444); // seed a)

  assert_true(generator.configure(fields, rndConfig), SPOT);
  generator.generateHeaders(20, headers1);

  rndConfig.seeds.clear();
  rndConfig.seeds.push_back(1234); // seed b)

  assert_true(generator.configure(fields, rndConfig), SPOT);
  generator.generateHeaders(20, headers2);

  PacketHeaderSet::const_iterator hdr1(headers1.begin());
  PacketHeaderSet::const_iterator hdr2(headers2.begin());
  while (hdr1 != headers2.end() && hdr2 != headers2.end()) {
    PacketHeaderLine::const_iterator line1((*hdr1)->begin());
    PacketHeaderLine::const_iterator line2((*hdr2)->begin());

    while (line1 != (*hdr1)->end() && line2 != (*hdr2)->end()) {
      // very unlikely to be equal
      assert_true((*line1)->value != (*line2)->value, SPOT);
      ++line1; ++line2;
    }
    ++hdr1; ++hdr2;
  }

  // reconfigure
  rndConfig.seeds.clear();
  rndConfig.seeds.push_back(888444); // seed a) again

  assert_true(generator.configure(fields, rndConfig), SPOT);
  generator.generateHeaders(20, headers2);

  hdr1 = headers1.begin();
  hdr2 = headers2.begin();
  while (hdr1 != headers2.end() && hdr2 != headers2.end()) {
    PacketHeaderLine::const_iterator line1((*hdr1)->begin());
    PacketHeaderLine::const_iterator line2((*hdr2)->begin());

    while (line1 != (*hdr1)->end() && line2 != (*hdr2)->end()) {
      // must be equal now
      assert_equal((*line1)->value, (*line2)->value, SPOT);
      ++line1; ++line2;
    }
    ++hdr1; ++hdr2;
  }
}

