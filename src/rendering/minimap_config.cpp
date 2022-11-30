#include "minimap_config.hpp"

#include <boost/log/trivial.hpp>

namespace rendering
{

minimap_config_api::minimap_config_api(config_data_type cfg)
: m_cfg { std::move(cfg) }
{
}

namespace
{
    inline auto in_range(minimap_config_api::coord_type c) -> bool
    {
        using coord_t = minimap_config_api::coord_type;
        return c <= static_cast< coord_t >(1.0)
            && c >= static_cast< coord_t >(-1.0);
    }
} // namespace

auto minimap_config_api::set_left(coord_type c) -> void
{
    if (!in_range(c))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid left coord: " << c;
        return;
    }
    config_data().left = c;
}

auto minimap_config_api::set_top(coord_type c) -> void
{
    if (!in_range(c))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid top coord: " << c;
        return;
    }
    config_data().top = c;
}

auto minimap_config_api::set_right(coord_type c) -> void
{
    if (!in_range(c))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid right coord: " << c;
        return;
    }
    config_data().right = c;
}

auto minimap_config_api::set_bottom(coord_type c) -> void
{
    if (!in_range(c))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid bottom coord: " << c;
        return;
    }
    config_data().bottom = c;
}

auto minimap_config_api::set_background_color(rgba_type col) -> void
{
    config_data().background_col = col;
}

auto minimap_config_api::set_render_shadows(bool v) -> void
{
    config_data().render_shadows = v;
}

auto minimap_config_api::set_render_sky(bool v) -> void
{
    config_data().render_sky = v;
}

auto minimap_config_api::set_zoom_out(dist_type dist) -> void
{
    if (dist < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "invalid negative zoom out";
        return;
    }
    config_data().zoom_out = dist;
}

auto minimap_config_api::set_render_vertices(bool v) -> void
{
    config_data().render_vertices = v;
}

auto minimap_config_api::set_render_vertex_ids(bool v) -> void
{
    config_data().render_vertex_ids = v;
}

auto minimap_config_api::set_render_edges(bool v) -> void
{
    config_data().render_edges = v;
}

auto minimap_config_api::set_render_edge_types(bool v) -> void
{
    config_data().render_edge_types = v;
}

auto minimap_config_api::set_render_edge_tips(bool v) -> void
{
    config_data().render_edge_tips = v;
}

auto minimap_config_api::set_render_particles(bool v) -> void
{
    config_data().render_particles = v;
}

} // namespace rendering