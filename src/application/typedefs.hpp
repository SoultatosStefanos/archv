// Contains some convenience application level typedefs.
// Soultatos Stefanos 2022

#ifndef APPLICATION_TYPEDEFS_HPP
#define APPLICATION_TYPEDEFS_HPP

#include "architecture/graph.hpp"
#include "architecture/symbol_table.hpp"
#include "architecture/symbols.hpp"
#include "dependencies/backend.hpp"
#include "dependencies/weight_map.hpp"
#include "layout/backend.hpp"
#include "layout/position_map.hpp"
#include "rendering/graph_renderer.hpp"
#include "scaling/scale_map.hpp"

namespace application
{

using weight_map = dependencies::
    weight_map< architecture::graph, architecture::dependency_map >;

using position_map = layout::position_map< architecture::graph, weight_map >;

using layout_backend = layout::backend< architecture::graph, weight_map >;

// TODO Tidy, put somewhere else
struct scaling_factors_counter
{
    using vertex_type = architecture::graph::vertex_descriptor;
    using tag_type = scaling::backend::tag_type;

    const architecture::symbol_table& st;

    auto operator()(vertex_type v, tag_type tag) const -> unsigned
    {
        const auto& id = boost::get(architecture::id_map(), v);
        const auto* sym = st.lookup(id);
        assert(sym);

        if (tag == "Fields")
        {
            return sym->fields.size();
        }
        else if (tag == "Methods")
        {
            return sym->methods.size();
        }
        else if (tag == "Nested")
        {
            return sym->nested.size();
        }
        else
        {
            assert(false);
            return 0;
        }
    }
};

using scale_map
    = scaling::scale_map< architecture::graph, scaling_factors_counter >;

using graph_renderer = rendering::graph_renderer<
    architecture::graph,
    architecture::id_map,
    position_map,
    weight_map,
    scale_map >;

} // namespace application

#endif // APPLICATION_TYPEDEFS_HPP
