// Contains the gui popup modal dialog which displays class info.
// Soultatos Stefanos 2022

#ifndef GUI_STRUCTURE_POPUP_HPP
#define GUI_STRUCTURE_POPUP_HPP

#include "point.hpp" // for point

#include <string_view> // for string_view
#include <vector>      // for vector

namespace gui
{

/***********************************************************
 * Structure View                                          *
 ***********************************************************/

struct structure_info
{
    using id_type = std::string_view;
    using name_type = std::string_view;
    using type = std::string_view;
    using namespace_type = std::string_view;
    using file_type = std::string_view;
    using methods_type = std::vector< id_type >;
    using fields_type = std::vector< id_type >;
    using bases_type = std::vector< id_type >;
    using nested_type = std::vector< id_type >;
    using friends_type = std::vector< id_type >;

    id_type id;
    name_type name;
    type t;
    namespace_type name_space;
    file_type file;
    methods_type methods;
    fields_type fields;
    bases_type bases;
    nested_type nested;
    friends_type friends;

    auto operator==(const structure_info&) const -> bool = default;
    auto operator!=(const structure_info&) const -> bool = default;
};

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
    auto visible() const -> bool;
    auto show() -> void;
    auto hide() -> void;

private:
    mutable bool m_do_open { false };
    structure_info m_structure;
    point m_pos;
};

} // namespace gui

#endif // GUI_STRUCTURE_POPUP_HPP
