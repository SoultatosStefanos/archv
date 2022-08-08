// Contains the application paused view state.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSED_STATE_HPP
#define GUI_PAUSED_STATE_HPP

#include "pause_menu.hpp"
#include "view/state.hpp"

#include <memory>

namespace view
{
class state_machine;
} // namespace view

namespace gui
{

class paused_state : public view::state
{
public:
    using state = view::state;
    using state_machine = view::state_machine;

    using layout_options = pause_menu::layout_options;
    using topology_options = pause_menu::topology_options;
    using scale_options = pause_menu::scale_options;

    paused_state(
        state_machine& machine,
        state* menu_state,
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    virtual ~paused_state() override = default;

    paused_state(const paused_state&) = delete;
    paused_state(paused_state&&) = delete;

    auto operator=(const paused_state&) -> paused_state& = delete;
    auto operator=(paused_state&&) -> paused_state& = delete;

    auto get_gui() const -> pause_menu* { return m_gui.get(); }

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
    std::unique_ptr< pause_menu > m_gui;
    state_machine& m_machine;
    state* m_menu_state { nullptr };

    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;
};

} // namespace gui

#endif // GUI_PAUSED_STATE_HPP
