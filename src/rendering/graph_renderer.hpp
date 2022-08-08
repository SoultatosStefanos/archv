// Contains the module responsible for rendering a graph in 3D.
// Soultatos Stefanos 2022

#ifndef RENDERING_GRAPH_RENDERER_HPP
#define RENDERING_GRAPH_RENDERER_HPP

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <string>
#include <vector>

namespace rendering
{

class graph_renderer
{
public:
    using vertex_id = std::string;
    using vertex_ids = std::vector< vertex_id >;

    graph_renderer(
        vertex_ids vertices,
        Ogre::RenderWindow& window,
        Ogre::SceneManager& scene);

    ~graph_renderer();

    auto get_scene() const -> Ogre::SceneManager* { return &m_scene; }
    auto get_cam_node() const -> Ogre::SceneNode* { return m_cam_node; }

    void lay_vertex(const vertex_id& id, double x, double y, double z);

private:
    void setup_lighting();
    void setup_camera();
    void setup_entities();

    void shutdown_entities();
    void shutdown_camera();
    void shutdown_lighting();

    vertex_ids m_vertices;

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;
    Ogre::SceneManager& m_scene;

    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };
};

} // namespace rendering

#endif // RENDERING_GRAPH_RENDERER_HPP
