// Contains a configuration module for the default degrees evaluation backend.
// Soultatos Stefanos 2022

#include "evaluation.hpp" // for evaluation_data

namespace degrees
{

struct backend_config
{
    evaluation_data in_data;
    evaluation_data out_data;

    auto operator==(const backend_config&) const -> bool = default;
    auto operator!=(const backend_config&) const -> bool = default;
};

} // namespace degrees
