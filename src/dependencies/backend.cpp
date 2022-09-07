#include "backend.hpp"

#include <cassert>

namespace dependencies
{

backend::backend(config_data_type config)
: m_repo { config }, m_config { std::move(config) }
{
}

auto backend::update_weight(const string& dependency, integer weight) -> void
{
    assert(config_data().contains(dependency));

    m_repo.set_weight(dependency, weight);

    m_signal(dependency, weight);
}

auto restore_defaults(backend& b) -> void
{
    for (const auto& [dependency, weight] : b.config_data())
        b.update_weight(dependency, weight);
}

} // namespace dependencies