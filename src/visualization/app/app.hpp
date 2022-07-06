// Containes the architecture visualizaer core application.
// Soultatos Stefanos 2022

#ifndef APP_APP_HPP
#define APP_APP_HPP

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "architecture/graph.hpp"
#include "visualization/layout/layout.hpp"

#include <OgreCameraMan.h>
#include <OgreInput.h>
#include <memory>

namespace Visualization
{

// The heart of the application.
class App : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
public:
    using Base = OgreBites::ApplicationContext;
    using Graph = Architecture::Graph;
    using UniqueLayoutPtr = std::unique_ptr<Layout>;

    App(const Graph& g, UniqueLayoutPtr layout);

    virtual ~App() override = default;

    auto layout() const -> const UniqueLayoutPtr& { return m_layout; }
    void layout(UniqueLayoutPtr l);

    virtual void setup() override;
    virtual void shutdown() override;

    virtual auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    void initialize_scene_manager();
    void initialize_light();
    void initialize_camera();
    void initialize_vertices();
    void initialize_edges();
    void initialize_input();

    const Graph& m_g;
    UniqueLayoutPtr m_layout;

    Ogre::SceneManager* m_scene{nullptr};
    Ogre::SceneNode* m_cam_node{nullptr};

    OgreBites::CameraMan* m_cameraman{nullptr};
};

} // namespace Visualization

#endif // APP_APP_HPP
