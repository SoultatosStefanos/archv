#include "app.hpp"

#include <OgreManualObject.h>

using namespace Ogre;
using namespace OgreBites;

app::app(const graph& g) : ApplicationContext("Architecture Visualizer"), m_g{g}
{}

// TODO Cleanup

// FIXME
void app::setup()
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

#if (0) // FIXME
    {   // layout

        // NOTE: Will get the initial configurations from file and delegate to
        // the presenter to unformat them
        using namespace layout;

        auto space =
            topology_factory::make_topology(typeid(sphere).name(), 100);
        auto lay = layout_factory::make_layout(
            typeid(gursoy_atun_layout).name(), m_g, *space);

        m_layout_core =
            std::make_unique<layout_presenter>(m_g,
                                               std::move(lay),
                                               std::move(space),
                                               view(*m_scene),
                                               update_layout_service(m_cmds),
                                               update_topology_service(m_cmds));

        m_layout_core->update_view();
    }

#endif
}

// FIXME
void app::shutdown()
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

auto app::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    static constexpr auto quit_sym = SDLK_ESCAPE;

    if (e.keysym.sym == quit_sym)
        getRoot()->queueEndRendering();
#if (0) // FIXME
    else if (e.keysym.sym == SDLK_LEFT)
        m_cmds.undo();
    else if (e.keysym.sym == SDLK_RIGHT)
        m_cmds.redo();
    else if (e.keysym.sym == SDLK_UP)
    {
        m_layout_core->get_view().send_topology_request(
            typeid(layout::sphere).name(), 80);
    }
    else if (e.keysym.sym == SDLK_DOWN)
    {
        m_layout_core->get_view().send_topology_request(
            typeid(layout::cube).name(), 80);
    }
#endif

    return true;
}