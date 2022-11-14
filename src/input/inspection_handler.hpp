// Contains the input listener class responsible for toggling code inspection
// metadata upon vertex clicks.
// Soultatos Stefanos 2022

#ifndef INPUT_INSPECTION_HANDLER_HPP
#define INPUT_INSPECTION_HANDLER_HPP

#include "architecture/all.hpp"
#include "rendering/graph_collision_checker.hpp"

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreRay.h>
#include <OGRE/OgreRenderWindow.h>

namespace input
{

class inspection_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using graph_collision_checker_type = rendering::graph_collision_checker<
        architecture::graph,
        architecture::id_map,
        architecture::dependency_map >;

    using render_window_type = Ogre::RenderWindow;
    using camera_type = Ogre::Camera;

    inspection_handler(
        const graph_collision_checker_type& collisions,
        const render_window_type& window,
        const camera_type& camera);

    ~inspection_handler() override = default;

    auto collisions() const -> const auto& { return m_collisions; }
    auto window() const -> const auto& { return m_win; }
    auto camera() const -> const auto& { return m_cam; }

    auto mousePressed(const OgreBites::MouseButtonEvent& e) -> bool;

private:
    const graph_collision_checker_type& m_collisions;
    const render_window_type& m_win;
    const camera_type& m_cam;
};

} // namespace input

#endif // INPUT_INSPECTION_HANDLER_HPP
