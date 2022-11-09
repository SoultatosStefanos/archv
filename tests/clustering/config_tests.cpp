#include "../utility/config.hpp"
#include "clustering/backend_config.hpp"
#include "clustering/config.hpp"

#include <gtest/gtest.h>

using namespace testing;
using namespace utility;

namespace
{

inline auto make_sample_0() -> clustering::config_data
{
    return clustering::config_data { .clusterers = { "Bob", "Alice" },
                                     .mst_finders = { "Popeye" },
                                     .clusterer = "M",
                                     .mst_finder = "N",
                                     .k = 20,
                                     .snn_threshold = 9,
                                     .min_modularity = 22 };
}

TEST(clustering_config_tests, clustering_sample_0)
{
    const auto root
        = read_json_root("../../data/testfiles/sample_clustering_0.json");
    const auto expected = make_sample_0();

    const auto actual = clustering::deserialize(root);

    ASSERT_EQ(actual, expected);
}

} // namespace