// Contains adaptor callables, that provide access to graph metadata.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_METADATA_HPP
#define ARCHITECTURE_METADATA_HPP

#include <string_view> // for string_view

namespace boost
{

struct vecS;
struct bidirectionalS;
struct listS;
struct no_property;

template <
    typename OutEdgeListS,
    typename VertexListS,
    typename DirectedS,
    typename VertexProperty,
    typename EdgeProperty,
    typename GraphProperty,
    typename EdgeListS >
class adjacency_list;

} // namespace boost

namespace architecture
{

class symbol_table;

using graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    std::string,
    std::string,
    boost::no_property,
    boost::listS >;

/***********************************************************
 * Metadata Counter                                        *
 ***********************************************************/

// Count graph symbols attributes by tags, e.g: "fields", "methods", etc.
struct metadata_counter
{
    using vertex_type = std::size_t;
    using tag_type = std::string_view;
    using count_type = unsigned;

    static constexpr auto fields_tag { "Fields" };
    static constexpr auto methods_tag { "Methods" };
    static constexpr auto nested_tag { "Nested" };

    const symbol_table& st;
    const graph& g;

    // NOTE: Returns zero if tag is unknown.
    auto operator()(vertex_type v, tag_type tag) const -> count_type;
};

} // namespace architecture

#endif // ARCHITECTURE_METADATA_HPP
