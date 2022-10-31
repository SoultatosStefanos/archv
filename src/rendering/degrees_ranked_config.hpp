// Contains a configuration module for the default degrees evaluation backend.
// Soultatos Stefanos 2022

#ifndef RENDERING_DEGREES_RANKED_CONFIG_HPP
#define RENDERING_DEGREES_RANKED_CONFIG_HPP

#include "degrees_ranked_evaluation.hpp"

namespace rendering
{

struct degrees_ranked_config
{
    degree_evaluation_data in_data;
    degree_evaluation_data out_data;

    auto operator==(const degrees_ranked_config&) const -> bool = default;
    auto operator!=(const degrees_ranked_config&) const -> bool = default;
};

} // namespace rendering

#endif // RENDERING_DEGREES_RANKED_CONFIG_HPP
