#include <libunittest/random.hpp>

namespace unittest {

random_value<bool>::random_value()
    : random_object<bool>(), distribution_(0, 1)
{}

bool
random_value<bool>::do_get()
{
    return distribution_(this->gen()) == 1;
}

random_value<bool>
make_random_bool()
{
    return random_value<bool>();
}

} // unittest
