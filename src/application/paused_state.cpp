#include "paused_state.hpp"

#include "gui/menu_bar.hpp"
#include "gui/menu_window.hpp"
#include "gui/overlay.hpp"
#include "gui/overlay_manager.hpp"
#include "state_machine.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL_keycode.h>
#include <boost/log/trivial.hpp>

namespace application
{

namespace
{
    static constexpr auto resume_key = 'p';
    static constexpr auto ctrl_key = SDLK_LCTRL;
    static constexpr auto undo_key = 'z';
    static constexpr auto redo_key = 'y';

} // namespace

paused_state::paused_state(
    state_machine& sm,
    overlay_manager& overlays,
    std::unique_ptr< menu_window > win,
    std::unique_ptr< menu_bar > bar)
: m_sm { sm }
, m_overlays { overlays }
, m_imgui_input { std::make_unique< OgreBites::ImGuiInputListener >() }
, m_win { std::move(win) }
, m_bar { std::move(bar) }
{
    assert(m_win);
    assert(m_bar);
}

auto paused_state::enter() -> void
{
    m_overlays.submit(m_win.get());
    m_overlays.submit(m_bar.get());
}

auto paused_state::exit() -> void
{
    m_overlays.withdraw(m_win.get());
    m_overlays.withdraw(m_bar.get());
}

auto paused_state::pause() -> void { }

auto paused_state::resume() -> void { }

auto paused_state::frameStarted(const Ogre::FrameEvent&) -> bool
{
    Ogre::ImGuiOverlay::NewFrame();
    m_overlays.draw_all();

    return true;
}

auto paused_state::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
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

auto paused_state::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
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

auto paused_state::mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool
{
    m_imgui_input->mouseMoved(e);
    return true;
}

auto paused_state::mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool
{
    m_imgui_input->mouseWheelRolled(e);
    return true;
}

auto paused_state::mousePressed(const OgreBites::MouseButtonEvent& e) -> bool
{
    m_imgui_input->mousePressed(e);
    return true;
}

auto paused_state::mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool
{
    m_imgui_input->mouseReleased(e);
    return true;
}

auto paused_state::textInput(const OgreBites::TextInputEvent& e) -> bool
{
    m_imgui_input->textInput(e);
    return true;
}

auto paused_state::buttonPressed(const OgreBites::ButtonEvent& e) -> bool
{
    m_imgui_input->buttonPressed(e);
    return true;
}

auto paused_state::buttonReleased(const OgreBites::ButtonEvent& e) -> bool
{
    m_imgui_input->buttonReleased(e);
    return true;
}

void paused_state::handle_undo_combination()
{
    if (ctrl_pressed and undo_pressed)
        get_menu_bar().undo_shortcut();
}

void paused_state::handle_redo_combination()
{
    if (ctrl_pressed and redo_pressed)
        get_menu_bar().redo_shortcut();
}

} // namespace application
