// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include "architecture/all.hpp"
#include "layout/all.hpp"
#include "utility/all.hpp"

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/property_map/function_property_map.hpp>
#include <memory>

class app : public OgreBites::ApplicationContext,
            public OgreBites::InputListener
{
public:
    explicit app(const architecture::graph& g);
    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

    virtual auto keyPressed(const OgreBites::KeyboardEvent& evt)
        -> bool override;

private:
    using graph = architecture::graph;
    using weight_function = std::function< int(graph::edge_descriptor) >;
    using weight_map = boost::
        function_property_map< weight_function, graph::edge_descriptor, int >;

    void setup_scene();
    void setup_lighting();
    void setup_camera();
    void setup_resources();
    void setup_entities();
    void setup_command_history();
    void setup_layout();
    void setup_gui();
    void setup_input();

    void shutdown_input();
    void shutdown_gui();
    void shutdown_layout();
    void shutdown_command_history();
    void shutdown_entities();
    void shutdown_resources();
    void shutdown_camera();
    void shutdown_lighting();
    void shutdown_scene();

    const architecture::graph& m_g;

    Ogre::SceneManager* m_scene = nullptr;
    Ogre::Light* m_light = nullptr;
    Ogre::SceneNode* m_light_node = nullptr;
    Ogre::Camera* m_cam = nullptr;
    Ogre::SceneNode* m_cam_node = nullptr;

    std::unique_ptr< utility::command_history > m_cmds;
    std::unique_ptr< layout::core< graph, weight_map > > m_layout_sys;

    std::unique_ptr< OgreBites::CameraMan > m_cameraman;

    std::unique_ptr< MyGUI::Gui > m_gui;
    std::unique_ptr< MyGUI::OgrePlatform > m_platform;
};

#endif // APP_HPP
