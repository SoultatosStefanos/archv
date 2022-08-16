#include "deserialization.hpp"

#include "gursoy_atun_layout.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <unordered_set>

namespace layout
{

namespace
{
    using layout_options = std::unordered_set< std::string >;
    using topology_options = std::unordered_set< std::string >;

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

    void verify_scales(const config_data& data)
    {
        for (auto scale : data.scales)
            if (scale < 0)
                BOOST_THROW_EXCEPTION(
                    invalid_negative_scale() << scale_info(scale));
    }

    template < typename Container >
    auto
    contains(const Container& data, const typename Container::value_type& val)
    {
        return std::find(std::begin(data), std::end(data), val)
            != std::end(data);
    }

    void verify_defaults(const config_data& data)
    {
        if (!contains(data.layouts, data.default_layout))
            BOOST_THROW_EXCEPTION(
                unknown_default() << layout_info(data.default_layout));

        if (!contains(data.topologies, data.default_topology))
            BOOST_THROW_EXCEPTION(
                unknown_default() << topology_info(data.default_topology));

        if (!contains(data.scales, data.default_scale))
            BOOST_THROW_EXCEPTION(
                unknown_default() << scale_info(data.default_scale));
    }

    void verify(const config_data& data)
    {
        verify_layouts(data);
        verify_topologies(data);
        verify_scales(data);
        verify_defaults(data);
    }

} // namespace

namespace
{
    using namespace json;

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
            std::back_inserter(res),
            [](const auto& val) { return as< value_type >(val); });

        return res;
    }

    inline auto deserialize_layouts(const Json::Value& root)
    {
        using json_layouts = std::vector< std::string >;
        BOOST_LOG_TRIVIAL(debug) << "reading layouts";
        return read_json_array< json_layouts >(get(root, "layouts"));
    }

    inline auto deserialize_topologies(const Json::Value& root)
    {
        using json_topologies = std::vector< std::string >;
        BOOST_LOG_TRIVIAL(debug) << "reading topologies";
        return read_json_array< json_topologies >(get(root, "topologies"));
    }

    inline auto deserialize_scales(const Json::Value& root)
    {
        using json_scales = std::vector< double >;
        BOOST_LOG_TRIVIAL(debug) << "reading scales";
        return read_json_array< json_scales >(get(root, "scales"));
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

auto deserialize_layout(const Json::Value& root) -> config_data
{
    auto&& layouts = deserialize_layouts(root);
    auto&& topologies = deserialize_topologies(root);
    auto&& scales = deserialize_scales(root);
    auto&& [layout, topology, scale] = deserialize_defaults(root);

    config_data res { .layouts = std::move(layouts),
                      .topologies = std::move(topologies),
                      .scales = std::move(scales),
                      .default_layout = std::move(layout),
                      .default_topology = std::move(topology),
                      .default_scale = scale };

    verify(res);

    return res;
}

} // namespace layout