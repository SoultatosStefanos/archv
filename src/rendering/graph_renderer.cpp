#include "graph_renderer.hpp"

#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <boost/log/trivial.hpp>

namespace rendering
{

using namespace Ogre;

auto graph_config_api::set_vertex_mesh(name_type name) -> void
{
    if (!MeshManager::getSingleton().getByName(name))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid vertex mesh: " << name;
        return;
    }
    config_data().vertex_mesh = std::move(name);
}

auto graph_config_api::set_vertex_scale(scale_type scale) -> void
{
    config_data().vertex_scale = std::move(scale);
}

auto graph_config_api::set_vertex_id_font_name(name_type name) -> void
{
    if (!FontManager::getSingleton().getByName(name))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid vertex font: " << name;
        return;
    }
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
    config_data().vertex_id_color = std::move(col);
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
    if (!MaterialManager::getSingleton().getByName(name))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid edge mat: " << name;
        return;
    }
    config_data().edge_material = std::move(name);
}

auto graph_config_api::set_edge_tip_mesh(name_type name) -> void
{
    if (!MeshManager::getSingleton().getByName(name))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid tip mesh: " << name;
        return;
    }
    config_data().edge_tip_mesh = std::move(name);
}

auto graph_config_api::set_edge_tip_scale(scale_type scale) -> void
{
    config_data().edge_tip_scale = std::move(scale);
}

auto graph_config_api::set_edge_type_font_name(name_type name) -> void
{
    if (!FontManager::getSingleton().getByName(name))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid edge font: " << name;
        return;
    }
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
    config_data().edge_type_color = std::move(col);
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