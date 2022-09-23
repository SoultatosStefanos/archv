#include "app.hpp"
#include "commands.hpp"
#include "ui/all.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>

namespace application
{

auto app::connect_gui_with_dependencies() -> void
{
    assert(m_gui);
    assert(m_dependencies_backend);

    auto& editor = m_gui->get_editor().get_dependencies_editor();

    editor.connect_to_dependency(
        [this](auto dependency, auto weight)
        {
            BOOST_LOG_TRIVIAL(info) << "assigned " << weight
                                    << " to: " << dependency << " dependency";

            update_dependency_weight(
                *m_commands, *m_dependencies_backend, dependency, weight);
        });

    editor.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected dependencies restore";

            restore_dependencies_defaults(*m_commands, *m_dependencies_backend);
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with dependencies management";
}

auto app::connect_gui_with_layout() -> void
{
    assert(m_gui);
    assert(m_layout_backend);

    auto& editor = m_gui->get_editor().get_layout_editor();

    editor.connect_to_layout(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " layout";

            update_layout(*m_commands, *m_layout_backend, selection);
        });

    editor.connect_to_topology(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " topology";

            update_layout_topology(*m_commands, *m_layout_backend, selection);
        });

    editor.connect_to_scale(
        [this](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " scale";

            layout::update_scale(*m_layout_backend, selection);
            // update_layout_scale(*m_commands, *m_layout_backend, selection);
        });

    editor.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout restore";

            restore_layout_defaults(*m_commands, *m_layout_backend);
        });

    BOOST_LOG_TRIVIAL(info) << "connected gui with layout management";
}

auto app::connect_gui_with_command_history() -> void
{
    assert(m_gui);

    auto& editor = m_gui->get_editor();

    editor.connect_to_undo([this]() { m_commands->undo(); });
    editor.connect_to_redo([this]() { m_commands->redo(); });

    BOOST_LOG_TRIVIAL(info) << "connected gui with command history";
}

auto app::connect_rendering_with_layout() -> void
{
    assert(m_graph_renderer);
    assert(m_layout_backend);

    m_layout_backend->connect_to_layout([this](const auto&)
                                        { m_graph_renderer->draw_layout(); });
}

auto app::connect_gui_background_configurator() -> void
{
    assert(m_gui);

    auto& iface = m_gui->get_configurator().get_background_configurator();

    iface.connect_to_skybox_material(
        [this](auto material)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background skybox material: " << material;

            m_background_renderer->config_api().config_data().skybox_material
                = std::string(material);
        });

    iface.connect_to_skybox_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background skybox distance:  " << dist;

            m_background_renderer->config_api().config_data().skybox_distance
                = dist;
        });

    iface.connect_to_ambient_color(
        [this](const auto& col)
        {
            assert(col.size() == 4);

            BOOST_LOG_TRIVIAL(info)
                << "selected background ambient color: " << col[0] << ", "
                << col[1] << ", " << col[2] << ", " << col[3];

            m_background_renderer->config_api().config_data().ambient_light
                = Ogre::ColourValue(col[0], col[1], col[2], col[3]);
        });

    iface.connect_to_diffuse_color(
        [this](const auto& col)
        {
            assert(col.size() == 4);

            BOOST_LOG_TRIVIAL(info)
                << "selected background diffuse color: " << col[0] << ", "
                << col[1] << ", " << col[2] << ", " << col[3];

            m_background_renderer->config_api().config_data().diffuse_light
                = Ogre::ColourValue(col[0], col[1], col[2], col[3]);
        });

    iface.connect_to_specular_color(
        [this](const auto& col)
        {
            assert(col.size() == 4);

            BOOST_LOG_TRIVIAL(info)
                << "selected background specular color: " << col[0] << ", "
                << col[1] << ", " << col[2] << ", " << col[3];

            m_background_renderer->config_api().config_data().specular_light
                = Ogre::ColourValue(col[0], col[1], col[2], col[3]);
        });

    iface.connect_to_cam_far_clip_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background camera far clip distance: " << dist;

            m_background_renderer->config_api().config_data().far_clip_distance
                = dist;
        });

    iface.connect_to_cam_near_clip_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background camera near clip distance: " << dist;

            m_background_renderer->config_api().config_data().near_clip_distance
                = dist;
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs apply";
            ui::apply_configs(*m_background_renderer);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs preview";
            ui::begin_preview(*m_background_renderer);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs cancel";
            ui::end_preview(*m_background_renderer);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs restore";
            ui::restore_defaults(*m_background_renderer);
        });
}

auto app::connect_gui_graph_configurator() -> void
{
    assert(m_gui);

    auto& iface = m_gui->get_configurator().get_graph_configurator();

    iface.connect_to_node_mesh(
        [this](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node mesh: " << mesh;

            m_graph_renderer->config_api().config_data().vertex_mesh
                = std::string(mesh);
        });

    iface.connect_to_node_scale(
        [this](const auto& scale)
        {
            assert(scale.size() == 3);

            BOOST_LOG_TRIVIAL(info) << "selected graph node scale: " << scale[0]
                                    << ", " << scale[1] << ", " << scale[2];

            m_graph_renderer->config_api().config_data().vertex_scale
                = Ogre::Vector3(scale[0], scale[1], scale[2]);
        });

    iface.connect_to_node_font(
        [this](auto font)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node font: " << font;

            m_graph_renderer->config_api().config_data().vertex_id_font_name
                = std::string(font);
        });

    iface.connect_to_node_font_color(
        [this](const auto& col)
        {
            assert(col.size() == 4);

            BOOST_LOG_TRIVIAL(info)
                << "selected graph node font color: " << col[0] << ", "
                << col[1] << ", " << col[2] << ", " << col[3];

            m_graph_renderer->config_api().config_data().vertex_id_color
                = Ogre::ColourValue(col[0], col[1], col[2], col[3]);
        });

    iface.connect_to_node_char_height(
        [this](auto h)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node char height: " << h;

            m_graph_renderer->config_api().config_data().vertex_id_char_height
                = h;
        });

    iface.connect_to_node_space_width(
        [this](auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node space width: " << w;

            m_graph_renderer->config_api().config_data().vertex_id_space_width
                = w;
        });

    iface.connect_to_edge_material(
        [this](auto mat)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge material: " << mat;

            m_graph_renderer->config_api().config_data().edge_material
                = std::string(mat);
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs apply";
            ui::apply_configs(*m_graph_renderer);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs preview";
            ui::begin_preview(*m_graph_renderer);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs cancel";
            ui::end_preview(*m_graph_renderer);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs restore";
            ui::restore_defaults(*m_graph_renderer);
        });
}

auto app::connect_gui_gui_configurator() -> void
{
    assert(m_gui);

    auto& iface = m_gui->get_configurator().get_gui_configurator();

    iface.connect_to_color_theme(
        [this](auto theme)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui color theme: " << theme;
            m_gui->config_api().config_data().color_theme = std::string(theme);
        });

    iface.connect_to_frame_rounding(
        [this](auto r)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame rounding: " << r;
            m_gui->config_api().config_data().frame_rounding = r;
        });

    iface.connect_to_window_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui window bordered: " << b;
            m_gui->config_api().config_data().window_bordered = b;
        });

    iface.connect_to_frame_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame bordered: " << b;
            m_gui->config_api().config_data().frame_bordered = b;
        });

    iface.connect_to_popup_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui popup bordered: " << b;
            m_gui->config_api().config_data().popup_bordered = b;
        });

    iface.connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs apply";
            ui::apply_configs(*m_gui);
        });

    iface.connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs preview";
            ui::begin_preview(*m_gui);
        });

    iface.connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs cancel";
            ui::end_preview(*m_gui);
        });

    iface.connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs restore";
            ui::restore_defaults(*m_gui);
        });
}

} // namespace application