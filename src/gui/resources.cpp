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

auto load_fonts(fonts_vector vec) -> void
{
    loaded_fonts = std::move(vec);

    for (const auto& font : loaded_fonts)
    {
        auto* atlas = ImGui::GetIO().Fonts;

        assert(atlas);

        [[maybe_unused]] const auto* ptr
            = atlas->AddFontFromFileTTF(font.file_name.data(), font.pixel_size);

        assert(ptr);
    }

    BOOST_LOG_TRIVIAL(info) << "loaded fonts for the gui";
}

auto load_meshes(meshes_vector vec) -> void
{
    loaded_meshes = std::move(vec);
    BOOST_LOG_TRIVIAL(info) << "loaded meshes for the gui";
}

auto load_materials(materials_vector vec) -> void
{
    loaded_materials = std::move(vec);
    BOOST_LOG_TRIVIAL(info) << "loaded materials for the gui";
}

} // namespace gui::resources