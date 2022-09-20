#include "config.hpp"

#include "backend.hpp"
#include "layout_plugin.hpp"
#include "topology_plugin.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>

namespace layout
{

namespace
{
    auto verify_layouts(const config_data& data)
    {
        for (const auto& lay : data.layouts)
            if (!is_layout_plugged_in(lay))
                BOOST_THROW_EXCEPTION(unknown_plugin() << layout_info(lay));
    }

    auto verify_topologies(const config_data& data)
    {
        for (const auto& s : data.topologies)
            if (!is_topology_plugged_in(s))
                BOOST_THROW_EXCEPTION(unknown_plugin() << topology_info(s));
    }

    template < typename Container >
    auto
    contains(const Container& data, const typename Container::value_type& val)
    {
        return std::find(std::begin(data), std::end(data), val)
            != std::end(data);
    }

    auto verify_defaults(const config_data& data)
    {
        if (!contains(data.layouts, data.layout))
            BOOST_THROW_EXCEPTION(
                unlisted_default() << layout_info(data.layout));

        if (!contains(data.topologies, data.topology))
            BOOST_THROW_EXCEPTION(
                unlisted_default() << topology_info(data.topology));
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

    verify(res);

    return res;
}

} // namespace layout