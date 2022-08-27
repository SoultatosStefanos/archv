#include "file_dialog.hpp"

#include "detail/imfilebrowser.h"

#include <imgui/imgui.h>

namespace gui
{

file_dialog::file_dialog()
: m_pimpl { std::make_unique< ImGui::FileBrowser >(
    ImGuiFileBrowserFlags_NoModal) }
{
    m_pimpl->SetTypeFilters({ ".json" });
}

file_dialog::~file_dialog() = default;

auto file_dialog::draw() const -> void
{
    m_pimpl->Display();
    if (m_pimpl->HasSelected())
    {
        m_signal(m_pimpl->GetSelected().string());
        m_pimpl->ClearSelected();
    }
}

auto file_dialog::open() -> void { m_pimpl->Open(); }

} // namespace gui