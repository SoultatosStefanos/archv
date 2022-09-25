#include "config.hpp"

#include <boost/log/trivial.hpp>

namespace scaling
{

namespace
{
    auto deserialize_dims(const Json::Value& val) -> factor::dims_type
    {
        auto iter = std::cbegin(val);

        const auto x_val = (*iter).asBool();
        std::advance(iter, 1);

        const auto y_val = (*iter).asBool();
        std::advance(iter, 1);

        const auto z_val = (*iter).asBool();

        return { x_val, y_val, z_val };
    }

    auto deserialize_factor(const Json::Value& val) -> factor
    {
        auto enabled = val["enabled"].asBool();
        auto&& dims = deserialize_dims(val["dimensions"]);
        auto baseline = val["baseline"].as< factor::baseline_type >();

        return factor { .applied_dims = std::move(dims),
                        .baseline = baseline,
                        .enabled = enabled };
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    config_data res;

    const auto& factors = root["factors"];

    for (const auto& val : factors)
    {
        const auto pair = std::begin(val);

        auto&& tag = pair.name();
        const auto& factor_val = *pair;
        auto&& factor = deserialize_factor(factor_val);

        res.insert(std::make_pair(std::move(tag), std::move(factor)));

        BOOST_LOG_TRIVIAL(debug) << "read scaling factor: " << tag;
    }

    return res;
}

} // namespace scaling