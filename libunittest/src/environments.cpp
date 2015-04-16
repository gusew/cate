#include <libunittest/environments.hpp>
#include <libunittest/argparser.hpp>
#include <libunittest/testsuite.hpp>
#include <libunittest/testresults.hpp>
#include <libunittest/utilities.hpp>
#include <iostream>
#include <fstream>

namespace unittest {

int
process(int argc, char **argv)
{
    internals::argparser arguments;
    try {
        arguments.parse(argc, argv);
    } catch (const internals::argparser_error& e) {
        std::cout << "Error: " << e.what();
        std::exit(EXIT_FAILURE);
    }

    auto suite = internals::testsuite::instance();
    suite->set_arguments(arguments);

    const auto class_runs = suite->get_class_runs();
    const int n_threads = arguments.concurrent_threads();
    internals::call_functions(class_runs, n_threads);

    const auto results = suite->get_results();
    write_error_info(std::cout, results.testlogs, results.successful);
    auto& threads = suite->get_lonely_threads();
    internals::make_threads_happy(std::cout, threads, arguments.verbose());
    const auto full_results = suite->get_results();

    std::vector<internals::testlog> delta_testlogs;
    bool successful = true;
    for (auto i=results.testlogs.size(); i<full_results.testlogs.size(); ++i) {
        delta_testlogs.push_back(full_results.testlogs[i]);
        if (!delta_testlogs.back().successful)
            successful = false;
    }
    write_error_info(std::cout, delta_testlogs, successful);

    write_summary(std::cout, full_results);
    if (arguments.generate_xml()) {
        std::ofstream file(arguments.xml_filename());
        write_xml(file, full_results);
    }

    return full_results.successful ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // unittest
