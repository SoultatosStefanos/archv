#include "app.hpp"

namespace Visualization
{

using namespace Ogre;
using namespace OgreBites;

App::App(const Graph& g, const Layout& layout)
    : Base{name}, m_g{g}, m_layout{layout}
{
    assert(std::all_of(boost::vertices(m_g).first,
                       boost::vertices(m_g).second,
                       [&](auto v) { return m_layout.maps(v); }));
}

// TODO Quick graph visualization test with ogre head mesh and camera man, plus
// test memory management.

// TODO
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

// TODO
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