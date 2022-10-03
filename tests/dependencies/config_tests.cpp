#include "config/archive.hpp"
#include "dependencies/config.hpp"
#include "misc/source.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace dependencies;
using namespace testing;

namespace
{

inline auto read_json_root(std::string_view path)
{
    return config::archive::get().at(misc::resolve_source_path(path).c_str());
}

auto make_sample_dependencies_0()
{
    return config_data { { "strong", 10 }, { "weak", 2 } };
}

TEST(dependencies_deserialization, sample_dependencies_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_dependencies_0.json");
    const auto expected = make_sample_dependencies_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace