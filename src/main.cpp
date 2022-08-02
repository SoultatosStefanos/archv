#include "app.hpp"
#include "architecture/all.hpp"
#include "config/all.hpp"
#include "utility/all.hpp"

#include <boost/exception/all.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>
#include <filesystem>
#include <string_view>

// NOTE: Demo currently

static void init_logging()
{
    namespace logging = boost::log;

#ifdef NDEBUG
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info);
#endif
}

auto main(int argc, char const* argv[]) -> int
{
    using namespace architecture;
    using namespace config;
    using namespace utility;

    try
    {
        if (argc != 2)
        {
            std::cout << "usage: `./<exec> <json file path>`\n";

            return EXIT_FAILURE;
        }

        init_logging();

        const auto st = deserialize_symbols(json_archive::get().at(argv[1]));
        const auto [g, vertex_cache]
            = deserialize_dependencies(json_archive::get().at(argv[1]), st);

        app archv { g };

        archv.initApp();
        archv.getRoot()->startRendering();
        archv.closeApp();

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
