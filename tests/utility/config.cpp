#include "config.hpp"

#include "misc/source.hpp"

#include <cassert>
#include <fstream>

namespace utility
{

auto read_json_root(std::string_view to) -> Json::Value
{
    const auto abs_path = misc::resolve_source_path(to);
    assert(std::filesystem::exists(abs_path));
    Json::Value root;
    std::ifstream stream { abs_path };
    assert(stream.good());
    stream >> root;
    return root;
}

} // namespace utility