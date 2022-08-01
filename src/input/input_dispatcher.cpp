#include "input_dispatcher.hpp"

#include <boost/log/trivial.hpp>
#include <ois/OISInputManager.h>

namespace input
{

/***********************************************************
 * Initialization                                          *
 ***********************************************************/

input_dispatcher::input_dispatcher(Ogre::RenderWindow& win)
{
    initialize_input_system(win);
    initialize_keyboard();
    initialize_mouse(win);
    initialize_joysticks();
}

void input_dispatcher::initialize_input_system(Ogre::RenderWindow& win)
{
    size_t win_handle = 0;
    win.getCustomAttribute("WINDOW", &win_handle);

    OIS::ParamList p{{std::string("WINDOW"), std::to_string(win_handle)}};

    m_input_system = OIS::InputManager::createInputSystem(p);
    assert(m_input_system);

    BOOST_LOG_TRIVIAL(info) << "OIS input system initialized";
}

void input_dispatcher::initialize_keyboard()
{
    if (m_input_system->getNumberOfDevices(OIS::OISKeyboard) > 0)
    {
        auto* obj = m_input_system->createInputObject(OIS::OISKeyboard, true);
        assert(obj);

        m_keyboard = static_cast<OIS::Keyboard*>(obj);
        m_keyboard->setEventCallback(this);

        BOOST_LOG_TRIVIAL(info) << "OIS keyboard initialized";
    }
}

void input_dispatcher::initialize_mouse(Ogre::RenderWindow& win)
{
    if (m_input_system->getNumberOfDevices(OIS::OISMouse) > 0)
    {
        auto* obj = m_input_system->createInputObject(OIS::OISMouse, true);
        assert(obj);

        m_mouse = static_cast<OIS::Mouse*>(obj);
        m_mouse->setEventCallback(this);

        unsigned w, h;
        int x, y;
        win.getMetrics(w, h, x, y);

        set_window_extents(w, h);

        BOOST_LOG_TRIVIAL(info) << "OIS mouse initialized";
    }
}

void input_dispatcher::set_window_extents(int w, int h)
{
    if (m_mouse)
    {
        const OIS::MouseState& state = m_mouse->getMouseState();
        state.width = w;
        state.height = h;

        BOOST_LOG_TRIVIAL(info)
            << "OIS window extents set to: w = " << w << ", h = " << h;
    }
}

void input_dispatcher::initialize_joysticks()
{
    if (m_input_system->getNumberOfDevices(OIS::OISJoyStick) > 0)
    {
        const auto num = m_input_system->getNumberOfDevices(OIS::OISJoyStick);
        m_joysticks.resize(num);

        for (auto& stick : m_joysticks)
        {
            auto* obj = static_cast<OIS::JoyStick*>(
                m_input_system->createInputObject(OIS::OISJoyStick, true));
            assert(obj);

            stick = static_cast<OIS::JoyStick*>(obj);

            stick->setEventCallback(this);

            BOOST_LOG_TRIVIAL(info) << "OIS joystick initialized";
        }
    }
}

/***********************************************************
 * Destruction                                             *
 ***********************************************************/

input_dispatcher::~input_dispatcher()
{
    assert(m_input_system);
    destroy_joysticks();
    destroy_mouse();
    destroy_keyboard();
    destroy_input_system();
}

void input_dispatcher::destroy_joysticks()
{
    for (auto& stick : m_joysticks)
    {
        m_input_system->destroyInputObject(stick);
        stick = nullptr;

        BOOST_LOG_TRIVIAL(info) << "OIS joystick destroyed";
    }
}

void input_dispatcher::destroy_mouse()
{
    if (m_mouse)
    {
        m_input_system->destroyInputObject(m_mouse);
        m_mouse = nullptr;

        BOOST_LOG_TRIVIAL(info) << "OIS mouse destroyed";
    }
}

void input_dispatcher::destroy_keyboard()
{
    if (m_keyboard)
    {
        m_input_system->destroyInputObject(m_keyboard);
        m_keyboard = nullptr;

        BOOST_LOG_TRIVIAL(info) << "OIS keyboard destroyed";
    }
}

void input_dispatcher::destroy_input_system()
{
    m_input_system->destroyInputSystem(m_input_system);
    m_input_system = nullptr;

    BOOST_LOG_TRIVIAL(info) << "OIS input system destroyed";
}

/***********************************************************
 * Listeners management                                    *
 ***********************************************************/

void input_dispatcher::add_key_listener(OIS::KeyListener* listener)
{
    assert(listener);
    m_key_listeners.push_back(listener);
}

void input_dispatcher::add_mouse_listener(OIS::MouseListener* listener)
{
    assert(listener);
    m_mouse_listeners.push_back(listener);
}

void input_dispatcher::add_joystick_listener(OIS::JoyStickListener* listener)
{
    assert(listener);
    m_joystick_listeners.push_back(listener);
}

void input_dispatcher::remove_key_listener(OIS::KeyListener* listener)
{
    assert(listener);
    m_key_listeners.remove(listener);
}

void input_dispatcher::remove_mouse_listener(OIS::MouseListener* listener)
{
    assert(listener);
    m_mouse_listeners.remove(listener);
}

void input_dispatcher::remove_joystick_listener(OIS::JoyStickListener* listener)
{
    assert(listener);
    m_joystick_listeners.remove(listener);
}

void input_dispatcher::clear_key_listeners()
{
    m_key_listeners.clear();
}

void input_dispatcher::clear_mouse_listeners()
{
    m_mouse_listeners.clear();
}

void input_dispatcher::clear_joystick_listeners()
{
    m_joystick_listeners.clear();
}

/***********************************************************
 * Event dispatching                                       *
 ***********************************************************/

void input_dispatcher::capture()
{
    if (m_keyboard)
        m_keyboard->capture();

    if (m_mouse)
        m_mouse->capture();

    for (auto* stick : m_joysticks)
        stick->capture();
}

auto input_dispatcher::keyPressed(const OIS::KeyEvent& e) -> bool
{
    for (auto* listener : m_key_listeners)
        if (!listener->keyPressed(e)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::keyReleased(const OIS::KeyEvent& e) -> bool
{
    for (auto* listener : m_key_listeners)
        if (!listener->keyReleased(e)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::mouseMoved(const OIS::MouseEvent& e) -> bool
{
    for (auto* listener : m_mouse_listeners)
        if (!listener->mouseMoved(e)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::mousePressed(const OIS::MouseEvent& e,
                                    OIS::MouseButtonID id) -> bool
{
    for (auto* listener : m_mouse_listeners)
        if (!listener->mousePressed(e, id)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::mouseReleased(const OIS::MouseEvent& e,
                                     OIS::MouseButtonID id) -> bool
{
    for (auto* listener : m_mouse_listeners)
        if (!listener->mouseReleased(e, id)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::povMoved(const OIS::JoyStickEvent& e, int pov) -> bool
{
    for (auto* listener : m_joystick_listeners)
        if (!listener->povMoved(e, pov)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::axisMoved(const OIS::JoyStickEvent& e, int axis) -> bool
{
    for (auto* listener : m_joystick_listeners)
        if (!listener->axisMoved(e, axis)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::sliderMoved(const OIS::JoyStickEvent& e, int id) -> bool
{
    for (auto* listener : m_joystick_listeners)
        if (!listener->sliderMoved(e, id)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::buttonPressed(const OIS::JoyStickEvent& e, int button)
    -> bool
{
    for (auto* listener : m_joystick_listeners)
        if (!listener->buttonPressed(e, button)) // Handler did not propagate
            break;

    return true;
}

auto input_dispatcher::buttonReleased(const OIS::JoyStickEvent& e, int button)
    -> bool
{
    for (auto* listener : m_joystick_listeners)
        if (!listener->buttonReleased(e, button)) // Handler did not propagate
            break;

    return true;
}

} // namespace input
