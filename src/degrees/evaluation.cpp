#include "evaluation.hpp"

#include <cassert>

namespace degrees
{

auto evaluate(degree_t degree, const evaluation_data& data) -> particle_system_t
{
    assert(degree >= 0);
    assert(data.thresholds.light >= 0);
    assert(data.thresholds.medium > data.thresholds.light);
    assert(data.thresholds.heavy > data.thresholds.medium);

    if (degree < data.thresholds.light or !data.applied)
        return null_particles;
    else if (degree >= data.thresholds.light && degree < data.thresholds.medium)
        return data.particles.light;
    else if (degree >= data.thresholds.medium && degree < data.thresholds.heavy)
        return data.particles.medium;
    else
        return data.particles.heavy;
}

} // namespace degrees