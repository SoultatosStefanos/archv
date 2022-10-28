// Contains a 3D cube topology class.
// Soultatos Stefanos 2022

#ifndef LAYOUT_CUBE_HPP
#define LAYOUT_CUBE_HPP

#include "topology.hpp"

#include <random>

namespace layout
{

class cube : public topology
{
public:
    explicit cube(scale_type scale);
    virtual ~cube() override = default;

    auto random_point() const -> point_type override;

    auto distance(const point_type& p1, const point_type& p2) const
        -> distance_type override;

    auto move_position_toward(
        const point_type& p1,
        fraction_type fraction,
        const point_type& p2) const -> point_type override;

    auto id() const -> id_type override;
    auto scale() const -> scale_type override { return m_scale; }

    auto clone() const -> std::unique_ptr< topology > override;

protected:
    using data_type = boost::cube_topology< std::minstd_rand >;

    // Raw topology data access.
    auto data() const -> const data_type& { return m_cube; }
    auto data() -> data_type& { return m_cube; }

private:
    data_type m_cube;
    scale_type m_scale;
};

} // namespace layout

#endif // LAYOUT_CUBE_HPP
