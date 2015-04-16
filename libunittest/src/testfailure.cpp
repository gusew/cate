#include <libunittest/testfailure.hpp>
#include <libunittest/utilities.hpp>


namespace unittest {

testfailure::testfailure(const std::string& assertion,
                         const std::string& message)
    : std::runtime_error(message),
      assertion_(assertion),
      spot_(std::make_pair("", -1))
{}

testfailure::testfailure(const std::string& assertion,
                         const std::string& message,
                         const std::string& user_msg)
    : std::runtime_error([&message,&user_msg]() -> std::string
        { const auto msg = internals::remove_file_and_line(user_msg);
          return msg.size() ? message + " - " + msg : message; }()),
      assertion_(assertion),
      spot_(internals::extract_file_and_line(user_msg))
{}

testfailure::~testfailure() noexcept
{}

std::string
testfailure::assertion() const
{
    return assertion_;
}

std::string
testfailure::filename() const
{
    return spot_.first;
}

int
testfailure::linenumber() const
{
    return spot_.second;
}

} // unittest
