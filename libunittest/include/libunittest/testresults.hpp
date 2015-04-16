/**
 * @brief The testresults class and functions working on it
 * @file testresults.hpp
 */
#pragma once
#include <libunittest/testlog.hpp>
#include <ostream>
#include <vector>
#include <chrono>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief Stores the test results
 */
struct testresults {
    /**
     * @brief Constructor
     */
    testresults();
    /**
     * @brief Whether all tests ran successfully
     */
    bool successful;
    /**
     * @brief The number of tests
     */
    long n_tests;
    /**
     * @brief The number of test successes
     */
    long n_successes;
    /**
     * @brief The number of test failures
     */
    long n_failures;
    /**
     * @brief The number of test errors
     */
    long n_errors;
    /**
     * @brief The number of skipped tests
     */
    long n_skipped;
    /**
     * @brief The number of tests that timed out
     */
    long n_timeouts;
    /**
     * @brief The duration of all tests in seconds
     */
    double duration;
    /**
     * @brief A vector of testlog instances. One instance for each test
     */
    std::vector<unittest::internals::testlog> testlogs;
};
/**
 * @brief Writes the test results as an XML to the given output stream
 * @param stream The output stream
 * @param results The test results
 * @param time_point The time point used in the root tag (defaults to now)
 * @param local_time Whether timestamps are displayed in local time
 */
void
write_xml(std::ostream& stream,
          const unittest::internals::testresults& results,
          const std::chrono::system_clock::time_point& time_point=std::chrono::system_clock::now(),
          bool local_time=true);
/**
 * @brief Writes the test summary to the given output stream
 * @param stream The output stream
 * @param results The test results
 */
void
write_summary(std::ostream& stream,
              const unittest::internals::testresults& results);
/**
 * @brief Writes the error info to the given output stream
 * @param stream The output stream
 * @param testlogs The test logs
 * @param successful Whether all tests passed
 */
void
write_error_info(std::ostream& stream,
                 const std::vector<unittest::internals::testlog>& testlogs,
                 bool successful);

} // internals
} // unittest
