#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "json_manager.hpp"
#include "visualization/visualization.hpp"

#include <boost/exception/all.hpp>
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
            std::cerr << "usage: `./<exec> <json file path>`\n";

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
        std::cerr << boost::diagnostic_information(e) << '\n';

        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';

        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "unknown error" << '\n';

        return EXIT_FAILURE;
    }
}
