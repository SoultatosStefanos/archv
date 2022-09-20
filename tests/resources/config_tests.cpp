#include "resources/config.hpp"
#include "resources/resources.hpp"
#include "utility/json.hpp"

#include <gtest/gtest.h>

using namespace resources;
using namespace testing;
using namespace utility;

namespace
{

auto sample_0_meshes()
{
    return mesh_vector { "a", "a" };
}

auto sample_0_materials()
{
    return material_vector { "b" };
}

auto sample_0_fonts()
{
    return font_vector { "c", "c", "c" };
}

TEST(resources_deserialization, sample_resources_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_resources_0.json");

    deserialize(root);

    ASSERT_EQ(sample_0_meshes(), meshes());
    ASSERT_EQ(sample_0_materials(), materials());
    ASSERT_EQ(sample_0_fonts(), fonts());
}

} // namespace