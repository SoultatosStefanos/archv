#include "layout.hpp"

#include <boost/graph/gursoy_atun_layout.hpp>
#include <boost/graph/topology.hpp>
#include <concepts>
#include <memory>
#include <random>
#include <unordered_map>

namespace Visualization
{

// ---------------------- GursoyAtunLayout ---------------------------------- //

namespace
{
    template <typename Topology>
    using PositionMap =
        std::unordered_map<Architecture::Vertex, typename Topology::point_type>;

    // Assigns a position, at a 3d space, to each graph vertex.
    // Distributes vertices uniformly within a topology, keeping vertices close
    // to their neighbours.
    //
    // Implementation details: Uses Gursoy-Atun Layout algorithm.
    // see
    // https://www.boost.org/doc/libs/1_79_0/libs/graph/doc/gursoy_atun_layout.html
    //
    // Convenience boost adaptor for our architecture, in 3D space.
    template <typename Topology>
    inline auto make_layout(const Architecture::Graph& g, const Topology& space)
        -> PositionMap<Topology>
    {
        PositionMap<Topology> layout;
        boost::gursoy_atun_layout(
            g,
            space,
            boost::make_assoc_property_map(layout),
            boost::weight_map(
                boost::get(&Architecture::Dependency::cardinality, g)));

        assert(std::all_of(boost::vertices(g).first,
                           boost::vertices(g).second,
                           [&layout](auto v) { return layout.contains(v); }));
        return layout;
    }

    template <typename Topology>
    concept Convex3DTopology =
        std::is_base_of_v<boost::convex_topology<3>, Topology>;

    template <Convex3DTopology Topology>
    class GursoyAtunLayout : public Layout
    {
    public:
        using PositionMap = PositionMap<Topology>;

        explicit GursoyAtunLayout(const Graph& g)
            : m_g{g}, m_map{make_layout(g, m_space)}
        {}

        virtual ~GursoyAtunLayout() override = default;

        virtual auto x(Vertex v) const -> double override
        {
            return m_map.at(v)[0];
        }

        virtual auto y(Vertex v) const -> double override
        {
            return m_map.at(v)[1];
        }

        virtual auto z(Vertex v) const -> double override
        {
            return m_map.at(v)[2];
        }

        virtual auto clone() const -> UniquePtr override
        {
            return std::make_unique<decltype(*this)>(*this);
        }

    private:
        Topology m_space;
        PositionMap m_map;
        const Graph& m_g;
    };

} // namespace

// -------------------------------------------------------------------------- //

// ------------------------- Topologies ------------------------------------- //

namespace
{
    using Cube = boost::cube_topology<std::minstd_rand>;
    using Sphere = boost::sphere_topology<std::minstd_rand>;

} // namespace

// -------------------------------------------------------------------------- //

auto LayoutFactory::make_layout(const Graph& g, Topology topology) -> UniquePtr
{
    using enum Topology;

    switch (topology)
    {
    case cube:
        return std::make_unique<GursoyAtunLayout<Cube>>(g);
    case sphere:
        return std::make_unique<GursoyAtunLayout<Sphere>>(g);
    default:
        assert(false);
        return nullptr;
    }
}

} // namespace Visualization