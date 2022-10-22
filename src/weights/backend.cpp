#include "backend.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace weights
{

backend::backend(config_data_type config)
: m_repo { config }, m_config { std::move(config) }
{
}

auto backend::update_weight(dependency_type dependency, weight_type weight)
    -> void
{
    if (!config_data().contains(dependency))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid weight update";
        return;
    }

    m_repo.set_weight(dependency, weight);
    m_signal(dependency, weight);
}

auto restore_defaults(backend& b) -> void
{
    for (const auto& [dependency, weight] : b.config_data())
        b.update_weight(dependency, weight);
}

} // namespace weights