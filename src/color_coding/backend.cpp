#include "backend.hpp"

#include <boost/log/trivial.hpp>

namespace color_coding
{

backend::backend(config_data_type cfg)
: m_cfg { cfg }, m_repo { std::move(cfg) }
{
}

auto backend::update_color(dependency_type dep, color_code_type col) -> void
{
    if (!config_data().contains(dep))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid color update";
        return;
    }
    m_repo.set_color(dep, col);
    m_sig(dep, col);
}

auto backend::connect(const slot& f) -> connection
{
    return m_sig.connect(f);
}

auto restore_defaults(backend& b) -> void
{
    for (const auto& [dep, col] : b.config_data())
        update_color_code(b, dep, col);
}

} // namespace color_coding