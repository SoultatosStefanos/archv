// Contains a private module for the presenter header.
// Soultatos Stefanos 2022

#ifndef SCALING_DETAIL_PRESENTER_HPP
#define SCALING_DETAIL_PRESENTER_HPP

#include "scaling/backend.hpp"

// NOTE: this is used to resolve function names

namespace scaling::detail
{

using tag_type = backend::tag_type;
using dims_type = backend::dims_type;
using baseline_type = backend::baseline_type;
using enabled_type = backend::enabled_type;
using ratio_type = backend::ratio_type;

inline auto update_factor_baseline(backend& b, tag_type tag, baseline_type base)
{
    scaling::update_factor_baseline(b, tag, base);
}

inline auto update_factor_dims(backend& b, tag_type tag, dims_type d)
{
    scaling::update_factor_dims(b, tag, d);
}

inline auto update_factor_enabled(backend& b, tag_type tag, enabled_type e)
{
    scaling::update_factor_enablement(b, tag, e);
}

inline auto update_factor_min_ratio(backend& b, tag_type tag, ratio_type r)
{
    scaling::update_factor_min_ratio(b, tag, r);
}

inline auto update_factor_max_ratio(backend& b, tag_type tag, ratio_type r)
{
    scaling::update_factor_max_ratio(b, tag, r);
}

} // namespace scaling::detail

#endif // SCALING_DETAIL_PRESENTER_HPP
