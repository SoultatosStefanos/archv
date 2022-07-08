// Contains the view of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include <OgreSceneManager.h>
#include <functional>
#include <string>
#include <vector>

namespace visualization::layout
{

class layout_view
{
public:
    using on_layout_input = std::function<void(const std::string&)>;
    using on_topology_input = std::function<void(const std::string&, double)>;

    explicit layout_view(const Ogre::SceneManager& scene) : m_scene{scene} {}

    void draw_vertex(const std::string& id, double x, double y, double z) const;

    void add_layout_input_listener(on_layout_input f);
    void add_topology_input_listener(on_topology_input f);

protected:
    void notify_layout_input(const std::string& type) const;
    void notify_topology_input(const std::string& type, double scale) const;

private:
    using layout_input_listeners = std::vector<on_layout_input>;
    using topology_input_listeners = std::vector<on_topology_input>;

    const Ogre::SceneManager& m_scene;

    layout_input_listeners m_layout_listeners;
    topology_input_listeners m_topology_listeners;
};

} // namespace visualization::layout

#endif // LAYOUT_VIEW_HPP
