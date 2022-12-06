// Contains a module which provides global access to rendered global resources.
// Soultatos Stefanos 2022

#ifndef GUI_RESOURCES_HPP
#define GUI_RESOURCES_HPP

#include <array>       // for array
#include <string_view> // for string_view
#include <vector>      // for vector

namespace gui::resources
{

/***********************************************************
 * Resources                                               *
 ***********************************************************/

using mesh = std::string_view;
using material = std::string_view;
using font = std::string_view;
using color_theme = std::string_view;
using particle_system = std::string_view;

using color_themes_array = std::array< color_theme, 3 >;
using fonts_vector = std::vector< font >;
using meshes_vector = std::vector< mesh >;
using materials_vector = std::vector< material >;
using particle_systems_vector = std::vector< particle_system >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

constexpr color_theme dark_theme { "Dark" };
constexpr color_theme light_theme { "Light" };
constexpr color_theme classic_theme { "Classic" };

constexpr color_themes_array color_themes { dark_theme,
                                            light_theme,
                                            classic_theme };

auto fonts() -> const fonts_vector&;
auto meshes() -> const meshes_vector&;
auto materials() -> const materials_vector&;
auto particle_systems() -> const particle_systems_vector&;

/***********************************************************
 * Loaders                                                 *
 ***********************************************************/

auto load_fonts(fonts_vector vec) -> void;
auto load_meshes(meshes_vector vec) -> void;
auto load_materials(materials_vector vec) -> void;
auto load_particle_systems(particle_systems_vector vec) -> void;

} // namespace gui::resources

#endif // GUI_RESOURCES_HPP
