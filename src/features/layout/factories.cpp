#include "factories.hpp"

#include <boost/functional/hash.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <concepts>
#include <unordered_map>

namespace features::layout
{

namespace
{
    using pool = std::unordered_map<std::size_t, std::unique_ptr<layout>>;

    static pool layouts;

    inline auto enumerate(const dependencies::graph& g)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, reinterpret_cast<std::size_t>(&g));

        return seed;
    }

    inline auto enumerate(const dependencies::graph& g, const topology& s)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, enumerate(g));
        boost::hash_combine(seed, s.desc());
        boost::hash_combine(seed, s.scale());

        return seed;
    }

    // Pool accessor for any layout type.
    // Convenience method.
    template <typename Layout, typename... Args>
    requires std::derived_from<Layout, layout> &&
             std::constructible_from<Layout, Args...>
             inline auto access_pool(std::size_t index, Args&&... args)
                 -> layout*
    {
        if (!layouts.contains(index))
        {
            layouts[index] =
                std::make_unique<Layout>(std::forward<Args>(args)...);

            BOOST_LOG_TRIVIAL(debug)
                << "created new layout of type: " << layouts.at(index)->desc()
                << ", at index: " << index;
        }

        return layouts.at(index).get();
    }

    template <typename Layout>
    requires std::derived_from<Layout, layout>
    inline auto get(const dependencies::graph& g,
                    const topology& space,
                    dependencies::weight_map edge_weight) -> layout*
    {
        return access_pool<Layout>(enumerate(g, space), g, space, edge_weight);
    }

    template <typename Layout>
    requires std::derived_from<Layout, layout>
    inline auto get(const dependencies::graph& g, const topology& space)
        -> layout*
    {
        return access_pool<Layout>(enumerate(g, space), g, space);
    }

    template <typename Layout>
    requires std::derived_from<Layout, layout>
    inline auto get(const dependencies::graph& g) -> layout*
    {
        return access_pool<Layout>(enumerate(g), g);
    }

} // namespace

auto layout_factory::make_layout(const descriptor& desc,
                                 const graph& g,
                                 const topology& space,
                                 weight_map edge_weight) -> pointer
{
    if (desc == gursoy_atun_desc)
    {
        return get<gursoy_atun_layout>(g, space, edge_weight);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid layout description: " << desc;
        assert(false);
        return nullptr;
    }
}

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    if (desc == cube_desc)
    {
        return std::make_unique<cube>(scale);
    }
    else if (desc == sphere_desc)
    {
        return std::make_unique<sphere>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

} // namespace features::layout