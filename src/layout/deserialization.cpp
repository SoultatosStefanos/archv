#include "deserialization.hpp"

namespace layout
{

namespace
{
    void sanitize(const config_data&) { }

} // namespace

auto deserialize_layout(const Json::Value& root) -> config_data { return {}; }

} // namespace layout