#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "utility/all.hpp"
#include "visualization/visualization.hpp"

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
    using namespace Visualization;
    using namespace Architecture;
    using namespace Generation;
    using namespace Utility;

    try
    {
        if (argc != 2)
        {
            std::cout << "usage: `./<exec> <json file path>`\n";

            return EXIT_FAILURE;
        }

        init_logging();

        const auto [graph, vertex_cache] = generate_graph(jsons.get(argv[1]));

        App app{graph,
                std::make_unique<GursoyAtunLayout<Sphere>>(graph, Sphere{80})};

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