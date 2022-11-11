#include "structure_dialog_manager.hpp"

#include <cassert>

namespace gui
{

auto structure_dialog_manager::manages(id_type id) const -> bool
{
    return m_map.find(id) != std::cend(m_map);
}

auto structure_dialog_manager::num_dialogs() const -> size_type
{
    return m_map.size();
}

auto structure_dialog_manager::num_active_dialogs() const -> size_type
{
    return m_actives.size();
}

auto structure_dialog_manager::is_active(id_type id) const -> bool
{
    return m_actives.find(id) != std::cend(m_actives);
}

auto structure_dialog_manager::submit(id_type id, structure_dialog dialog)
    -> void
{
    m_map[id] = std::move(dialog);
    assert(manages(id));
}

auto structure_dialog_manager::withdraw(id_type id) -> void
{
    m_map.erase(id);
    m_actives.erase(id);
    assert(!manages(id));
    assert(!is_active(id));
}

auto structure_dialog_manager::activate(id_type id) -> void
{
    assert(manages(id));
    m_actives.insert(id);
    assert(is_active(id));
}

auto structure_dialog_manager::deactivate(id_type id) -> void
{
    assert(manages(id));
    m_actives.erase(id);
    assert(!is_active(id));
}

auto structure_dialog_manager::render_active_dialogs() const -> void
{
    for (auto id : m_actives)
    {
        assert(manages(id));
        assert(is_active(id));
        assert(m_map.contains(id));

        const auto& dialog = m_map.at(id);
        dialog.render();
    }
}

structure_dialog_manager structure_dialogs;

} // namespace gui