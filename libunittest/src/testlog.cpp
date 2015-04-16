#include <libunittest/testlog.hpp>
#include <libunittest/teststatus.hpp>
#include <libunittest/utilities.hpp>
#include <algorithm>
#include <mutex>

namespace unittest {
namespace internals {

testlog::testlog()
    : class_name(""), test_name(""), successful(true),
      status(teststatus::skipped), message(""), duration(0),
      has_timed_out(false), timeout(-1), assertion(""),
      method_id(""), text(""), filename(""), linenumber(-1)
{}

void
write_test_start_message(std::ostream& stream,
                         const testlog& log,
                         bool verbose)
{
    static std::mutex write_test_start_message_mutex_;
    std::lock_guard<std::mutex> lock(write_test_start_message_mutex_);
    if (verbose) {
        stream << make_full_test_name(log.class_name, log.test_name) << " ... ";
        stream << std::flush;
    }
}

void
write_test_end_message(std::ostream& stream,
                       const testlog& log,
                       bool verbose)
{
    static std::mutex write_test_end_message_mutex_;
    std::lock_guard<std::mutex> lock(write_test_end_message_mutex_);
    if (!log.has_timed_out) {
        if (verbose) {
            stream << "[" << log.duration << "s] ";
            switch (log.status) {
            case teststatus::success: stream << "ok"; break;
            case teststatus::failure: stream << "FAIL"; break;
            case teststatus::error: stream << "ERROR"; break;
            case teststatus::skipped: stream << "SKIP " << trim(log.text); break;
            default: break;
            }
            stream << "\n";
        } else {
            switch (log.status) {
            case teststatus::success: stream << "."; break;
            case teststatus::failure: stream << "F"; break;
            case teststatus::error: stream << "E"; break;
            case teststatus::skipped: stream << "S"; break;
            default: break;
            }
        }
        stream << std::flush;
    }
}

void
write_test_timeout_message(std::ostream& stream,
                           bool verbose)
{
    static std::mutex write_test_timeout_message_mutex_;
    std::lock_guard<std::mutex> lock(write_test_timeout_message_mutex_);
    std::string message("T");
    if (verbose)
        message = "TIMEOUT\n";
    stream << message << std::flush;
}

std::string
make_full_test_name(const std::string& class_name,
                    const std::string& test_name)
{
    return class_name.length() ? class_name + "::" + test_name : test_name;
}

bool
is_test_executed(const std::string& full_test_name,
                 const std::string& exact_name,
                 const std::string& filter_name)
{
    if (exact_name!="") {
        if (exact_name==full_test_name)
            return true;
        else
            return false;
    }

    if (filter_name!="") {
        if (full_test_name.substr(0, filter_name.size())==filter_name)
            return true;
        else
            return false;
    }

    return true;
}

bool
keep_running(const testlog& log,
             bool failure_stop)
{
    const bool failed = log.status==teststatus::failure ||
                        log.status==teststatus::error;
    if (failed && failure_stop)
        return false;
    else
        return true;
}

} // internals
} // unittest
