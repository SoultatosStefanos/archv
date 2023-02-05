// Contains a configuration module for the default degrees evaluation backend.
// Soultatos Stefanos 2022

#ifndef DEGREES_BACKEND_CONFIG_HPP
#define DEGREES_BACKEND_CONFIG_HPP

#include "evaluation.hpp" // for evaluation_data

namespace degrees
{

/***********************************************************
 * Backend Config                                          *
 ***********************************************************/

struct backend_config
{
    evaluation_data in_data;
    evaluation_data out_data;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

} // namespace degrees

#endif // DEGREES_BACKEND_CONFIG_HPP
