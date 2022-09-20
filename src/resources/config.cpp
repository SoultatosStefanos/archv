#include "config.hpp"

#include "resources.hpp"

namespace resources
{

namespace
{
    auto deserialize_resources(const Json::Value& val)
    {
        using resource = std::string;
        using resource_vector = std::vector< resource >;

        auto res = resource_vector();

        std::transform(
            std::cbegin(val),
            std::cend(val),
            std::back_inserter(res),
            [](const Json::Value& v) { return v.as< resource >(); });

        return res;
    }

} // namespace

auto deserialize(const Json::Value& root) -> void
{
    auto&& meshes = deserialize_resources(root["meshes"]);
    auto&& materials = deserialize_resources(root["materials"]);
    auto&& fonts = deserialize_resources(root["fonts"]);

    load_meshes(std::move(meshes));
    load_materials(std::move(materials));
    load_fonts(std::move(fonts));
}

} // namespace resources