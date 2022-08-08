#include "pause_menu.hpp"

#include "pause_menu_private.hpp"
#include "view/state_machine.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>

namespace gui::detail
{

pause_menu_gui::pause_menu_gui(
    layout_options layouts, topology_options topologies, scale_options scales)
: m_layouts { std::move(layouts) }
, m_topologies { std::move(topologies) }
, m_scales { std::move(scales) }
{
}

void pause_menu_gui::draw() const
{
    static bool open = true;

    if (!ImGui::Begin("ARCHV", &open))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Architecture visualization in 3D!");
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Dependencies"))
    {
    }

    if (ImGui::CollapsingHeader("Layout/Topology"))
    {
        if (ImGui::TreeNode("Layout"))
        {
            static std::size_t selected = -1;

            for (std::size_t i = 0; i < m_layouts.size(); ++i)
            {
                if (ImGui::Selectable(m_layouts.at(i).c_str(), selected == i))
                    selected = i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Topology"))
        {
            static std::size_t selected = -1;

            for (std::size_t i = 0; i < m_topologies.size(); ++i)
            {
                if (ImGui::Selectable(
                        m_topologies.at(i).c_str(), selected == i))
                    selected = i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scale"))
        {
            static std::size_t selected = -1;

            for (std::size_t i = 0; i < m_scales.size(); ++i)
            {
                const auto scale = m_scales.at(i);
                const auto scale_str = std::to_string(scale);
                if (ImGui::Selectable(scale_str.c_str(), selected == i))
                    selected = i;
            }

            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader("Clustering"))
    {
    }

    if (ImGui::CollapsingHeader("Code Inspection"))
    {
    }
}

} // namespace gui::detail

namespace gui
{

pause_menu::pause_menu(
    state_machine& sm,
    layout_options layouts,
    topology_options topologies,
    scale_options scales)
: m_sm { sm }
, m_imgui_input { std::make_unique< OgreBites::ImGuiInputListener >() }
, m_gui { std::move(layouts), std::move(topologies), std::move(scales) }
{
}

void pause_menu::enter()
{
    m_scene = Ogre::Root::getSingleton().getSceneManager("graph visualization");
    assert(m_scene);

    m_scene->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::exit()
{
    m_scene->removeRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::pause() { }

void pause_menu::resume() { }

auto pause_menu::frameStarted(const Ogre::FrameEvent&) -> bool
{
    Ogre::ImGuiOverlay::NewFrame();
    m_gui.draw();

    return true;
}

auto pause_menu::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'p')
        m_sm.fallback();

    m_imgui_input->keyPressed(e);
    return true;
}

auto pause_menu::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    m_imgui_input->keyReleased(e);
    return true;
}

auto pause_menu::mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool
{
    m_imgui_input->mouseMoved(e);
    return true;
}

auto pause_menu::mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool
{
    m_imgui_input->mouseWheelRolled(e);
    return true;
}

auto pause_menu::mousePressed(const OgreBites::MouseButtonEvent& e) -> bool
{
    m_imgui_input->mousePressed(e);
    return true;
}

auto pause_menu::mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool
{
    m_imgui_input->mouseReleased(e);
    return true;
}

auto pause_menu::textInput(const OgreBites::TextInputEvent& e) -> bool
{
    m_imgui_input->textInput(e);
    return true;
}

auto pause_menu::buttonPressed(const OgreBites::ButtonEvent& e) -> bool
{
    m_imgui_input->buttonPressed(e);
    return true;
}

auto pause_menu::buttonReleased(const OgreBites::ButtonEvent& e) -> bool
{
    m_imgui_input->buttonReleased(e);
    return true;
}

} // namespace gui