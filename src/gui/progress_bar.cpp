#include "progress_bar.hpp"

#include <cassert>
#include <imgui/imgui.h>

namespace gui
{

progress_bar::progress_bar(caption_type caption)
: m_caption { std::move(caption) }
{
}

auto progress_bar::draw() const -> void
{
    if (!ImGui::Begin(""))
    {
        ImGui::End();
        return;
    }

    ImGui::Text(caption().c_str());
    ImGui::Spacing();

    ImGui::ProgressBar(progress());
}

} // namespace gui