/**
 * @brief The test suite collecting test information
 * @file testsuite.hpp
 */
#pragma once
#include <libunittest/userargs.hpp>
#include <libunittest/testresults.hpp>
#include <libunittest/testlog.hpp>
#include <string>
#include <thread>
#include <map>
#include <atomic>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief The test suite collecting test information (singleton, thread-safe)
 */
class testsuite {
public:
    /**
     * @brief Returns a pointer to the instance of this class
     * @returns A pointer to the instance of this class
     */
    static testsuite*
    instance();
    /**
     * @brief Destructor
     */
    ~testsuite();
    /**
     * @brief Assigns the user arguments
     * @param arguments The user arguments
     */
    void
    set_arguments(const unittest::internals::userargs& arguments);
    /**
     * @brief Returns a reference to the user arguments
     * @returns A reference to the user arguments
     */
    unittest::internals::userargs&
    get_arguments() const;
    /**
     * @brief Returns a reference to the vector of registered class runs
     * @returns A reference to the vector of registered class runs
     */
    std::vector<std::function<void()>>&
    get_class_runs() const;
    /**
     * @brief Returns a reference to the mappings from test class IDs to
     *  test class names
     * @returns A reference to mappings from test class IDs to test class names
     */
    std::map<std::string, std::string>&
    get_class_maps() const;
    /**
     * @brief Returns a reference to the vector of lonely threads
     * @returns A reference to the vector of lonely threads
     */
    std::vector<std::pair<std::thread, std::shared_ptr<std::atomic_bool>>>&
    get_lonely_threads() const;
    /**
     * @brief Returns the accumulated test results
     * @returns The test results
     */
    unittest::internals::testresults
    get_results() const;
    /**
     * @brief Logs text for a given method
     * @param method_id The method ID
     * @param text The text to be logged
     */
    void
    log_text(const std::string& method_id,
             const std::string& text);

private:

    testsuite();

    testsuite(const testsuite&) = delete;

    testsuite&
    operator=(const testsuite&) = delete;

    testsuite(testsuite&&) = delete;

    testsuite&
    operator=(testsuite&&) = delete;

    friend class testmonitor;

    template<typename TestCase>
    friend void
    register_class(const std::string& class_name);

    friend void
    observe_and_wait(std::thread&& thread,
                     std::shared_ptr<std::atomic_bool> done,
                     const std::string& method_id,
                     std::shared_ptr<std::atomic_bool> has_timed_out,
                     double timeout,
                     std::chrono::milliseconds resolution);

    void
    make_keep_running(const unittest::internals::testlog& log);

    void
    start_timing();

    void
    stop_timing();

    void
    collect(const unittest::internals::testlog& log);

    bool
    is_test_run(const std::string& class_name,
                const std::string& test_name) const;

    void
    add_class_run(const std::function<void()>& class_run);

    void
    add_class_map(const std::string& typeid_name,
                  const std::string& class_name);

    void
    add_lonely_thread(std::thread&& thread,
                      std::shared_ptr<std::atomic_bool> done);

    struct impl;
    std::unique_ptr<impl> impl_;
};

} // internals
} // unittest
