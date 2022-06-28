#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "utility.hpp"
#include "visualization/visualization.hpp"

#include <boost/exception/all.hpp>
#include <cstdlib>
#include <filesystem>
#include <string_view>

// NOTE: Demo currently

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
