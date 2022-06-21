// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#include "graph_generation.hpp"

#include <cassert>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <unordered_map>

namespace Architecture
{

constexpr auto unknown_access{"unknown"};

namespace // rapidjson utils
{
    // Safe json access.
    inline auto get(const Json::Value& val, const char* at)
    {
        assert(at);
        assert(val.isMember(at));
        return val[at];
    }

    // Safe json val string conversion.
    inline auto as_string(const Json::Value& val)
    {
        assert(val.isString());
        return val.asString();
    }

    // ID based json object traversal.
    template <typename BinaryOperation>
    requires std::invocable<BinaryOperation,
                            std::decay_t<Json::String>,
                            std::decay_t<Json::Value>>
    void visit(const Json::Value& val, BinaryOperation visitor)
    {
        assert(!val.isArray());
        for (auto iter = std::begin(val); iter != std::end(val); ++iter)
            visitor(iter.name(), *iter);
    }

} // namespace

namespace // deserializers  // TODO
{
    auto deserialize_structure(const Json::String& id, const Json::Value& val)
        -> Structure;

    inline auto deserialize_source_location(const Json::Value& val)
    {
        return SourceLocation{get(val, "file").asString(),
                              get(val, "line").asInt(),
                              get(val, "col").asInt()};
    }

    inline auto deserialize_nested(const Json::Value& val)
    {
        Structure::Nested nested;
        visit(val, [&nested](const auto& id, const auto& val) {
            nested.push_back(deserialize_structure(id, val));
        });

        return nested;
    }

    inline auto deserialize_bases(const Json::Value& val)
    {
        assert(val.isArray());

        Structure::Bases bases;
        std::transform(std::begin(val),
                       std::end(val),
                       std::back_inserter(bases),
                       [](const auto& val) { return as_string(val); });

        return bases;
    }

    auto deserialize_fields(const Json::Value& val) -> Structure::Fields {}

    auto deserialize_friends(const Json::Value& val) -> Structure::Friends {}

    auto deserialize_methods(const Json::Value& val) -> Structure::Methods {}

    auto deserialize_template_args(const Json::Value& val)
        -> Structure::TemplateArguments
    {}

    auto deserialize_structure(const Json::String& id, const Json::Value& val)
        -> Structure
    {
        auto name = as_string(get(val, "name"));
        auto name_space = as_string(get(val, "namespace"));
        auto type = as_string(get(val, "structure_type"));
        auto src_loc = deserialize_source_location(get(val, "src_info"));
        auto nested = deserialize_nested(get(val, "contains"));
        auto bases = deserialize_bases(get(val, "bases"));
        auto fields = deserialize_fields(get(val, "fields"));
        auto friends = deserialize_friends(get(val, "friends"));
        auto methods = deserialize_methods(get(val, "methods"));
        auto template_args =
            deserialize_template_args(get(val, "template_args"));

        return Structure{id,
                         std::move(name),
                         std::move(name_space),
                         std::move(src_loc),
                         unknown_access,
                         std::move(type),
                         std::move(methods),
                         std::move(fields),
                         std::move(bases),
                         std::move(nested),
                         std::move(friends),
                         std::move(template_args)};
    }

} // namespace

namespace // graph builders
{
    using Vertex = Graph::vertex_descriptor;
    using VertexCache = std::unordered_map<Structure::ID, Vertex>;

    inline void add_vertex(Structure&& s, Graph& g, VertexCache& cache)
    {
        assert(!cache.contains(s.id));
        cache[s.id] = boost::add_vertex(std::move(s), g);
    }

    inline void add_structure(const Json::String& id,
                              const Json::Value& val,
                              Graph& g,
                              VertexCache& cache)
    {
        auto&& structure = deserialize_structure(id, val);
        add_vertex(std::move(structure), g, cache);
    }

    inline void
    add_vertices(const Json::Value& val, Graph& g, VertexCache& cache)
    {
        visit(val, [&g, &cache](const auto& id, const auto& val) {
            add_structure(id, val, g, cache);
        });
    }

    inline void // TODO
    add_edges(const Json::Value& val, Graph& g, const VertexCache& cache)
    {
        assert(val.isArray());
        for (const auto& v : val)
        {
            const auto from = as_string(get(v, "from"));
            const auto to = as_string(get(v, "to"));
            // TODO cardinality

            boost::add_edge(cache.at(from), cache.at(to), g);
        }
    }

} // namespace

void generate_graph(Graph& g, const std::string_view json_fpath) // TODO
{
    assert(std::filesystem::exists(json_fpath));
    assert(boost::num_edges(g) == 0);
    assert(boost::num_vertices(g) == 0);

    std::ifstream from{json_fpath.data()};
    assert(from.good());

    Json::Value root;
    from >> root;

    VertexCache cache;
    add_vertices(get(root, "structures"), g, cache);
    add_edges(get(root, "dependencies"), g, cache);
}

} // namespace Architecture
