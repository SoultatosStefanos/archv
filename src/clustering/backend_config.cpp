#include "backend_config.hpp"

#include "misc/algorithm.hpp"
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

auto is_clusterer_listed(const backend_config& cfg) -> bool
{
    return is_clusterer_listed(cfg, cfg.clusterer);
}

auto is_clusterer_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool
{
    return misc::container_contains(cfg.clusterers, id);
}

auto is_mst_finder_listed(const backend_config& cfg) -> bool
{
    return is_mst_finder_listed(cfg, cfg.mst_finder);
}

auto is_mst_finder_listed(
    const backend_config& cfg, backend_config::id_view_type id) -> bool
{
    return misc::container_contains(cfg.mst_finders, id);
}

auto all_clusterers() -> backend_config::ids_type
{
    using ids_type = backend_config::ids_type;
    return ids_type { std::cbegin(clusterer_ids), std::cend(clusterer_ids) };
}

auto all_mst_finders() -> backend_config::ids_type
{
    using ids_type = backend_config::ids_type;
    return ids_type { std::cbegin(mst_finders_ids),
                      std::cend(mst_finders_ids) };
}

} // namespace clustering