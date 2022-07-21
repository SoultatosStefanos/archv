// Contains some json deserialization utilities.
// Soultatos Stefanos 2022

#ifndef UTILITY_JSON_HPP
#define UTILITY_JSON_HPP

#include <boost/exception/all.hpp>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <concepts>
#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace utility
{

struct json_error : virtual std::exception, virtual boost::exception
{
};

struct json_member_not_found : virtual json_error
{
};

struct invalid_json_value_type : virtual json_error
{
};

using json_member_info = boost::error_info<struct tag_json_member, const char*>;
using json_value_type_info =
    boost::error_info<struct tag_json_value_type, Json::ValueType>;
using json_value_info = boost::error_info<struct tage_json_value, Json::Value>;

inline auto get(const Json::Value& val, const char* at) -> Json::Value
{
    assert(at);

    BOOST_LOG_TRIVIAL(debug) << "reading json value member: " << at;

    if (!val.isMember(at))
        BOOST_THROW_EXCEPTION(json_member_not_found()
                              << json_member_info(at) << json_value_info(val));

    return val[at];
}

// Safe json value type conversion
template <typename T>
inline auto as(const Json::Value& val, bool required = true) -> T
{
    if (!required and val.isNull())
        return T{};

    if (!val.is<T>())
        BOOST_THROW_EXCEPTION(invalid_json_value_type()
                              << json_value_type_info(val.type())
                              << json_value_info(val));

    return val.as<T>();
}

// Traverse json objects with ids.
template <typename BinaryOperation>
requires std::invocable<BinaryOperation,
                        std::decay_t<Json::String>,
                        std::decay_t<Json::Value>>
void for_each_object(const Json::Value& val, BinaryOperation func)
{
    if (val.isNull())
        return;

    if (val.isArray())
        BOOST_THROW_EXCEPTION(invalid_json_value_type()
                              << json_value_type_info(val.type())
                              << json_value_info(val));

    for (auto iter = std::begin(val); iter != std::end(val); ++iter)
        func(iter.name(), *iter);
}

} // namespace utility

#endif // UTILITY_JSON_HPP