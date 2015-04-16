#include <libunittest/testrun.hpp>
#include <libunittest/utilities.hpp>
#include <libunittest/teststatus.hpp>
#include <libunittest/testlog.hpp>
#include <libunittest/testcollection.hpp>
#include <chrono>
#include <iostream>
#include <typeinfo>

namespace unittest {
namespace internals {

void
observe_and_wait(std::thread&& thread,
                 std::shared_ptr<std::atomic_bool> done,
                 const std::string& method_id,
                 std::shared_ptr<std::atomic_bool> has_timed_out,
                 double timeout,
                 std::chrono::milliseconds resolution)
{
    if (timeout > 0) {
        const double wait_sec = duration_in_seconds(resolution);
        double duration = 0;
        while (done->load() != true) {
            if (duration > timeout) {
                has_timed_out->store(true);
                auto suite = testsuite::instance();
                write_test_timeout_message(std::cout, suite->get_arguments().verbose());
                suite->add_lonely_thread(std::move(thread), done);
                break;
            }
            std::this_thread::sleep_for(resolution);
            duration += wait_sec;
        }
    }
    if (!has_timed_out->load())
        thread.join();
}

struct testmonitor::impl {

    testlog log_;
    std::chrono::high_resolution_clock::time_point start_;
    bool is_executed_;

    impl()
    	: start_(std::chrono::high_resolution_clock::time_point::min()),
    	  is_executed_(true)
    {}

};

testmonitor::testmonitor(const std::string& class_name,
                         const std::string& test_name,
                         const std::string& method_id)
    : impl_(make_unique<impl>())
{
    auto suite = testsuite::instance();
    impl_->is_executed_ = suite->is_test_run(class_name, test_name);
    impl_->log_.class_name = class_name;
    impl_->log_.test_name = test_name;
    impl_->log_.method_id = method_id;
    if (impl_->is_executed_) {
        suite->start_timing();
        impl_->start_ = std::chrono::high_resolution_clock::now();
        write_test_start_message(std::cout, impl_->log_, suite->get_arguments().verbose());
    }
}

testmonitor::~testmonitor()
{
    auto suite = testsuite::instance();
    if (impl_->is_executed_) {
        if (impl_->log_.status!=teststatus::skipped && !suite->get_arguments().dry_run()) {
            const auto end = std::chrono::high_resolution_clock::now();
            suite->stop_timing();
            impl_->log_.duration = duration_in_seconds(end - impl_->start_);
        }
        suite->make_keep_running(impl_->log_);
        impl_->log_.successful = impl_->log_.status==teststatus::success ||
                                 impl_->log_.status==teststatus::skipped;
        write_test_end_message(std::cout, impl_->log_, suite->get_arguments().verbose());
    }
    suite->collect(impl_->log_);
}

bool
testmonitor::is_executed()
{
    return impl_->is_executed_;
}

void
testmonitor::log_success()
{
    impl_->log_.status = teststatus::success;
    impl_->log_.message = "ok";
    impl_->log_.error_type = "";
}

void
testmonitor::log_skipped(const std::string& message)
{
    impl_->log_.status = teststatus::skipped;
    impl_->log_.text = message;
    impl_->log_.error_type = "";
}

void
testmonitor::log_failure(const testfailure& e)
{
    impl_->log_.status = teststatus::failure;
    impl_->log_.message = e.what();
    impl_->log_.error_type = "testfailure";
    impl_->log_.assertion = e.assertion();
    impl_->log_.filename = e.filename();
    impl_->log_.linenumber = e.linenumber();
}

void
testmonitor::log_error(const std::exception& e)
{
    impl_->log_.status = teststatus::error;
    impl_->log_.message = e.what();
    impl_->log_.error_type = typeid(e).name();
}
void
testmonitor::log_unknown_error()
{
    impl_->log_.status = teststatus::error;
    impl_->log_.message = "Unknown message";
    impl_->log_.error_type = "Unknown exception";
}

void
testmonitor::has_timed_out(double timeout)
{
    impl_->log_.has_timed_out = true;
    impl_->log_.timeout = timeout;
}

void
update_local_timeout(double& local_timeout,
                     double global_timeout)
{
    if (!(local_timeout > 0))
        local_timeout = global_timeout;
}

void
update_class_name(std::string& class_name,
                  const std::string& class_id,
                  const std::map<std::string, std::string>& class_maps)
{
    if (class_name=="") {
        class_name = get_from_map(class_maps, class_id);
    } else if (class_name==testcollection::inactive_name()) {
        class_name = "";
    }
    class_name = remove_white_spaces(class_name);
}

void
update_test_name(std::string& test_name,
                 const std::string& class_id,
                 const std::map<std::string, std::string>& class_maps)
{
    if (test_name=="") {
        test_name = get_from_map(class_maps, class_id);
        test_name += "::test";
    }
    test_name = remove_white_spaces(test_name);
}

void
update_testrun_info(const std::string& class_id,
                    std::string& class_name,
                    std::string& test_name,
                    double& local_timeout)
{
    auto suite = internals::testsuite::instance();
    const auto& class_maps = suite->get_class_maps();
    update_class_name(class_name, class_id, class_maps);
    update_test_name(test_name, class_id, class_maps);
    const double global_timeout = suite->get_arguments().timeout();
    update_local_timeout(local_timeout, global_timeout);
}

} // internals
} // unittest
