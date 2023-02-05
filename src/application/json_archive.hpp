// Contains a utility json archive class.
// Soultatos Stefanos 2022

#ifndef APPLICATION_JSON_ARCHIVE_HPP
#define APPLICATION_JSON_ARCHIVE_HPP

#include "misc/heterogeneous.hpp" // for unordered_string_map

#include <boost/exception/error_info.hpp> // for error_info
#include <boost/exception/exception.hpp>  // for exception
#include <filesystem>                     // for path
#include <jsoncpp/json/json.h>            // for Value
#include <stdexcept>                      // for exception
#include <string>                         // for string
#include <string_view>                    // for string_view

namespace application
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct invalid_file_path : virtual std::exception, virtual boost::exception
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using file_path_info = boost::error_info< struct tag_json, std::string >;

/***********************************************************
 * Typedefs                                                *
 ***********************************************************/

using json_t = Json::Value;
using file_path_t = std::filesystem::path;

/***********************************************************
 * Json Archive                                            *
 ***********************************************************/

class json_archive
{
public:
    using json_type = json_t;
    using str_type = std::string;
    using str_ref_type = std::string_view;

    auto archived(str_ref_type file) const -> bool;

    auto get(str_ref_type file) const -> const json_type&;
    auto get(str_ref_type file) -> json_type&;

    auto put(str_type file, json_type&& json) -> void;

private:
    using json_map = misc::unordered_string_map< json_type >;

    json_map m_map;
};

/***********************************************************
 * Import/Export                                           *
 ***********************************************************/

// Throw: invalid_file_path when needed.

auto import(const file_path_t& from) -> json_t;
auto import(json_archive& archive, const file_path_t& from) -> void;

auto dump(const json_t& json, const file_path_t& to) -> void;
auto dump(const json_archive& archive, const file_path_t& to) -> void;

} // namespace application

#endif // APPLICATION_JSON_ARCHIVE_HPP
