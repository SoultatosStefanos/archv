// Contains general purpose flyweight utilites.
// Soultatos Stefanos 2022

#ifndef UTILITY_FLYWEIGHT_HPP
#define UTILITY_FLYWEIGHT_HPP

#include <boost/exception/all.hpp>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unordered_map>

namespace utility
{

// ------------------------------ general ----------------------------------- //

namespace detail
{
    struct constructor
    {
        template <typename Resource, typename Tag>
        requires std::constructible_from<Resource, Tag>
        constexpr auto operator()(const Tag& tag) const
            noexcept(std::is_nothrow_constructible_v<Resource, Tag>)
        {
            return Resource(tag);
        }
    };

} // namespace detail

// A general purpose resource pool for tagged resources.
template <typename Resource,
          typename Tag,
          typename Factory = detail::constructor,
          typename AssociativeContainer = std::unordered_map<Tag, Resource>>
class pool
{
public:
    static_assert(std::is_invocable_r_v<Resource, Factory, std::decay_t<Tag>>);
    static_assert(std::is_same_v<typename AssociativeContainer::key_type, Tag>);
    static_assert(
        std::is_same_v<typename AssociativeContainer::mapped_type, Resource>);

    pool() = default;

    explicit pool(AssociativeContainer data) : m_data{std::move(data)} {}

    pool(AssociativeContainer data, Factory factory)
        : m_data{std::move(data)}, m_factory{std::move(factory)}
    {}

    auto get(const Tag& tag) const -> const Resource&
    {
        return m_data.contains(tag) ? m_data.at(tag)
                                    : m_data[tag] = m_factory(tag);
    }

    auto get(const Tag& tag) -> Resource&
    {
        return const_cast<Resource&>(std::as_const(*this).get(tag));
    }

private:
    Factory m_factory;
    mutable AssociativeContainer m_data;
};

// TODO Move somewhere else

// -------------------------------------------------------------------------- //

// ----------------------- flyweight with json files ------------------------ //

struct invalid_json_archive : virtual std::exception, virtual boost::exception
{
};

using json_archive_info =
    boost::error_info<struct tag_json_archive, std::string_view>;

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

// -------------------------------------------------------------------------- //

} // namespace utility

#endif // UTILITY_FLYWEIGHT_HPP
