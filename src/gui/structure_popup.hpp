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
    explicit structure_popup(structure_info s);

    auto structure() const -> const structure_info& { return m_structure; }
    auto pos() const -> const point& { return m_pos; }
    auto pos() -> point& { return m_pos; }

    auto render() const -> void;
    auto visible() const -> bool { return m_visible; }
    auto show() -> void { m_visible = true; }
    auto hide() -> void { m_visible = false; }

private:
    mutable bool m_visible { false };
    structure_info m_structure;
    point m_pos;
};

} // namespace gui

#endif // GUI_STRUCTURE_POPUP_HPP
