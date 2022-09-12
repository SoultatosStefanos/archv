// Contains a module which provides global access to rendered global resources.
// Soultatos Stefanos 2022

#ifndef GUI_RESOURCES_HPP
#define GUI_RESOURCES_HPP

#include <string>
#include <vector>

namespace gui::resources
{

/***********************************************************
 * Resources                                               *
 ***********************************************************/

struct font
{
    std::string file_name;
    float pixel_size;

    auto operator==(const font&) const -> bool = default;
    auto operator!=(const font&) const -> bool = default;
};

using mesh = const char*;
using material = const char*;

using fonts_vector = std::vector< font >;
using meshes_vector = std::vector< mesh >;
using materials_vector = std::vector< material >;

/***********************************************************
 * Observers                                               *
 ***********************************************************/

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
