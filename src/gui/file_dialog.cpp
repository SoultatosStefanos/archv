#include "file_dialog.hpp"

#include <imgui/imgui.h>

namespace gui
{

file_dialog::file_dialog() : m_impl { ImGuiFileBrowserFlags_NoModal }
{
    m_impl.SetTypeFilters({ ".json" });
}

auto file_dialog::draw() const -> void
{
    m_impl.Display();
    if (m_impl.HasSelected())
    {
        m_signal(m_impl.GetSelected().string());
        m_impl.ClearSelected();
    }
}

auto file_dialog::open() -> void { m_impl.Open(); }

} // namespace gui