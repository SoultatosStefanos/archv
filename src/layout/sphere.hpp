#ifndef LAYOUT_SPHERE_HPP
#define LAYOUT_SPHERE_HPP

#include "topology.hpp"

#include <random>

namespace layout
{

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

    virtual void accept(const topology_visitor& visitor) const override;

    virtual auto clone() const -> std::unique_ptr<topology> override;

private:
    data_type m_sphere;
    scale_type m_scale;
};

} // namespace layout

#endif // LAYOUT_SPHERE_HPP
