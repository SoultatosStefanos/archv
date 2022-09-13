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
 * Constructor                                             *
 ***********************************************************/

app::app(int argc, const char** argv) : base("ARCHV")
{
    if (argc != 2)
    {
        std::cout << "usage: `./<exec> <path/to/graph.json>`\n";
        std::exit(EXIT_FAILURE);
    }

    m_graph_path = argv[1];

    const auto& jsons = config::archive::get();

    m_dependencies_config
        = dependencies::deserialize(jsons.at(ARCHV_DEPS_CONFIG_PATH));

    m_layout_config = layout::deserialize(jsons.at(ARCHV_LAYOUT_CONFIG_PATH));

    m_rendering_config
        = rendering::deserialize(jsons.at(ARCHV_RENDERING_CONFIG_PATH));

    assert(!paused());
}

/***********************************************************
 * Events                                                  *
 ***********************************************************/

auto app::frameStarted(const Ogre::FrameEvent& e) -> bool
{
    base::frameStarted(e);
    Ogre::ImGuiOverlay::NewFrame();
    multithreading::poll_message();
    if (paused())
        m_gui->render();
    // if (paused())
    //     ImGui::ShowDemoWindow();
    return true;
}

auto app::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    switch (e.keysym.sym)
    {
    case OgreBites::SDLK_ESCAPE:
        getRoot()->queueEndRendering();
        break;

    case 'p':
        toggle_pause_resume();
        break;

    case 'f':
        toggle_frame_stats();
        break;

    default:
        break;
    }

    return true;
}

auto app::toggle_pause_resume() -> void
{
    if (!paused())
        pause();
    else
        resume();
}

auto app::toggle_frame_stats() -> void
{
    assert(m_tray_manager);

    if (m_tray_manager->areFrameStatsVisible())
        m_tray_manager->hideFrameStats();
    else
        m_tray_manager->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);
}

auto app::pause() -> void
{
    assert(!paused());

    m_cameraman->manualStop();
    removeInputListener(m_cameraman.get());

    addInputListener(m_gui_input_listener.get());

    m_paused = true;

    BOOST_LOG_TRIVIAL(info) << "paused";

    assert(paused());
}

auto app::resume() -> void
{
    assert(paused());
    assert(m_gui_input_listener);
    assert(m_cameraman);

    removeInputListener(m_gui_input_listener.get());

    addInputListener(m_cameraman.get());

    m_paused = false;

    BOOST_LOG_TRIVIAL(info) << "resumed";

    assert(!paused());
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void app::setup()
{
    base::setup();

    setup_commands();
    setup_architecture();
    setup_dependencies();
    setup_layout();

    setup_background_rendering();
    setup_graph_rendering();

    setup_gui();

    setup_input();

    connect_gui_with_dependencies();
    connect_gui_with_layout();
    connect_gui_with_command_history();
    connect_rendering_with_layout();
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

auto app::shutdown() -> void
{
    shutdown_input();

    shutdown_gui();
    shutdown_graph_rendering();
    shutdown_background_rendering();

    shutdown_layout();
    shutdown_dependencies();
    shutdown_commands();
    shutdown_architecture();

    base::shutdown();
}

/***********************************************************
 * Main loop                                               *
 ***********************************************************/

auto app::go() -> void
{
    getRoot()->startRendering();
}

} // namespace application