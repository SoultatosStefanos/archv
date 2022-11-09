#include "graph_config.hpp"

#include <boost/log/trivial.hpp>

namespace rendering
{

using namespace Ogre;

auto graph_config_api::set_vertex_mesh(name_type name) -> void
{
    config_data().vertex_mesh = std::move(name);
}

auto graph_config_api::set_vertex_material(name_type name) -> void
{
    config_data().vertex_material = std::move(name);
}

auto graph_config_api::set_vertex_scale(scale_type scale) -> void
{
    config_data().vertex_scale = scale;
}

auto graph_config_api::set_vertex_id_font_name(name_type name) -> void
{
    config_data().vertex_id_font_name = std::move(name);
}

auto graph_config_api::set_vertex_id_char_height(height_type h) -> void
{
    if (h < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid vertex height: " << h;
        return;
    }
    config_data().vertex_id_char_height = h;
}

auto graph_config_api::set_vertex_id_color(rgba_type col) -> void
{
    config_data().vertex_id_color = col;
}

auto graph_config_api::set_vertex_space_width(width_type w) -> void
{
    if (w < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring vertex space width: " << w;
        return;
    }
    config_data().vertex_id_space_width = w;
}

auto graph_config_api::set_edge_material(name_type name) -> void
{
    config_data().edge_material = std::move(name);
}

auto graph_config_api::set_edge_tip_mesh(name_type name) -> void
{
    config_data().edge_tip_mesh = std::move(name);
}

auto graph_config_api::set_edge_tip_material(name_type name) -> void
{
    config_data().edge_tip_material = std::move(name);
}

auto graph_config_api::set_edge_tip_scale(scale_type scale) -> void
{
    config_data().edge_tip_scale = scale;
}

auto graph_config_api::set_edge_type_font_name(name_type name) -> void
{
    config_data().edge_type_font_name = std::move(name);
}

auto graph_config_api::set_edge_type_char_height(height_type h) -> void
{
    if (h < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid edge height: " << h;
        return;
    }
    config_data().edge_type_char_height = h;
}

auto graph_config_api::set_edge_type_color(rgba_type col) -> void
{
    config_data().edge_type_color = col;
}

auto graph_config_api::set_edge_type_space_width(width_type w) -> void
{
    if (w < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring edge space width: " << w;
        return;
    }
    config_data().edge_type_space_width = w;
}

} // namespace rendering