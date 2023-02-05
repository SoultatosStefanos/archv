#include "file_browser.hpp"

#include "detail/imfilebrowser.h"

#include <imgui/imgui.h>

namespace gui
{

file_browser::file_browser()
: m_impl { std::make_unique< impl_type >(
    ImGuiFileBrowserFlags_NoModal | ImGuiFileBrowserFlags_EnterNewFilename
    | ImGuiFileBrowserFlags_CreateNewDir) }
{
    m_impl->SetTypeFilters({ ".json" });
}

file_browser::~file_browser() = default;

auto file_browser::render() const -> void
{
    m_impl->Display();

    if (m_impl->HasSelected())
    {
        emit_file(m_impl->GetSelected().c_str());
        m_impl->ClearSelected();
    }
}

auto file_browser::open() -> void
{
    m_impl->Open();
}

auto file_browser::connect(const file_slot& slot) -> connection
{
    return m_file_sig.connect(slot);
}

auto file_browser::emit_file(file_type file) const -> void
{
    m_file_sig(file);
}

} // namespace gui