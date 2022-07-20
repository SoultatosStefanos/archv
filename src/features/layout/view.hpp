// Contains the view of the layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include <OgreSceneManager.h>
#include <boost/log/trivial.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>
#include <string>

namespace layout
{

class view
{
    using layout_request = boost::signals2::signal<void(const std::string&)>;
    using space_request =
        boost::signals2::signal<void(const std::string&, double)>;

public:
    using layout_request_listener = layout_request::slot_type;
    using topology_request_listener = space_request::slot_type;

    explicit view(const Ogre::SceneManager& scene);

    void draw_vertex(const std::string& id, double x, double y, double z);
    // TODO draw edge

    void update_layout_selection(std::string l);
    void update_topology_selection(std::string space, double scale);

    void on_layout_request(const layout_request_listener& f)
    {
        m_layout_signal.connect(f);
    }

    void on_topology_request(const topology_request_listener& f)
    {
        m_topology_signal.connect(f);
    }

    void send_layout_request(const std::string& l) const
    {
        BOOST_LOG_TRIVIAL(info) << "layout selected with type: " << l;

        m_layout_signal(l);
    }

    void send_topology_request(const std::string& space, double scale) const
    {
        BOOST_LOG_TRIVIAL(info) << "topology selected with type: " << space
                                << " and scale: " << scale;

        m_topology_signal(space, scale);
    }

private:
    layout_request m_layout_signal;
    space_request m_topology_signal;

    const Ogre::SceneManager& m_scene;
};

} // namespace layout

#endif // LAYOUT_VIEW_HPP