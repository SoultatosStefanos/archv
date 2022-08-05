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

/***********************************************************
 * Setup                                                   *
 ***********************************************************/

void paused_state::enter()
{
    // setup_scene();
    // setup_gui();
}

void paused_state::setup_scene() // FIXME
{
    m_scene = m_root.getSceneManager("bob");
    assert(m_scene);
}

// TODO Config
void paused_state::setup_gui()
{
    m_platform = std::make_unique< MyGUI::OgrePlatform >();
    m_platform->initialise(&m_window, m_scene);
    m_gui = std::make_unique< MyGUI::Gui >();
    m_gui->initialise();

    auto* menu_bar = m_gui->createWidget< MyGUI::MenuBar >(
        "MenuBar", 20, 40, 1880, 60, MyGUI::Align::Default, "Main");

    menu_bar->setVisible(false);

    auto* layout_top_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 0, 0, 270, 60, MyGUI::Align::Left, "Main");
    layout_top_btn->setCaption("Layout / Topology");

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 270, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* clustering_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 270, 0, 270, 60, MyGUI::Align::Left, "Main");
    clustering_btn->setCaption("Clustering");

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 540, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* code_inspection_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 540, 0, 270, 60, MyGUI::Align::Left, "Main");
    code_inspection_btn->setCaption("Code Inspection");

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 810, 10, 40, 40, MyGUI::Align::Left, "Main");

    menu_bar->createWidget< MyGUI::Widget >(
        "MenuBarSeparator", 1630, 10, 40, 40, MyGUI::Align::Left, "Main");

    auto* settings_btn = menu_bar->createWidget< MyGUI::Button >(
        "MenuBarButton", 1630, 0, 270, 60, MyGUI::Align::Left, "Main");
    settings_btn->setCaption("Settings");
}

/***********************************************************
 * Shutdown                                                *
 ***********************************************************/

void paused_state::exit()
{
    // shutdown_gui();
    // shutdown_scene();
}

void paused_state::shutdown_gui()
{
    m_platform->shutdown();
    m_gui->shutdown();

    m_platform.reset();
    m_gui.reset();
}

void paused_state::shutdown_scene()
{
    RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(m_scene);
    m_root.destroySceneManager(m_scene);
}

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
    //  native_manager().injectKeyPress(translate(e.keysym.sym), e.type);

    if (e.keysym.sym == 'p')
        m_machine.fallback();

    return true;
}

auto paused_state::keyReleased(const KeyboardEvent& e) -> bool
{
    //  native_manager().injectKeyRelease(translate(e.keysym.sym));
    return true;
}

auto paused_state::mouseMoved(const MouseMotionEvent& e) -> bool
{
    //  native_manager().injectMouseMove(e.x, e.y, 0); // 2D
    return true;
}

auto paused_state::mousePressed(const MouseButtonEvent& e) -> bool
{
    //  native_manager().injectMousePress(e.x, e.y, translate(e.button));
    return true;
}

auto paused_state::mouseReleased(const MouseButtonEvent& e) -> bool
{
    //  native_manager().injectMouseRelease(e.x, e.y, translate(e.button));
    return true;
}

} // namespace view