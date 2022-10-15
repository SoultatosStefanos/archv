// Contains a module for picking an effect (particle system, etc), based on
// given data and a vertex degree.
// Soultatos Stefanos 2022

#ifndef DEGREES_EFFECT_EVALUATION_HPP
#define DEGREES_EFFECT_EVALUATION_HPP

#include <optional>
#include <string>

namespace degrees
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

struct effect_evaluation_data
{
    using threshold_type = int;
    using effect_type = std::string;
    using thresholds_type = ranked< threshold_type >;
    using effects_type = ranked< effect_type >;

    thresholds_type thresholds;
    effects_type effects;

    auto operator==(const effect_evaluation_data&) const -> bool = default;
    auto operator!=(const effect_evaluation_data&) const -> bool = default;
};

/***********************************************************
 * Degree evaluator                                        *
 ***********************************************************/

using degree_t = int;
using effect_t = std::optional< effect_evaluation_data::effect_type >;

constexpr auto null_effect = std::nullopt;

// Effect evaluation strategy, given a degree.
auto evaluate(degree_t degree, const effect_evaluation_data& data) -> effect_t;

} // namespace degrees

#endif // DEGREES_EFFECT_EVALUATION_HPP
