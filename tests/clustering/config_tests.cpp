#include "../utility/config.hpp"
#include "clustering/backend_config.hpp"
#include "clustering/config.hpp"

#include <gtest/gtest.h>
#include <jsoncpp/json/json.h>

using namespace testing;
using namespace utility;
using namespace clustering;

namespace
{

inline auto make_sample_0() -> config_data
{
    return config_data { .clusterers = { "Bob", "Alice" },
                         .mst_finders = { "Popeye" },
                         .clusterer = "M",
                         .intensity = 208,
                         .mst_finder = "N",
                         .k = 20,
                         .snn_threshold = 9,
                         .min_modularity = 22,
                         .llp_gamma = 0.12,
                         .llp_steps = 3 };
}

TEST(clustering_config_tests, clustering_sample_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_clustering_0.json");
    const auto expected = make_sample_0();

    const auto actual = deserialize(root);

    ASSERT_EQ(actual, expected);
}

TEST(clustering_config_tests, clustering_sample_0_serialization)
{
    auto root = json_val();
    const auto cfg = make_sample_0();

    serialize(root, cfg);
    auto deserialized = deserialize(root);

    EXPECT_EQ(deserialized, cfg);
}

} // namespace