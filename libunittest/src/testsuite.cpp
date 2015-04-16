#include <libunittest/testsuite.hpp>
#include <libunittest/teststatus.hpp>
#include <libunittest/utilities.hpp>
#include <mutex>
#include <memory>
#include <chrono>
#include <vector>
#include <map>

namespace unittest {
namespace internals {

struct testsuite::impl {

    bool keep_running_;
    std::chrono::high_resolution_clock::time_point start_;
    std::chrono::high_resolution_clock::time_point end_;
    userargs arguments_;
    testresults results_;
    std::vector<std::function<void()>> class_runs_;
    std::map<std::string, std::string> class_maps_;
    std::vector<std::pair<std::thread, std::shared_ptr<std::atomic_bool>>> lonely_threads_;
    std::map<std::string, std::string> logged_texts_;

    impl()
    	: keep_running_(true),
    	  start_(std::chrono::high_resolution_clock::time_point::min()),
    	  end_(std::chrono::high_resolution_clock::time_point::min())
    {}

    void
    assign_logged_texts(std::vector<testlog>& testlogs)
    {
        static std::mutex assign_logged_texts_mutex_;
        std::lock_guard<std::mutex> lock(assign_logged_texts_mutex_);
        for (auto& log : testlogs) {
            const auto& element = logged_texts_.find(log.method_id);
            if (element!=logged_texts_.end())
                log.text = element->second;
        }
    }

};

testsuite*
testsuite::instance()
{
    static std::unique_ptr<testsuite> instance_ = nullptr;
    if (instance_==nullptr) {
        static std::mutex instance_mutex_;
        std::lock_guard<std::mutex> lock(instance_mutex_);
        if (instance_==nullptr)
            instance_.reset(new testsuite());
    }
    return instance_.get();
}

testsuite::testsuite()
    : impl_(make_unique<impl>())
{}

testsuite::~testsuite()
{}

void
testsuite::set_arguments(const userargs& arguments)
{
    static std::mutex set_arguments_mutex_;
    std::lock_guard<std::mutex> lock(set_arguments_mutex_);
    impl_->arguments_ = arguments;
}

userargs&
testsuite::get_arguments() const
{
    return impl_->arguments_;
}

std::vector<std::function<void()>>&
testsuite::get_class_runs() const
{
    return impl_->class_runs_;
}

testresults
testsuite::get_results() const
{
    testresults results(impl_->results_);
    results.successful = results.n_tests==results.n_successes;
    if (!get_arguments().dry_run())
        results.duration = duration_in_seconds(impl_->end_ - impl_->start_);
    impl_->assign_logged_texts(results.testlogs);
    return results;
}

void
testsuite::make_keep_running(const testlog& log)
{
    static std::mutex make_keep_running_mutex_;
    std::lock_guard<std::mutex> lock(make_keep_running_mutex_);
    impl_->keep_running_ = keep_running(log, get_arguments().failure_stop());
}

void
testsuite::start_timing()
{
    static std::mutex start_timing_mutex_;
    std::lock_guard<std::mutex> lock(start_timing_mutex_);
    if (impl_->start_==std::chrono::high_resolution_clock::time_point::min())
        impl_->start_ = std::chrono::high_resolution_clock::now();
}

void
testsuite::stop_timing()
{
    static std::mutex stop_timing_mutex_;
    std::lock_guard<std::mutex> lock(stop_timing_mutex_);
    impl_->end_ = std::chrono::high_resolution_clock::now();
}

void
testsuite::collect(const testlog& log)
{
    static std::mutex collect_mutex_;
    std::lock_guard<std::mutex> lock(collect_mutex_);
    if (log.status != teststatus::skipped)
        ++impl_->results_.n_tests;
    if (log.has_timed_out)
        ++impl_->results_.n_timeouts;
    switch (log.status) {
    case teststatus::success: ++impl_->results_.n_successes; break;
    case teststatus::failure: ++impl_->results_.n_failures; break;
    case teststatus::error: ++impl_->results_.n_errors; break;
    default: ++impl_->results_.n_skipped; break;
    }
    impl_->results_.testlogs.push_back(log);
}

bool
testsuite::is_test_run(const std::string& class_name,
                       const std::string& test_name) const
{
    if (!impl_->keep_running_) {
    	return false;
    } else {
    	const std::string full_name = make_full_test_name(class_name, test_name);
    	return is_test_executed(full_name, get_arguments().test_name(), get_arguments().name_filter());
    }
}

void
testsuite::add_class_run(const std::function<void()>& class_run)
{
    static std::mutex add_class_run_mutex_;
    std::lock_guard<std::mutex> lock(add_class_run_mutex_);
    impl_->class_runs_.push_back(class_run);
}

void
testsuite::add_class_map(const std::string& typeid_name,
                         const std::string& class_name)
{
    static std::mutex add_class_map_mutex_;
    std::lock_guard<std::mutex> lock(add_class_map_mutex_);
    impl_->class_maps_[typeid_name] = class_name;
}

std::map<std::string, std::string>&
testsuite::get_class_maps() const
{
    return impl_->class_maps_;
}

void
testsuite::add_lonely_thread(std::thread&& thread,
                             std::shared_ptr<std::atomic_bool> done)
{
    static std::mutex add_lonely_thread_mutex_;
    std::lock_guard<std::mutex> lock(add_lonely_thread_mutex_);
    impl_->lonely_threads_.push_back(std::make_pair(std::move(thread), done));
}

std::vector<std::pair<std::thread, std::shared_ptr<std::atomic_bool>>>&
testsuite::get_lonely_threads() const
{
    return impl_->lonely_threads_;
}

void
testsuite::log_text(const std::string& method_id,
                    const std::string& text)
{
    static std::mutex log_text_mutex_;
    std::lock_guard<std::mutex> lock(log_text_mutex_);
    impl_->logged_texts_[method_id] = text;
}

} // internals
} // unittest
