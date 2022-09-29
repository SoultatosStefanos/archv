#include "backend.hpp"

#include <ranges>

namespace scaling
{

namespace
{
    inline auto verify_baseline(backend::baseline_type baseline)
    {
        if (baseline <= 0)
            BOOST_THROW_EXCEPTION(
                invalid_baseline() << baseline_info(baseline));
    }

    inline auto verify_ratio(backend::ratio_type ratio)
    {
        if (ratio < 0)
            BOOST_THROW_EXCEPTION(invalid_ratio() << ratio_info(ratio));
    }

} // namespace

backend::backend(config_data_type cfg)
: m_cfg { cfg }, m_repo { std::move(cfg) }
{
    using std::ranges::views::values;

    for (const auto& factor : values(get_factor_repo()))
    {
        verify_baseline(factor.baseline);
        verify_ratio(factor.min_ratio);
        verify_ratio(factor.max_ratio);
    }
}

auto backend::update_factor(
    tag_type tag,
    dims_type dims,
    baseline_type baseline,
    bool enabled,
    ratio_type min_ratio,
    ratio_type max_ratio) -> void
{
    verify_baseline(baseline);
    verify_ratio(min_ratio);
    verify_ratio(max_ratio);

    auto& f = m_repo.get_factor(tag);
    f.applied_dims = std::move(dims);
    f.baseline = baseline;
    f.enabled = enabled;
    f.min_ratio = min_ratio;
    f.max_ratio = max_ratio;

    emit_factor(f);
}

auto backend::connect(const factor_slot& f) -> connection
{
    return m_signal.connect(f);
}

auto backend::emit_factor(const factor& f) const -> void
{
    m_signal(f);
}

auto is_factor_applied_on_x(const backend& b, backend::tag_type tag) -> bool
{
    return b.get_factor_repo().get_factor(tag).applied_dims[0];
}

auto is_factor_applied_on_y(const backend& b, backend::tag_type tag) -> bool
{
    return b.get_factor_repo().get_factor(tag).applied_dims[1];
}

auto is_factor_applied_on_z(const backend& b, backend::tag_type tag) -> bool
{
    return b.get_factor_repo().get_factor(tag).applied_dims[2];
}

auto is_factor_enabled(const backend& b, backend::tag_type tag) -> bool
{
    return b.get_factor_repo().get_factor(tag).enabled;
}

auto get_factor_dims(const backend& b, backend::tag_type tag)
    -> const backend::dims_type&
{
    return b.get_factor_repo().get_factor(tag).applied_dims;
}

auto get_factor_baseline(const backend& b, backend::tag_type tag)
    -> factor::baseline_type
{
    return b.get_factor_repo().get_factor(tag).baseline;
}

auto get_factor_min_ratio(const backend& b, backend::tag_type tag)
    -> backend::baseline_type
{
    return b.get_factor_repo().get_factor(tag).min_ratio;
}

auto get_factor_max_ratio(const backend& b, backend::tag_type tag)
    -> backend::baseline_type
{
    return b.get_factor_repo().get_factor(tag).max_ratio;
}

auto apply_factor_on_x_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { true, curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto apply_factor_on_y_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], true, curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto apply_factor_on_z_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], true },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto deny_factor_on_x_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { false, curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto deny_factor_on_y_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], false, curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto deny_factor_on_z_axis(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], false },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto enable_factor(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        true,
        curr.min_ratio,
        curr.max_ratio);
}

auto disable_factor(backend& b, backend::tag_type tag) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        false,
        curr.min_ratio,
        curr.max_ratio);
}

auto update_factor_dims(
    backend& b, backend::tag_type tag, backend::dims_type dims) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        std::move(dims),
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto update_factor_baseline(
    backend& b, backend::tag_type tag, backend::baseline_type baseline) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        baseline,
        curr.enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto update_factor_enablement(backend& b, backend::tag_type tag, bool enabled)
    -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        enabled,
        curr.min_ratio,
        curr.max_ratio);
}

auto update_factor_min_ratio(
    backend& b, backend::tag_type tag, backend::ratio_type ratio) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        ratio,
        curr.max_ratio);
}

auto update_factor_max_ratio(
    backend& b, backend::tag_type tag, backend::ratio_type ratio) -> void
{
    const auto& curr = b.get_factor_repo().get_factor(tag);
    b.update_factor(
        tag,
        { curr.applied_dims[0], curr.applied_dims[1], curr.applied_dims[2] },
        curr.baseline,
        curr.enabled,
        curr.min_ratio,
        ratio);
}

auto restore_defaults(backend& b) -> void
{
    using std::ranges::views::keys;

    for (const auto& tag : keys(b.get_factor_repo()))
    {
        const auto& original = b.config_data().at(tag);
        b.update_factor(
            tag,
            original.applied_dims,
            original.baseline,
            original.enabled,
            original.min_ratio,
            original.max_ratio);
    }
}

} // namespace scaling