#include "config.hpp"

#include "gursoy_atun_layout.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <unordered_set>

namespace layout
{

namespace
{
    using layout_options = config_data::layout_options;
    using topology_options = config_data::topology_options;

    static const layout_options valid_layouts { "Gursoy Atun" };
    static const topology_options valid_topologies { "Cube", "Sphere" };

    auto verify_layouts(const config_data& data)
    {
        for (const auto& lay : data.layouts)
            if (!valid_layouts.contains(lay))
                BOOST_THROW_EXCEPTION(unknown_layout() << layout_info(lay));
    }

    auto verify_topologies(const config_data& data)
    {
        for (const auto& s : data.topologies)
            if (!valid_topologies.contains(s))
                BOOST_THROW_EXCEPTION(unknown_topology() << topology_info(s));
    }

    auto verify_defaults(const config_data& data)
    {
        if (!data.layouts.contains(data.layout))
            BOOST_THROW_EXCEPTION(
                unknown_default() << layout_info(data.layout));

        if (!data.topologies.contains(data.topology))
            BOOST_THROW_EXCEPTION(
                unknown_default() << topology_info(data.topology));
    }

    auto verify(const config_data& data)
    {
        verify_layouts(data);
        verify_topologies(data);
        verify_defaults(data);
    }

} // namespace

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
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< layout_options >(root["layouts"]);
    }

    inline auto deserialize_topologies(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< topology_options >(root["topologies"]);
    }

    auto deserialize_defaults(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading defaults";

        const auto& defaults = root["defaults"];

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

    verify(res);

    return res;
}

} // namespace layout