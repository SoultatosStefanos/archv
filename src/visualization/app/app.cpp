#include "app.hpp"

#include <boost/log/trivial.hpp>

namespace Visualization
{

using namespace Ogre;
using namespace OgreBites;

App::App(const Graph& g, UniqueLayoutPtr layout)
    : Base{"Architecture Visualizer"}, m_g{g}, m_layout{std::move(layout)}
{
    assert(m_layout);
}

void App::layout(UniqueLayoutPtr l)
{
    assert(l);
    m_layout = std::move(l);
}

// FIXME
void App::setup()
{
    ApplicationContext::setup();

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
        auto* node = m_scene->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setPosition(m_layout->x(v), m_layout->y(v), m_layout->z(v));
        node->setScale(0.15, 0.15, 0.15);

        BOOST_LOG_TRIVIAL(trace)
            << "made vertex entity at: (" << node->getPosition().x << ", "
            << node->getPosition().y << ", " << node->getPosition().z << ')';
    }
}

// FIXME
void App::shutdown()
{
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

} // namespace Visualization