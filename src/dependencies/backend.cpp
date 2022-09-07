#include "backend.hpp"

#include <cassert>

namespace dependencies
{

backend::backend(config_data_type config)
: m_repo { config }, m_config { std::move(config) }
{
}

auto backend::update_weight(const std::string& dep, double w) -> void
{
    assert(config_data().contains(dep));

    m_repo.set_weight(dep, w);

    m_signal(dep, w);
}

auto restore_defaults(backend& b) -> void
{
    for (const auto& [dep, w] : b.config_data())
        b.update_weight(dep, w);
}

} // namespace dependencies