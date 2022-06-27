// Containes the architecture visualizaer core application.
// Soultatos Stefanos 2022

#ifndef VISUALIZATION_APP_HPP
#define VISUALIZATION_APP_HPP

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

    App(const Graph& g, const Layout& layout)
        : Base{name}, m_g{g}, m_layout{layout}
    {}

    virtual ~App() override = default;

    void setup() override;
    void shutdown() override;

private:
    const Graph& m_g;
    const Layout& m_layout;
};

} // namespace Visualization

#endif // VISUALIZATION_APP_HPP
