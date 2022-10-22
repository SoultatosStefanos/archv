#include "backend.hpp"

#include <boost/log/trivial.hpp>
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
    if (baseline <= 0 or min_ratio < 0 or max_ratio < 0)
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid scaling factor update";
        return;
    }

    auto& f = m_repo.get_factor(tag);
    f.applied_dims = dims;
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