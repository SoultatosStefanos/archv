// Containes a 3D topology visitor interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_VISITOR_HPP
#define LAYOUT_TOPOLOGY_VISITOR_HPP

namespace layout
{

class cube;
class sphere;

class topology_visitor
{
public:
    topology_visitor() = default;
    topology_visitor(const topology_visitor&) = default;
    topology_visitor(topology_visitor&&) = default;
    virtual ~topology_visitor() = default;

    auto operator=(const topology_visitor&) -> topology_visitor& = default;
    auto operator=(topology_visitor&&) -> topology_visitor& = default;

    virtual void visit(const cube& c) const = 0;
    virtual void visit(const sphere& s) const = 0;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_VISITOR_HPP
