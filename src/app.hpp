// Containes the architecture visualizer core application.
// Soultatos Stefanos 2022

#ifndef APP_HPP
#define APP_HPP

#include "architecture/graph.hpp"
#include "layout/core.hpp"
#include "states/all.hpp"
#include "utility/undo_redo.hpp"

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <memory>

class app : public OgreBites::ApplicationContext
{
public:
    explicit app(const architecture::graph& g);

    virtual ~app() override = default;

    virtual void setup() override;
    virtual void shutdown() override;

private:
    using weight_map = boost::
        constant_property_map< architecture::graph::edge_descriptor, int >;

    void setup_resources();
    void setup_states();
    void setup_command_history();
    void setup_layout();

    void shutdown_layout();
    void shutdown_command_history();
    void shutdown_states();
    void shutdown_resources();

    const architecture::graph& m_g;

    std::unique_ptr< utility::command_history > m_cmds;

    std::unique_ptr< layout::core< architecture::graph, weight_map > >
        m_layout_sys;

    std::unique_ptr< states::state_machine > m_sm;
    std::unique_ptr< states::rendering_state > m_rendering;
    std::unique_ptr< states::paused_state > m_paused;
    std::unique_ptr <states::state_event_dispatcher> m_dispatcher;
};

#endif // APP_HPP
