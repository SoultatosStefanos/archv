// Contains configuration archive manager classes.
// Soultatos Stefanos 2022

#ifndef CONFIG_ARCHIVES_HPP
#define CONFIG_ARCHIVES_HPP

#include "error.hpp"

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <string_view>
#include <unordered_map>

namespace config
{

using archive = std::string_view;

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct invalid_archive : virtual error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using archive_info = boost::error_info<struct tag_archive, archive>;

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

    auto at(archive fname) const -> const Json::Value&;

private:
    using pool = std::unordered_map<archive, Json::Value>;

    json_archive() = default;
    ~json_archive() = default;

    mutable pool m_roots;
};

} // namespace config

#endif // CONFIG_ARCHIVES_HPP
