// Contains a private header for the Maximal Clique Enumeration clustering
// implementation.
// Soultatos Stefanos 2022

#ifndef CLUSTERING_DETAIL_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
#define CLUSTERING_DETAIL_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP

#include <numeric> // for numeric_limits

namespace clustering::detail
{

// NOTE: Use of custom output iterator to comply with boost vistor.clique()
// api.
template < typename CliqueMap >
struct clique_size_record_iterator
{
    explicit clique_size_record_iterator(CliqueMap& map) : cliques { map } { }

    template < typename Clique, typename Graph >
    inline auto clique(const Clique& p, const Graph&) -> void
    {
        cliques.insert({ p.size(), p });
    }

private:
    CliqueMap& cliques;
};

// Factory for type deduction.
template < typename CliqueMap >
inline auto clique_size_recorder(CliqueMap& map)
{
    return clique_size_record_iterator< CliqueMap > { map };
}

template < typename Cluster >
inline auto shared_cluster() -> Cluster
{
    return std::numeric_limits< Cluster >::max();
}

template < typename Cluster >
inline auto advance_shared_cluster(Cluster cluster) -> Cluster
{
    return cluster - 1;
}

} // namespace clustering::detail

#endif // CLUSTERING_DETAIL_MAXIMAL_CLIQUE_ENUMERATION_CLUSTERING_HPP
