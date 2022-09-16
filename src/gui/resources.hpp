// Contains a module which provides global access to rendered global resources.
// Soultatos Stefanos 2022

#ifndef GUI_RESOURCES_HPP
#define GUI_RESOURCES_HPP

#include <array>
#include <vector>

namespace gui::resources
{

/***********************************************************
 * Resources                                               *
 ***********************************************************/

using mesh = const char*;
using material = const char*;
using font = const char*;

using color_themes_array = std::array< const char*, 3 >;
using fonts_vector = std::vector< font >;
using meshes_vector = std::vector< mesh >;
using materials_vector = std::vector< material >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

constexpr auto dark_theme = "Dark";
constexpr auto light_theme = "Light";
constexpr auto classic_theme = "Classic";

constexpr auto color_themes
    = color_themes_array { dark_theme, light_theme, classic_theme };

auto fonts() -> const fonts_vector&;
auto meshes() -> const meshes_vector&;
auto materials() -> const materials_vector&;

/***********************************************************
 * Loaders                                                 *
 ***********************************************************/

auto load_fonts(fonts_vector vec) -> void;
auto load_meshes(meshes_vector vec) -> void;
auto load_materials(materials_vector vec) -> void;

} // namespace gui::resources

#endif // GUI_RESOURCES_HPP
