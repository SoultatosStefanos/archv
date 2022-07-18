// Contains the view of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include <OgreSceneManager.h>
#include <boost/signals2/signal.hpp>
#include <memory>
#include <string>
#include <string_view>

namespace layout
{

class view
{
public:
    using vertex_id = std::string;
    using coord = double;
    using layout_selection = std::string;
    using topology_selection = std::string;
    using topology_scale_selection = double;

private:
    using layout_request =
        boost::signals2::signal<void(const layout_selection&)>;
    using topology_request = boost::signals2::signal<void(
        const topology_selection&, topology_scale_selection)>;

public:
    using layout_request_listener = layout_request::slot_type;
    using topology_request_listener = topology_request::slot_type;

    explicit view(const Ogre::SceneManager& scene);

    void draw_vertex(const vertex_id& id, coord x, coord y, coord z);
    // TODO draw edge

    void update_layout_selection(layout_selection l);
    void update_topology_selection(topology_selection space,
                                   topology_scale_selection scale);

    void on_layout_request(const layout_request_listener& f);
    void on_topology_request(const topology_request_listener& f);

    void send_layout_request(const layout_selection& l) const;
    void send_topology_request(const topology_selection& space,
                               topology_scale_selection scale) const;

private:
    layout_request m_layout_signal;
    topology_request m_topology_signal;

    const Ogre::SceneManager& m_scene;
};

} // namespace layout

#endif // LAYOUT_VIEW_HPP
