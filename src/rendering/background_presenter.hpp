// Contains a class responsible for connecting the background frontends.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_PRESENTER_HPP
#define RENDERING_BACKGROUND_PRESENTER_HPP

#include "background_renderer.hpp"
#include "ui/all.hpp"

#include <boost/log/trivial.hpp>

namespace rendering
{

template < typename View, typename Renderer = background_renderer >
class background_presenter
{
public:
    using renderer_type = Renderer;
    using renderer_config_data_type = typename renderer_type::config_data_type;
    using renderer_name_type = typename renderer_config_data_type::name_type;
    using renderer_distance_type =
        typename renderer_config_data_type::distance_type;
    using renderer_rgba_type = typename renderer_config_data_type::rgba_type;

    using view_type = View;
    using view_name_type = typename view_type::name_type;
    using view_distance_type = typename view_type::distance_type;
    using view_rgba_type = typename view_type::rgba_type;

    static_assert(
        std::is_constructible_v< view_name_type, renderer_name_type >);

    background_presenter(renderer_type& renderer, view_type& view)
    : m_renderer { renderer }, m_view { view }
    {
        initialize_view();
        install_view_responses();
    }

    auto renderer() const -> const renderer_type& { return m_renderer; }
    auto view() const -> const view_type& { return m_view; }

    auto fetch_skybox_material() -> void
    {
        m_view.set_skybox_material(m_renderer.config_data().skybox_material);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg skybox material";
    }

    auto fetch_skybox_distance() -> void
    {
        m_view.set_skybox_distance(m_renderer.config_data().skybox_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg skybox distance";
    }

    auto fetch_ambient_color() -> void
    {
        m_view.set_ambient_color(
            translate(m_renderer.config_data().ambient_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg ambient color";
    }

    auto fetch_diffuse_color() -> void
    {
        m_view.set_diffuse_color(
            translate(m_renderer.config_data().diffuse_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg diffuse color";
    }

    auto fetch_specular_color() -> void
    {
        m_view.set_specular_color(
            translate(m_renderer.config_data().specular_color));
        BOOST_LOG_TRIVIAL(info) << "fetched bkg specular color";
    }

    auto fetch_cam_far_clip_distance() -> void
    {
        m_view.set_cam_far_clip_distance(
            m_renderer.config_data().cam_far_clip_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg camera far clip distance";
    }

    auto fetch_cam_near_clip_distance() -> void
    {
        m_view.set_cam_near_clip_distance(
            m_renderer.config_data().cam_near_clip_distance);
        BOOST_LOG_TRIVIAL(info) << "fetched bkg camera near clip distance";
    }

    auto select_skybox_material(view_name_type material) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg skybox material " << material;
        m_renderer.config_api().set_skybox_material(translate(material));
    }

    auto select_skybox_distance(view_distance_type dist) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg skybox distance " << dist;
        m_renderer.config_api().set_skybox_distance(dist);
    }

    auto select_ambient_color(const view_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg ambient color";
        m_renderer.config_api().set_ambient_color(translate(rgba));
    }

    auto select_diffuse_color(const view_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg diffuse color";
        m_renderer.config_api().set_diffuse_color(translate(rgba));
    }

    auto select_specular_color(const view_rgba_type& rgba) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg specular color";
        m_renderer.config_api().set_specular_color(translate(rgba));
    }

    auto select_cam_far_clip_distance(view_distance_type dist) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected bkg cam far clip dist " << dist;
        m_renderer.config_api().set_cam_far_clip_distance(dist);
    }

    auto select_cam_near_clip_distance(view_distance_type dist) -> void
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
        initialize_view(); // back to defaults
    }

protected:
    static inline auto translate(const view_rgba_type& rgba)
    {
        assert(rgba.size() == 4);
        return renderer_rgba_type { rgba[0], rgba[1], rgba[2], rgba[3] };
    }

    static inline auto translate(const view_name_type& name)
    {
        return renderer_name_type { name };
    }

    static inline auto translate(const renderer_rgba_type& rgba)
    {
        return view_rgba_type { rgba.r, rgba.g, rgba.b, rgba.a };
    }

private:
    auto initialize_view() -> void
    {
        fetch_skybox_material();
        fetch_skybox_distance();
        fetch_ambient_color();
        fetch_diffuse_color();
        fetch_specular_color();
        fetch_cam_far_clip_distance();
        fetch_cam_near_clip_distance();

        BOOST_LOG_TRIVIAL(debug) << "initialized bkg view";
    }

    auto install_view_responses() -> void
    {
        m_view.connect_to_skybox_material([this](const auto& mat)
                                          { select_skybox_material(mat); });

        m_view.connect_to_skybox_distance([this](auto dist)
                                          { select_skybox_distance(dist); });

        m_view.connect_to_ambient_color([this](const auto& col)
                                        { select_ambient_color(col); });

        m_view.connect_to_diffuse_color([this](const auto& col)
                                        { select_diffuse_color(col); });

        m_view.connect_to_specular_color([this](const auto& col)
                                         { select_specular_color(col); });

        m_view.connect_to_cam_far_clip_distance(
            [this](auto dist) { select_cam_far_clip_distance(dist); });

        m_view.connect_to_cam_near_clip_distance(
            [this](auto dist) { select_cam_near_clip_distance(dist); });

        m_view.connect_to_apply([this]() { select_apply(); });
        m_view.connect_to_preview([this]() { select_preview(); });
        m_view.connect_to_cancel([this]() { select_cancel(); });
        m_view.connect_to_restore([this]() { select_restore(); });

        BOOST_LOG_TRIVIAL(debug) << "installed bkg view responses";
    }

    renderer_type& m_renderer;
    view_type& m_view;
};

// Utility factory for type deduction.
template < typename View, typename Renderer = background_renderer >
inline auto make_background_presenter(Renderer& renderer, View& view)
{
    return background_presenter< View, Renderer >(renderer, view);
}

} // namespace rendering

#endif // RENDERING_BACKGROUND_PRESENTER_HPP
