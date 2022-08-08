// Contains a utility module for initializing/shutting down MYGUI.
// Soultatos Stefanos 2022

#ifndef GUI_MYGUI_MANAGER_HPP
#define GUI_MYGUI_MANAGER_HPP

#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreSceneManager.h>

namespace gui
{

// Primarily used for RAII.
class mygui_manager final
{
public:
    mygui_manager(const mygui_manager&) = delete;
    mygui_manager(mygui_manager&&) = delete;

    auto operator=(const mygui_manager&) -> mygui_manager& = delete;
    auto operator=(mygui_manager&&) -> mygui_manager& = delete;

    static auto get() -> mygui_manager&
    {
        static mygui_manager singleton;
        return singleton;
    }

    auto get_platform() const -> MyGUI::OgrePlatform* { return m_platform; }
    auto get_gui() const -> MyGUI::Gui* { return m_gui; }

    auto is_initialized() const -> bool;

    void initialize(Ogre::RenderWindow* win, Ogre::SceneManager* scene);
    void shutdown();

private:
    mygui_manager() = default;
    ~mygui_manager();

    MyGUI::OgrePlatform* m_platform { nullptr };
    MyGUI::Gui* m_gui { nullptr };
};

} // namespace gui

#endif // GUI_MYGUI_MANAGER_HPP
