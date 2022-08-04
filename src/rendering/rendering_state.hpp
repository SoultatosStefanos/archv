// Contains a class responsible for drawing entities during the
// visualization/running/rendering state of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_RENDERING_STATE_HPP
#define RENDERING_RENDERING_STATE_HPP

#include "states/state.hpp"

#include <OGRE/Bites/OgreInput.h>
#include <memory>
#include <vector>

namespace Ogre
{
class Root;
class RenderWindow;
class SceneManager;
class Light;
class SceneNode;
class Camera;
class SceneNode;
} // namespace Ogre

namespace OgreBites
{
class CameraMan;
}

namespace states
{
class state_machine;
}

namespace rendering
{

class rendering_state : public states::state
{
public:
    using vertex_id = std::string;
    using vertices = std::vector< vertex_id >;

    rendering_state(
        vertices ids,
        Ogre::Root& root,
        Ogre::RenderWindow& window,
        states::state_machine& machine,
        states::state* paused_state);

    virtual ~rendering_state() override = default;

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    void position_vertex(const vertex_id& id, double x, double y, double z);

    void frameRendered(const Ogre::FrameEvent& e);
    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto touchMoved(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchPressed(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto touchReleased(const OgreBites::TouchFingerEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mouseWheelRolled(const OgreBites::MouseWheelEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto textInput(const OgreBites::TextInputEvent& e) -> bool override;
    auto axisMoved(const OgreBites::AxisEvent& e) -> bool override;
    auto buttonPressed(const OgreBites::ButtonEvent& e) -> bool override;
    auto buttonReleased(const OgreBites::ButtonEvent& e) -> bool override;

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

    vertices m_ids;

    Ogre::Root& m_root;
    Ogre::RenderWindow& m_window;
    states::state_machine& m_machine;
    states::state* m_paused_state { nullptr };

    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;
};

} // namespace rendering

#endif // RENDERING_RENDERING_STATE_HPP
