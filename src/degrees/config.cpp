#include "config.hpp"

#include "backend_config.hpp"
#include "misc/deserialization.hpp"

#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>
#include <string>

namespace degrees
{

using namespace misc;

namespace
{
    template < typename T >
    inline auto deserialize_degree_ranks(const json_val& val)
    {
        auto&& light = as< T >(get(val, "light"));
        auto&& medium = as< T >(get(val, "medium"));
        auto&& heavy = as< T >(get(val, "heavy"));

        return std::make_tuple(
            std::move(light), std::move(medium), std::move(heavy));
    }

    inline auto deserialize_degrees_section(const json_val& val)
    {
        using threshold_type = evaluation_data::threshold_type;
        using system_type = evaluation_data::particle_system_type;
        using applied_type = evaluation_data::applied_type;

        auto&& [light_threshold, medium_threshold, heavy_threshold]
            = deserialize_degree_ranks< threshold_type >(
                get(val, "thresholds"));

        auto&& [light_particles, medium_particles, heavy_particles]
            = deserialize_degree_ranks< system_type >(
                get(val, "particle-systems"));

        const auto applied = as< applied_type >(get(val, "applied"));

        BOOST_LOG_TRIVIAL(debug) << "deserialized degree effects";

        return evaluation_data(
            make_ranked(light_threshold, medium_threshold, heavy_threshold),
            make_ranked(
                std::move(light_particles),
                std::move(medium_particles),
                std::move(heavy_particles)),
            applied);
    }

} // namespace

auto deserialize(const json_val& root) -> config_data
{
    return config_data(
        deserialize_degrees_section(get(root, "in-degree")),
        deserialize_degrees_section(get(root, "out-degree")));
}

namespace
{
    template < typename Ranked >
    inline auto serialize_degree_ranks(json_val& val, const Ranked& ranked)
    {
        val["light"] = ranked.light;
        val["medium"] = ranked.medium;
        val["heavy"] = ranked.heavy;
    }

    inline auto serialize_degrees_section(
        json_val& val, //
        const evaluation_data& data)
    {
        serialize_degree_ranks(val["thresholds"], data.thresholds);
        serialize_degree_ranks(val["particle-systems"], data.particles);
        val["applied"] = data.applied;
    }

} // namespace

auto serialize(json_val& root, const config_data& cfg) -> void
{
    serialize_degrees_section(root["in-degree"], cfg.in_data);
    serialize_degrees_section(root["out-degree"], cfg.out_data);
}

} // namespace degrees