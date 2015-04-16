#include <libunittest/userargs.hpp>
#include <libunittest/utilities.hpp>

namespace unittest {
namespace internals {

struct userargs::impl {

    bool verbose_;
    bool failure_stop_;
    bool generate_xml_;
    bool handle_exceptions_;
    bool dry_run_;
    int concurrent_threads_;
    std::string name_filter_;
    std::string test_name_;
    double timeout_;
    std::string xml_filename_;
    bool disable_timeout_;
    int max_value_precision_;
    int max_string_length_;

    impl()
        : verbose_(false), failure_stop_(false), generate_xml_(false),
          handle_exceptions_(true), dry_run_(false), concurrent_threads_(0),
          name_filter_(""), test_name_(""), timeout_(-1),
          xml_filename_("libunittest.xml"), disable_timeout_(false),
          max_value_precision_(10), max_string_length_(500)
    {}

};

userargs::userargs()
    : impl_(make_unique<impl>())
{}

userargs::~userargs()
{}

userargs::userargs(const userargs& other)
    : impl_(make_unique<impl>(*(other.impl_)))
{}

userargs& userargs::operator=(const userargs& other)
{
    *impl_ = *other.impl_;
    return *this;
}

bool
userargs::verbose() const
{
    return impl_->verbose_;
}

void
userargs::verbose(bool value)
{
    impl_->verbose_ = value;
}

bool
userargs::failure_stop() const
{
    return impl_->failure_stop_;
}

void
userargs::failure_stop(bool value)
{
    impl_->failure_stop_ = value;
}

bool
userargs::generate_xml() const
{
    return impl_->generate_xml_;
}

void
userargs::generate_xml(bool value)
{
    impl_->generate_xml_ = value;
}

bool
userargs::handle_exceptions() const
{
    return impl_->handle_exceptions_;
}

void
userargs::handle_exceptions(bool value)
{
    impl_->handle_exceptions_ = value;
}

std::string
userargs::name_filter() const
{
    return impl_->name_filter_;
}

void
userargs::name_filter(const std::string& value)
{
    impl_->name_filter_ = value;
}

std::string
userargs::test_name() const
{
    return impl_->test_name_;
}

void
userargs::test_name(const std::string& value)
{
    impl_->test_name_ = value;
}

double
userargs::timeout() const
{
    return impl_->timeout_;
}

void
userargs::timeout(double value)
{
    impl_->timeout_ = value;
}

std::string
userargs::xml_filename() const
{
    return impl_->xml_filename_;
}

void
userargs::xml_filename(const std::string& value)
{
    impl_->xml_filename_ = value;
}

bool
userargs::dry_run() const
{
    return impl_->dry_run_;
}

void
userargs::dry_run(bool value)
{
    impl_->dry_run_ = value;
}

int
userargs::concurrent_threads() const
{
    return impl_->concurrent_threads_;
}

void
userargs::concurrent_threads(int value)
{
    impl_->concurrent_threads_ = value;
}

bool
userargs::disable_timeout() const
{
    return impl_->disable_timeout_;
}

void
userargs::disable_timeout(bool value)
{
    impl_->disable_timeout_ = value;
}

int
userargs::max_value_precision() const
{
    return impl_->max_value_precision_;
}

void
userargs::max_value_precision(int value)
{
    impl_->max_value_precision_ = value;
}

int
userargs::max_string_length() const
{
    return impl_->max_string_length_;
}

void
userargs::max_string_length(int value)
{
    impl_->max_string_length_ = value;
}

} // internals
} // unittest
