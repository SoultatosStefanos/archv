// Contains the basic architecture components (structures).
// Soultatos Stefanos 2022.

#ifndef ARCHITECTURE_SYMBOLS_HPP
#define ARCHITECTURE_SYMBOLS_HPP

#include <string>
#include <vector>

namespace architecture
{

/***********************************************************
 * Source Location/Info                                    *
 ***********************************************************/

struct source_location
{
    using file_type = std::string;
    using line_type = int;
    using column_type = int;

    auto operator==(const source_location&) const -> bool = default;
    auto operator!=(const source_location&) const -> bool = default;

    file_type file;
    line_type line{-1};
    column_type col{-1};
};

/***********************************************************
 * Symbol                                                  *
 ***********************************************************/

struct symbol
{
    using id_type = std::string;
    using name_type = std::string;
    using namespace_type = std::string;
    using access_specifier = std::string;

    auto operator==(const symbol&) const -> bool = default;
    auto operator!=(const symbol&) const -> bool = default;

    id_type id;
    name_type name;
    namespace_type name_space;
    source_location source;
    access_specifier access{"unknown"};
};

template <typename T>
using composites = std::vector<T>;

using references = std::vector<symbol::id_type>;

/***********************************************************
 * Definition                                              *
 ***********************************************************/

struct definition
{
    using id_type = symbol::id_type;
    using full_type = std::string;
    using type = std::string;

    auto operator==(const definition&) const -> bool = default;
    auto operator!=(const definition&) const -> bool = default;

    symbol sym;
    full_type full_t;
    type t;
};

/***********************************************************
 * Method                                                  *
 ***********************************************************/

struct method
{
    using id_type = symbol::id_type;
    using type = std::string;
    using return_type = std::string;
    using arguments_type = composites<definition>;
    using definitions_type = composites<definition>;
    using template_args_type = references;
    using count_type = int;
    using depth_type = int;

    auto operator==(const method&) const -> bool = default;
    auto operator!=(const method&) const -> bool = default;

    symbol sym;
    type t;
    return_type ret_type;
    arguments_type arguments;
    definitions_type definitions;
    template_args_type template_args;
    count_type literals{-1};
    count_type statements{-1};
    count_type branches{-1};
    count_type loops{-1};
    count_type lines{-1};
    depth_type max_scope{-1};
    bool is_virtual{false};
};

/***********************************************************
 * Structure                                               *
 ***********************************************************/

struct structure
{
    using id_type = symbol::id_type;
    using type = std::string;
    using methods_type = composites<method>;
    using fields_type = composites<definition>;
    using bases_type = references;
    using nested_type = references;
    using friends_type = references;
    using template_args_type = references;

    auto operator==(const structure&) const -> bool = default;
    auto operator!=(const structure&) const -> bool = default;

    symbol sym;
    type t;
    methods_type methods;
    fields_type fields;
    bases_type bases;
    nested_type nested;
    friends_type friends;
    template_args_type template_args;
};

} // namespace architecture

#endif // ARCHITECTURE_SYMBOLS_HPP