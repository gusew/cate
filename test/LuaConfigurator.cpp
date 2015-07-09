#include <libunittest/all.hpp>
#include <configuration/LuaConfigurator.hpp>
#include <configuration/Configuration.hpp>
#include <generics/RuleAtom.hpp>
#include <memory>
#include <vector>
#include <cstdint>
#include <string>

using namespace unittest::assertions;

TEST(test_luaconfigurator_algorithm)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);

  assert_true(cfgPtr->getBenchmarkSet().empty(), SPOT);

  // add first benchmark
  configurator.addBenchmark();
  assert_false(cfgPtr->getBenchmarkSet().empty(), SPOT);
  assert_equal(cfgPtr->getBenchmarkSet().size(), (unsigned)1, SPOT);

  const char* caption1 = "first benchmark";
  const char* filename1 = "file/one.so";

  configurator.setCaption(caption1);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->caption, "first benchmark", SPOT);

  configurator.setAlgFilename(filename1);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->algFilename, "file/one.so", SPOT);

  configurator.addAlgParameter(0.4);
  configurator.addAlgParameter(42.0);
  configurator.addAlgParameter(123.456);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->algParameter.size(), (unsigned)3, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[0], 0.4, 0.0001, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[1], 42.0, 0.0001, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[2], 123.456, 0.0001, SPOT);

  // add second benchmark
  configurator.addBenchmark();
  assert_equal(cfgPtr->getBenchmarkSet().size(), (unsigned)2, SPOT);

  const char* caption2 = "second benchmark";
  const char* filename2 = "file/sub/two.so";

  configurator.setCaption(caption2);
  assert_equal(cfgPtr->getBenchmarkSet()[1]->caption, "second benchmark", SPOT);

  configurator.setAlgFilename(filename2);
  assert_equal(cfgPtr->getBenchmarkSet()[1]->algFilename, "file/sub/two.so", SPOT);

  configurator.addAlgParameter(99.11);
  assert_equal(cfgPtr->getBenchmarkSet()[1]->algParameter.size(), (unsigned)1, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[1]->algParameter[0], 99.11, 0.0001, SPOT);

  // check again, if first benchmark still the same
  assert_equal(cfgPtr->getBenchmarkSet()[0]->caption, "first benchmark", SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->algFilename, "file/one.so", SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->algParameter.size(), (unsigned)3, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[0], 0.4, 0.0001, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[1], 42.0, 0.0001, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->algParameter[2], 123.456, 0.0001, SPOT);
}

TEST(test_luaconfigurator_fieldstructure)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);
  configurator.addBenchmark();

  assert_true(cfgPtr->getBenchmarkSet()[0]->fieldStructure.empty(), SPOT);
  
  configurator.addFieldStructure(32);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure.size(), (unsigned)1, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure[0], (unsigned)32, SPOT);

  configurator.addFieldStructure(16);
  configurator.addFieldStructure(32);
  configurator.addFieldStructure(5);

  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure.size(), (unsigned)4, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure[0], (unsigned)32, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure[1], (unsigned)16, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure[2], (unsigned)32, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->fieldStructure[3], (unsigned)5, SPOT);
}

TEST(test_luaconfigurator_rules32)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);
  configurator.addBenchmark();

  // set field structure for rules in benchmark
  configurator.addFieldStructure(32);
  configurator.addFieldStructure(16);
  configurator.addFieldStructure(32);

  assert_true(cfgPtr->getBenchmarkSet()[0]->rules.empty(), SPOT);
  
  // add first rule with 32 bit values
  configurator.addRule();
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules.size(), (unsigned)1, SPOT);
  assert_true(cfgPtr->getBenchmarkSet()[0]->rules[0]->empty(), SPOT);

  configurator.addRuleAtomExact(333444);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->size(), (unsigned)1, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(0)->getType(), Generic::RuleAtom::EXACT, SPOT);
  Generic::RuleAtomExact* atom1 = static_cast<Generic::RuleAtomExact*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(0).get());
  assert_equal(atom1->value, (unsigned)333444, SPOT);

  configurator.addRuleAtomRange(1000, 9999);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->size(), (unsigned)2, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(1)->getType(), Generic::RuleAtom::RANGE, SPOT);
  Generic::RuleAtomRange* atom2 = static_cast<Generic::RuleAtomRange*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(1).get());
  assert_equal(atom2->minValue, (unsigned)1000, SPOT);
  assert_equal(atom2->maxValue, (unsigned)9999, SPOT);

  configurator.addRuleAtomPrefix(0xA88E3000, 0xFFFF0000);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->size(), (unsigned)3, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(2)->getType(), Generic::RuleAtom::PREFIX, SPOT);
  Generic::RuleAtomPrefix* atom3 = static_cast<Generic::RuleAtomPrefix*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(2).get());
  assert_equal(atom3->prefix, (unsigned)0xA88E3000, SPOT);
  assert_equal(atom3->mask, (unsigned)0xFFFF0000, SPOT);
}


TEST(test_luaconfigurator_rules64)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);
  configurator.addBenchmark();

  // set field structure for rules in benchmark
  configurator.addFieldStructure(64);
  configurator.addFieldStructure(64);
  configurator.addFieldStructure(64);

  assert_true(cfgPtr->getBenchmarkSet()[0]->rules.empty(), SPOT);

  // add second rule with 64 bit values
  configurator.addRule();
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules.size(), (unsigned)1, SPOT);
  assert_true(cfgPtr->getBenchmarkSet()[0]->rules[0]->empty(), SPOT);

  std::string values1 = "0x1234567887654321";
  std::string values2 = "0x8888444422221111";
  std::string values3 = "0xFFFFFF0000000000";

  configurator.addRuleAtomExact(values1);
  configurator.addRuleAtomRange(values1, values2);
  configurator.addRuleAtomPrefix(values1, values3);

  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->size(), (unsigned)3, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(0)->getType(), Generic::RuleAtom::EXACT, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(1)->getType(), Generic::RuleAtom::RANGE, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(2)->getType(), Generic::RuleAtom::PREFIX, SPOT);

  Generic::RuleAtomExact* atom21 = static_cast<Generic::RuleAtomExact*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(0).get());
  Generic::RuleAtomRange* atom22 = static_cast<Generic::RuleAtomRange*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(1).get());
  Generic::RuleAtomPrefix* atom23 = static_cast<Generic::RuleAtomPrefix*>(cfgPtr->getBenchmarkSet()[0]->rules[0]->at(2).get());

  double cmp1 = 1.3117684671392817E18;
  double cmp2 = 9.838188445411971E18;
  double cmp3 = 1.8446742974197924E19;
  assert_approx_equal(atom21->value.get_d(), cmp1, 0.999, SPOT);

  assert_approx_equal(atom22->minValue.get_d(), cmp1, 0.999, SPOT);
  assert_approx_equal(atom22->maxValue.get_d(), cmp2, 0.999, SPOT);

  assert_approx_equal(atom23->prefix.get_d(), cmp1, 0.999, SPOT);
  assert_approx_equal(atom23->mask.get_d(), cmp3, 0.999, SPOT);
}

TEST(test_luaconfigurator_headers)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);
  configurator.addBenchmark();

  assert_true(cfgPtr->getBenchmarkSet()[0]->headers.empty(), SPOT);
  
  // add first header
  configurator.addHeader();
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers.size(), (unsigned)1, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->size(), (unsigned)0, SPOT);
  
  configurator.addHeaderValue(1234);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->size(), (unsigned)1, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->at(0)->value.get_ui(), (unsigned)1234, SPOT);

  configurator.addHeaderValue(42);
  configurator.addHeaderValue(0xAFFE0BAD);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->size(), (unsigned)3, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->at(1)->value.get_ui(), (unsigned)42, SPOT);
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[0]->at(2)->value.get_ui(), (unsigned)0xAFFE0BAD, SPOT);

  // add second header
  configurator.addHeader();
  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers.size(), (unsigned)2, SPOT);

  std::string values1 = "0x1234567887654321";
  std::string values2 = "0x8888444422221111";
  std::string values3 = "0xFFFFFF0000000000";

  configurator.addHeaderValue(values1);
  configurator.addHeaderValue(values2);
  configurator.addHeaderValue(values3);

  assert_equal(cfgPtr->getBenchmarkSet()[0]->headers[1]->size(), (unsigned)3, SPOT);
  
  double cmp1 = 1.3117684671392817E18;
  double cmp2 = 9.838188445411971E18;
  double cmp3 = 1.8446742974197924E19;
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->headers[1]->at(0)->value.get_d(), cmp1, 0.999, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->headers[1]->at(1)->value.get_d(), cmp2, 0.999, SPOT);
  assert_approx_equal(cfgPtr->getBenchmarkSet()[0]->headers[1]->at(2)->value.get_d(), cmp3, 0.999, SPOT);
}

TEST(test_luaconfigurator_fullrelpath)
{
  std::shared_ptr<Configuration> cfgPtr = std::make_shared<Configuration>();
  LuaConfigurator configurator(cfgPtr);

  assert_equal(cfgPtr->getProgRelativePath(), "", SPOT);
  std::string result;
  configurator.makeFullRelativePath("", result);
  assert_equal(result, "", SPOT);

  configurator.makeFullRelativePath("some example STRING", result);
  assert_equal(result, "some example STRING", SPOT);

  cfgPtr->setProgRelativePath("./sub/folder/deep/");
  configurator.makeFullRelativePath("justAWordLong", result);
  assert_equal(result, "./sub/folder/deep/justAWordLong", SPOT);

  cfgPtr->setProgRelativePath("/usr/bin/sub/");
  configurator.makeFullRelativePath("a-simple_filename.ext", result);
  assert_equal(result, "/usr/bin/sub/a-simple_filename.ext", SPOT);
}

