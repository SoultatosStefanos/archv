// Contains a module for pooling json roots.
// Soultatos Stefanos 2022

#ifndef JSON_ARCHIVE_HPP
#define JSON_ARCHIVE_HPP

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace json
{

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
 * Archive                                                 *
 ***********************************************************/

class archive final
{
public:
    archive(const archive&) = delete;
    archive(archive&&) = delete;

    auto operator=(const archive&) -> archive& = delete;
    auto operator=(archive&&) -> archive& = delete;

    static auto get() -> archive&
    {
        static archive singleton;
        return singleton;
    }

    auto at(std::string_view fname) const -> const Json::Value&;

private:
    using pool = std::unordered_map< std::string_view, Json::Value >;

    archive() = default;
    ~archive() = default;

    mutable pool m_roots;
};

} // namespace json

#endif // JSON_ARCHIVE_HPP
