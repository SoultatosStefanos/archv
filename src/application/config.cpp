#include "config.hpp"

#include "json/deserialization.hpp"

using namespace json;

namespace application
{

auto deserialize(const Json::Value& root) -> config_data
{
    const auto config_path = as< std::string >(get(root, "config-path"));
    const auto dependencies = as< std::string >(get(root, "dependencies"));
    const auto layout = as< std::string >(get(root, "layout"));

    return { .dependencies_path = config_path + dependencies,
             .layout_path = config_path + layout };
}

} // namespace application