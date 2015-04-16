/**
 * @brief A test collection
 * @file testcollection.hpp
 */
#pragma once
#include <string>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief A test collection
 */
class testcollection {
public:
    /**
     * @brief Constructor
     */
    testcollection();
    /**
     * @brief Destructor
     */
    virtual
    ~testcollection();
    /**
     * @brief Returns the collection name
     * @returns The collection name
     */
    virtual std::string
    get_name() const;
    /**
     * @brief Returns the name that is returned by default by get_name()
     * @returns The name that is returned by default by get_name()
     */
    static std::string
    inactive_name();

private:

    testcollection(const testcollection&) = delete;

    testcollection&
    operator=(const testcollection&) = delete;

    testcollection(testcollection&&) = delete;

    testcollection&
    operator=(testcollection&&) = delete;

};

} // internals
} // unittest

/**
 * @brief The test collection type
 */
typedef unittest::internals::testcollection __testcollection_type__;
