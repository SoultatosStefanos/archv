// Containes an architecture graph 3D topology interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>       // for topology
#include <boost/smart_ptr/shared_ptr.hpp> // for shared_ptr
#include <memory>                         // for unique_ptr
#include <string_view>                    // for string_view

// NOTE: The bgl topology abstractions are not meant to be used polymorphically.
// Plus, we need to access the scale of each topolgy.
// (No virtual destructors, some functions are not declared at the bases, etc).
// This file provides a scalable inheritance chain with which the bgl layout
// algorithms can interface implicitly.

namespace layout
{

// In 3D space.
// Slight boost adaptor for runtime polymorphism.
class topology
{
public:
    using id_type = std::string_view;
    using point_type = typename boost::convex_topology< 3 >::point_type;
    using distance_type = double;
    using fraction_type = double;
    using scale_type = double;

    topology() = default;
    topology(const topology&) = default;
    topology(topology&&) = default;
    virtual ~topology() = default;

    auto operator=(const topology&) -> topology& = default;
    auto operator=(topology&&) -> topology& = default;

    virtual auto id() const -> id_type = 0;
    virtual auto scale() const -> scale_type = 0;

    virtual auto random_point() const -> point_type = 0;

    virtual auto distance(const point_type& p1, const point_type& p2) const
        -> distance_type
        = 0;

    virtual auto move_position_toward(
        const point_type& p1,
        fraction_type fraction,
        const point_type& p2) const -> point_type
        = 0;

    virtual auto clone() const -> std::unique_ptr< topology > = 0;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_HPP
