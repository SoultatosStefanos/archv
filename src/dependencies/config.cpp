#include "config.hpp"

#include <boost/log/trivial.hpp>

namespace dependencies
{

auto deserialize(const Json::Value& root) -> config_data
{
    using weight_type = weight_repo::weight_type;

    config_data res;

    const auto& deps = root["dependencies"];

    for (const auto& val : deps)
    {
        const auto pair = std::begin(val);

        auto&& dependency = pair.name();
        const auto weight = (*pair).as< weight_type >();

        res.insert(std::make_pair(std::move(dependency), weight));

        BOOST_LOG_TRIVIAL(debug)
            << "read dependency: " << dependency << " with weight: " << weight;
    }

    return res;
}

} // namespace dependencies
