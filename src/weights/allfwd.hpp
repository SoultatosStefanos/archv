// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_ALLFWD_HPP
#define WEIGHTS_ALLFWD_HPP

namespace weights
{

class backend;
class weight_repo;

template < typename Graph, typename DependencyMap >
class weight_map;

struct config_data;

template < typename View, typename Backend >
class presenter;

} // namespace weights

#endif // WEIGHTS_ALLFWD_HPP
