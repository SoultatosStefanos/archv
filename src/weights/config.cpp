#include "config.hpp"

#include "misc/deserialization.hpp"

#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace weights
{

using namespace misc;

auto deserialize(const json_val& root) -> config_data
{
    using weight_type = weight_repo::weight_type;

    config_data res;

    const auto& deps = get(root, "dependencies");

    for (const auto& val : deps)
    {
        const auto pair = std::begin(val);

        auto&& dependency = pair.name();
        const auto weight = as< weight_type >(*pair);

        res.insert(std::make_pair(std::move(dependency), weight));

        BOOST_LOG_TRIVIAL(debug)
            << "read dependency: " << dependency << " with weight: " << weight;
    }

    return res;
}

auto serialize(json_val& root, const config_data& cfg) -> void
{
    for (const auto& [dependency, weight] : cfg)
    {
        json_val val;

        val[dependency] = weight;

        root["dependencies"].append(std::move(val));
    }
}

} // namespace weights
