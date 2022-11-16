// Contains the gui popup modal dialog which displays class info.
// Soultatos Stefanos 2022

#ifndef GUI_STRUCTURE_POPUP_HPP
#define GUI_STRUCTURE_POPUP_HPP

#include "point.hpp"

#include <string_view>

namespace gui
{

/***********************************************************
 * Structure View                                          *
 ***********************************************************/

// TODO Pass more structure data
using id_type = std::string_view;
using structure_info = id_type;

/***********************************************************
 * Structure Popup                                         *
 ***********************************************************/

class structure_popup
{
public:
    structure_popup() = default;
    structure_popup(structure_info s, point pos);

    auto structure() const -> const structure_info& { return m_structure; }
    auto pos() const -> const point& { return m_pos; }
    auto pos() -> point& { return m_pos; }

    auto render() const -> void;
    auto visible() const -> bool;
    auto show() -> void;
    auto hide() -> void;

private:
    structure_info m_structure;
    point m_pos;
};

} // namespace gui

#endif // GUI_STRUCTURE_POPUP_HPP
