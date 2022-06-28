// Containes the architecture visualizaer core application.
// Soultatos Stefanos 2022

#ifndef APP_APP_HPP
#define APP_APP_HPP

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "architecture/graph.hpp"
#include "visualization/layout/layout.hpp"

#include <memory>

namespace Visualization
{

// The heart of the application
class App : public OgreBites::ApplicationContext
{
public:
    using Base = OgreBites::ApplicationContext;
    using Graph = Architecture::Graph;

    static constexpr auto name{"Architecture Visualizer"};

    App(const Graph& g, const Layout& layout);

    virtual ~App() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

private:
    const Graph& m_g;
    const Layout& m_layout;

    Ogre::SceneManager* m_scene{nullptr};
};

} // namespace Visualization

#endif // APP_APP_HPP
