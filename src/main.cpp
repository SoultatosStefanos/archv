#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "json_manager.hpp"
#include "visualization/visualization.hpp"

#include <boost/exception/all.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>
#include <filesystem>
#include <string_view>

// NOTE: Demo currently

// TODO Print good stacktraces on exceptions.
// TODO Quick graph visualization test with ogre head mesh and camera man, plus
// test memory management.

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

        const auto [graph, vertex_cache] =
            generate_graph(JsonManager::get().croot(argv[1]));

        App app{
            graph,
            LayoutFactory::make_layout(graph, LayoutFactory::Topology::cube)};

        app.initApp();
        // render
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
