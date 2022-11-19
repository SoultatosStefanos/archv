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
    using modularity_type = config_data::modularity_type;
    using gamma_type = config_data::gamma_type;
    using steps_type = config_data::steps_type;

    auto&& clusterers = deserialize_ids(root["clusterers"]);
    auto&& mst_finders = deserialize_ids(root["min-spanning-tree-finders"]);
    auto&& clusterer = root["clusterer"].as< id_type >();
    auto&& mst_finder = root["min-spanning-tree-finder"].as< id_type >();
    auto k = root["k"].as< k_type >();
    auto snn_thres = root["snn-threshold"].as< snn_thres_type >();

    static_assert(std::is_floating_point_v< modularity_type >);
    auto min_q = root["min-modularity"].as< double >();

    static_assert(std::is_floating_point_v< gamma_type >);
    auto llp_gamma = root["llp-gamma"].as< double >();
    auto llp_steps = root["llp-steps"].as< steps_type >();

    return config_data { .clusterers = std::move(clusterers),
                         .mst_finders = std::move(mst_finders),
                         .clusterer = std::move(clusterer),
                         .mst_finder = std::move(mst_finder),
                         .k = k,
                         .snn_threshold = snn_thres,
                         .min_modularity
                         = static_cast< modularity_type >(min_q),
                         .llp_gamma = static_cast< gamma_type >(llp_gamma),
                         .llp_steps = llp_steps };
}

} // namespace clustering