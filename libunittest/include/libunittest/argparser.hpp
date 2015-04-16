/**
 * @brief An argument parser and corresponding exception class
 * @file argparser.hpp
 */
#pragma once
#include <libunittest/userargs.hpp>
#include <string>
#include <stdexcept>
#include <memory>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief An argument parser
 */
class argparser : public unittest::internals::userargs {
public:
    /**
     * @brief Constructor
     */
    argparser();
    /**
     * @brief Destructor
     */
    ~argparser();
    /**
     * @brief Parses the arguments and assigns given values
     * @param argc The number of arguments
     * @param argv The array of arguments
     */
    void
    parse(int argc, char **argv);

private:

    argparser(const argparser& other) = delete;

    argparser& operator=(const argparser& other) = delete;

    argparser(argparser&& other) = delete;

    argparser& operator=(argparser&& other) = delete;

    struct impl;
    std::unique_ptr<impl> impl_;
};
/**
 * @brief The exception class to indicate argument errors
 */
class argparser_error : public std::runtime_error {
public:
    /**
     * @brief Constructor
     * @param message The exception message
     */
    explicit
    argparser_error(const std::string& message);
    /**
     * @brief Destructor
     */
    ~argparser_error() noexcept;
};

} // internals
} // unittest
