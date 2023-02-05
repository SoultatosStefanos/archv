#include "config.hpp"

#include "misc/deserialization.hpp"

#include <boost/log/trivial.hpp>
#include <jsoncpp/json/json.h>

namespace color_coding
{

using namespace misc;

namespace
{
    inline auto deserialize_rgba(const json_val& val)
    {
        using rgba_val = float;
        using rgba_type = color_code::rgba_type;

        auto extract_val = [iter = std::begin(val)]() mutable -> double
        {
            auto val = as< double >(*iter);
            std::advance(iter, 1);
            return val;
        };

        auto r = extract_val();
        auto g = extract_val();
        auto b = extract_val();
        auto a = extract_val();

        return rgba_type({ static_cast< rgba_val >(r),
                           static_cast< rgba_val >(g),
                           static_cast< rgba_val >(b),
                           static_cast< rgba_val >(a) });
    }
} // namespace

auto deserialize(const json_val& root) -> config_data
{
    config_data res;
    for (const auto& val : get(root, "dependencies"))
    {
        auto&& dependency = as< std::string >(get(val, "dependency"));

        BOOST_LOG_TRIVIAL(debug)
            << "read dependency (color coding): " << dependency;

        const auto col = deserialize_rgba(get(val, "color"));
        const auto active = as< bool >(get(val, "active"));

        res.insert(
            std::make_pair(std::move(dependency), color_code(col, active)));
    }

    return res;
}

namespace
{

    auto serialize_rgba(json_val& root, const color_code::rgba_type& col)
    {
        for (auto i = 0; i < 4; ++i)
            root[i] = col[i];
    }

} // namespace

auto serialize(json_val& root, const config_data& cfg) -> void
{
    for (const auto& [dependency, code] : cfg)
    {
        json_val val;

        val["dependency"] = dependency;
        serialize_rgba(val["color"], code.rgba);
        val["active"] = code.active;

        root["dependencies"].append(std::move(val));
    }
}

} // namespace color_coding