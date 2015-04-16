#include <libunittest/all.hpp>
#include <data/Statistics.hpp>

using namespace unittest::assertions;

TEST(test_statistics_series_empty)
{
  Series<int> s;

  try {
    s.calcMinValue();
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  try {
    s.calcMaxValue();
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  try {
    s.calcMeanValue();
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  try {
    s.calcMedianValue();
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  try {
    s.calcStdDev();
    assert_true(false, SPOT);
  } catch (const char* ex) {}

  int sum = s.calcSum();
  assert_equal(sum, 0, SPOT);
}

TEST(test_statistics_series_int)
{
  Series<int> s;
  s.data.push_back(42);
  s.data.push_back(4);
  s.data.push_back(17);
  s.data.push_back(0xAFFE);
  s.data.push_back(-12);
  s.data.push_back(0);
  s.data.push_back(-666);
  s.data.push_back(100);

  int min = s.calcMinValue();
  int max = s.calcMaxValue();
  double mean = s.calcMeanValue();
  int median = s.calcMedianValue();
  double stddev = s.calcStdDev();
  double sum = s.calcSum();

  assert_equal(min, -666, SPOT);
  assert_equal(max, 0xAFFE, SPOT);
  assert_equal(mean, 5567.375, SPOT);
  assert_equal(median, 4, SPOT);
  assert_approx_equal(stddev, 15956.8765735, 0.001, SPOT);
  assert_equal(sum, 44539, SPOT);
}

TEST(test_statistics_series_char)
{
  Series<unsigned char> s;
  s.data.push_back(42);
  s.data.push_back(4);
  s.data.push_back(17);
  s.data.push_back(1);
  s.data.push_back(128);
  s.data.push_back(42);

  int min = s.calcMinValue();
  int max = s.calcMaxValue();
  double mean = s.calcMeanValue();
  int median = s.calcMedianValue();
  double stddev = s.calcStdDev();
  double sum = s.calcSum();

  assert_equal(min, 1, SPOT);
  assert_equal(max, 128, SPOT);
  assert_equal(mean, 39, SPOT);
  assert_equal(median, 17, SPOT);
  assert_approx_equal(stddev, 47.0998938, 0.001, SPOT);
  assert_equal(sum, 234, SPOT);
}

TEST(test_statistics_histbin_incr)
{
  HistogramBin bin(42);

  assert_equal(bin.id, (unsigned)42, SPOT);
  assert_equal(bin.counter, (unsigned)0, SPOT);

  HistogramBin cp = bin++;

  assert_equal(cp.id, (unsigned)42, SPOT);
  assert_equal(cp.counter, (unsigned)0, SPOT);

  assert_equal(bin.id, (unsigned)42, SPOT);
  assert_equal(bin.counter, (unsigned)1, SPOT);

  ++bin;

  assert_equal(bin.id, (unsigned)42, SPOT);
  assert_equal(bin.counter, (unsigned)2, SPOT);
}

TEST(test_statistics_histbin_rel)
{
  HistogramBin bin1(17);
  HistogramBin bin2(555);
  HistogramBin bin3(17);

  ++bin1; ++bin1;
  ++bin3;

  assert_true(bin2 < bin1, SPOT);
  assert_true(bin3 < bin1, SPOT);
  assert_true(bin2 < bin3, SPOT);
  
  assert_true(bin1 == bin3, SPOT);

  ++bin2; ++bin2; ++bin2;

  assert_true(bin1 < bin2, SPOT);
  assert_true(bin3 < bin2, SPOT);
}

TEST(test_statistics_histogram_sorted)
{
  Histogram hist(6);
  hist.occured(3); hist.occured(3);
  hist.occured(5); hist.occured(5); hist.occured(5);
  hist.occured(2); hist.occured(2);
  hist.occured(0);

  std::vector<HistogramPair> result;
  hist.getSorted(result);

  assert_equal(result.size(), (unsigned)6, SPOT);
  
  assert_equal(result[0].first, (unsigned)5, SPOT);
  assert_equal(result[0].second, (unsigned)3, SPOT);
  assert_equal(result[1].first, (unsigned)2, SPOT);
  assert_equal(result[1].second, (unsigned)2, SPOT);
  assert_equal(result[2].first, (unsigned)3, SPOT);
  assert_equal(result[2].second, (unsigned)2, SPOT);
  assert_equal(result[3].first, (unsigned)0, SPOT);
  assert_equal(result[3].second, (unsigned)1, SPOT);
  assert_equal(result[4].first, (unsigned)1, SPOT);
  assert_equal(result[4].second, (unsigned)0, SPOT);
  assert_equal(result[5].first, (unsigned)4, SPOT);
  assert_equal(result[5].second, (unsigned)0, SPOT);
}

TEST(test_statistics_histogram_unsorted)
{
  Histogram hist(6);
  hist.occured(3); hist.occured(3);
  hist.occured(5); hist.occured(5); hist.occured(5);
  hist.occured(2); hist.occured(2);
  hist.occured(0);

  std::vector<HistogramPair> result;
  hist.getUnsorted(result);

  assert_equal(result.size(), (unsigned)6, SPOT);
  
  assert_equal(result[0].first, (unsigned)0, SPOT);
  assert_equal(result[0].second, (unsigned)1, SPOT);
  assert_equal(result[1].first, (unsigned)1, SPOT);
  assert_equal(result[1].second, (unsigned)0, SPOT);
  assert_equal(result[2].first, (unsigned)2, SPOT);
  assert_equal(result[2].second, (unsigned)2, SPOT);
  assert_equal(result[3].first, (unsigned)3, SPOT);
  assert_equal(result[3].second, (unsigned)2, SPOT);
  assert_equal(result[4].first, (unsigned)4, SPOT);
  assert_equal(result[4].second, (unsigned)0, SPOT);
  assert_equal(result[5].first, (unsigned)5, SPOT);
  assert_equal(result[5].second, (unsigned)3, SPOT);
}


