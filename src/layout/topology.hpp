// Containes an architecture graph 3D topology interface.
// Soultatos Stefanos 2022

#ifndef LAYOUT_TOPOLOGY_HPP
#define LAYOUT_TOPOLOGY_HPP

#include <boost/graph/topology.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>
#include <random>
#include <variant>

namespace layout
{

// In 3D space.
// Slight boost wrapper for runtime polymorphism.
class topology
{
public:
    // NOTE: Must enumerate all spaces here with std::variant since the boost
    // topologies are not meant to be used polymorphically.
    using point = typename boost::convex_topology<3>::point_type;
    using cube_variant = boost::cube_topology<std::minstd_rand>;
    using sphere_variant = boost::sphere_topology<std::minstd_rand>;
    using space_type = std::variant<cube_variant, sphere_variant>;
    using scale_type = double;

    topology() = default;
    topology(const topology&) = default;
    topology(topology&&) = default;
    virtual ~topology() = default;

    auto operator=(const topology&) -> topology& = default;
    auto operator=(topology&&) -> topology& = default;

    virtual auto space() const -> const space_type& = 0;
    virtual auto scale() const -> scale_type = 0;

    virtual auto clone() const -> std::unique_ptr<topology> = 0;
};

class cube : public topology
{
public:
    explicit cube(scale_type scale)
        : m_space{cube_variant(scale)}, m_scale{scale}
    {}
    virtual ~cube() override = default;

    virtual auto space() const -> const space_type& override { return m_space; }
    virtual auto scale() const -> scale_type override { return m_scale; }

    virtual auto clone() const -> std::unique_ptr<topology> override
    {
        return std::make_unique<cube>(*this);
    }

private:
    space_type m_space;
    scale_type m_scale;
};

class sphere : public topology
{
public:
    explicit sphere(scale_type scale)
        : m_space{sphere_variant(scale)}, m_scale{scale}
    {}
    virtual ~sphere() override = default;

    virtual auto space() const -> const space_type& override { return m_space; }
    virtual auto scale() const -> scale_type override { return m_scale; }

    virtual auto clone() const -> std::unique_ptr<topology> override
    {
        return std::make_unique<sphere>(*this);
    }

private:
    space_type m_space;
    scale_type m_scale;
};

} // namespace layout

#endif // LAYOUT_TOPOLOGY_HPP
