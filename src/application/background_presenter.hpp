// Contains a class responsible for connecting the background frontends.
// Soultatos Stefanos 2022

#ifndef APPLICATION_BACKGROUND_PRESENTER_HPP
#define APPLICATION_BACKGROUND_PRESENTER_HPP

#include "gui/background_configurator.hpp"
#include "rendering/background_renderer.hpp"
#include "ui/all.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>

namespace application
{

template <
    typename Renderer = rendering::background_renderer,
    typename Configurator = gui::background_configurator >
class background_presenter
{
public:
    using renderer_type = Renderer;
    using renderer_config_data_type = typename Renderer::config_data_type;
    using renderer_name_type = typename renderer_config_data_type::name_type;
    using renderer_distance_type =
        typename renderer_config_data_type::distance_type;
    using renderer_rgba_type = typename renderer_config_data_type::rgba_type;

    using configurator_type = Configurator;
    using configurator_name_type = typename Configurator::name_type;
    using configurator_distance_type = typename Configurator::distance_type;
    using configurator_rgba_type = typename Configurator::rgba_type;

    static_assert(std::is_convertible_v<
                  renderer_distance_type,
                  configurator_distance_type >);

    static_assert(std::is_convertible_v<
                  configurator_distance_type,
                  renderer_distance_type >);

    background_presenter(renderer_type& renderer, configurator_type& cfg)
    : m_renderer { renderer }, m_gui { cfg }
    {
        initialize_configurator();
        install_configurator_responses();
    }

    auto renderer() const -> const renderer_type& { return m_renderer; }
    auto configurator() const -> const configurator_type& { return m_gui; }

    auto fetch_skybox_material() -> void
    {
        m_gui.set_skybox_material(m_renderer.config_data().skybox_material);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg skybox material";
    }

    auto fetch_skybox_distance() -> void
    {
        m_gui.set_skybox_distance(m_renderer.config_data().skybox_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg skybox distance";
    }

    auto fetch_ambient_color() -> void
    {
        m_gui.set_ambient_color(
            translate(m_renderer.config_data().ambient_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg ambient color";
    }

    auto fetch_diffuse_color() -> void
    {
        m_gui.set_diffuse_color(
            translate(m_renderer.config_data().diffuse_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg diffuse color";
    }

    auto fetch_specular_color() -> void
    {
        m_gui.set_specular_color(
            translate(m_renderer.config_data().specular_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg specular color";
    }

    auto fetch_cam_far_clip_distance() -> void
    {
        m_gui.set_cam_far_clip_distance(
            m_renderer.config_data().cam_far_clip_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg camera far clip distance";
    }

    auto fetch_cam_near_clip_distance() -> void
    {
        m_gui.set_cam_near_clip_distance(
            m_renderer.config_data().cam_near_clip_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg camera near clip distance";
    }

    auto select_skybox_material(const configurator_name_type& material) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg skybox material " << material;
        m_renderer.config_api().set_skybox_material(translate(material));
    }

    auto select_skybox_distance(configurator_distance_type dist) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg skybox distance " << dist;
        m_renderer.config_api().set_skybox_distance(dist);
    }

    auto select_ambient_color(const configurator_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg ambient color";
        m_renderer.config_api().set_ambient_color(translate(rgba));
    }

    auto select_diffuse_color(const configurator_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg diffuse color";
        m_renderer.config_api().set_diffuse_color(translate(rgba));
    }

    auto select_specular_color(const configurator_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg specular color";
        m_renderer.config_api().set_specular_color(translate(rgba));
    }

    auto select_cam_far_clip_distance(configurator_distance_type dist) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg cam far clip dist " << dist;
        m_renderer.config_api().set_cam_far_clip_distance(dist);
    }

    auto select_cam_near_clip_distance(configurator_distance_type dist) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg cam near clip dist " << dist;
        m_renderer.config_api().set_cam_near_clip_distance(dist);
    }

    auto select_apply() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg apply";
        ui::apply_configs(m_renderer);
    }

    auto select_preview() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg preview";
        ui::begin_preview(m_renderer);
    }

    auto select_cancel() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg cancel";
        ui::end_preview(m_renderer);
    }

    auto select_restore() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg restore";
        ui::restore_defaults(m_renderer);
        initialize_configurator(); // back to defaults
    }

protected:
    static inline auto translate(const configurator_rgba_type& rgba)
        -> renderer_rgba_type
    {
        assert(rgba.size() == 4);
        return renderer_rgba_type { rgba[0], rgba[1], rgba[2], rgba[3] };
    }

    static inline auto translate(const configurator_name_type& name)
        -> renderer_name_type
    {
        static_assert(std::is_constructible_v<
                      configurator_name_type,
                      renderer_name_type >);

        if constexpr (std::is_convertible_v<
                          configurator_name_type,
                          renderer_name_type >)
            return name;
        else
            return renderer_name_type { name };
    }

    static inline auto translate(const renderer_rgba_type& rgba)
        -> configurator_rgba_type
    {
        return configurator_rgba_type { rgba.r, rgba.g, rgba.b, rgba.a };
    }

private:
    auto initialize_configurator() -> void
    {
        fetch_skybox_material();
        fetch_skybox_distance();
        fetch_ambient_color();
        fetch_diffuse_color();
        fetch_specular_color();
        fetch_cam_far_clip_distance();
        fetch_cam_near_clip_distance();

        BOOST_LOG_TRIVIAL(debug) << "initialized bkg configurator";
    }

    auto install_configurator_responses() -> void
    {
        m_gui.connect_to_skybox_material([this](const auto& mat)
                                         { select_skybox_material(mat); });

        m_gui.connect_to_skybox_distance([this](auto dist)
                                         { select_skybox_distance(dist); });

        m_gui.connect_to_ambient_color([this](const auto& col)
                                       { select_ambient_color(col); });

        m_gui.connect_to_diffuse_color([this](const auto& col)
                                       { select_diffuse_color(col); });

        m_gui.connect_to_specular_color([this](const auto& col)
                                        { select_specular_color(col); });

        m_gui.connect_to_cam_far_clip_distance(
            [this](auto dist) { select_cam_far_clip_distance(dist); });

        m_gui.connect_to_cam_near_clip_distance(
            [this](auto dist) { select_cam_near_clip_distance(dist); });

        m_gui.connect_to_apply([this]() { select_apply(); });
        m_gui.connect_to_preview([this]() { select_preview(); });
        m_gui.connect_to_cancel([this]() { select_cancel(); });
        m_gui.connect_to_restore([this]() { select_restore(); });

        BOOST_LOG_TRIVIAL(debug) << "installed bkg configurator responses";
    }

    renderer_type& m_renderer;
    configurator_type& m_gui;
};

} // namespace application

#endif // APPLICATION_BACKGROUND_PRESENTER_HPP
