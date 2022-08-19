#include "visualization_state.hpp"

#include "gui/overlay.hpp"
#include "gui/overlay_manager.hpp"
#include "state_machine.hpp"

#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>

using namespace Ogre;
using namespace OgreBites;

namespace application
{

namespace
{
    static constexpr auto pause_key = 'p';
    static constexpr auto quit_key = SDLK_ESCAPE;

} // namespace

visualization_state::visualization_state(
    state_machine& machine,
    overlay_manager& overlays,
    state* paused_state,
    graph_renderer renderer)
: m_machine { machine }
, m_overlays { overlays }
, m_paused_state { paused_state }
, m_renderer { std::move(renderer) }
{
}

void visualization_state::enter()
{
    get_renderer().setup();

    auto* ogre_overlay = OverlaySystem::getSingletonPtr();
    get_renderer().get_scene()->addRenderQueueListener(ogre_overlay);

    m_cameraman = std::make_unique< CameraMan >(get_renderer().get_cam_node());
}

void visualization_state::exit()
{
    m_cameraman.reset();

    auto* ogre_overlay = OverlaySystem::getSingletonPtr();
    get_renderer().get_scene()->removeRenderQueueListener(ogre_overlay);

    get_renderer().shutdown();
}

void visualization_state::pause() { m_cameraman->manualStop(); }

void visualization_state::resume() { }

auto visualization_state::frameStarted(const FrameEvent&) -> bool
{
    ImGuiOverlay::NewFrame();
    m_overlays.draw_all();
    return true;
}

void visualization_state::frameRendered(const FrameEvent& e)
{
    assert(m_cameraman);
    m_cameraman->frameRendered(e);
}

auto visualization_state::keyPressed(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    assert(m_paused_state);

    m_cameraman->keyPressed(e);

    switch (e.keysym.sym)
    {
    case pause_key:
        m_machine.transition_to(m_paused_state);
        break;

    case quit_key:
        m_machine.fallback();
        break;

    default:
        break;
    }

    return true;
}

auto visualization_state::keyReleased(const KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->keyReleased(e);
}

auto visualization_state::mouseMoved(const MouseMotionEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseMoved(e);
}

auto visualization_state::mouseWheelRolled(const MouseWheelEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseWheelRolled(e);
}

auto visualization_state::mousePressed(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mousePressed(e);
}

auto visualization_state::mouseReleased(const MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseReleased(e);
}

} // namespace application