// Distributes used types from the boost graph module fwd declared.
// Soultatos Stefanos 2022

#ifndef MISC_BOOST_GRAPHFWD_HPP
#define MISC_BOOST_GRAPHFWD_HPP

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

#endif // MISC_BOOST_GRAPHFWD_HPP
