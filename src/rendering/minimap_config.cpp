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

auto minimap_config_api::set_material(name_type name) -> void
{
    config_data().material = std::move(name);
}

} // namespace rendering