// Contains a configuration json archive manager class.
// Soultatos Stefanos 2022

#ifndef CONFIG_JSON_ARCHIVE_HPP
#define CONFIG_JSON_ARCHIVE_HPP

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace config
{

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct json_archive_error : virtual std::exception, virtual boost::exception
{
};

struct invalid_json_file : virtual json_archive_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using json_file_info = boost::error_info<struct tag_file, std::string_view>;

/***********************************************************
 * Json Archive                                            *
 ***********************************************************/

class json_archive final
{
public:
    json_archive(const json_archive&) = delete;
    json_archive(json_archive&&) = delete;

    auto operator=(const json_archive&) -> json_archive& = delete;
    auto operator=(json_archive&&) -> json_archive& = delete;

    static auto get() -> json_archive&
    {
        static json_archive singleton;
        return singleton;
    }

    auto at(std::string_view fname) const -> const Json::Value&;

private:
    using pool = std::unordered_map<std::string_view, Json::Value>;

    json_archive() = default;
    ~json_archive() = default;

    mutable pool m_roots;
};

} // namespace config

#endif // CONFIG_JSON_ARCHIVE_HPP
