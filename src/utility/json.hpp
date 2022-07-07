// Contains some useful json utilities.
// Soultatos Stefanos 2022

#ifndef UTILITY_JSON_HPP
#define UTILITY_JSON_HPP

#include "pool.hpp"

#include <boost/exception/all.hpp>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace utility
{

// --------------------------- Runtime Errors ------------------------------- //

struct invalid_json_archive : virtual std::exception, virtual boost::exception
{
};

// -------------------------------------------------------------------------- //

// ----------------------- Runtime Error Info ------------------------------- //

using json_archive_info =
    boost::error_info<struct tag_json_archive, std::string_view>;

// -------------------------------------------------------------------------- //

namespace detail
{
    struct json_root_factory
    {
        auto operator()(const std::string_view tag) const -> Json::Value;
    };

} // namespace detail

using json_pool =
    pool<Json::Value, std::string_view, detail::json_root_factory>;

// Global json file resource pool.
extern const json_pool jsons;

} // namespace utility

#endif // UTILITY_JSON_HPP
