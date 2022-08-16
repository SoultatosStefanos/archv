#include "json.hpp"

#include "path.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

namespace utility
{

auto read_json_root(std::string_view to) -> Json::Value
{
    const auto path = resolve_path(to);
    assert(std::filesystem::exists(path));

    std::ifstream archive { path };
    assert(archive.good());

    Json::Value root;
    archive >> root;

    return root;
}

} // namespace utility