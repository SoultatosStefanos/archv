// Contains a class that handles the OIS initialization and dispatches physical
// events to listeners.
// Soultatos Stefanos 2022

#ifndef INPUT_INPUT_DISPATCHER_HPP
#define INPUT_INPUT_DISPATCHER_HPP

#include <OGRE/OgreRenderWindow.h>
#include <list>
#include <memory>
#include <ois/OISJoyStick.h>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <vector>

namespace input
{

// General purpose OIS physical input events dispatcher.
class input_dispatcher : public OIS::KeyListener,
                         public OIS::MouseListener,
                         public OIS::JoyStickListener
{
public:
    explicit input_dispatcher(Ogre::RenderWindow& win);
    input_dispatcher(const input_dispatcher&) = default;
    input_dispatcher(input_dispatcher&&) = default;
    virtual ~input_dispatcher() override;

    auto operator=(const input_dispatcher&) -> input_dispatcher& = default;
    auto operator=(input_dispatcher&&) -> input_dispatcher& = default;

    // If window resizes.
    void set_window_extents(int w, int h);

    auto keyboard() const -> const OIS::Keyboard* { return m_keyboard; }
    auto keyboard() -> OIS::Keyboard* { return m_keyboard; }

    auto mouse() const -> const OIS::Mouse* { return m_mouse; }
    auto mouse() -> OIS::Mouse* { return m_mouse; }

    auto joysticks() const -> const auto& { return m_joysticks; }
    auto joysticks() -> auto& { return m_joysticks; }

    void add_key_listener(OIS::KeyListener* listener);
    void add_mouse_listener(OIS::MouseListener* listener);
    void add_joystick_listener(OIS::JoyStickListener* listener);

    void remove_key_listener(OIS::KeyListener* listener);
    void remove_mouse_listener(OIS::MouseListener* listener);
    void remove_joystick_listener(OIS::JoyStickListener* listener);

    void clear_key_listeners();
    void clear_mouse_listeners();
    void clear_joystick_listeners();

    void capture();

private:
    void initialize_input_system(Ogre::RenderWindow& win);
    void initialize_keyboard();
    void initialize_mouse(Ogre::RenderWindow& win);
    void initialize_joysticks();

    void destroy_joysticks();
    void destroy_mouse();
    void destroy_keyboard();
    void destroy_input_system();

    auto keyPressed(const OIS::KeyEvent& e) -> bool;
    auto keyReleased(const OIS::KeyEvent& e) -> bool;

    auto mouseMoved(const OIS::MouseEvent& e) -> bool;
    auto mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) -> bool;
    auto mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) -> bool;

    auto povMoved(const OIS::JoyStickEvent& e, int pov) -> bool;
    auto axisMoved(const OIS::JoyStickEvent& e, int axis) -> bool;
    auto sliderMoved(const OIS::JoyStickEvent& e, int id) -> bool;

    auto buttonPressed(const OIS::JoyStickEvent& e, int button) -> bool;
    auto buttonReleased(const OIS::JoyStickEvent& e, int button) -> bool;

    OIS::InputManager* m_input_system = nullptr;

    OIS::Keyboard* m_keyboard = nullptr;
    OIS::Mouse* m_mouse = nullptr;
    std::vector<OIS::JoyStick*> m_joysticks;

    std::list<OIS::KeyListener*> m_key_listeners;
    std::list<OIS::MouseListener*> m_mouse_listeners;
    std::list<OIS::JoyStickListener*> m_joystick_listeners;
};

} // namespace input

#endif // INPUT_INPUT_DISPATCHER_HPP
