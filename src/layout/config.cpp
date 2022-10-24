#include "config.hpp"

#include "backend_config.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>

namespace layout
{

namespace
{
    template < typename Container >
    inline auto read_json_array(const Json::Value& val) -> Container
    {
        using value_type = typename Container::value_type;

        Container res;

        std::transform(
            std::begin(val),
            std::end(val),
            std::inserter(res, std::begin(res)),
            [](const Json::Value& val) { return val.as< value_type >(); });

        return res;
    }

    inline auto deserialize_layouts(const Json::Value& root)
    {
        using json_layouts = std::vector< config_data::id_type >;
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< json_layouts >(root["layouts"]);
    }

    inline auto deserialize_topologies(const Json::Value& root)
    {
        using json_topologies = std::vector< config_data::id_type >;
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< json_topologies >(root["topologies"]);
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& layouts = deserialize_layouts(root);
    auto&& topologies = deserialize_topologies(root);
    auto&& layout = root["layout"].as< config_data::id_type >();
    auto&& topology = root["topology"].as< config_data::id_type >();
    auto&& scale = root["scale"].as< config_data::scale_type >();

    config_data res { .layouts = std::move(layouts),
                      .topologies = std::move(topologies),
                      .layout = std::move(layout),
                      .topology = std::move(topology),
                      .scale = scale };

    return res;
}

} // namespace layout