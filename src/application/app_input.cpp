#include "app.hpp"
#include "commands.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <boost/log/trivial.hpp>

namespace application
{

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

auto app::setup_input() -> void
{
    assert(m_background_renderer);

    m_cameraman = std::make_unique< OgreBites::CameraMan >(
        m_background_renderer->cam_node());

    m_gui_input_listener = std::make_unique< OgreBites::ImGuiInputListener >();

    m_shortcut_input_listener
        = std::make_unique< shortcut_input_listener >(*m_menu_bar);

    addInputListener(this);
    addInputListener(m_cameraman.get());
    addInputListener(m_shortcut_input_listener.get());

#ifdef NDEBUG
    SDL_SetRelativeMouseMode(SDL_TRUE);
#else
    SDL_ShowCursor(SDL_FALSE);
#endif

    BOOST_LOG_TRIVIAL(info) << "setup input";
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown_input() -> void
{
    removeInputListener(m_shortcut_input_listener.get());
    removeInputListener(m_gui_input_listener.get());
    removeInputListener(m_cameraman.get());

    m_shortcut_input_listener.reset();
    m_gui_input_listener.reset();
    m_cameraman.reset();

    BOOST_LOG_TRIVIAL(info) << "shutdown input";
}

} // namespace application