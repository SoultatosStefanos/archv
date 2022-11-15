// Contains a utility class used for caching all of the gui popups.
// Soultatos Stefanos 2022

#ifndef GUI_POPUP_HOLDER_HPP
#define GUI_POPUP_HOLDER_HPP

#include "structure_popup.hpp"

#include <string_view>
#include <unordered_map>

namespace gui
{

/***********************************************************
 * Popup holder                                            *
 ***********************************************************/

class popup_holder
{
public:
    using popup_type = structure_popup; // This will be a std::variant later.
    using id_type = std::string_view;
    using size_type = std::size_t;

    auto get(id_type id) const -> const popup_type&;
    auto get(id_type id) -> popup_type&;

    auto contains(id_type id) const -> bool;
    auto size() const -> size_type;

    auto insert(id_type id, const popup_type& popup) -> void;
    auto insert(id_type id, popup_type&& popup) -> void;
    auto remove(id_type id) -> void;
    auto clear() -> void;

private:
    using map = std::unordered_map< id_type, popup_type >;

    map m_popups;
};

/***********************************************************
 * Global instance                                         *
 ***********************************************************/

extern popup_holder popups;

} // namespace gui

#endif // GUI_POPUP_HOLDER_HPP
