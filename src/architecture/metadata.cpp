#include "metadata.hpp"

#include "graph.hpp"
#include "symbol_table.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace architecture
{

static_assert(
    std::is_same_v< metadata_counter::vertex_type, graph::vertex_descriptor >);

auto metadata_counter::operator()(vertex_type v, tag_type tag) const
    -> count_type
{
    const auto& id = boost::get(boost::get(boost::vertex_bundle, g), v);
    const auto* sym = st.lookup(id);

    assert(sym);

    if (tag == fields_tag)
    {
        return sym->fields.size();
    }
    else if (tag == methods_tag)
    {
        return sym->methods.size();
    }
    else if (tag == nested_tag)
    {
        return sym->nested.size();
    }
    else
    {
        BOOST_LOG_TRIVIAL(warning) << "unknown attribute: " << tag;
        return 0;
    }
}

} // namespace architecture