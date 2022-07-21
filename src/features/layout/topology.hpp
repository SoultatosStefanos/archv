// Containes an architecture graph 3D topology interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>
#include <random>
#include <variant>

// NOTE: The bgl topology abstractions are not meant to be used polymorphically.
// Plus, we need to access the scale of each topolgy.
// (No virtual destructors, some functions are not declared at the bases, etc).
// This file provides a scalable inheritance chain with which the bgl layout
// algorithms can interface implicitly.

namespace features::layout
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

// In 3D space.
// Slight boost adaptor for runtime polymorphism.
class topology
{
public:
    using point_type = typename boost::convex_topology<3>::point_type;
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

    virtual auto clone() const -> std::unique_ptr<topology> = 0;
};

/***********************************************************
 * Cube Topology                                           *
 ***********************************************************/

class cube : public topology
{
public:
    using data_type = boost::cube_topology<std::minstd_rand>;

    static constexpr auto description = "Cube";

    explicit cube(scale_type scale) : m_cube{scale}, m_scale{scale} {}
    virtual ~cube() override = default;

    // Raw topology data access.
    auto data() const -> const data_type& { return m_cube; }
    auto data() -> data_type& { return m_cube; }

    virtual auto desc() const -> descriptor override { return description; }

    virtual auto scale() const -> scale_type override { return m_scale; }

    virtual void accept(const topology_visitor& visitor) const override
    {
        visitor.visit(*this);
    }

    virtual auto clone() const -> std::unique_ptr<topology> override
    {
        return std::make_unique<cube>(*this);
    }

private:
    data_type m_cube;
    scale_type m_scale;
};

/***********************************************************
 * Sphere Topology                                         *
 ***********************************************************/

class sphere : public topology
{
public:
    using data_type = boost::sphere_topology<std::minstd_rand>;

    static constexpr auto description = "Sphere";

    explicit sphere(scale_type scale) : m_sphere{scale}, m_scale{scale} {}
    virtual ~sphere() override = default;

    // Raw topology data access.
    auto data() const -> const data_type& { return m_sphere; }
    auto data() -> data_type& { return m_sphere; }

    virtual auto desc() const -> descriptor override { return description; }

    virtual auto scale() const -> scale_type override { return m_scale; }

    virtual void accept(const topology_visitor& visitor) const override
    {
        visitor.visit(*this);
    }

    virtual auto clone() const -> std::unique_ptr<topology> override
    {
        return std::make_unique<sphere>(*this);
    }

private:
    data_type m_sphere;
    scale_type m_scale;
};

} // namespace features::layout

#endif // LAYOUT_TOPOLOGY_HPP
