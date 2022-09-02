// Contains a module for pooling json roots.
// Soultatos Stefanos 2022

#ifndef JSON_ARCHIVE_HPP
#define JSON_ARCHIVE_HPP

#include <boost/exception/all.hpp>
#include <cassert>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace json
{

/***********************************************************
 * Archive                                                 *
 ***********************************************************/

class archive final
{
public:
    using file_name_type = std::string_view;
    using json_root_type = Json::Value;

    archive(const archive&) = delete;
    archive(archive&&) = delete;

    auto operator=(const archive&) -> archive& = delete;
    auto operator=(archive&&) -> archive& = delete;

    static auto get() -> archive&
    {
        static archive singleton;
        return singleton;
    }

    auto contains(file_name_type fname) const -> bool;

    auto operator[](file_name_type fname) -> json_root_type&;

    auto at(file_name_type fname) const -> const json_root_type&;

    auto begin() -> auto { return std::begin(m_roots); }
    auto begin() const -> auto { return std::begin(m_roots); }
    auto cbegin() const -> auto { return std::cbegin(m_roots); }

    auto end() -> auto { return std::end(m_roots); }
    auto end() const -> auto { return std::end(m_roots); }
    auto cend() const -> auto { return std::cend(m_roots); }

private:
    using holder = std::unordered_map< file_name_type, json_root_type >;

    archive() = default;
    ~archive() = default;

    mutable holder m_roots;
};

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct archive_error : virtual std::exception, virtual boost::exception
{
};

struct invalid_json_file : virtual archive_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using json_file_info = boost::error_info< struct tag_file, std::string_view >;

/***********************************************************
 * Read/Write                                              *
 ***********************************************************/

auto read_archive(archive::file_name_type fname) -> void;
auto read_archive_once(archive::file_name_type fname) -> void;

auto write_archive(archive::file_name_type fname) -> void;
auto write_all_archives() -> void;

} // namespace json

#endif // JSON_ARCHIVE_HPP
