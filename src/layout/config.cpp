#include "config.hpp"

#include "backend.hpp"

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
        using json_layouts = std::vector< std::string >;
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< json_layouts >(root["layouts"]);
    }

    inline auto deserialize_topologies(const Json::Value& root)
    {
        using json_topologies = std::vector< std::string >;
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< json_topologies >(root["topologies"]);
    }

    auto deserialize_defaults(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading defaults";

        const auto& defaults = root;

        std::string layout, topology;
        double scale;

        for (auto iter = std::begin(defaults); iter != std::end(defaults);
             ++iter)
        {
            if (iter.name() == "layout")
            {
                layout = (*iter).asString();
            }
            else if (iter.name() == "topology")
            {
                topology = (*iter).asString();
            }
            else if (iter.name() == "scale")
            {
                scale = (*iter).asDouble();
            }
        }

        return std::make_tuple(std::move(layout), std::move(topology), scale);
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& layouts = deserialize_layouts(root);
    auto&& topologies = deserialize_topologies(root);
    auto&& [layout, topology, scale] = deserialize_defaults(root);

    config_data res { .layouts = std::move(layouts),
                      .topologies = std::move(topologies),
                      .layout = std::move(layout),
                      .topology = std::move(topology),
                      .scale = scale };

    return res;
}

} // namespace layout