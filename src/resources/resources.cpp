#include "resources.hpp"

namespace resources
{

namespace
{
    mesh_vector loaded_meshes;
    material_vector loaded_materials;
    font_vector loaded_fonts;

} // namespace

auto meshes() -> const mesh_vector&
{
    return loaded_meshes;
}

auto materials() -> const material_vector&
{
    return loaded_materials;
}

auto fonts() -> const font_vector&
{
    return loaded_fonts;
}

auto load_meshes(mesh_vector vec) -> void
{
    loaded_meshes = std::move(vec);
}

auto load_materials(material_vector vec) -> void
{
    loaded_materials = std::move(vec);
}

auto load_fonts(font_vector vec) -> void
{
    loaded_fonts = std::move(vec);
}

} // namespace resources