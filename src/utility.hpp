// Contains some general utilities.
// Soultatos Stefanos 2022

#ifndef SRC_UTILITY_HPP
#define SRC_UTILITY_HPP

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace Utility
{

struct InvalidJsonArchive : virtual std::exception, virtual boost::exception
{
};

using JsonArchive =
    boost::error_info<struct JsonArchiveTag, const std::string_view>;

// Pools json archive roots from file paths.
class JsonManager final
{
public:
    using JsonVal = Json::Value;
    using FilePath = std::string_view;

    JsonManager(const JsonManager&) = delete;
    JsonManager(JsonManager&&) = delete;

    auto operator=(const JsonManager&) -> JsonManager& = delete;
    auto operator=(JsonManager&&) -> JsonManager& = delete;

    static auto get() -> JsonManager&
    {
        static JsonManager singleton;
        return singleton;
    }

    auto croot(const FilePath from) const -> const JsonVal&;
    auto root(const FilePath from) -> JsonVal&;

private:
    using JsonPool = std::unordered_map<FilePath, JsonVal>;

    JsonManager() = default;
    ~JsonManager() = default;

    mutable JsonPool cache;
};

} // namespace Utility

#endif // SRC_UTILITY_HPP
