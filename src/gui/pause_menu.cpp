#include "pause_menu.hpp"

#include "pause_menu_private.hpp"
#include "view/state_machine.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL_keycode.h>
#include <boost/log/trivial.hpp>
#include <imgui/imgui_stdlib.h>

namespace gui
{

namespace
{
    static constexpr auto prev_scene_name = "graph visualization";

    static constexpr auto resume_key = 'p';
    static constexpr auto ctrl_key = SDLK_LCTRL;
    static constexpr auto undo_key = 'z';
    static constexpr auto redo_key = 'y';

} // namespace

pause_menu::pause_menu(
    state_machine& sm,
    dependency_options dependencies,
    layout_options layouts,
    topology_options topologies,
    scale_options scales)
: m_sm { sm }
, m_imgui_input { std::make_unique< OgreBites::ImGuiInputListener >() }
, m_win { std::move(dependencies),
          std::move(layouts),
          std::move(topologies),
          std::move(scales) }
{
}

void pause_menu::enter()
{
    scene()->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::exit()
{
    scene()->removeRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::pause() { }

void pause_menu::resume() { }

auto pause_menu::frameStarted(const Ogre::FrameEvent&) -> bool
{
    Ogre::ImGuiOverlay::NewFrame();
    m_win.draw();
    m_bar.draw();

    return true;
}

auto pause_menu::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case resume_key:
        m_sm.fallback();
        break;

    case ctrl_key:
        ctrl_pressed = true;
        BOOST_LOG_TRIVIAL(debug) << "ctrl key pressed";

        break;

    case undo_key:
        undo_pressed = true;
        BOOST_LOG_TRIVIAL(debug) << "undo key pressed";
        handle_undo_combination();

        break;

    case redo_key:
        redo_pressed = true;
        BOOST_LOG_TRIVIAL(debug) << "redo key pressed";
        handle_redo_combination();
        break;

    default:
        break;
    }

    m_imgui_input->keyPressed(e);
    return true;
}

auto pause_menu::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case ctrl_key:
        ctrl_pressed = false;
        BOOST_LOG_TRIVIAL(debug) << "ctrl key released";
        break;

    case undo_key:
        undo_pressed = false;
        BOOST_LOG_TRIVIAL(debug) << "undo key released";
        break;

    case redo_key:
        redo_pressed = false;
        BOOST_LOG_TRIVIAL(debug) << "redo key released";
        break;

    default:
        break;
    }

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

auto pause_menu::scene() const -> Ogre::SceneManager*
{
    auto* s = Ogre::Root::getSingleton().getSceneManager(prev_scene_name);
    assert(s);
    return s;
}

void pause_menu::handle_undo_combination()
{
    if (ctrl_pressed and undo_pressed)
        m_bar.undo_shortcut();
}

void pause_menu::handle_redo_combination()
{
    if (ctrl_pressed and redo_pressed)
        m_bar.redo_shortcut();
}

} // namespace gui

namespace gui::detail
{

/***********************************************************
 * Pause menu window                                       *
 ***********************************************************/

pause_menu_window::pause_menu_window(
    dependency_options deps,
    layout_options layouts,
    topology_options topologies,
    scale_options scales)
: m_dependencies { std::move(deps) }
, m_layouts { std::move(layouts) }
, m_topologies { std::move(topologies) }
, m_scales { std::move(scales) }
, m_weight_strs { m_dependencies.size() }
{
    assert(m_weight_strs.size() == m_dependencies.size());
}

void pause_menu_window::draw() const
{
    if (!ImGui::Begin("ARCHV"))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Architecture visualization in 3D!");
    ImGui::Spacing();

    draw_dependencies_header();
    draw_layout_header();
    draw_clustering_header();
    draw_code_inspection_header();
}

void pause_menu_window::draw_dependencies_header() const
{
    if (ImGui::CollapsingHeader("Dependencies"))
    {
        for (auto i = 0; const auto& dependency : m_dependencies)
        {
            auto& weight_str = m_weight_strs[i++];
            const auto* buffer = dependency.c_str();

            if (ImGui::InputText(
                    buffer,
                    &weight_str,
                    ImGuiInputTextFlags_CharsDecimal
                        | ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (weight_str == "")
                    weight_str = "0";

                m_dependency_signal(dependency, std::stod(weight_str));
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Restore Defaults..."))
            m_dependencies_restore_sig();
    }
}

void pause_menu_window::draw_layout_header() const
{
    if (ImGui::CollapsingHeader("Layout/Topology"))
    {
        if (ImGui::TreeNode("Layout"))
        {
            for (auto i = 0; const auto& option : m_layouts)
            {
                const auto* buffer = option.c_str();

                if (ImGui::Selectable(buffer, m_selected_layout == i))
                {
                    m_selected_layout = i;

                    m_layout_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Topology"))
        {
            for (auto i = 0; const auto& option : m_topologies)
            {
                const auto* buffer = option.c_str();

                if (ImGui::Selectable(buffer, m_selected_topology == i))
                {
                    m_selected_topology = i;

                    m_topology_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Scale"))
        {
            for (auto i = 0; const auto& option : m_scales)
            {
                const auto* buffer = std::to_string(option).c_str();

                if (ImGui::Selectable(buffer, m_selected_scale == i))
                {
                    m_selected_scale = i;

                    m_scale_signal(option);
                }

                ++i;
            }

            ImGui::TreePop();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Restore Defaults..."))
            m_layout_restore_sig();
    }
}

void pause_menu_window::draw_clustering_header() const
{
    if (ImGui::CollapsingHeader("Clustering"))
    {
    }
}

void pause_menu_window::draw_code_inspection_header() const
{
    if (ImGui::CollapsingHeader("Code Inspection"))
    {
    }
}

namespace
{
    // Returns index == data.size() if the key was not found.
    template < typename AssociativeContainer >
    inline auto find_assoc_index(
        const AssociativeContainer& data,
        const typename AssociativeContainer::key_type& key)
    {
        const auto iter = data.find(key);
        return std::distance(std::begin(data), iter);
    }

} // namespace

void pause_menu_window::set_dependency(const std::string& type, int weight)
{
    const auto index = find_assoc_index(m_dependencies, type);
    assert(static_cast< std::size_t >(index) != m_dependencies.size());

    auto& weight_str = m_weight_strs.at(index);
    weight_str = std::to_string(weight);

    BOOST_LOG_TRIVIAL(info) << "dependency " << type << " set to " << weight;
}

void pause_menu_window::set_layout(const std::string& type)
{
    const auto index = find_assoc_index(m_layouts, type);
    assert(static_cast< std::size_t >(index) != m_layouts.size());

    m_selected_layout = index;

    BOOST_LOG_TRIVIAL(info) << "layout set to " << type;
}

void pause_menu_window::set_topology(const std::string& type)
{
    const auto index = find_assoc_index(m_topologies, type);
    assert(static_cast< std::size_t >(index) != m_topologies.size());

    m_selected_topology = index;

    BOOST_LOG_TRIVIAL(info) << "topology set to " << type;
}

void pause_menu_window::set_scale(double val)
{
    const auto index = find_assoc_index(m_scales, val);
    assert(static_cast< std::size_t >(index) != m_scales.size());

    m_selected_scale = index;

    BOOST_LOG_TRIVIAL(info) << "scale set to " << val;
}

/***********************************************************
 * Pause menu bar                                          *
 ***********************************************************/

pause_menu_bar::pause_menu_bar(
    undo_enabled is_undo_enabled, redo_enabled is_redo_enabled)
: m_undo_enabled { std::move(is_undo_enabled) }
, m_redo_enabled { std::move(is_redo_enabled) }
{
    assert(m_undo_enabled);
    assert(m_redo_enabled);
}

void pause_menu_bar::draw() const
{
    if (ImGui::BeginMainMenuBar())
    {
        draw_file_submenu();
        draw_edit_submenu();

        ImGui::EndMainMenuBar();
    }
}

// TODO
void pause_menu_bar::draw_file_submenu() const
{
    if (ImGui::BeginMenu("File"))
    {
        ImGui::EndMenu();
    }
}

void pause_menu_bar::draw_edit_submenu() const
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, m_undo_enabled()))
            m_undo_sig();

        if (ImGui::MenuItem("Redo", "CTRL+Y", false, m_redo_enabled()))
            m_redo_sig();

        ImGui::Separator();

        ImGui::EndMenu();
    }
}

void pause_menu_bar::undo_shortcut()
{
    if (m_undo_enabled())
        m_undo_sig();
}

void pause_menu_bar::redo_shortcut()
{
    if (m_redo_enabled())
        m_redo_sig();
}

} // namespace gui::detail