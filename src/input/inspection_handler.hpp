// Contains the input listener class responsible for toggling code inspection
// metadata upon vertex clicks.
// Soultatos Stefanos 2022

#ifndef INPUT_INSPECTION_HANDLER_HPP
#define INPUT_INSPECTION_HANDLER_HPP

#include "architecture/all.hpp"
#include "gui/allfwd.hpp"
#include "rendering/graph_collision_checker.hpp"
#include "rendering/graph_renderer.hpp"

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreRay.h>
#include <OGRE/OgreRenderWindow.h>
#include <stack>

namespace input
{

class inspection_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using graph_renderer_type = rendering::graph_renderer<
        architecture::graph,
        architecture::id_map,
        architecture::dependency_map >;

    using graph_collision_checker_type = rendering::graph_collision_checker<
        architecture::graph,
        architecture::id_map,
        architecture::dependency_map >;

    using overlay_manager_type = gui::overlay_manager;
    using popup_holder_type = gui::popup_holder;

    using render_window_type = Ogre::RenderWindow;
    using camera_type = Ogre::Camera;

    inspection_handler(
        graph_renderer_type& renderer,
        const graph_collision_checker_type& collisions,
        overlay_manager_type& overlays,
        popup_holder_type& popups,
        const render_window_type& window,
        const camera_type& camera);

    ~inspection_handler() override = default;

    auto renderer() const -> const auto& { return m_renderer; }
    auto renderer() -> auto& { return m_renderer; }
    auto collisions() const -> const auto& { return m_collisions; }
    auto overlays() const -> const auto& { return m_overlays; }
    auto overlays() -> auto& { return m_overlays; }
    auto popups() const -> const auto& { return m_popups; }
    auto popups() -> auto& { return m_popups; }
    auto window() const -> const auto& { return m_win; }
    auto camera() const -> const auto& { return m_cam; }

    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool;
    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool;

private:
    using collision_id_type = graph_collision_checker_type::id_type;
    using collision_stack = std::stack< collision_id_type >;

    graph_renderer_type& m_renderer;
    const graph_collision_checker_type& m_collisions;
    overlay_manager_type& m_overlays;
    popup_holder_type& m_popups;
    const render_window_type& m_win;
    const camera_type& m_cam;

    collision_stack m_collision_stack;
};

} // namespace input

#endif // INPUT_INSPECTION_HANDLER_HPP
