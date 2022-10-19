// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef SCALING_ALLFWD_HPP
#define SCALING_ALLFWD_HPP

namespace scaling
{

class backend;
struct factor;
class factor_repo;
struct scale_vector;
struct config_data;

template < typename Graph, typename FactorCounter >
struct scale_map;

} // namespace scaling

#endif // SCALING_ALLFWD_HPP
