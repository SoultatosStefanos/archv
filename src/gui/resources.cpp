#include "resources.hpp"

#include <boost/log/trivial.hpp>
#include <imgui/imgui.h>

namespace gui::resources
{

namespace
{
    fonts_vector loaded_fonts;
    meshes_vector loaded_meshes;
    materials_vector loaded_materials;
    particle_systems_vector loaded_particles;

} // namespace

auto fonts() -> const fonts_vector&
{
    return loaded_fonts;
}

auto meshes() -> const meshes_vector&
{
    return loaded_meshes;
}

auto materials() -> const materials_vector&
{
    return loaded_materials;
}

auto particle_systems() -> const particle_systems_vector&
{
    return loaded_particles;
}

auto load_fonts(fonts_vector vec) -> void
{
    loaded_fonts = std::move(vec);
}

auto load_meshes(meshes_vector vec) -> void
{
    loaded_meshes = std::move(vec);
}

auto load_materials(materials_vector vec) -> void
{
    loaded_materials = std::move(vec);
}

auto load_particle_systems(particle_systems_vector vec) -> void
{
    loaded_particles = std::move(vec);
}

} // namespace gui::resources