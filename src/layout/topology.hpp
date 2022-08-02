// Containes an architecture graph 3D topology interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>

// NOTE: The bgl topology abstractions are not meant to be used polymorphically.
// Plus, we need to access the scale of each topolgy.
// (No virtual destructors, some functions are not declared at the bases, etc).
// This file provides a scalable inheritance chain with which the bgl layout
// algorithms can interface implicitly.

namespace layout
{

class topology_visitor;

// In 3D space.
// Slight boost adaptor for runtime polymorphism.
class topology
{
public:
    using point_type = typename boost::convex_topology< 3 >::point_type;
    using scale_type = double;
    using descriptor = std::string;

    topology() = default;
    topology(const topology&) = default;
    topology(topology&&) = default;
    virtual ~topology() = default;

    auto operator=(const topology&) -> topology& = default;
    auto operator=(topology&&) -> topology& = default;

    // For runtime type identification.
    virtual auto desc() const -> descriptor = 0;

    virtual auto scale() const -> scale_type = 0;

    virtual void accept(const topology_visitor&) const = 0;

    virtual auto clone() const -> std::unique_ptr< topology > = 0;
};

template < typename Topology >
struct topology_traits
{
    using point_type = typename Topology::point_type;
    using scale_type = typename Topology::scale_type;
    using descriptor = typename Topology::descriptor;

    static constexpr auto desc() -> auto { return Topology::description; }
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_HPP
