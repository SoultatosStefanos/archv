// Contains a 3D sphere topology class.
// Soultatos Stefanos 2022

#ifndef LAYOUT_SPHERE_HPP
#define LAYOUT_SPHERE_HPP

#include "topology.hpp"

#include <random>

namespace layout
{

class sphere : public topology
{
public:
    using data_type = boost::sphere_topology< std::minstd_rand >;

    explicit sphere(scale_type scale);
    virtual ~sphere() override = default;

    auto data() const -> const data_type& { return m_sphere; }
    auto data() -> data_type& { return m_sphere; }

    auto id() const -> id_type override;
    auto scale() const -> scale_type override { return m_scale; }

    auto random_point() const -> point_type override;

    auto distance(const point_type& p1, const point_type& p2) const
        -> distance_type override;

    auto move_position_toward(
        const point_type& p1,
        fraction_type fraction,
        const point_type& p2) const -> point_type override;

    auto accept(const visitor_type& visitor) const -> void override;

    auto clone() const -> std::unique_ptr< topology > override;

private:
    data_type m_sphere;
    scale_type m_scale;
};

} // namespace layout

#endif // LAYOUT_SPHERE_HPP
