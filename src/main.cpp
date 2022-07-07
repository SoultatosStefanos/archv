#include "architecture/all.hpp"
#include "clustering/all.hpp"
#include "generation/generation.hpp"
#include "utility/all.hpp"
#include "visualization/all.hpp"

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
    logging::core::get()->set_filter(logging::trivial::severity >=
                                     logging::trivial::info);
#endif
}

auto main(int argc, char const* argv[]) -> int
{
    using namespace visualization;
    using namespace architecture;
    using namespace generation;
    using namespace utility;

    try
    {
        if (argc != 2)
        {
            std::cout << "usage: `./<exec> <json file path>`\n";

            return EXIT_FAILURE;
        }

        init_logging();

        const auto [g, vertex_cache] = generate_graph(jsons.get(argv[1]));

        application app{g};

        app.initApp();
        app.getRoot()->startRendering();
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
