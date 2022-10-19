#include "background_presenter.hpp"

#include "gui/background_configurator.hpp"
#include "rendering/background_renderer.hpp"
#include "ui/ui_component.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>

namespace application
{

background_presenter::background_presenter(
    renderer_type& renderer, configurator_type& configurator)
: m_renderer { renderer }, m_configurator { configurator }
{
    prepare_configurator();
    connect_configurator_with_renderer();
}

namespace
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        return std::array { val.r, val.g, val.b, val.a };
    }
} // namespace

auto background_presenter::prepare_configurator() -> void
{
    const auto& cfg = renderer().config_data();

    configurator().set_skybox_material(cfg.skybox_material);
    configurator().set_skybox_distance(cfg.skybox_distance);
    configurator().set_ambient_color(to_rgba(cfg.ambient_light));
    configurator().set_diffuse_color(to_rgba(cfg.diffuse_light));
    configurator().set_specular_color(to_rgba(cfg.specular_light));
    configurator().set_cam_far_clip_distance(cfg.far_clip_distance);
    configurator().set_cam_near_clip_distance(cfg.near_clip_distance);

    BOOST_LOG_TRIVIAL(debug) << "setup gui background values";
}

namespace
{
    inline auto
    to_string(const background_presenter::configurator_type::rgba_type& rgba)
    {
        assert(rgba.size() == 4);
        return std::to_string(rgba[0]) + ", " + std::to_string(rgba[1]) + ", "
            + std::to_string(rgba[2]) + ", " + std::to_string(rgba[3]);
    }

    inline auto
    from_rgba(const background_presenter::configurator_type::rgba_type& rgba)
    {
        assert(rgba.size() == 4);
        return Ogre::ColourValue(rgba[0], rgba[1], rgba[2], rgba[3]);
    }
} // namespace

auto background_presenter::connect_configurator_with_renderer() -> void
{
    configurator().connect_to_skybox_material(
        [this](auto mat)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background skybox material: " << mat;

            renderer().config_api().config_data().skybox_material
                = std::string(mat);
        });

    configurator().connect_to_skybox_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background skybox distance:  " << dist;

            renderer().config_api().config_data().skybox_distance = dist;
        });

    configurator().connect_to_ambient_color(
        [this](const auto& c)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background ambient color: " << to_string(c);

            renderer().config_api().config_data().ambient_light = from_rgba(c);
        });

    configurator().connect_to_diffuse_color(
        [this](const auto& c)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background diffuse color: " << to_string(c);

            renderer().config_api().config_data().diffuse_light = from_rgba(c);
        });

    configurator().connect_to_specular_color(
        [this](const auto& c)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background specular color: " << to_string(c);
            renderer().config_api().config_data().specular_light = from_rgba(c);
        });

    configurator().connect_to_cam_far_clip_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background camera far clip distance: " << dist;

            renderer().config_api().config_data().far_clip_distance = dist;
        });

    configurator().connect_to_cam_near_clip_distance(
        [this](auto dist)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected background camera near clip distance: " << dist;

            renderer().config_api().config_data().near_clip_distance = dist;
        });

    configurator().connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs apply";
            ui::apply_configs(renderer());
        });

    configurator().connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs preview";
            ui::begin_preview(renderer());
        });

    configurator().connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs cancel";
            ui::end_preview(renderer());
        });

    configurator().connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected background configs restore";
            ui::restore_defaults(renderer());
        });
}

} // namespace application