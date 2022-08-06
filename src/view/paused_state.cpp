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

    // Menubar

    auto* menubar = m_gui->createWidget< MyGUI::MenuBar >(
        "MenuBar",
        20,
        40,
        1880,
        60,
        MyGUI::Align::HStretch | MyGUI::Align::Top,
        "Main");

    // Layout

    auto* layout_btn
        = menubar->addItem("Layout / Topology", MyGUI::MenuItemType::Popup);

    auto* p = layout_btn->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p->setPopupAccept(true);

    auto* la = p->addItem("Layout", MyGUI::MenuItemType::Popup);

    auto* p1 = la->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p1->setPopupAccept(true);
    p1->addItem("Gursoy Atun", MyGUI::MenuItemType::Normal);

    auto* tt = p->addItem("Topology", MyGUI::MenuItemType::Popup);

    auto* p2 = tt->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p2->setPopupAccept(true);
    p2->addItem("Cube", MyGUI::MenuItemType::Normal);
    p2->addItem("Sphere", MyGUI::MenuItemType::Normal);

    auto* ts = p->addItem("Scale", MyGUI::MenuItemType::Popup);

    auto* p3 = ts->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p3->setPopupAccept(true);
    p3->addItem("100", MyGUI::MenuItemType::Normal);
    p3->addItem("60", MyGUI::MenuItemType::Normal);
    p3->addItem("80", MyGUI::MenuItemType::Normal);
    p3->addItem("200", MyGUI::MenuItemType::Normal);

    // Rest

    auto* clustering_btn
        = menubar->addItem("Clustering", MyGUI::MenuItemType::Popup);
    auto* code_btn
        = menubar->addItem("Code Inspection", MyGUI::MenuItemType::Popup);
    auto* settings_btn
        = menubar->addItem("Settings", MyGUI::MenuItemType::Popup);

    auto* s1 = menubar->addItem("Seperator1", MyGUI::MenuItemType::Separator);
    auto* s2 = menubar->addItem("Seperator2", MyGUI::MenuItemType::Separator);
    auto* s3 = menubar->addItem("Seperator3", MyGUI::MenuItemType::Separator);
    auto* s4 = menubar->addItem("Seperator4", MyGUI::MenuItemType::Separator);

    // Coords

    layout_btn->setCoord(0, 0, 270, 60);
    clustering_btn->setCoord(270, 0, 270, 60);
    code_btn->setCoord(540, 0, 270, 60);
    settings_btn->setCoord(1630, 0, 270, 60);

    s1->setCoord(270, 10, 40, 40);
    s2->setCoord(540, 10, 40, 40);
    s3->setCoord(810, 10, 40, 40);
    s4->setCoord(1630, 10, 40, 40);
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