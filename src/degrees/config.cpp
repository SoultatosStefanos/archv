#include "config.hpp"

#include <boost/log/trivial.hpp>

namespace degrees
{

namespace
{
    template < typename T >
    inline auto deserialize_degree_ranks(const Json::Value& val)
    {
        auto&& light = val["light"].as< T >();
        auto&& medium = val["medium"].as< T >();
        auto&& heavy = val["heavy"].as< T >();

        return std::make_tuple(
            std::move(light), std::move(medium), std::move(heavy));
    }

    inline auto deserialize_degree_effects(const Json::Value& val)
    {
        using threshold_type = effect_evaluation_data::threshold_type;
        using effect_type = effect_evaluation_data::effect_type;

        auto&& [light_threshold, medium_threshold, heavy_threshold]
            = deserialize_degree_ranks< threshold_type >(val["thresholds"]);

        auto&& [light_effect, medium_effect, heavy_effect]
            = deserialize_degree_ranks< effect_type >(val["particle-systems"]);

        BOOST_LOG_TRIVIAL(debug) << "deserialized degree effects";

        return effect_evaluation_data(
            make_ranked(light_threshold, medium_threshold, heavy_threshold),
            make_ranked(
                std::move(light_effect),
                std::move(medium_effect),
                std::move(heavy_effect)));
    }

} // namespace

auto deserialize(const Json::Value& root) -> config_data
{
    auto&& in = deserialize_degree_effects(root["in-degree"]);
    auto&& out = deserialize_degree_effects(root["out-degree"]);

    return std::make_pair(std::move(in), std::move(out));
}

} // namespace degrees