// Distributes all types from this directory fwd declared.
// Soultatos Stefanos 2022

#ifndef DEGREES_ALLFWD_HPP
#define DEGREES_ALLFWD_HPP

#include <optional> // for optional
#include <string>   // for string

namespace degrees
{

template < typename T >
struct ranked;

struct evaluation_data;

using degree_t = int;
using particle_system_t = std::optional< std::string >;

struct backend_config;

class evaluator;

using config_data = backend_config;

} // namespace degrees

#endif // DEGREES_ALLFWD_HPP
