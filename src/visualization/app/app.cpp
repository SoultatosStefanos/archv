#include "app.hpp"

#include <OgreManualObject.h>
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

void App::setup()
{
    ApplicationContext::setup();

    initialize_scene_manager();
    initialize_light();
    initialize_camera();
    initialize_vertices();
    initialize_edges();
    initialize_input();
}

void App::initialize_scene_manager()
{
    m_scene = getRoot()->createSceneManager();
    RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene);
    m_scene->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
}

void App::initialize_light()
{
    auto* light_node =
        m_scene->getRootSceneNode()->createChildSceneNode("LightNode");
    light_node->attachObject(m_scene->createLight());
    light_node->setPosition(20, 80, 50);
}

void App::initialize_camera()
{
    m_cam_node = m_scene->getRootSceneNode()->createChildSceneNode("Cam");
    auto* cam = m_scene->createCamera("C");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    m_cam_node->attachObject(cam);
    m_cam_node->setPosition(0, 0, 140);
    getRenderWindow()->addViewport(cam);
}

void App::initialize_vertices()
{
    for (auto v : boost::make_iterator_range(boost::vertices(m_g)))
    {
        auto* entity = m_scene->createEntity("ogrehead.mesh");
        auto* node = m_scene->getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setPosition(m_layout->x(v), m_layout->y(v), m_layout->z(v));
        node->setScale(0.15, 0.15, 0.15);

        BOOST_LOG_TRIVIAL(debug)
            << "made vertex entity at: (" << node->getPosition().x << ", "
            << node->getPosition().y << ", " << node->getPosition().z << ')';
    }
}

void App::initialize_edges()
{
    auto mat = Ogre::MaterialManager::getSingleton().create("Mat", "General");

    for (auto e : boost::make_iterator_range(boost::edges(m_g)))
    {
        auto* manual = m_scene->createManualObject();

        manual->begin("Mat", Ogre::RenderOperation::OT_LINE_LIST);

        const auto src = boost::source(e, m_g);
        const auto des = boost::target(e, m_g);

        manual->position(m_layout->x(src), m_layout->y(src), m_layout->z(src));
        manual->position(m_layout->x(des), m_layout->y(des), m_layout->z(des));

        manual->end();

        auto* manual_node = m_scene->getRootSceneNode()->createChildSceneNode();
        manual_node->attachObject(manual);
    }
}

void App::initialize_input()
{
    m_cameraman = new CameraMan(m_cam_node);
    addInputListener(m_cameraman);
    addInputListener(this);
}

// FIXME
void App::shutdown()
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

auto App::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    static constexpr auto quit_sym = SDLK_ESCAPE;

    if (e.keysym.sym == quit_sym)
        getRoot()->queueEndRendering();

    return true;
}

} // namespace Visualization