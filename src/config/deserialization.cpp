#include "deserialization.hpp"

#include "json/deserialization.hpp"

using namespace json;

namespace config
{

auto deserialize_config(const Json::Value& root) -> config_data
{
    const auto config_path = as< std::string >(get(root, "config-path"));
    const auto dependencies = as< std::string >(get(root, "dependencies"));
    const auto graph = as< std::string >(get(root, "graph"));
    const auto layout = as< std::string >(get(root, "layout"));

    return { .dependencies_path = config_path + dependencies,
             .graph_path = config_path + graph,
             .layout_path = config_path + layout };
}

} // namespace config