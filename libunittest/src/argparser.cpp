#include <libunittest/argparser.hpp>
#include <libunittest/utilities.hpp>
#include <libunittest/version.hpp>
#include <iostream>
#include <sstream>

namespace unittest {
namespace internals {

struct argparser::impl {

    void
    write_help(std::ostream& stream,
               const std::string& app_name) const
    {
        stream << "This is your testing application using libunittest-";
        stream << version << "\n\n";
        stream << "Usage: " << app_name << " [Options]\n\n";
        stream << "Options:\n";
        stream << "-h            Displays this help message\n";
        stream << "-v            Sets verbose output for running tests\n";
        stream << "-d            A dry run without actually executing any tests\n";
        stream << "-s            Stops running tests after the first test fails\n";
        stream << "-x            Enables the generation of the XML output\n";
        stream << "-e            Turns off handling of unexpected exceptions\n";
        stream << "-i            Disables the measurement of any test timeouts\n";
        stream << "-p number     Runs tests in parallel with a given number of threads\n";
        stream << "-f filter     A filter applied to the beginning of the test names\n";
        stream << "-n name       A certain test to be run superseding the name filter\n";
        stream << "-t timeout    A timeout in seconds for tests without local timeouts\n";
        stream << "-o xmlfile    The XML output file name (default: libunittest.xml)\n";
        stream << "-l length     The maximum displayed string length (default: 500)\n";
        stream << "-r precision  The maximum displayed value precision (default: 10)\n";
        stream << std::flush;
    }

    void
    help_and_throw(const std::string& app_name,
                   const std::string& message) const
    {
        std::ostringstream stream;
        write_help(stream, app_name);
        throw argparser_error(join(message, "\n\n", stream.str()));
    }

    template<typename T>
    T
    make_number(const std::string& app_name,
                const std::string& argument,
                const std::string& value) const
    {
        T result = 0;
        try {
            result = to_number<T>(value);
        } catch (const std::invalid_argument&) {
            const std::string message = join("The value to '", argument,"' must be numeric, not: ", value);
            help_and_throw(app_name, message);
        }
        return result;
    }

    std::vector<std::string>
    expand_arguments(int argc, char **argv) const
    {
        std::vector<std::string> arguments;
        arguments.reserve(argc - 1);
        for (int i=1; i<argc; ++i)
            arguments.push_back(argv[i]);
        return expand_commandline_arguments(arguments);
    }

};

argparser::argparser()
    : userargs(),
      impl_(internals::make_unique<impl>())
{}

argparser::~argparser()
{}

void
argparser::parse(int argc, char **argv)
{
    const std::string app_name = argv[0];
    const auto args = impl_->expand_arguments(argc, argv);
    const auto length = args.size();
    for (size_t i=0; i<length; ++i) {
        if (args[i]=="-h") {
            impl_->write_help(std::cout, app_name);
            std::exit(EXIT_SUCCESS);
        } else if (args[i]=="-v") {
            verbose(true);
        } else if (args[i]=="-s") {
            failure_stop(true);
        } else if (args[i]=="-x") {
            generate_xml(true);
        } else if (args[i]=="-e") {
            handle_exceptions(false);
        } else if (args[i]=="-d") {
            dry_run(true);
        } else if (args[i]=="-i") {
            disable_timeout(true);
        } else if (args[i]=="-f") {
            if (++i<length)
                name_filter(args[i]);
            else
                impl_->help_and_throw(app_name, "Option '-f' needs a filter string");
        } else if (args[i]=="-n") {
            if (++i<length)
                test_name(args[i]);
            else
                impl_->help_and_throw(app_name, "Option '-n' needs a test name");
        } else if (args[i]=="-t") {
            if (++i<length)
                timeout(impl_->make_number<double>(app_name, "-t", args[i]));
            else
                impl_->help_and_throw(app_name, "Option '-t' needs a timeout");
        } else if (args[i]=="-o") {
            if (++i<length) {
                xml_filename(args[i]);
                generate_xml(true);
            } else
                impl_->help_and_throw(app_name, "Option '-o' needs an XML file name");
        } else if (args[i]=="-p") {
            if (++i<length)
                concurrent_threads(impl_->make_number<int>(app_name, "-p", args[i]));
            else
                impl_->help_and_throw(app_name, "Option '-p' needs the number of threads");
        } else if (args[i]=="-l") {
            if (++i<length)
                max_string_length(impl_->make_number<int>(app_name, "-s", args[i]));
            else
                impl_->help_and_throw(app_name, "Option '-s' needs the maximum displayed string length");
        } else if (args[i]=="-r") {
            if (++i<length)
                max_value_precision(impl_->make_number<int>(app_name, "-r", args[i]));
            else
                impl_->help_and_throw(app_name, "Option '-r' needs the maximum displayed value precision");
        } else {
        	impl_->help_and_throw(app_name, join("Unknown option '", args[i], "'"));
        }
    }
}

argparser_error::argparser_error(const std::string& message)
    : std::runtime_error(message)
{}

argparser_error::~argparser_error() noexcept
{}

} // internals
} // unittest
