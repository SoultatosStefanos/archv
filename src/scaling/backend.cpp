#include "backend.hpp"

namespace scaling
{

backend::backend(config_data_type cfg)
: m_cfg { cfg }, m_repo { std::move(cfg) }
{
}

auto backend::set_factor_dims(tag_type tag, dims_type dims) -> void
{
    auto& f = m_repo.get_factor(tag);
    f.applied_dims = std::move(dims);
    emit_factor(f);
}

auto backend::set_factor_enabled(tag_type tag, bool enabled) -> void
{
    auto& f = m_repo.get_factor(tag);
    f.enabled = enabled;
    emit_factor(f);
}

auto backend::set_factor_baseline(tag_type tag, baseline_type baseline) -> void
{
    auto& f = m_repo.get_factor(tag);
    f.baseline = baseline;
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

} // namespace scaling