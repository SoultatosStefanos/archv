// Contains a module for picking an effect (particle system, etc), based on
// given data and a vertex degree.
// Soultatos Stefanos 2022

#ifndef RENDERING_DEGREES_RANKED_EVALUATION_HPP
#define RENDERING_DEGREES_RANKED_EVALUATION_HPP

#include <optional>
#include <string>

namespace rendering
{

/***********************************************************
 * Ranked                                                  *
 ***********************************************************/

template < typename T >
struct ranked
{
    T light;
    T medium;
    T heavy;

    auto operator==(const ranked&) const -> bool = default;
    auto operator!=(const ranked&) const -> bool = default;
};

template < typename T >
constexpr auto make_ranked(T light, T medium, T heavy)
{
    return ranked< T >(std::move(light), std::move(medium), std::move(heavy));
}

/***********************************************************
 * Evaluation data                                         *
 ***********************************************************/

struct degree_evaluation_data
{
    using threshold_type = int;
    using particle_system_type = std::string;
    using thresholds_type = ranked< threshold_type >;
    using particle_systems_type = ranked< particle_system_type >;
    using applied_type = bool;

    thresholds_type thresholds;
    particle_systems_type particles;
    applied_type applied;

    auto operator==(const degree_evaluation_data&) const -> bool = default;
    auto operator!=(const degree_evaluation_data&) const -> bool = default;
};

/***********************************************************
 * Degree evaluation                                       *
 ***********************************************************/

using degree_t = int;
using particle_system_t
    = std::optional< degree_evaluation_data::particle_system_type >;

constexpr auto null_particles = std::nullopt;

// Effect evaluation strategy, given a degree.
auto evaluate(degree_t degree, const degree_evaluation_data& data)
    -> particle_system_t;

} // namespace rendering

#endif // RENDERING_DEGREES_RANKED_EVALUATION_HPP
