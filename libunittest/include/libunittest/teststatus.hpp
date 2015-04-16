/**
 * @brief An enumeration for the test status
 * @file teststatus.hpp
 */
#pragma once
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief An enumeration for the test status
 */
enum teststatus : unsigned int {
    success = 0,
    failure = 1,
    error = 2,
    skipped = 3
};

} // internals
} // unittest
