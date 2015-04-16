#include <libunittest/formatting.hpp>
#include <libunittest/utilities.hpp>

namespace unittest {
namespace internals {

std::string
stream_to_string(const std::ostringstream& stream)
{
    return "'" + limit_string_length(stream.str(), unittest::internals::testsuite::instance()->get_arguments().max_string_length()) + "'";
}

} // internals
} // unittest
