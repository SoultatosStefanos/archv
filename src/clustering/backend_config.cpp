#include "backend_config.hpp"

#include "plugin.hpp"

#include <algorithm>

namespace clustering
{

auto are_clusterers_plugged_in(const backend_config& cfg) -> bool
{
    return std::all_of(
        std::cbegin(cfg.clusterers),
        std::cend(cfg.clusterers),
        &is_clusterer_plugged_in);
}

auto are_mst_finders_plugged_in(const backend_config& cfg) -> bool
{
    return std::all_of(
        std::cbegin(cfg.mst_finders),
        std::cend(cfg.mst_finders),
        &is_mst_finder_plugged_in);
}

namespace
{
    inline auto is_listed_at(
        const backend_config::ids_type& ids, backend_config::id_view_type id)
        -> bool
    {
        return std::find(std::cbegin(ids), std::cend(ids), id)
            != std::cend(ids);
    }
} // namespace

auto is_clusterer_listed(const backend_config& cfg) -> bool
{
    return is_clusterer_listed(cfg, cfg.clusterer);
}

auto is_clusterer_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool
{
    return is_listed_at(cfg.clusterers, id);
}

auto is_mst_finder_listed(const backend_config& cfg) -> bool
{
    return is_mst_finder_listed(cfg, cfg.mst_finder);
}

auto is_mst_finder_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool
{
    return is_listed_at(cfg.mst_finders, id);
}

auto all_clusterers() -> backend_config::ids_type
{
    using id_type = backend_config::id_type;

    return backend_config::ids_type { id_type(k_spanning_tree_clusterer_id) };
}

auto all_mst_finders() -> backend_config::ids_type
{
    using id_type = backend_config::id_type;

    return backend_config::ids_type { id_type(prim_mst_id),
                                      id_type(kruskal_mst_id) };
}

} // namespace clustering