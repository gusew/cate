#include <libunittest/all.hpp>
#include <core/ChronoManager.hpp>
#include <limits>

using namespace unittest::assertions;

void doWork() {
  for (int i = 0; i < 0xFFFFFF; i++) if (i%0xABC == 0) assert_true(i%0xABC == 0, SPOT);
}

TEST(test_chronomanager_reset)
{
  ChronoManager mgr;
  assert_equal(mgr.getTimeMicro(), (unsigned)0, SPOT); // initially zero

  mgr.start("abc");
  doWork();
  mgr.stop("abc");
  
  mgr.reset();

  assert_equal(mgr.getTimeNano(), 0, SPOT);
  assert_equal(mgr.getTimeMicro(), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeMilli(), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeSec(), (unsigned)0, SPOT); 

  try {
    mgr.getTimeNano("abc");
    assert_true(false, SPOT);
  } catch (const char* ch) {}
  try {
    mgr.getTimeMicro("abc");
    assert_true(false, SPOT);
  } catch (const char* ch) {}
  try {
    mgr.getTimeMilli("abc"); 
    assert_true(false, SPOT);
  } catch (const char* ch) {}
  try {
    mgr.getTimeSec("abc"); 
    assert_true(false, SPOT);
  } catch (const char* ch) {}
}

TEST(test_chronomanager_startstop)
{
  ChronoManager mgr;

  mgr.start("outer-loop");
  doWork();
  mgr.stop("outer-loop");

  assert_true(mgr.getTimeNano() > 0, SPOT);
  assert_true(mgr.getTimeMicro() > 0, SPOT);
  assert_true(mgr.getTimeNano("outer-loop") > 0, SPOT);
  assert_true(mgr.getTimeMicro("outer-loop") > 0, SPOT);
}

TEST(test_chronomanager_parallel)
{
  ChronoManager mgr;

  mgr.start("outer-loop");
  for (int i = 0; i < 0xF; i++) { 
    mgr.start("inner-loop");
    doWork();
    mgr.stop("inner-loop");
  }
  mgr.stop("outer-loop");

  assert_true(mgr.getTimeNano("outer-loop") > 0, SPOT); 
  assert_true(mgr.getTimeMicro("outer-loop") > 0, SPOT); 
  assert_true(mgr.getTimeMicro("inner-loop") > 0, SPOT); 
  assert_true(mgr.getTimeNano("inner-loop") > 0, SPOT); 
  assert_true(mgr.getTimeMicro("outer-loop") > mgr.getTimeMicro("inner-loop"), SPOT); 
  assert_true(mgr.getTimeNano("outer-loop") > mgr.getTimeNano("inner-loop"), SPOT); 
}

TEST(test_chronomanager_nostop)
{
  ChronoManager mgr;

  mgr.start("abc");
  doWork();
  //mgr.stop("abc");
  
  assert_equal(mgr.getTimeNano(), 0, SPOT);
  assert_equal(mgr.getTimeMicro(), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeMilli(), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeSec(), (unsigned)0, SPOT); 

  assert_equal(mgr.getTimeNano("abc"), 0, SPOT);
  assert_equal(mgr.getTimeMicro("abc"), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeMilli("abc"), (unsigned)0, SPOT);
  assert_equal(mgr.getTimeSec("abc"), (unsigned)0, SPOT);
 
  mgr.stop("abc");
  assert_true(mgr.getTimeNano() > 0, SPOT);
  assert_true(mgr.getTimeMicro() > 0, SPOT);
  assert_true(mgr.getTimeNano("abc") > 0, SPOT);
  assert_true(mgr.getTimeMicro("abc") > 0, SPOT);
}

TEST(test_chronomanager_lapmode)
{
  ChronoManager mgr;

  mgr.start("in laps");
  mgr.start("total");
  doWork();
  mgr.stop("in laps");

  double lap1nano = mgr.getTimeNano();
  unsigned int lap1 = mgr.getTimeMicro();

  doWork();
  mgr.stop("in laps");
  mgr.stop("total");

  assert_true(lap1 > 0, SPOT);
  assert_true(lap1nano > 0, SPOT);

  assert_approx_equal(mgr.getTimeMicro("in laps"), 2*lap1, 5000, SPOT);
  assert_approx_equal(mgr.getTimeNano("in laps"), 2*lap1nano, 5000000, SPOT);
  assert_approx_equal(mgr.getTimeMicro("in laps"), mgr.getTimeMicro("total"), 5000, SPOT);
  assert_approx_equal(mgr.getTimeNano("in laps"), mgr.getTimeNano("total"), 5000000, SPOT);

  assert_approx_equal(lap1nano * 1000, lap1, 1000, SPOT);
  assert_approx_equal(mgr.getTimeNano("total") * 1000, mgr.getTimeMicro("total"), 1000, SPOT);
}

TEST(test_chronomanager_gettotal)
{
  ChronoManager mgr;

  mgr.start("abc");
  for (int i=0; i < 0x4F; i++) doWork();
  mgr.stop("abc");

  assert_true(mgr.getTimeNano() > 0, SPOT);
  assert_true(mgr.getTimeMicro() > 0, SPOT);
  assert_true(mgr.getTimeMilli() > 0, SPOT);
  assert_true(mgr.getTimeSec() > 0, SPOT);

  assert_true(mgr.getTimeSec() < mgr.getTimeMilli(), SPOT);
  assert_true(mgr.getTimeMilli() < mgr.getTimeMicro(), SPOT);
  assert_true(mgr.getTimeMicro() < mgr.getTimeNano(), SPOT);
}

TEST(test_chronomanager_getone)
{
  ChronoManager mgr;

  mgr.start("abc");
  for (int i=0; i < 0x4F; i++) doWork();
  mgr.stop("abc");

  assert_true(mgr.getTimeNano("abc") > 0, SPOT);
  assert_true(mgr.getTimeMicro("abc") > 0, SPOT);
  assert_true(mgr.getTimeMilli("abc") > 0, SPOT);
  assert_true(mgr.getTimeSec("abc") > 0, SPOT);

  assert_true(mgr.getTimeSec("abc") < mgr.getTimeMilli("abc"), SPOT);
  assert_true(mgr.getTimeMilli("abc") < mgr.getTimeMicro("abc"), SPOT);
  assert_true(mgr.getTimeMicro("abc") < mgr.getTimeNano("abc"), SPOT);
}

TEST(test_chronomanager_allresults)
{
  ChronoManager mgr;
  ChronoResults results;

  mgr.start("group1");
  mgr.start("group2");
  doWork();
  mgr.stop("group2");
  mgr.start("group3");
  for (int i=0; i < 0xF; i++) doWork();
  mgr.stop("group1");
  mgr.stop("group3");

  mgr.getAllResults(results, 10);
  assert_equal(results.size(), (unsigned)3, SPOT);
  
  unsigned int res1, res2, res3;
  for (auto iter(results.begin()); iter != results.end(); ++iter) {
    if (iter->first == "group1") res1 = iter->second;
    else if (iter->first == "group2") res2 = iter->second;
    else if (iter->first == "group3") res3 = iter->second;
  }

  assert_equal(res1, mgr.getTimeMicro("group1") / 10, SPOT);
  assert_equal(res2, mgr.getTimeMicro("group2") / 10, SPOT);
  assert_equal(res3, mgr.getTimeMicro("group3") / 10, SPOT);

  assert_true(res1 > res2, SPOT);
  assert_true(res1 > res3, SPOT);
  assert_true(res2 < res3, SPOT);
  assert_approx_equal(res1, res2 + res3, 500, SPOT);
}
