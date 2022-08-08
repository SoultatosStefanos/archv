// Contains the application paused view state.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSED_STATE_HPP
#define GUI_PAUSED_STATE_HPP

#include "pause_menu.hpp"
#include "view/state.hpp"
#include "view/state_machine.hpp"

namespace gui
{

class paused_state : public view::state
{
public:
    using state = view::state;
    using state_machine = view::state_machine;

    paused_state(state_machine& machine, state* menu_state);
    virtual ~paused_state() override;

    paused_state(const paused_state&) = delete;
    paused_state(paused_state&&) = delete;

    auto operator=(const paused_state&) -> paused_state& = delete;
    auto operator=(paused_state&&) -> paused_state& = delete;

    auto get_pause_menu() const -> pause_menu* { return m_pause_menu; }

    virtual void enter() override;
    virtual void exit() override;

    virtual void pause() override;
    virtual void resume() override;

    auto keyPressed(const OgreBites::KeyboardEvent& e) -> bool override;
    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;
    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool override;
    auto mouseReleased(const OgreBites::MouseButtonEvent& e) -> bool override;

private:
    pause_menu* m_pause_menu { nullptr };
    state_machine& m_machine;
    state* m_menu_state { nullptr };
};

} // namespace gui

#endif // GUI_PAUSED_STATE_HPP
