// Contains the entities to drive the dynamic weight management system.
// Soultatos Stefanos 2022

#ifndef WEIGHT_MANAGEMENT_HPP
#define WEIGHT_MANAGEMENT_HPP

#include "dependencies/all.hpp"

namespace weight
{

class weight_repo
{
public:
    using dependency_type = dependencies::dependency_type;
    using weight = dependencies::weight;

    auto get_weight(const dependency_type& type) const -> weight;
    void set_weight(const dependency_type& type, weight w);
};

auto make_weight_map(const weight_repo& repo) -> dependencies::weight_map;

} // namespace weight

#endif // WEIGHT_MANAGEMENT_HPP
