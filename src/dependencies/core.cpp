#include "core.hpp"

namespace dependencies
{

core::core(command_history& cmds, hash_table table)
    : m_cmds{cmds}, m_repo{std::move(table)}
{}

void core::set_weight(const dependency_type& type, weight score)
{
    if (m_repo.get_weight(type) != score)
    {
        m_repo.set_weight(type, score);
        m_signal(type, score);
    }
}

} // namespace dependencies