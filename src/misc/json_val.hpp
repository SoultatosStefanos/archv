// Contains a wrapper over a 3rd party json value object for error handling.
// Soultatos Stefanos 2022

#ifndef MISC_JSON_VAL_HPP
#define MISC_JSON_VAL_HPP

#include "jsoncppfwd.hpp" // for Value, ValueConstIterator

#include <boost/exception/error_info.hpp> // for error_info
#include <boost/exception/exception.hpp>  // for exception
#include <cstdint>                        // for int64_t, uint64_t
#include <stdexcept>                      // for exception
#include <string>                         // for string
#include <string_view>                    // for string_view

namespace misc
{
/***********************************************************
 * Errors                                                  *
 ***********************************************************/

struct json_val_error : virtual std::exception, virtual boost::exception
{
};

struct key_not_found : virtual json_val_error
{
};

struct type_mismatch : virtual json_val_error
{
};

/***********************************************************
 * Error Info                                              *
 ***********************************************************/

using key_info = boost::error_info< struct tag_key, std::string_view >;
using type_info = boost::error_info< struct tag_type, std::string_view >;

/***********************************************************
 * Json Val                                                *
 ***********************************************************/

// Mirrors the Json::Value api, but throws exceptions if must.
// NOTE: Supports read access only currently.
class json_val
{
public:
    using wrapped_type = Json::Value;
    using key_type = std::string_view;
    using const_iterator = Json::ValueConstIterator;

    json_val(const wrapped_type& val); // Implicit on purpose.
    json_val(wrapped_type&&) = delete; // Disallow temporaries.

    auto get() const -> const wrapped_type& { return *m_impl; }

    // Throws key_not_found error if the given key is not found.
    auto operator[](key_type key) const -> json_val;

    // clang-format off
    template < typename T >
    auto is() const -> bool = delete;
    // clang-format on

    // Throws type_mismatch error if given T is not the underlying one.
    template < typename T >
    auto as() const -> T = delete;

    auto isBool() const -> bool;
    auto isInt() const -> bool;
    auto isUInt() const -> bool;
    auto isInt64() const -> bool;
    auto isUInt64() const -> bool;
    auto isDouble() const -> bool;
    auto isString() const -> bool;

    auto asBool() const -> bool;
    auto asInt() const -> int;
    auto asUInt() const -> unsigned int;
    auto asInt64() const -> int64_t;
    auto asUInt64() const -> uint64_t;
    auto asDouble() const -> double;
    auto asString() const -> std::string;

    auto begin() const -> const_iterator;
    auto cbegin() const -> const_iterator;
    auto end() const -> const_iterator;
    auto cend() const -> const_iterator;

private:
    const wrapped_type* m_impl { nullptr };
};

/***********************************************************
 * Allowed Specializations                                 *
 ***********************************************************/

template <>
inline bool json_val::is< bool >() const
{
    return isBool();
}

template <>
inline bool json_val::is< int >() const
{
    return isInt();
}

template <>
inline bool json_val::is< unsigned int >() const
{
    return isUInt();
}

template <>
inline bool json_val::is< int64_t >() const
{
    return isInt64();
}

template <>
inline bool json_val::is< uint64_t >() const
{
    return isUInt64();
}

template <>
inline bool json_val::is< double >() const
{
    return isDouble();
}

template <>
inline bool json_val::is< std::string >() const
{
    return isString();
}

template <>
inline bool json_val::as< bool >() const
{
    return asBool();
}

template <>
inline int json_val::as< int >() const
{
    return asInt();
}

template <>
inline unsigned int json_val::as< unsigned int >() const
{
    return asUInt();
}

template <>
inline int64_t json_val::as< int64_t >() const
{
    return asInt64();
}

template <>
inline uint64_t json_val::as< uint64_t >() const
{
    return asUInt64();
}

template <>
inline double json_val::as< double >() const
{
    return asDouble();
}

template <>
inline std::string json_val::as< std::string >() const
{
    return asString();
}

} // namespace misc

#endif // MISC_JSON_VAL_HPP
