#include "config.hpp"

#include "misc/deserialization.hpp"

#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace scaling
{

using namespace misc;

namespace
{
    auto deserialize_dims(const json_val& val) -> factor::dims_type
    {
        auto iter = std::cbegin(val);

        const auto x_val = as< bool >(*iter);
        std::advance(iter, 1);

        const auto y_val = as< bool >(*iter);
        std::advance(iter, 1);

        const auto z_val = as< bool >(*iter);

        return { x_val, y_val, z_val };
    }

    auto deserialize_factor(const json_val& val) -> factor
    {
        using baseline_type = factor::baseline_type;
        using ratio_type = factor::ratio_type;

        auto enabled = as< bool >(get(val, "enabled"));
        auto&& dims = deserialize_dims(get(val, "dimensions"));
        auto baseline = as< double >(get(val, "baseline"));

        const auto& ratio_val = get(val, "ratio");
        auto min_ratio = as< double >(get(ratio_val, "min"));
        auto max_ratio = as< double >(get(ratio_val, "max"));

        return factor { .applied_dims = dims,
                        .baseline = static_cast< baseline_type >(baseline),
                        .enabled = enabled,
                        .min_ratio = static_cast< ratio_type >(min_ratio),
                        .max_ratio = static_cast< ratio_type >(max_ratio) };
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    config_data res;

    const auto& factors = get(root, "factors");

    for (const auto& val : factors)
    {
        const auto pair = std::begin(val);

        auto&& tag = pair.name();
        const auto& factor_val = *pair;
        auto&& factor = deserialize_factor(factor_val);

        res.insert(std::make_pair(std::move(tag), factor));

        BOOST_LOG_TRIVIAL(debug) << "read scaling factor: " << tag;
    }

    return res;
}

} // namespace scaling