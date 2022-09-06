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

    void verify_layouts(const config_data& data)
    {
        for (const auto& lay : data.layouts)
            if (!valid_layouts.contains(lay))
                BOOST_THROW_EXCEPTION(unknown_layout() << layout_info(lay));
    }

    void verify_topologies(const config_data& data)
    {
        for (const auto& s : data.topologies)
            if (!valid_topologies.contains(s))
                BOOST_THROW_EXCEPTION(unknown_topology() << topology_info(s));
    }
    void verify_defaults(const config_data& data)
    {
        if (!data.layouts.contains(data.default_layout))
            BOOST_THROW_EXCEPTION(
                unknown_default() << layout_info(data.default_layout));

        if (!data.topologies.contains(data.default_topology))
            BOOST_THROW_EXCEPTION(
                unknown_default() << topology_info(data.default_topology));
    }

    void verify(const config_data& data)
    {
        verify_layouts(data);
        verify_topologies(data);
        verify_defaults(data);
    }

} // namespace

namespace
{
    using namespace config;

    template < typename Container >
    inline auto read_json_array(const Json::Value& val) -> Container
    {
        using value_type = typename Container::value_type;

        Container res;

        if (!val.isArray())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type()
                << json_type_info(val.type()) << json_value_info(val));

        std::transform(
            std::begin(val),
            std::end(val),
            std::inserter(res, std::begin(res)),
            [](const auto& val) { return as< value_type >(val); });

        return res;
    }

    inline auto deserialize_layouts(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< layout_options >(get(root, "layouts"));
    }

    inline auto deserialize_topologies(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< topology_options >(get(root, "topologies"));
    }

    auto deserialize_defaults(const Json::Value& root)
    {
        BOOST_LOG_TRIVIAL(debug) << "reading defaults";

        const auto defaults = get(root, "defaults");

        if (!defaults.isObject())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type() << json_type_info(defaults.type())
                                          << json_value_info(defaults));

        if (defaults.size() != 3)
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type() << json_value_info(defaults));

        std::string default_layout, default_topology;
        double default_scale;

        for (auto iter = std::begin(defaults); iter != std::end(defaults);
             ++iter)
        {
            if (iter.name() == "layout")
            {
                default_layout = as< std::string >(*iter);
            }
            else if (iter.name() == "topology")
            {
                default_topology = as< std::string >(*iter);
            }
            else if (iter.name() == "scale")
            {
                default_scale = as< double >(*iter);
            }
            else
            {
                BOOST_THROW_EXCEPTION(
                    invalid_json_value_type() << json_value_info(*iter));
            }
        }

        return std::make_tuple(
            std::move(default_layout),
            std::move(default_topology),
            default_scale);
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& layouts = deserialize_layouts(root);
    auto&& topologies = deserialize_topologies(root);
    auto&& [layout, topology, scale] = deserialize_defaults(root);

    config_data res { .layouts = std::move(layouts),
                      .topologies = std::move(topologies),
                      .default_layout = std::move(layout),
                      .default_topology = std::move(topology),
                      .default_scale = scale };

    verify(res);

    return res;
}

} // namespace layout