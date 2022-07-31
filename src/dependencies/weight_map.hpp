// Contains a function for creating a managed architecture graph weight map.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_MAP_HPP
#define DEPENDENCIES_WEIGHT_MAP_HPP

#include "architecture/graph.hpp"
#include "weight_repo.hpp"

namespace dependencies
{

// Creates a runtime managed edge-weight property map from a weight repository.
inline auto make_dynamic_weight_map(const weight_repo& repo,
                                    architecture::edge_type_map edge_type)
{
    return architecture::weight_map([&repo, edge_type](auto edge) {
        const auto& dependency = boost::get(edge_type, edge);
        return repo.get_weight(dependency);
    });
}

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_MAP_HPP
