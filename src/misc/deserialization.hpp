// Contains some utility functions that wrap the jsoncpp api for error handling.
// Soultatos Stefanos 2022

#ifndef MISC_DESERIALIZATION_HPP
#define MISC_DESERIALIZATION_HPP

#include <boost/core/demangle.hpp> // for demangle
#include <boost/exception/all.hpp> // for error_info, exception
#include <jsoncpp/json/json.h>     // for Value
#include <stdexcept>               // for exception
#include <string>                  // for string
#include <string_view>             // for string_view

namespace misc
{

/***********************************************************
 * Concepts                                                *
 ***********************************************************/

template < typename T >
concept deserializable = std::same_as< T, bool > || std::
    same_as< T, int > || std::same_as< T, unsigned int > || std::
        same_as< T, int64_t > || std::same_as< T, uint64_t > || std::
            same_as< T, double > || std::same_as< T, std::string >;

/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct deserialization_error : virtual std::exception, virtual boost::exception
{
};

struct key_not_found : virtual deserialization_error
{
};

struct type_mismatch : virtual deserialization_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using key_info = boost::error_info< struct tag_key, std::string >;
using type_info = boost::error_info< struct tag_type, std::string >;

/***********************************************************
 * Functions                                               *
 ***********************************************************/

// Throws key_not_found error if the given key is not a json value member.
inline auto get(const Json::Value& val, const char* key) -> const auto&
{
    if (!val.isMember(key))
        BOOST_THROW_EXCEPTION(key_not_found() << key_info(key));

    return val[key];
}

// Throws type_mismatch error if given T is not the underlying one.
template < deserializable T >
inline auto as(const Json::Value& val) -> T
{
    if (!val.is< T >())
        BOOST_THROW_EXCEPTION(
            type_mismatch()
            << type_info(boost::core::demangle(typeid(T).name())));

    return val.as< T >();
}

} // namespace misc

#endif // MISC_DESERIALIZATION_HPP
