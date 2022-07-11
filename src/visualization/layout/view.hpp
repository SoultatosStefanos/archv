// Contains the view of the visualization/layout subsystem.
// Soultatos Stefanos 2022

#ifndef LAYOUT_VIEW_HPP
#define LAYOUT_VIEW_HPP

#include "events.hpp"
#include "visualization/communication/all.hpp"

#include <OgreSceneManager.h>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

namespace visualization::layout
{

class view
{
public:
    using vertex_id = std::string;

    struct vertex_pos
    {
        double x, y, z;
    };

    using layout_data = std::unordered_map<vertex_id, vertex_pos>;

    view() = default;
    view(const view&) = default;
    view(view&&) = default;
    virtual ~view() = default;

    auto operator=(const view&) -> view& = default;
    auto operator=(view&&) -> view& = default;

    // TODO Pass options

    virtual void initialize(const std::string& layout_type,
                            const std::string& topology_type,
                            double topology_scale) = 0;

    virtual void draw(const layout_data& data) = 0;

    virtual auto clone() const -> std::unique_ptr<view> = 0;
};

// TODO Fire requests from gui
class ogre_view : public view
{
public:
    using event_bus = communication::event_bus;

    ogre_view(event_bus& pipeline, const Ogre::SceneManager& scene);
    virtual ~ogre_view() override = default;

    // FIXME Set gui
    virtual void initialize(const std::string& layout_type,
                            const std::string& topology_type,
                            double topology_scale) override
    {}

    virtual void draw(const layout_data& data) override;

    virtual auto clone() const -> std::unique_ptr<view> override
    {
        return std::make_unique<ogre_view>(*this);
    }

protected:
    void draw_vertices(const layout_data& data);
    void draw_edges(const layout_data& data);

private:
    event_bus& m_pipeline;
    const Ogre::SceneManager& m_scene;
};

} // namespace visualization::layout

#endif // LAYOUT_VIEW_HPP
