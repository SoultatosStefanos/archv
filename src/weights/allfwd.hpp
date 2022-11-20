// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_ALLFWD_HPP
#define WEIGHTS_ALLFWD_HPP

namespace weights
{

class backend;
class weight_repo;

namespace detail
{
    template < typename Graph, typename DependencyMap >
    class weight_dispatcher;
} // namespace detail

template < typename Graph, typename DependencyMap >
class weight_map;

} // namespace weights

#endif // WEIGHTS_ALLFWD_HPP
