#include "effect_evaluation.hpp"

#include <cassert>

namespace degrees
{

auto evaluate(degree_t degree, const effect_evaluation_data& data) -> effect_t
{
    assert(degree >= 0);
    assert(data.thresholds.light >= 0);
    assert(data.thresholds.medium > data.thresholds.light);
    assert(data.thresholds.heavy > data.thresholds.medium);

    if (degree < data.thresholds.light)
        return null_effect;
    else if (degree >= data.thresholds.light && degree < data.thresholds.medium)
        return data.effects.light;
    else if (degree >= data.thresholds.medium && degree < data.thresholds.heavy)
        return data.effects.medium;
    else
        return data.effects.heavy;
}

} // namespace degrees