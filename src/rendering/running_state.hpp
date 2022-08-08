// Contains the running rendering state of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_RUNNING_STATE_HPP
#define RENDERING_RUNNING_STATE_HPP

#include "graph_renderer.hpp"
#include "view/state.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/OgreRenderWindow.h>

namespace view
{
class state_machine;
} // namespace view

namespace rendering
{

class running_state : public view::state
{
public:
    using state = view::state;
    using state_machine = view::state_machine;

    using vertex_ids = graph_renderer::vertex_ids;

    running_state(
        state_machine& machine,
        state* paused_state,
        vertex_ids vertices,
        Ogre::RenderWindow& window,
        Ogre::SceneManager& scene);

    virtual ~running_state() override = default;

    auto get_renderer() const -> graph_renderer* { return m_renderer.get(); }

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    void frameRendered(const Ogre::FrameEvent& e) override;
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;

private:
    std::unique_ptr< graph_renderer > m_renderer;
    state_machine& m_machine;
    state* m_paused_state { nullptr };

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;

    vertex_ids m_vertices;
    Ogre::RenderWindow& m_window;
    Ogre::SceneManager& m_scene;
};

} // namespace rendering

#endif // RENDERING_RUNNING_STATE_HPP
