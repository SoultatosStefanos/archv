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
    using point_type = typename boost::convex_topology< 3 >::point_type;
    using data_type = boost::cube_topology< std::minstd_rand >;

    explicit cube(scale_type scale);
    virtual ~cube() override = default;

    // Raw topology data access.
    auto data() const -> const data_type& { return m_cube; }
    auto data() -> data_type& { return m_cube; }

    virtual auto desc() const -> descriptor override;
    virtual auto scale() const -> scale_type override { return m_scale; }

    virtual auto accept(const visitor_type& visitor) const -> void override;

    virtual auto clone() const -> std::unique_ptr< topology > override;

private:
    data_type m_cube;
    scale_type m_scale;
};

} // namespace layout

#endif // LAYOUT_CUBE_HPP
