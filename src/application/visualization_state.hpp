// Contains the main/running state of the application.
// Soultatos Stefanos 2022

#ifndef APPLICATION_VISUALIZATION_STATE_HPP
#define APPLICATION_VISUALIZATION_STATE_HPP

#include "gui/allfwd.hpp"
#include "rendering/graph_renderer.hpp"
#include "state.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <memory>

namespace application
{

class state_machine;

class visualization_state : public state
{
public:
    using overlay_manager = gui::overlay_manager;
    using graph_renderer = rendering::graph_renderer;

    visualization_state(
        state_machine& machine,
        overlay_manager& overlays,
        state* paused_state,
        graph_renderer renderer);

    virtual ~visualization_state() override = default;

    auto get_renderer() const -> const graph_renderer& { return m_renderer; }
    auto get_renderer() -> graph_renderer& { return m_renderer; }

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    auto frameStarted(const Ogre::FrameEvent&) -> bool override;
    void frameRendered(const Ogre::FrameEvent& e) override;
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;

private:
    state_machine& m_machine;
    overlay_manager& m_overlays;
    state* m_paused_state { nullptr };

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;

    graph_renderer m_renderer;
};

} // namespace application

#endif // APPLICATION_VISUALIZATION_STATE_HPP
