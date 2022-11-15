#include "popup_holder.hpp"

#include <cassert>

namespace gui
{

auto popup_holder::get(id_type id) const -> const popup_type&
{
    assert(contains(id));
    return m_popups.at(id);
}

auto popup_holder::get(id_type id) -> popup_type&
{
    assert(contains(id));
    return m_popups.at(id);
}

auto popup_holder::contains(id_type id) const -> bool
{
    return m_popups.contains(id);
}

auto popup_holder::size() const -> size_type
{
    return m_popups.size();
}

auto popup_holder::insert(id_type id, const popup_type& popup) -> void
{
    // NOTE: deliberately not modifying the state of the map if popup is managed
    m_popups.insert(std::make_pair(id, popup));
    assert(contains(id));
    assert(size() > 0);
}

auto popup_holder::insert(id_type id, popup_type&& popup) -> void
{
    // NOTE: deliberately not modifying the state of the map if popup is managed
    m_popups.insert(std::make_pair(id, std::move(popup)));
    assert(contains(id));
    assert(size() > 0);
}

auto popup_holder::remove(id_type id) -> void
{
    m_popups.erase(id);
    assert(!contains(id));
    assert(size() < m_popups.max_size());
}

auto popup_holder::clear() -> void
{
    m_popups.clear();
    assert(size() == 0);
}

popup_holder popups;

} // namespace gui