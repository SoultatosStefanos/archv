#include "inspection_handler.hpp"

#include "gui/overlay_manager.hpp"
#include "gui/popup_holder.hpp"

#include <boost/log/trivial.hpp>

namespace input
{

inspection_handler::inspection_handler(
    graph_renderer_type& renderer,
    const graph_collision_checker_type& collisions,
    overlay_manager_type& overlays,
    popup_holder_type& popups,
    const render_window_type& window,
    const camera_type& camera)
: m_renderer { renderer }
, m_collisions { collisions }
, m_overlays { overlays }
, m_popups { popups }
, m_win { window }
, m_cam { camera }
{
}

namespace
{
    // Returns a ray in world coordinates from a mouse event.
    template < typename MouseEvent >
    requires std::same_as< MouseEvent, OgreBites::MouseMotionEvent > || std::
        same_as< MouseEvent, OgreBites::MouseButtonEvent >
    inline auto mouse_ray(
        const inspection_handler::render_window_type& win,
        const inspection_handler::camera_type& cam,
        const MouseEvent& e) -> Ogre::Ray
    {
        using Ogre::Real;
        return cam.getCameraToViewportRay(
            static_cast< Real >(e.x) / static_cast< Real >(win.getWidth()),
            static_cast< Real >(e.y) / static_cast< Real >(win.getHeight()));
    }

    // Convertes a mouse event position into a gui position.
    template < typename MouseEvent >
    requires std::same_as< MouseEvent, OgreBites::MouseMotionEvent > || std::
        same_as< MouseEvent, OgreBites::MouseButtonEvent >
    inline auto gui_pos(const MouseEvent& e) -> gui::point
    {
        using coord = gui::point::coord;
        return { static_cast< coord >(e.x), static_cast< coord >(e.y) };
    }
} // namespace

auto inspection_handler::mouseMoved(const OgreBites::MouseMotionEvent& e)
    -> bool
{
    const auto ray = mouse_ray(window(), camera(), e);
    const auto collision = collisions().vertex_collision(ray);

    if (collision)
    {
        // Did not dispatch collision.
        if (m_collision_stack.empty() || m_collision_stack.top() != *collision)
        {
            BOOST_LOG_TRIVIAL(debug) << "hovered inside: " << *collision;
            m_collision_stack.push(*collision);
            renderer().render_vertex_pop_out_effect(*collision);
        }
    }
    else
    {
        // Stopped colliding with vertex.
        if (!m_collision_stack.empty())
        {
            const auto prev = m_collision_stack.top();
            m_collision_stack.pop();
            BOOST_LOG_TRIVIAL(debug) << "hovered outside: " << prev;
            renderer().hide_vertex_pop_out_effect(prev);
        }
    }

    return true;
}

auto inspection_handler::mousePressed(const OgreBites::MouseButtonEvent& e)
    -> bool
{
    const auto ray = mouse_ray(window(), camera(), e);
    const auto collision = collisions().vertex_collision(ray);

    if (collision)
    {
        BOOST_LOG_TRIVIAL(debug) << "colliding with: " << *collision;
        auto& popup = popups().get(*collision);
        popup.show();
        popup.pos() = gui_pos(e);
        overlays().submit(&popup); // Queued to be rendered now.
    }

    return true;
}

} // namespace input