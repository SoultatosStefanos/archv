#include "config.hpp"

#include "config/deserialization.hpp"

#include <boost/log/trivial.hpp>

using namespace config;

namespace dependencies
{

auto deserialize(const Json::Value& root) -> config_data
{
    using dependency_type = weight_repo::dependency_type;
    using weight_type = weight_repo::weight_type;

    config_data res;

    const auto deps = get(root, "dependencies");

    if (!deps.isArray())
        BOOST_THROW_EXCEPTION(
            invalid_json_value_type() << json_value_info(deps.type()));

    for (const auto& val : deps)
    {
        if (!val.isObject())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type()
                << json_type_info(val.type()) << json_value_info(val));

        if (val.empty())
            BOOST_THROW_EXCEPTION(
                invalid_json_value_type() << json_value_info(val));

        const auto pair = std::begin(val);

        auto&& dependency = as< dependency_type >(pair.name());
        const auto weight = as< weight_type >(*pair);

        res.insert(std::make_pair(std::move(dependency), weight));

        BOOST_LOG_TRIVIAL(debug)
            << "read dependency: " << dependency << " with weight: " << weight;
    }

    return res;
}

} // namespace dependencies
