// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#include "graph_generation.hpp"

#include <cassert>
#include <concepts>
#include <jsoncpp/json/json.h>

namespace Architecture
{

namespace
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);
        assert(val.isMember(at));
        return val[at];
    }

    // ID based json traversal.
    template <typename Function>
    requires std::invocable<Function,
                            std::decay_t<Json::String>,
                            std::decay_t<Json::Value>>
    void visit(const Json::Value& val, Function visitor)
    {
        for (auto iter = std::begin(val); iter != std::end(val); ++iter)
            visitor(iter.name(), *iter);
    }

} // namespace

namespace
{} // namespace

void generate_graph(Graph& g, JsonArchive& from) // TODO
{
    assert(from.good());
    assert(boost::num_edges(g) == 0);
    assert(boost::num_vertices(g) == 0);

    assert(from.good());
}

} // namespace Architecture
