#include "backend.hpp"

#include <ranges>

namespace scaling
{

backend::backend(config_data_type cfg)
: m_cfg { cfg }, m_repo { std::move(cfg) }
{
}

auto backend::update_factor_dims(tag_type tag, dims_type dims) -> void
{
    auto& f = m_repo.get_factor(tag);
    f.applied_dims = std::move(dims);
    emit_factor(f);
}

auto backend::update_factor_enabled(tag_type tag, bool enabled) -> void
{
    auto& f = m_repo.get_factor(tag);
    f.enabled = enabled;
    emit_factor(f);
}

auto backend::update_factor_baseline(tag_type tag, baseline_type baseline)
    -> void
{
    if (baseline == 0)
        BOOST_THROW_EXCEPTION(invalid_baseline() << baseline_info(baseline));

    auto& f = m_repo.get_factor(tag);
    f.baseline = baseline;
    emit_factor(f);
}

auto backend::update_factor(
    tag_type tag, dims_type dims, baseline_type baseline, bool enabled) -> void
{
    if (baseline == 0)
        BOOST_THROW_EXCEPTION(invalid_baseline() << baseline_info(baseline));

    auto& f = m_repo.get_factor(tag);
    f.applied_dims = std::move(dims);
    f.baseline = baseline;
    f.enabled = enabled;

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
            tag, original.applied_dims, original.baseline, original.enabled);
    }
}

} // namespace scaling