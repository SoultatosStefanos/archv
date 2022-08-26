// Contains the 3d graph renderer of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

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

namespace rendering
{

// Once setup, will latch and render to a render window.
class graph_renderer
{
public:
    using vertex = std::string;
    using vertices = std::vector< vertex >;

    explicit graph_renderer(
        Ogre::RenderWindow& window, vertices ids = vertices());

    auto get_scene() const -> auto* { return m_scene; }
    auto get_light() const -> auto* { return m_light; }
    auto get_cam() const -> auto* { return m_cam; }
    auto get_light_node() const -> auto* { return m_light_node; }
    auto get_cam_node() const -> auto* { return m_cam_node; }

    auto setup() -> void;
    auto shutdown() -> void;

    auto lay_vertex(const vertex& id, double x, double y, double z) -> void;

private:
    void setup_scene();
    void setup_lighting();
    void setup_camera();
    void setup_entities();

    void shutdown_entities();
    void shutdown_camera();
    void shutdown_lighting();
    void shutdown_scene();

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;
    vertices m_ids;

    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
