#include "progress_bar.hpp"

#include <cassert>
#include <imgui/imgui.h>

namespace gui
{

auto progress_bar::draw() const -> void
{
    if (!ImGui::Begin(""))
    {
        ImGui::End();
        return;
    }

    ImGui::TextUnformatted(caption().c_str());
    ImGui::Spacing();

    ImGui::ProgressBar(progress());

    if (ImGui::Button("Cancel"))
        m_cancel();
}

} // namespace gui