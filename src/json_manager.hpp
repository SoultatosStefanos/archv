// Contains a general purpose json archive root pool.
// Soultatos Stefanos 2022

#ifndef SRC_JSON_MANAGER_HPP
#define SRC_JSON_MANAGER_HPP

#include <boost/exception/all.hpp>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace Utility
{

// --------------------------- Runtime Errors ------------------------------- //

struct InvalidJsonArchive : virtual std::exception, virtual boost::exception
{
};

// -------------------------------------------------------------------------- //

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

#endif // SRC_JSON_MANAGER_HPP
