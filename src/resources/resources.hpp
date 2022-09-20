// Contains a facade module for listing all of the used resources.
// Soultatos Stefanos 2022

#ifndef RESOURCES_RESOURCES_HPP
#define RESOURCES_RESOURCES_HPP

#include <string>
#include <vector>

namespace resources
{

using mesh = std::string;
using material = std::string;
using font = std::string;

using mesh_vector = std::vector< mesh >;
using material_vector = std::vector< material >;
using font_vector = std::vector< font >;

auto meshes() -> const mesh_vector&;
auto materials() -> const material_vector&;
auto fonts() -> const font_vector&;

auto load_meshes(mesh_vector vec) -> void;
auto load_materials(material_vector vec) -> void;
auto load_fonts(font_vector vec) -> void;

} // namespace resources

#endif // RESOURCES_RESOURCES_HPP
