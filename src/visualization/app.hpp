#ifndef VISUALIZATION_APP_HPP
#define VISUALIZATION_APP_HPP

#include "Ogre.h"
#include "OgreApplicationContext.h"

namespace Visualization
{

// The heart of the application
class App : public OgreBites::ApplicationContext
{
public:
    using Base = OgreBites::ApplicationContext;

    static constexpr auto name{"Architecture Visualizer"};

    App() : Base{name} {}
    virtual ~App() override = default;

    void setup() override;    // TODO
    void shutdown() override; // TODO
};

} // namespace Visualization

#endif // VISUALIZATION_APP_HPP
