// Contains the class responsible for the visualization of the arch graph.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_VISUALIZATION_HPP
#define RENDERING_GRAPH_VISUALIZATION_HPP

#include "view/state.hpp"

#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <memory>
#include <vector>

namespace view
{
class state_machine;
class state_machine;
} // namespace view

namespace rendering
{

class graph_visualization : public view::state
{
public:
    using state = view::state;
    using state_machine = view::state_machine;

    using vertex_id = std::string;
    using vertex_ids = std::vector< vertex_id >;

    graph_visualization(
        state_machine& machine,
        state* paused_state,
        Ogre::RenderWindow& window,
        vertex_ids ids = vertex_ids());

    virtual ~graph_visualization() override = default;

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    void lay_vertex(const vertex_id& id, double x, double y, double z);

    void frameRendered(const Ogre::FrameEvent& e) override;
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;

private:
    void setup_scene();
    void setup_lighting();
    void setup_camera();
    void setup_entities();
    void setup_input();

    void shutdown_input();
    void shutdown_entities();
    void shutdown_camera();
    void shutdown_lighting();
    void shutdown_scene();

    state_machine& m_machine;
    state* m_paused_state { nullptr };

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;

    vertex_ids m_ids;

    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;
};

} // namespace rendering

#endif // RENDERING_GRAPH_VISUALIZATION_HPP
