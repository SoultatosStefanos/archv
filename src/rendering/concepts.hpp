// Contains some rendering concepts.
// Soultatos Stefanos 2022

#ifndef RENDERING_CONCEPTS_HPP
#define RENDERING_CONCEPTS_HPP

#include <concepts>
#include <functional>

namespace rendering
{

/***********************************************************
 * Concepts                                                *
 ***********************************************************/

// clang-format off
template < typename Class >
concept degrees_evaluator 
= requires
{
    typename Class::degree_type;
    typename Class::particles_type;
} && requires(Class val, typename Class::degree_type degrees)
{
    { val.in_degree_particles(degrees) } 
        -> std::same_as< typename Class::particles_type >;
    { val.out_degree_particles(degrees) }  
        -> std::same_as< typename Class::particles_type >;
};
// clang-format on

// clang-format off
template < typename Class >
concept cluster_color_coder 
= requires
{
    typename Class::cluster;
    typename Class::rgba_type;
} && requires(Class val,
    typename Class::cluster c, 
    typename Class::rgba_type rgba)
{
    { std::invoke(val, c) } -> std::same_as< decltype(rgba) >;
};
// clang-format on

} // namespace rendering

#endif // RENDERING_CONCEPTS_HPP
