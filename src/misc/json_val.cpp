#include "json_val.hpp"

#include <boost/exception/all.hpp>
#include <cassert>
#include <concepts>
#include <jsoncpp/json/value.h>

namespace misc
{

json_val::json_val(const wrapped_type& val) : m_impl { &val }
{
    assert(m_impl);
}

auto json_val::operator[](key_type key) const -> json_val
{
    const auto& val = get()[key.data()];

    if (val.isNull())
        BOOST_THROW_EXCEPTION(key_not_found() << key_info(key));

    return json_val(val);
}

namespace
{
    // Jsoncpp deserializable types.
    template < typename T >
    concept json_supported = std::same_as< T, bool > || std::
        same_as< T, int > || std::same_as< T, unsigned int > || std::
            same_as< T, int64_t > || std::same_as< T, uint64_t > || std::
                same_as< T, double > || std::same_as< T, std::string >;

    template < json_supported T >
    inline auto is_impl(const json_val& val) -> bool
    {
        return val.get().is< T >();
    }

    template < json_supported T >
    inline auto as_impl(const json_val& val) -> T
    {
        if (!is_impl< T >(val))
            BOOST_THROW_EXCEPTION(
                type_mismatch() << type_info(typeid(T).name()));

        return val.get().as< T >();
    }

} // namespace

auto json_val::isBool() const -> bool
{
    return is_impl< bool >(*this);
}

auto json_val::isInt() const -> bool
{
    return is_impl< int >(*this);
}

auto json_val::isUInt() const -> bool
{
    return is_impl< unsigned int >(*this);
}

auto json_val::isInt64() const -> bool
{
    return is_impl< int64_t >(*this);
}

auto json_val::isUInt64() const -> bool
{
    return is_impl< uint64_t >(*this);
}

auto json_val::isDouble() const -> bool
{
    return is_impl< double >(*this);
}

auto json_val::isString() const -> bool
{
    return is_impl< std::string >(*this);
}

auto json_val::asBool() const -> bool
{
    return as_impl< bool >(*this);
}

auto json_val::asInt() const -> int
{
    return as_impl< int >(*this);
}

auto json_val::asUInt() const -> unsigned int
{
    return as_impl< unsigned int >(*this);
}

auto json_val::asInt64() const -> int64_t
{
    return as_impl< int64_t >(*this);
}

auto json_val::asUInt64() const -> uint64_t
{
    return as_impl< uint64_t >(*this);
}

auto json_val::asDouble() const -> double
{
    return as_impl< double >(*this);
}

auto json_val::asString() const -> std::string
{
    return as_impl< std::string >(*this);
}

auto json_val::begin() const -> const_iterator
{
    return std::begin(get());
}

auto json_val::cbegin() const -> const_iterator
{
    return std::cbegin(get());
}

auto json_val::end() const -> const_iterator
{
    return std::end(get());
}

auto json_val::cend() const -> const_iterator
{
    return std::cend(get());
}

} // namespace misc