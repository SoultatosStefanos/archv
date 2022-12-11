#include "application/application.hpp"
#include "config/config.hpp"

#include <boost/exception/all.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <cstdlib>

static void init_logging()
{
    namespace logging = boost::log;

#ifdef NDEBUG
    logging::add_file_log(
        logging::keywords::file_name = "archv%N.log",
        logging::keywords::rotation_size = 10 * 1024 * 1024, // 10 MiB.
        logging::keywords::format = "[%TimeStamp%] [%Severity%]   %Message%");

    logging::add_console_log(
        std::cout,
        logging::keywords::format = "[%TimeStamp%] [%Severity%]   %Message%");

    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info);

    logging::add_common_attributes();
#else
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::trace);
#endif
}

static auto log_config_vars() -> void
{
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_RENDERING_CONFIG_PATH: " << ARCHV_RENDERING_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_GUI_CONFIG_PATH: " << ARCHV_GUI_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_WEIGHTS_CONFIG_PATH: " << ARCHV_WEIGHTS_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_LAYOUT_CONFIG_PATH: " << ARCHV_LAYOUT_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_SCALING_CONFIG_PATH: " << ARCHV_SCALING_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info) << "Found ARCHV_CLUSTERING_CONFIG_PATH: "
                            << ARCHV_CLUSTERING_CONFIG_PATH;
    BOOST_LOG_TRIVIAL(info)
        << "Found ARCHV_RESOURCE_GROUP: " << ARCHV_RESOURCE_GROUP;
}

auto main(int argc, const char* argv[]) -> int
{
    try
    {
        init_logging();
        log_config_vars();

        application::application app { argc, argv };
        app.initApp();
        app.go();
        app.closeApp();

        return EXIT_SUCCESS;
    }
    catch (const boost::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "error:\n"
                                 << boost::diagnostic_information(e);

        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "error: " << e.what();

        return EXIT_FAILURE;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << "unknown error";

        return EXIT_FAILURE;
    }
}
