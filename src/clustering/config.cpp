#include "config.hpp"

#include "backend_config.hpp"
#include "misc/deserialization.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace clustering
{

using namespace misc;

namespace
{
    inline auto deserialize_ids(const json_val& root)
    {
        using id_type = config_data::id_type;
        using ids_type = config_data::ids_type;

        ids_type ids;
        std::transform(
            std::cbegin(root),
            std::cend(root),
            std::back_inserter(ids),
            [](const json_val& val) { return as< id_type >(val); });

        return ids;
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    using id_type = config_data::id_type;
    using intensity_type = config_data::intensity_type;
    using k_type = config_data::k_type;
    using snn_thres_type = config_data::snn_threshold_type;
    using modularity_type = config_data::modularity_type;
    using gamma_type = config_data::gamma_type;
    using steps_type = config_data::steps_type;

    auto&& clusterers = deserialize_ids(get(root, "clusterers"));
    auto&& mst_finders
        = deserialize_ids(get(root, "min-spanning-tree-finders"));
    auto&& clusterer = as< id_type >(get(root, "clusterer"));
    auto intensity = as< intensity_type >(get(root, "intensity"));
    auto&& mst_finder = as< id_type >(get(root, "min-spanning-tree-finder"));
    auto k = as< k_type >(get(root, "k"));
    auto snn_thres = as< snn_thres_type >(get(root, "snn-threshold"));

    static_assert(std::is_floating_point_v< modularity_type >);
    auto min_q = as< double >(get(root, "min-modularity"));

    static_assert(std::is_floating_point_v< gamma_type >);
    auto llp_gamma = as< double >(get(root, "llp-gamma"));
    auto llp_steps = as< steps_type >(get(root, "llp-steps"));

    return config_data { .clusterers = std::move(clusterers),
                         .mst_finders = std::move(mst_finders),
                         .clusterer = std::move(clusterer),
                         .intensity = intensity,
                         .mst_finder = std::move(mst_finder),
                         .k = k,
                         .snn_threshold = snn_thres,
                         .min_modularity
                         = static_cast< modularity_type >(min_q),
                         .llp_gamma = static_cast< gamma_type >(llp_gamma),
                         .llp_steps = llp_steps };
}

namespace
{

    auto serialize_ids(json_val& val, const config_data::ids_type& ids)
    {
        for (const auto& id : ids)
            val.append(id);
    }

} // namespace

auto serialize(json_val& root, const config_data& cfg) -> void
{
    serialize_ids(root["clusterers"], cfg.clusterers);
    serialize_ids(root["min-spanning-tree-finders"], cfg.mst_finders);
    root["clusterer"] = cfg.clusterer;
    root["intensity"] = cfg.intensity;
    root["min-spanning-tree-finder"] = cfg.mst_finder;
    root["k"] = cfg.k;
    root["snn-threshold"] = cfg.snn_threshold;
    root["min-modularity"] = cfg.min_modularity;
    root["llp-gamma"] = cfg.llp_gamma;
    root["llp-steps"] = cfg.llp_steps;

    BOOST_LOG_TRIVIAL(debug) << "serialized clustering";
}

} // namespace clustering