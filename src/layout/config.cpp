#include "config.hpp"

#include "backend_config.hpp"
#include "misc/deserialization.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace layout
{

using namespace misc;

namespace
{
    template < typename Container >
    inline auto read_json_array(const json_val& val) -> Container
    {
        using value_type = typename Container::value_type;

        Container res;

        std::transform(
            std::begin(val),
            std::end(val),
            std::inserter(res, std::begin(res)),
            [](const json_val& val) { return as< value_type >(val); });

        return res;
    }

    inline auto deserialize_layouts(const json_val& root)
    {
        using json_layouts = std::vector< config_data::id_type >;
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< json_layouts >(get(root, "layouts"));
    }

    inline auto deserialize_topologies(const json_val& root)
    {
        using json_topologies = std::vector< config_data::id_type >;
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< json_topologies >(get(root, "topologies"));
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    auto&& layouts = deserialize_layouts(root);
    auto&& topologies = deserialize_topologies(root);
    auto&& layout = as< config_data::id_type >(get(root, "layout"));
    auto&& topology = as< config_data::id_type >(get(root, "topology"));
    auto&& scale = as< config_data::scale_type >(get(root, "scale"));

    config_data res { .layouts = std::move(layouts),
                      .topologies = std::move(topologies),
                      .layout = std::move(layout),
                      .topology = std::move(topology),
                      .scale = scale };

    return res;
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
    serialize_ids(root["layouts"], cfg.layouts);
    serialize_ids(root["topologies"], cfg.topologies);
    root["layout"] = cfg.layout;
    root["topology"] = cfg.topology;
    root["scale"] = cfg.scale;

    BOOST_LOG_TRIVIAL(debug) << "serialized layout";
}

} // namespace layout