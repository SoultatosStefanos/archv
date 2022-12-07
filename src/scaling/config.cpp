#include "config.hpp"

#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace scaling
{

namespace
{
    auto deserialize_dims(const json_val& val) -> factor::dims_type
    {
        auto iter = std::cbegin(val);

        const auto x_val = (*iter).asBool();
        std::advance(iter, 1);

        const auto y_val = (*iter).asBool();
        std::advance(iter, 1);

        const auto z_val = (*iter).asBool();

        return { x_val, y_val, z_val };
    }

    auto deserialize_factor(const json_val& val) -> factor
    {
        auto enabled = val["enabled"].asBool();
        auto&& dims = deserialize_dims(val["dimensions"]);
        auto baseline = val["baseline"].as< factor::baseline_type >();

        const auto& ratio_val = val["ratio"];
        auto min_ratio = ratio_val["min"].as< factor::ratio_type >();
        auto max_ratio = ratio_val["max"].as< factor::ratio_type >();

        return factor { .applied_dims = dims,
                        .baseline = baseline,
                        .enabled = enabled,
                        .min_ratio = min_ratio,
                        .max_ratio = max_ratio };
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    config_data res;

    const auto& factors = root["factors"];

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