// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>

class app : public OgreBites::ApplicationContext
{
public:
    app();
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

private:
    void setup_scene() const;
    void setup_lighting() const;
    void setup_camera() const;
    void setup_resources() const;

    void shutdown_resources() const;
    void shutdown_camera() const;
    void shutdown_lighting() const;
    void shutdown_scene() const;
};

#endif // APP_HPP
