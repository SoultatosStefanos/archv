#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "utility.hpp"
#include "visualization/visualization.hpp"

#include <boost/exception/all.hpp>
#include <cstdlib>
#include <filesystem>
#include <string_view>

using namespace Visualization;
using namespace Architecture;
using namespace Generation;
using namespace Utility;

// NOTE: Demo currently

// TODO generate graph
// TODO Catch and log exceptions
auto main(int argc, char const* argv[]) -> int
{
    if (argc != 2)
    {
        std::cerr << "usage: `./<exec> <json file path>`\n";
        return EXIT_FAILURE;
    }

    const auto [g, cache] = generate_graph(JsonManager::get().croot(argv[1]));

    App app{g, LayoutFactory::make_layout(g, LayoutFactory::Topology::cube)};
    app.initApp();
    // render
    app.closeApp();

    return EXIT_SUCCESS;
}
