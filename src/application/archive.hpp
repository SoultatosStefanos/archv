// Contains a module for pooling json roots.
// Soultatos Stefanos 2022

#ifndef APPLICATION_ARCHIVE_HPP
#define APPLICATION_ARCHIVE_HPP

#include <jsoncpp/json/json.h>
#include <string_view>
#include <unordered_map>

namespace application
{

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

    auto at(file_name_type fname) const -> const json_root_type&;
    auto at(file_name_type fname) -> json_root_type&;

private:
    using holder = std::unordered_map< file_name_type, json_root_type >;

    archive() = default;
    ~archive();

    mutable holder m_roots;
};

} // namespace application

#endif // APPLICATION_ARCHIVE_HPP
