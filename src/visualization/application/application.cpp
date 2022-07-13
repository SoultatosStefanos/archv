#include "application.hpp"

#include <OgreManualObject.h>

namespace visualization::application
{

using namespace Ogre;
using namespace OgreBites;

application::application(const graph& g)
    : ApplicationContext("Architecture Visualizer"), m_g{g}
{}

// FIXME
void application::setup()
{
    ApplicationContext::setup();
    addInputListener(this);

    // scene manager
    m_scene = getRoot()->createSceneManager();
    RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene);
    m_scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    // light
    auto* light = m_scene->createLight();
    auto* light_node =
        m_scene->getRootSceneNode()->createChildSceneNode("LightNode");
    light_node->attachObject(light);
    light_node->setPosition(20, 80, 50);

    // camera
    auto* cam_node = m_scene->getRootSceneNode()->createChildSceneNode("Cam");
    auto* cam = m_scene->createCamera("C");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    cam_node->attachObject(cam);
    cam_node->setPosition(0, 0, 140);
    getRenderWindow()->addViewport(cam);

    // nodes
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
    {
        auto* entity = m_scene->createEntity("ogrehead.mesh");
        auto* node =
            m_scene->getRootSceneNode()->createChildSceneNode(m_g[v].sym.id);
        node->attachObject(entity);
        node->setScale(0.15, 0.15, 0.15);
    }

    // cameraman
    m_cameraman = new CameraMan(cam_node);
    addInputListener(m_cameraman);

    // layout
    // TODO Pass from config
    layout::core::get().initialize(
        m_cmds, "gursoy_atun", "sphere", 100, m_g, *m_scene);
}

// FIXME
void application::shutdown()
{
    // cameraman
    removeInputListener(m_cameraman);
    delete m_cameraman;

    // nodes
    // TODO remove and destroy nodes from root scene
    m_scene->destroyAllEntities();

    // camera
    m_scene->getRootSceneNode()->removeAndDestroyChild("Cam");
    m_scene->destroyAllCameras();

    // light
    m_scene->getRootSceneNode()->removeAndDestroyChild("LightNode");
    m_scene->destroyAllLights();

    // scene manager
    RTShader::ShaderGenerator::getSingleton().removeSceneManager(m_scene);
    RTShader::ShaderGenerator::getSingleton().destroy();
    getRoot()->destroySceneManager(m_scene);

    ApplicationContext::shutdown();
}

auto application::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    static constexpr auto quit_sym = SDLK_ESCAPE;

#if (0) // FIXME
    static std::string layout_type = "gursoy_atun";
    static std::string topology_type = "sphere";
    static double topology_scale = 100;
#endif

    if (e.keysym.sym == quit_sym)
        getRoot()->queueEndRendering();
#if (0) // FIXME
    else if (e.keysym.sym == SDLK_LEFT)
        m_cmds.undo();
    else if (e.keysym.sym == SDLK_RIGHT)
        m_cmds.redo();
    else if (e.keysym.sym == SDLK_UP)
    {
        layout::core::get().pipeline().post(
            layout::topology_request_event{.layout_type = layout_type,
                                           .old_type = topology_type,
                                           .old_scale = topology_scale,
                                           .new_type = "cube",
                                           .new_scale = 200});
        topology_type = "cube";
        topology_scale = 200;
    }
    else if (e.keysym.sym == SDLK_DOWN)
    {
        layout::core::get().pipeline().post(
            layout::topology_request_event{.layout_type = layout_type,
                                           .old_type = topology_type,
                                           .old_scale = topology_scale,
                                           .new_type = "sphere",
                                           .new_scale = 200});
        topology_type = "sphere";
        topology_scale = 200;
    }
#endif

    return true;
}

} // namespace visualization::application