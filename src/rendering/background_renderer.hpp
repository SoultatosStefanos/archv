// Contains the background renderer of the application.
// Soultatos Stefanos 2022

#ifndef RENDERING_BACKGROUND_RENDERER_HPP
#define RENDERING_BACKGROUND_RENDERER_HPP

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>

namespace rendering
{

// Will prepare a scene at a render window upon initialization.
class background_renderer
{
public:
    explicit background_renderer(Ogre::RenderWindow& window);
    ~background_renderer();

    background_renderer(const background_renderer&) = default;
    background_renderer(background_renderer&&) = default;

    auto operator=(const background_renderer&) -> background_renderer& = delete;
    auto operator=(background_renderer&&) -> background_renderer& = delete;

    auto scene() const -> auto* { return m_scene; }
    auto light() const -> auto* { return m_light; }
    auto cam() const -> auto* { return m_cam; }
    auto light_node() const -> auto* { return m_light_node; }
    auto cam_node() const -> auto* { return m_cam_node; }

private:
    auto setup_scene() -> void;
    auto setup_lighting() -> void;
    auto setup_camera() -> void;

    auto shutdown_camera() -> void;
    auto shutdown_lighting() -> void;
    auto shutdown_scene() -> void;

    Ogre::Root& m_root; // Obtained from global context.
    Ogre::RenderWindow& m_window;

    Ogre::SceneManager* m_scene { nullptr };
    Ogre::Light* m_light { nullptr };
    Ogre::SceneNode* m_light_node { nullptr };
    Ogre::Camera* m_cam { nullptr };
    Ogre::SceneNode* m_cam_node { nullptr };
};

} // namespace rendering

#endif // RENDERING_BACKGROUND_RENDERER_HPP
