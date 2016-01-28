#include <libunittest/all.hpp>
#include <metering/LogTagManager.hpp>

using namespace unittest::assertions;

TEST(test_logtagmanager_reset)
{
  LogTagManager mgr;
  assert_equal(mgr.getTags().size(), (unsigned)0, SPOT); // initially zero

  mgr.tag("abc");
  mgr.reset();

  assert_equal(mgr.getTags().size(), (unsigned)0, SPOT);

  mgr.tag("abc123");
  mgr.tag("xyz456");
  mgr.reset();

  assert_equal(mgr.getTags().size(), (unsigned)0, SPOT);
}

TEST(test_logtagmanager_get)
{
  LogTagManager mgr;

  // create a new tag
  mgr.tag("hello");
  assert_equal(mgr.getTags().size(), (unsigned)1, SPOT); // some tag was added
  assert_equal(mgr.getTags()[0]->tag, "hello", SPOT);

  // create another tag
  mgr.tag("this is a test of a very long tag");
  assert_equal(mgr.getTags().size(), (unsigned)2, SPOT); // another tag was added
  assert_equal(mgr.getTags()[0]->tag, "hello", SPOT); // first unchanged
  assert_equal(mgr.getTags()[1]->tag, "this is a test of a very long tag", SPOT); // second correct

  // create another tag
  mgr.tag("");
  assert_equal(mgr.getTags().size(), (unsigned)3, SPOT); // third tag was added
  assert_equal(mgr.getTags()[0]->tag, "hello", SPOT); // first unchanged
  assert_equal(mgr.getTags()[1]->tag, "this is a test of a very long tag", SPOT); // second unchanged
  assert_equal(mgr.getTags()[2]->tag, "", SPOT); // third correct 
}

TEST(test_logtagmanager_time)
{
  LogTagManager mgr;

  // create some tags
  mgr.tag("t1");
  mgr.tag("t2");
  mgr.tag("t3");

  // check assigned timestamps
  assert_true(mgr.getTags()[0]->time < mgr.getTags()[1]->time, SPOT);
  assert_true(mgr.getTags()[0]->time < mgr.getTags()[2]->time, SPOT);
  assert_true(mgr.getTags()[1]->time < mgr.getTags()[2]->time, SPOT);
}

TEST(test_logtag_string)
{
  LogTag lt;
  lt.time = Logclock::now();
  
  std::string timestr1;
  lt.getTimeStr(timestr1);

  // should have format YYYY-mm-dd_HH:MM:ss
  assert_equal(timestr1.length(), (unsigned)19, SPOT);
  assert_equal(timestr1[4], '-', SPOT);
  assert_equal(timestr1[7], '-', SPOT);
  assert_equal(timestr1[10], '_', SPOT);
  assert_equal(timestr1[13], ':', SPOT);
  assert_equal(timestr1[16], ':', SPOT);

  // repeat the conversion
  std::string timestr2;
  lt.getTimeStr(timestr2);
  assert_equal(timestr1, timestr2, SPOT); // results should be identical
}

