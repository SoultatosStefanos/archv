#include "config.hpp"

#include "backend_config.hpp"

#include <algorithm>

namespace clustering
{

namespace
{
    inline auto deserialize_ids(const Json::Value& root)
    {
        using id_type = config_data::id_type;
        using ids_type = config_data::ids_type;

        ids_type ids;
        std::transform(
            std::cbegin(root),
            std::cend(root),
            std::back_inserter(ids),
            [](const Json::Value& val) { return val.as< id_type >(); });

        return ids;
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    using id_type = config_data::id_type;
    using k_type = config_data::k_type;
    using snn_thres_type = config_data::snn_threshold_type;

    auto&& clusterers = deserialize_ids(root["clusterers"]);
    auto&& mst_finders = deserialize_ids(root["min-spanning-tree-finders"]);
    auto&& clusterer = root["clusterer"].as< id_type >();
    auto&& mst_finder = root["min-spanning-tree-finder"].as< id_type >();
    auto k = root["k"].as< k_type >();
    auto snn_thres = root["snn-threshold"].as< snn_thres_type >();

    return config_data { .clusterers = std::move(clusterers),
                         .mst_finders = std::move(mst_finders),
                         .clusterer = std::move(clusterer),
                         .mst_finder = std::move(mst_finder),
                         .k = k,
                         .snn_threshold = snn_thres };
}

} // namespace clustering