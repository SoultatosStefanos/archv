#include "app.hpp"

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
}

// FIXME
void App::shutdown()
{
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