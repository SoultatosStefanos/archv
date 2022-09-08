#include "config.hpp"

#include "layout_plugin.hpp"
#include "topology_plugin.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <unordered_set>

namespace layout
{

namespace
{

    auto verify_layout(const config_data& data) -> void
    {
        if (!layout_plugin::enumerates(data.layout))
            BOOST_THROW_EXCEPTION(unknown_layout() << layout_info(data.layout));
    }

    auto verify_topology(const config_data& data) -> void
    {
        if (!topology_plugin::enumerates(data.topology))
            BOOST_THROW_EXCEPTION(
                unknown_topology() << topology_info(data.topology));
    }

    auto verify(const config_data& data)
    {
        verify_layout(data);
        verify_topology(data);
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
    auto&& [layout, topology, scale] = deserialize_defaults(root);

    config_data res { .layout = std::move(layout),
                      .topology = std::move(topology),
                      .scale = scale };

    verify(res);

    return res;
}

} // namespace layout