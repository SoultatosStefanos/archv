// Contains some useful typedefs to drive the dynamic edge weight management
// subsystem.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_HPP
#define DEPENDENCIES_WEIGHT_HPP

#include "graph.hpp"

#include <boost/property_map/function_property_map.hpp>

namespace dependencies
{

using weight_type = int;

class weight_repo
{
public:
    auto get_weight(const dependency_type& type) const -> weight_type;
    void set_weight(const dependency_type& type, weight_type weight);
};

class weight_dispatcher
{
public:
    using vertex = graph::vertex_descriptor;

    explicit weight_dispatcher(const weight_repo& repo);

    auto operator()(vertex v) const -> weight_type;
};

using weight_map = boost::function_property_map<weight_dispatcher,
                                                graph::vertex_descriptor,
                                                weight_type>;

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_HPP
