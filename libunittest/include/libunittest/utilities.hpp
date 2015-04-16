/**
 * @brief Utilities used throughout this library
 * @file utilities.hpp
 */
#pragma once
#include <ostream>
#include <string>
#include <chrono>
#include <sstream>
#include <regex>
#include <limits>
#include <thread>
#include <atomic>
#include <memory>
#include <type_traits>
#include <utility>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief Escapes a string for use in an XML document
 * @param data Some string
 * @returns The escaped string
 */
std::string
xml_escape(const std::string& data);
/**
 * @brief Generates the ISO8601 time stamp from a time point object
 * @param time_point The time point object
 * @param local_time Whether to convert to local time (if false converts to UTC)
 * @returns The ISO8601 time stamp (e.g. 2013-06-29T14:12:05)
 */
std::string
make_iso_timestamp(const std::chrono::system_clock::time_point& time_point,
                   bool local_time=true);
/**
 * @brief An overload doing nothing to finish template recursion
 * @param stream An output stream
 */
void
write_to_stream(std::ostream& stream);
/**
 * @brief Writes arguments to an output stream via template recursion
 * @param stream An output stream
 * @param arg An argument
 * @param args An arbitrary number of arguments
 */
template<typename T,
         typename... Args>
void
write_to_stream(std::ostream& stream,
                const T& arg,
                const Args&... args)
{
    stream << arg;
    unittest::internals::write_to_stream(stream, args...);
}
/**
 * @brief Writes a horizontal bar to the given output stream
 * @param stream The output stream
 * @param character The character to repeat
 * @param length The length of the bar
 */
void
write_horizontal_bar(std::ostream& stream,
                     char character,
                     int length=79);
/**
 * @brief Casts a duration value to seconds
 * @param duration The duration value
 * @returns The duration in seconds
 */
double
duration_in_seconds(const std::chrono::duration<double>& duration);
/**
 * @brief Checks if two values are equal up to some epsilon
 * @param first A value
 * @param second Another value
 * @param eps The epsilon
 * @returns Whether the values are equal up to some epsilon
 */
template<typename T,
         typename U,
         typename V>
bool
is_approx_equal(const T& first,
                const U& second,
                const V& eps)
{
    V diff = 0;
    if (first > second)
        diff = static_cast<V>(first - second);
    else if (first < second)
        diff = static_cast<V>(second - first);
    return diff < eps;
}
/**
 * @brief Checks if a value is in a given range.
 *  The bounds are included
 * @param value A value
 * @param lower The lower bound
 * @param upper The upper bound
 * @returns Whether the value is in a given range
 */
template<typename T,
         typename U,
         typename V>
bool
is_in_range(const T& value,
            const U& lower,
            const V& upper)
{
    return !(value < lower) && !(value > upper);
}
/**
 * @brief Checks if a value is in a container
 * @param value A value
 * @param container A container
 * @returns Whether the value is in the container
 */
template<typename T,
         typename Container>
bool
is_contained(const T& value,
             const Container& container)
{
    auto first = std::begin(container);
    auto last = std::end(container);
    return std::find(first, last, value) != last;
}
/**
 * @brief Checks if a value is approx. in a container
 *  up to some epsilon
 * @param value A value
 * @param container A container
 * @param eps The epsilon
 * @returns Whether the value is approx. in the container
 */
template<typename T,
         typename Container,
         typename U>
bool
is_approx_contained(const T& value,
                    const Container& container,
                    const U& eps)
{
    auto first = std::begin(container);
    auto last = std::end(container);
    while (first!=last) {
        if (unittest::internals::is_approx_equal(*first, value, eps)) return true;
        ++first;
    }
    return false;
}
/**
 * @brief Checks if two containers are equal
 * @param first A container
 * @param second Another container
 * @returns Whether the containers are equal
 */
template<typename Container1,
         typename Container2>
bool
is_containers_equal(const Container1& first,
                    const Container2& second)
{
    auto begin1 = std::begin(first);
    auto end1 = std::end(first);
    auto begin2 = std::begin(second);
    auto end2 = std::end(second);
    while (begin1!=end1 && begin2!=end2) {
        if (*begin1!=*begin2) return false;
        ++begin1;
        ++begin2;
    }
    return begin1==end1 && begin2==end2;
}
/**
 * @brief Checks if two containers are approx. equal
 *  up to some epsilon
 * @param first A container
 * @param second Another container
 * @param eps The epsilon
 * @returns Whether the two containers are approx. equal
 */
template<typename Container1,
         typename Container2,
         typename V>
bool
is_containers_approx_equal(const Container1& first,
                           const Container2& second,
                           const V& eps)
{
    auto begin1 = std::begin(first);
    auto end1 = std::end(first);
    auto begin2 = std::begin(second);
    auto end2 = std::end(second);
    while (begin1!=end1 && begin2!=end2) {
        if (!unittest::internals::is_approx_equal(*begin1, *begin2, eps)) return false;
        ++begin1;
        ++begin2;
    }
    return begin1==end1 && begin2==end2;
}
/**
 * @brief Checks if a value matches a regular expression
 * @param value A value
 * @param regex A regular expression
 * @returns Whether a value matches the regular expression
 */
bool
is_regex_matched(const std::string& value,
                 const std::string& regex);
/**
 * @brief Checks if a value matches a regular expression
 * @param value A value
 * @param regex A regular expression
 * @returns Whether a value matches the regular expression
 */
bool
is_regex_matched(const std::string& value,
                 const std::regex& regex);
/**
 * @brief Calls each function of a given vector of functions
 * @param functions The vector of functions
 * @param n_threads The number of concurrent threads, ignored if <= 1
 * @returns The number of function calls
 */
int
call_functions(const std::vector<std::function<void()>>& functions,
               int n_threads=1);
/**
 * @brief Limits a given string to some maximum length
 * @param value The string
 * @param max_length The maximum length
 * @returns A string not exceeding a length of max_length
 */
std::string
limit_string_length(const std::string& value,
                    int max_length);
/**
 * @brief Builds a string from given filename and line number
 * @param filename The name of the file
 * @param linenumber The line number within the file
 * @returns A string build from given filename and line number
 */
std::string
string_of_file_and_line(const std::string& filename,
                        int linenumber);
/**
 * @brief Ensures that all threads passed will be finished and joined.
 *  This makes the threads happy.
 * @param stream An output stream
 * @param threads The vector of lonely threads
 * @param verbose Whether output is verbose
 */
void
make_threads_happy(std::ostream& stream,
                   std::vector<std::pair<std::thread, std::shared_ptr<std::atomic_bool>>>& threads,
                   bool verbose);
/**
 * @brief Gets a value from a map for a given key
 * @param map The map
 * @param key The key
 * @returns The value
 * @throws std::runtime_error if key not found in map
 */
template<typename Map>
typename Map::mapped_type
get_from_map(const Map& map,
             const typename Map::key_type& key)
{
    const auto& element = map.find(key);
    if (element==map.end())
        throw std::runtime_error("key not found in map");
    return element->second;
}
/**
 * @brief Returns a unique type id
 * @returns A unique type id
 */
template<typename T>
std::string
get_type_id()
{
    return typeid(T).name();
}
/**
 * @brief Removes leading and trailing white characters
 * @param value The string to trim
 * @returns The new string
 */
std::string
trim(std::string value);
/**
 * @brief Removes all white spaces
 * @param value The string to remove white spaces from
 * @returns The new string
 */
std::string
remove_white_spaces(std::string value);
/**
 * @brief Checks whether a given string has a valid numeric representation
 * @param value The string to check
 * @returns Whether a given string has a valid numeric representation
 */
bool
is_numeric(const std::string& value);
/**
 * @brief Converts a given string to a number of type T
 * @param value The string to convert
 * @returns The number
 * @throws std::invalid_argument if given string is not numeric
 */
template<typename T>
T
to_number(const std::string& value)
{
    std::istringstream stream(value);
    double number;
    if (stream >> number) {
        return static_cast<T>(number);
    } else {
        throw std::invalid_argument("Not numeric: " + value);
    }
}
/**
 * @brief Expands command line arguments (e.g. -ag becomes -a -g)
 * @param arguments The raw arguments
 * @returns The expanded arguments
 */
std::vector<std::string>
expand_commandline_arguments(const std::vector<std::string>& arguments);
/**
 * @brief Extracts file and line information from the given message
 * @param message The message
 * @returns A pair of file name and line number
 */
std::pair<std::string, int>
extract_file_and_line(const std::string& message);
/**
 * @brief Removes file and line information from the given message
 * @param message The message
 * @returns The new message
 */
std::string
remove_file_and_line(std::string message);
/**
 * @brief Creates a unique pointer to an object of type T
 * @param is_array Whether T is an array type
 * @param args The parameters to construct type T
 * @returns A unique pointer
 */
template<typename T,
         typename... Args>
std::unique_ptr<T>
make_unique_helper(std::false_type is_array,
                   Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
/**
 * @brief Creates a unique pointer to an object of type T
 * @param is_array Whether T is an array type
 * @param args The parameters to construct type T
 * @returns A unique pointer
 */
template<typename T,
         typename... Args>
std::unique_ptr<T>
make_unique_helper(std::true_type is_array,
                   Args&&... args)
{
    static_assert(std::extent<T>::value == 0,
          "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");
    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}
/**
 * @brief Creates a unique pointer to an object of type T
 * @param args The parameters to construct type T
 * @returns A unique pointer
 */
template<typename T,
         typename... Args>
std::unique_ptr<T>
make_unique(Args&&... args)
{
    return unittest::internals::make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

} // internals

/**
 * @brief Joins an arbitrary number of input arguments to a single string.
 *  All arguments must implement the << operator
 * @param arg An argument
 * @param args An arbitrary number of arguments, can be omitted
 * @returns The joined string
 */
template<typename T,
         typename... Args>
std::string
join(const T& arg,
     const Args&... args)
{
    std::ostringstream stream;
    stream << arg;
    unittest::internals::write_to_stream(stream, args...);
    return stream.str();
}
/**
 * @brief Machine epsilon of float
 */
const float feps = std::numeric_limits<float>::epsilon();
/**
 * @brief Machine epsilon of double
 */
const double deps = std::numeric_limits<double>::epsilon();
/**
 * @brief A utility class to indicate 'no type' property
 */
class notype {};
/**
 * @brief A utility class to indicate 'some type' property
 */
struct sometype {};

} // unittest
