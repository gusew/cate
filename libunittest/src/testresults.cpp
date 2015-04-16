#include <libunittest/testresults.hpp>
#include <libunittest/teststatus.hpp>
#include <libunittest/utilities.hpp>
#include <mutex>

namespace unittest {
namespace internals {

testresults::testresults()
    : successful(true), n_tests(0), n_successes(0), n_failures(0),
      n_errors(0), n_skipped(0), n_timeouts(0), duration(0), testlogs(0)
{}

void
write_xml(std::ostream& stream,
          const testresults& results,
          const std::chrono::system_clock::time_point& time_point,
          bool local_time)
{
    static std::mutex write_xml_mutex_;
    std::lock_guard<std::mutex> lock(write_xml_mutex_);
    stream.setf(std::ios_base::fixed);
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    stream << "\n";
    stream << "<testsuite name=\"libunittest";
    stream << "\" timestamp=\"" << make_iso_timestamp(time_point, local_time);
    stream << "\" tests=\"" << results.n_tests + results.n_skipped;
    stream << "\" errors=\"" << results.n_errors;
    stream << "\" failures=\"" << results.n_failures;
    stream << "\" timeouts=\"" << results.n_timeouts;
    stream << "\" skipped=\"" << results.n_skipped;
    stream << "\" time=\"" << results.duration << "\">";
    stream << "\n";
    for (auto& log : results.testlogs) {
        std::string system_out;
        if (log.text.size()) {
            system_out += "\t\t<system-out>\n";
            system_out += "\t\t\t" + xml_escape(trim(log.text)) + "\n";
            system_out += "\t\t</system-out>\n";
        }
        stream << "\t<testcase ";
        if (log.class_name.size())
            stream << "classname=\"" << xml_escape(log.class_name) << "\" ";
        stream << "name=\"" << xml_escape(log.test_name);
        if (log.assertion.size())
            stream << "\" assertions=\"" << xml_escape(log.assertion);
        if (log.filename.size())
            stream << "\" file=\"" << xml_escape(log.filename);
        if (log.linenumber>-1)
            stream << "\" line=\"" << log.linenumber;
        stream << "\" time=\"" << log.duration << "\"";
        if (log.has_timed_out)
            stream << " timeout=\"" << log.timeout << "\"";
        if (log.successful) {
            if (log.status==teststatus::skipped) {
                stream << ">";
                stream << "\n";
                stream << "\t\t<skipped/>";
                stream << "\n";
                stream << system_out;
                stream << "\t</testcase>";
            } else {
                if (system_out.size()) {
                    stream << ">\n";
                    stream << system_out;
                    stream << "\t</testcase>";
                } else {
                    stream << "/>";
                }
            }
            stream << "\n";
        } else {
            stream << ">";
            stream << "\n";
            std::string name("error");
            if (log.status==teststatus::failure)
                name = "failure";
            stream << "\t\t<" << name << " ";
            stream << "type=\"" << xml_escape(log.error_type);
            stream << "\" message=\"" << xml_escape(trim(log.message)) << "\"/>";
            stream << "\n";
            stream << system_out;
            stream << "\t</testcase>";
            stream << "\n";
        }
    }
    stream << "</testsuite>";
    stream << "\n";
    stream << std::flush;
    stream.unsetf(std::ios_base::fixed);
}

void
write_summary(std::ostream& stream,
              const testresults& results)
{
    static std::mutex write_summary_mutex_;
    std::lock_guard<std::mutex> lock(write_summary_mutex_);
    stream << "\n";
    write_horizontal_bar(stream, '-');
    stream << "\n";
    stream << "Ran " << results.n_tests << " tests in ";
    stream << results.duration << "s\n\n";
    if (results.n_tests==results.n_successes) {
        stream << "OK";
        if (results.n_skipped>0 && results.n_timeouts>0) {
            stream << " (skipped=" << results.n_skipped;
            stream << ", timeouts=" << results.n_timeouts << ")";
        } else if (results.n_skipped>0) {
            stream << " (skipped=" << results.n_skipped << ")";
        } else if (results.n_timeouts>0) {
            stream << " (timeouts=" << results.n_timeouts << ")";
        }
        stream << "\n";
    } else {
        stream << "FAILED (";
        if (results.n_failures>0 && results.n_errors>0) {
            stream << "failures=" << results.n_failures;
            stream << ", errors=" << results.n_errors;
        } else if (results.n_failures>0) {
            stream << "failures=" << results.n_failures;
        } else if (results.n_errors>0) {
            stream << "errors=" << results.n_errors;
        }
        if (results.n_skipped>0)
            stream << ", skipped=" << results.n_skipped;
        if (results.n_timeouts>0)
            stream << ", timeouts=" << results.n_timeouts;
        stream << ")\n";
    }
    stream << std::flush;
}

void
write_error_info(std::ostream& stream,
                 const std::vector<testlog>& testlogs,
                 bool successful)
{
    static std::mutex write_error_info_mutex_;
    std::lock_guard<std::mutex> lock(write_error_info_mutex_);
    if (!successful) {
        stream << "\n";
        for (auto& log : testlogs) {
            const auto status = log.status;
            if (status==teststatus::failure || status==teststatus::error) {
                write_horizontal_bar(stream, '=');
                stream << "\n";
                std::string flag("FAIL");
                if (status==teststatus::error)
                    flag = "ERROR";
                stream << flag << ": " << make_full_test_name(log.class_name, log.test_name);
                stream << " [" << log.duration << "s]";
                if (log.has_timed_out)
                    stream << " (TIMEOUT)";
                stream << "\n";
                write_horizontal_bar(stream, '-');
                stream << "\n";
                stream << log.error_type << ": " << trim(log.message);
                if (log.assertion.size()) {
                    stream << "\n";
                    stream << "assertion: " << log.assertion;
                    if (log.filename.size()) {
                        stream << " in " << trim(log.filename);
                        if (log.linenumber>-1)
                            stream << " at line " << log.linenumber;
                    }
                }
                if (log.text.size()) {
                    stream << "\n";
                    write_horizontal_bar(stream, '-');
                    stream << "\n";
                    stream << "INFO: " << trim(log.text);
                }
                stream << "\n\n";
            }
        }
        stream << std::flush;
    }
}

} // internals
} // unittest
