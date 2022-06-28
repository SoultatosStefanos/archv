#include "architecture/architecture.hpp"
#include "clustering/clustering.hpp"
#include "generation/generation.hpp"
#include "visualization/visualization.hpp"

#include <cstdlib>

using namespace Visualization;
using namespace Architecture;

// NOTE: Demo currently

// TODO generate graph
// TODO Catch and log exceptions
auto main(int argc, char const* argv[]) -> int
{
    const auto g = Graph();
    const auto l = LayoutFactory::make_layout(g, LayoutFactory::Topology::cube);

    App app{g, *l};
    app.initApp();
    // render
    app.closeApp();

    return EXIT_SUCCESS;
}
