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

namespace Utility
{

// --------------------------- Runtime Errors ------------------------------- //

struct InvalidJsonArchive : virtual std::exception, virtual boost::exception
{
};

// -------------------------------------------------------------------------- //

// ----------------------- Runtime Error Info ------------------------------- //

using JsonArchiveInfo =
    boost::error_info<struct JsonArchiveTag, const std::string_view>;

// -------------------------------------------------------------------------- //

namespace Impl
{
    struct JsonRootFactory
    {
        auto operator()(const std::string_view tag) const -> Json::Value;
    };

} // namespace Impl

using JsonPool = Pool<Json::Value, std::string_view, Impl::JsonRootFactory>;

// Global json file resource pool.
extern const JsonPool jsons;

} // namespace Utility

#endif // UTILITY_JSON_HPP
