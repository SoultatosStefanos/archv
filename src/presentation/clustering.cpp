#include "clustering.hpp"

#include "def.hpp"
#include "graph_interface.hpp"

#include <boost/log/trivial.hpp>

namespace presentation
{

auto show_clusters(const graph_interface& g, graph_renderer& renderer) -> void
{
    if (clustering::get_clusters(g.get_clustering_backend()).empty())
    {
        hide_clusters(g, renderer); // essentially render 'unclustered' clusters
        return;
    }

    renderer.render_clusters(vertex_cluster(g));

    const auto untangled = layout::untangle_layout(
        g.get_graph(),
        vertex_cluster(g),
        g.get_layout_backend(),
        clustering::get_intensity(g.get_clustering_backend()));

    renderer.render_layout(layout::make_lposition_map(*untangled));

    BOOST_LOG_TRIVIAL(debug) << "showed clusters";
}

auto hide_clusters(const graph_interface& g, graph_renderer& renderer) -> void
{
    renderer.hide_clusters();
    renderer.render_layout(vertex_position(g));

    BOOST_LOG_TRIVIAL(debug) << "hid clusters";
}

} // namespace presentation