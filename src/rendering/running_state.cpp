#include "running_state.hpp"

#include "view/state_machine.hpp"

#include <SDL2/SDL_mouse.h>

namespace rendering
{

running_state::running_state(
    state_machine& machine,
    state* paused_state,
    vertex_ids vertices,
    Ogre::RenderWindow& window,
    Ogre::SceneManager& scene)
: m_machine { machine }
, m_paused_state { paused_state }
, m_vertices { std::move(vertices) }
, m_window { window }
, m_scene { scene }
{
}

void running_state::enter()
{
    m_renderer
        = std::make_unique< graph_renderer >(m_vertices, m_window, m_scene);

    m_cameraman
        = std::make_unique< OgreBites::CameraMan >(m_renderer->get_cam_node());
}

void running_state::exit()
{
    m_renderer.reset();
    m_cameraman.reset();
}

void running_state::pause()
{
    SDL_ShowCursor(false);
    m_cameraman->manualStop();
}

void running_state::resume() { SDL_ShowCursor(true); }

void running_state::frameRendered(const Ogre::FrameEvent& e)
{
    assert(m_cameraman);
    m_cameraman->frameRendered(e);
}

auto running_state::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    assert(m_paused_state);

    m_cameraman->keyPressed(e);

    // FIXME

    assert(m_machine.get_active_state() == this);

    static constexpr auto paused_key { 'p' };
    static constexpr auto quit_key { OgreBites::SDLK_ESCAPE };

    const auto pressed_key = e.keysym.sym;

    switch (pressed_key)
    {
    case paused_key:
        m_machine.transition_to(m_paused_state);
        break;

    case quit_key:
        m_machine.fallback();
        Ogre::Root::getSingleton().queueEndRendering();
        break;

    default:
        break;
    }

    return true;
}

auto running_state::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->keyReleased(e);
}

auto running_state::mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseMoved(e);
}

auto running_state::mouseWheelRolled(const OgreBites::MouseWheelEvent& e)
    -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseWheelRolled(e);
}

auto running_state::mousePressed(const OgreBites::MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mousePressed(e);
}

auto running_state::mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool
{
    assert(m_cameraman);
    return m_cameraman->mouseReleased(e);
}

} // namespace rendering
