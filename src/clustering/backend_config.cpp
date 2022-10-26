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
        const backend_config::ids_type& ids, const backend_config::id_type& id)
        -> bool
    {
        return std::find(std::cbegin(ids), std::cend(ids), id)
            != std::cend(ids);
    }
} // namespace

auto is_clusterer_listed(const backend_config& cfg) -> bool
{
    return is_listed_at(cfg.clusterers, cfg.clusterer);
}

auto is_mst_finder_listed(const backend_config& cfg) -> bool
{
    return is_listed_at(cfg.mst_finders, cfg.mst_finder);
}

} // namespace clustering