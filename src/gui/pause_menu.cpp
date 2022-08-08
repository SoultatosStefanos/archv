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
            static int selected = -1;

            if (ImGui::Selectable("Gursoy Atun", selected == 0))
            {
                selected = 0;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Topology"))
        {
            static int selected = -1;

            if (ImGui::Selectable("Cube", selected == 0))
                selected = 0;

            if (ImGui::Selectable("Sphere", selected == 1))
                selected = 1;

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scale"))
        {
            static int selected = -1;

            if (ImGui::Selectable("200", selected == 0))
                selected = 0;

            if (ImGui::Selectable("100", selected == 1))
                selected = 1;

            if (ImGui::Selectable("80", selected == 1))
                selected = 1;

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

pause_menu::pause_menu(state_machine& sm)
: m_sm { sm }
, m_imgui_input { std::make_unique< OgreBites::ImGuiInputListener >() }
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