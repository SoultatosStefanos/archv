#include "paused_state.hpp"

#include "state_machine.hpp"

#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <boost/log/trivial.hpp>

using namespace Ogre;
using namespace OgreBites;

namespace view
{

paused_state::paused_state(
    RenderWindow& window, state_machine& machine, state* menu_state)
: m_root { Root::getSingleton() }
, m_window { window }
, m_machine { machine }
, m_menu_state { menu_state }
{
}

paused_state::~paused_state()
{
    if (m_gui)
        shutdown_gui();

    shutdown_gui_platform();

    shutdown_scene();
}

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void paused_state::enter()
{
    setup_scene();

    if (!m_platform)
        setup_gui_platform();

    setup_gui();
}

void paused_state::setup_gui_platform()
{
    m_platform = new MyGUI::OgrePlatform();
    m_platform->initialise(&m_window, m_scene);
}

void paused_state::setup_scene()
{
    m_scene = m_root.getSceneManager("running_state");
}

// TODO Config
void paused_state::setup_gui()
{
    m_gui = new MyGUI::Gui();
    m_gui->initialise();

    auto* menu_bar = m_gui->createWidget< MyGUI::MenuBar >(
        "MenuBar", 20, 40, 1880, 60, MyGUI::Align::Default, "Main");

    auto* layout_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 0, 0, 270, 60, MyGUI::Align::Left, "Main");
    layout_btn->setCaption("Layout / Topology");
    layout_btn->setFontHeight(20);

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 270, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* clustering_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 270, 0, 270, 60, MyGUI::Align::Left, "Main");
    clustering_btn->setCaption("Clustering");
    clustering_btn->setFontHeight(20);

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 540, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* code_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 540, 0, 270, 60, MyGUI::Align::Left, "Main");
    code_btn->setCaption("Code Inspection");
    code_btn->setFontHeight(20);

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 810, 10, 40, 40, MyGUI::Align::Left, "Main");

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 1630, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* settings_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 1630, 0, 270, 60, MyGUI::Align::Left, "Main");
    settings_btn->setCaption("Settings");
    settings_btn->setFontHeight(20);

    auto* popup_txt_bar = m_gui->createWidget< MyGUI::MenuBar >(
        "PopupMenu", 0, 140, 390, 60, MyGUI::Align::Default, "Main");
    auto* popup_txt = popup_txt_bar->createWidget< MyGUI::TextBox >(
        "TextBox", 120, 0, 270, 60, MyGUI::Align::Top, "Main");
    popup_txt->setCaption("Layout/Topology");
    popup_txt->setTextAlign(MyGUI::Align::Center);
    popup_txt->setFontHeight(24);

    auto* popup_menu = m_gui->createWidget< MyGUI::MenuBar >(
        "PopupMenu", 120, 200, 270, 500, MyGUI::Align::Default, "Main");

    auto* popup_btn1 = popup_menu->createWidget< MyGUI::Button >(
        "MenuBarButton", 0, 0, 270, 40, MyGUI::Align::Top, "Main");
    popup_btn1->setCaption("Layout Algorithm");
    popup_btn1->setFontHeight(20);

    auto* popup_btn2 = popup_menu->createWidget< MyGUI::Button >(
        "MenuBarButton", 0, 40, 270, 40, MyGUI::Align::Top, "Main");
    popup_btn2->setCaption("Topology Type");
    popup_btn2->setFontHeight(20);

    auto* popup_btn3 = popup_menu->createWidget< MyGUI::Button >(
        "MenuBarButton", 0, 80, 270, 40, MyGUI::Align::Top, "Main");
    popup_btn3->setCaption("Topology Scale");
    popup_btn3->setFontHeight(20);
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void paused_state::exit()
{
    shutdown_gui();
    shutdown_scene();
}

void paused_state::shutdown_gui_platform()
{
    m_platform->shutdown();
    delete m_platform;
    m_platform = nullptr;
}

void paused_state::shutdown_gui()
{
    m_gui->shutdown();
    delete m_gui;
    m_gui = nullptr;
}

void paused_state::shutdown_scene() { m_scene = nullptr; }

void paused_state::pause() { }

void paused_state::resume() { }

/***********************************************************
 * Input                                                   *
 ***********************************************************/

namespace
{
    inline auto native_manager() -> auto&
    {
        return MyGUI::InputManager::getInstance();
    }

    // TODO Test this
    inline auto translate(OgreBites::Keycode code) -> MyGUI::KeyCode
    {
        return MyGUI::KeyCode::Enum(code);
    }

    inline auto translate(unsigned char mouse_button) -> MyGUI::MouseButton
    {
        return MyGUI::MouseButton::Enum(mouse_button - 1); // Check enums.
    }

} // namespace

auto paused_state::keyPressed(const KeyboardEvent& e) -> bool
{
    native_manager().injectKeyPress(translate(e.keysym.sym), e.type);

    if (e.keysym.sym == 'p')
        m_machine.fallback();

    return true;
}

auto paused_state::keyReleased(const KeyboardEvent& e) -> bool
{
    native_manager().injectKeyRelease(translate(e.keysym.sym));
    return true;
}

auto paused_state::mouseMoved(const MouseMotionEvent& e) -> bool
{
    native_manager().injectMouseMove(e.x, e.y, 0); // 2D
    return true;
}

auto paused_state::mousePressed(const MouseButtonEvent& e) -> bool
{
    native_manager().injectMousePress(e.x, e.y, translate(e.button));
    return true;
}

auto paused_state::mouseReleased(const MouseButtonEvent& e) -> bool
{
    native_manager().injectMouseRelease(e.x, e.y, translate(e.button));
    return true;
}

} // namespace view